#include "gtplatformagent.h"
#include "gamegateserver.hpp"
#include "gdeliveryserver.hpp"
#include "rolefriendupdate.hpp"
#include "rolegroupupdate.hpp"
#include "removerole.hpp"
#include "removefaction.hpp"
#include "factionmemberupdate.hpp"
#include "factioninfoupdate.hpp"
#include "gateonlinelist.hpp"
#include "mapuser.h"
#include "factionmanager.h"
#include "familymanager.h"
#include "base64.h"
#include "gatefactionchat.hpp"
#include "factionchat.hpp"
#include "updatefriendgtstatus.hpp"
#include "gategetgtrolestatus.hpp"
#include "gaterolelogin.hpp"
#include "gaterolelogout.hpp"
#include "gaterolecreate.hpp"

#include "gfolk"
#include "gfamily"
#include "gfactioninfo"
#include "groleinfo"
#include "ggateonlineinfo"
#include "ggaterolegtstatus"
#include "gfriendinfo"
#include "ggateroleinfo"

namespace GNET
{

bool GTPlatformAgent::gtopen = false;

int GTPlatformAgent::ModifyFriend(FriendOperation oper,int roleid,int friendid,char groupid)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::ModifyFriend friendid=%d roleid=%d",friendid,roleid);
	RoleFriendUpdate msg;
	msg.roleid = roleid;
	msg.rolefriend =RoleBean(RoleInfoBean(friendid));
	if(oper == FRIEND_ADD)
	{
               	GRoleInfo *prole = RoleInfoCache::Instance().Get(friendid);
		if(prole)
		{
			RoleInfoBean & r = msg.rolefriend.info;
			r.rolename = prole->name;
			r.gender = prole->gender;
			r.occupation = prole->occupation;
			r.level = prole->level;
			Marshal::OctetsStream os;
		        os << prole->reborn_cnt << prole->cultivation;
			r.extinfo = os;
		}
	}

	msg.groupid = groupid;
	msg.gtype = FRIENDTYPE;
	msg.operation = oper;
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::ModifyFriendGroup(GroupOperation oper,int roleid,char groupid,const Octets& groupname)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::ModifyFriendGroup groupid=%d roleid=%d",groupid,roleid);
	RoleGroupUpdate msg;
	msg.roleid = roleid;
	msg.groupid = groupid;
	msg.gtype = FRIENDTYPE;
	msg.operation = oper;
	msg.groupname = groupname;
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::DeleteRole(int userid,int roleid)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::DeleteRole roleid=%d",roleid);
	RemoveRole msg;
	msg.userid = userid;
	msg.roleid = roleid;
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::DeleteFaction(FType type,int fid)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::DeleteFaction fid=%d",fid);
	RemoveFaction msg;
	msg.factionid = FactionIDBean(type,fid);
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::FactionDelMember(int factionid,const GFamily& family,const RoleContainer& members)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::FactionDelMember fid=%d",factionid);
	FactionMemberUpdate msg;
	msg.factionid = FactionIDBean(FACTIONTYPE,factionid);
	msg.operation = MEMBER_DEL;
	std::list<FamilyRole>::const_iterator it,ite=members.end();
	for(it=members.begin();it!=ite;++it)
		msg.roles.push_back(RoleBean(RoleInfoBean(it->role.rid)));
	//title not need
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::FamilyDelMember(int familyid,int roleid,int factionid)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::FamilyDelMember facid=%d famid=%d",factionid,familyid);
	FactionMemberUpdate msg;
	msg.factionid = FactionIDBean(FAMILYTYPE,familyid);
	msg.operation = MEMBER_DEL;
	msg.roles.push_back(RoleBean(RoleInfoBean(roleid)));
	//title not need
	GameGateServer::GetInstance()->GTSend(msg);
	if(factionid!=0)
	{
		msg.factionid = FactionIDBean(FACTIONTYPE,factionid);
		GameGateServer::GetInstance()->GTSend(msg);
	}
	return 0;

}

int GTPlatformAgent::FactionModMember(FType type,int fid,int roleid,unsigned char title)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::FactionModMember fid=%d ftype=%d",fid,type);	
	FactionMemberUpdate msg;
	msg.factionid = FactionIDBean(type,fid);
	msg.operation = MEMBER_MOVE;
	msg.roles.push_back(RoleBean(RoleInfoBean(roleid)));
	msg.titleid = title;
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::FactionAddMember(const GFactionInfo& faction,const GFamily& family,const RoleContainer& members)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::FactionAddMember facid=%d famid=%d",faction.fid,family.id); 
	FactionMemberUpdate msg;
	msg.factionid = FactionIDBean(FACTIONTYPE,faction.fid);
	msg.operation = MEMBER_ADD;
	FactionMemberUpdate msg_master = msg;
	
	std::list<FamilyRole>::const_iterator it,ite=members.end();
	Marshal::OctetsStream os;
	for(it=members.begin();it!=ite;++it)
	{
		os.clear();
		int reborn = it->reborn_cnt;
		int cultivation = it->cultivation;
        	os << reborn << cultivation;
		if(it->role.rid == family.master)
		{
			msg_master.roles.push_back(RoleBean(RoleInfoBean(it->role.rid,it->role.name,0,0,it->role.occupation,it->role.level)));
			if(family.master == faction.master)
				msg_master.titleid = TITLE_MASTER;
			else
				msg_master.titleid = TITLE_HEADER;
		}
		else
		{
			msg.roles.push_back(RoleBean(RoleInfoBean(it->role.rid,it->role.name,0,0,it->role.occupation,it->role.level)));
			msg.titleid = TITLE_MEMBER;
		}
	}
	if(msg_master.roles.size()>0)
		GameGateServer::GetInstance()->GTSend(msg_master);
	if(msg.roles.size()>0)
		GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::FamilyAddMember(int familyid,const GFolk& role,int reborn,int cultivation,unsigned char gender,int factionid)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::FamilyAddMember add family facid=%d famid=%d",factionid,familyid);
	FactionMemberUpdate msg;
	msg.factionid = FactionIDBean(FAMILYTYPE,familyid);
	msg.operation = MEMBER_ADD;

	Marshal::OctetsStream os;
        os << reborn << cultivation;
	msg.roles.push_back(RoleBean(RoleInfoBean(role.rid,role.name,gender,0,role.occupation,role.level,os)));
	msg.titleid = role.title;
	GameGateServer::GetInstance()->GTSend(msg);

	if(factionid!=0)
	{
		DEBUG_PRINT("GTPlatformAgent::FamilyAddMember add Faction, facid=%d famid=%d",factionid,familyid);
		msg.factionid = FactionIDBean(FACTIONTYPE,factionid);
		GameGateServer::GetInstance()->GTSend(msg);
	}	
	return 0;
}

int GTPlatformAgent::FactionUpdate(FType type,int fid,const Octets& data, int updatetype)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::FactionUpdate fid=%d ftype=%d",fid,type);
	FactionInfoBean info;
	if (updatetype == FAC_UPDATE_NAME)
		info.factionname = data;
	else if (updatetype == FAC_UPDATE_ANNOUNCE)
		info.announcement = data;
	else if (updatetype == FAC_UPDATE_EXT)
		info.extinfo = data;
	else
	{
		Log::log(LOG_ERR, "FactionUpdate fid=%d ftype=%d invalid updateflag %d", fid, type, updatetype);
		return -2;
	}
	FactionInfoUpdate msg;
	msg.factionid = FactionIDBean(type,fid);
	msg.updateflag = updatetype;
	msg.factioninfo = info;
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::SyncTOGate(int startrole,bool start)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::SyncTOGate startrole=%d",startrole);
	GateOnlineList msg;
	if(start)
		msg.start = 1;//start
	else
		msg.start = 0;

	std::vector<int> rolelist;
	char finish;
	UserContainer::GetInstance().GetOnlineRolelist(startrole,MAX_RECORD_NUM_IN_GATEONLINELIST,rolelist,finish);
	msg.finish = finish;
	msg.onlinelist.reserve(rolelist.size());
	std::vector<int>::iterator it,ite=rolelist.end();
	for(it=rolelist.begin();it!=ite;++it)
	{
		msg.onlinelist.push_back(GGateOnlineInfo(*it));
	}
	
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::OnGateFactionChat(const GateFactionChat& gchat)
{
	if(!IsGTOpen())
		return -1;
	FactionChat chat;
	chat.channel = gchat.channel;
	chat.src = gchat.src;
	chat.msg = gchat.msg;
	chat.emotion = gchat.emotion;
	chat.src_route = FROM_GT;
	switch(gchat.channel)
	{
	case GP_CHAT_FACTION: //faction
		if(FactionManager::Instance()->Broadcast(gchat.fid, chat, chat.localsid))
		{
			Octets out;
			Base64Encoder::Convert(out, chat.msg);
			Log::log(LOG_CHAT,"GTFaction: src=%d fid=%d msg=%.*s",chat.src,gchat.fid,out.size(),(char*)out.begin());
		}
		break;
	case GP_CHAT_FAMILY: // family
		{
		FamilyInfo * family = FamilyManager::Instance()->Find(gchat.fid);
                if (NULL == family)
                       return -1;	
		family->Broadcast(chat, chat.localsid);
		Octets out;
		Base64Encoder::Convert(out, chat.msg);
		Log::log(LOG_CHAT,"GTFamily: src=%d fid=%d msg=%.*s",chat.src,gchat.fid,out.size(),(char*)out.begin());
		}
		break;
	default:
		DEBUG_PRINT("GateFactionChat: channel=%d err",chat.channel);
		break;
	}
	return 0;
}

int GTPlatformAgent::UpdateClientGTStatus(const std::vector<int>& rolelist,const GGateRoleGTStatus & status)
{
	if(!IsGTOpen())
		return -1;
//	DEBUG_PRINT("GTPlatformAgent::UpdateClientGTStatus roleid=%d ",status.roleid);
	std::vector<int>::const_iterator it,ite=rolelist.end();
	PlayerInfo * ruser;
	int roleid = status.roleid;
	UpdateFriendGTStatus stat(status,0);
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	for(it = rolelist.begin();it!=ite;++it)
	{
		ruser = UserContainer::GetInstance().FindRoleOnline(*it);
		if(NULL != ruser )
		{
			for(GFriendInfoVector::iterator k=ruser->friends.begin(),ke=ruser->friends.end();k!=ke;++k)
			{
				if(k->rid==roleid)
				{
					stat.localsid = ruser->localsid;
					dsm->Send(ruser->linksid,stat);
					break;
				}
			}
		}
	}
	return 0;
}

int GTPlatformAgent::GetFriendStatus(int roleid,int friendid)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::GetFriendStatus roleid=%d friend=%d",roleid,friendid);
	GateGetGTRoleStatus msg;
	msg.roleid =roleid;
	msg.rolelist.push_back(friendid);
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::GetFriendStatus(int roleid,const GFriendInfoVector& flist)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::GetFriendStatus roleid=%d ",roleid);
	GateGetGTRoleStatus msg;
	if(flist.size()==0)
		return 0;
	msg.roleid =roleid;
	msg.rolelist.reserve(flist.size());
	GFriendInfoVector::const_iterator it,ite=flist.end();
	for(it=flist.begin();it!=ite;++it)
	{
		msg.rolelist.push_back(it->rid);
	}
	GameGateServer::GetInstance()->GTSend(msg);
	return 0;
}

int GTPlatformAgent::OnPlayerLogout(int roleid,int occupation,int level)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::OnPlayerLogout roleid=%d",roleid);
	GGateRoleInfo roledata(roleid,0,occupation,level,0,0);
	GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
	if(info)
	{
                roledata.reborntimes = info->reborn_cnt;
                roledata.cultivation = info->cultivation;
                roledata.gender = info->gender;
                roledata.rolename = info->name;
	}
	roledata.gameid = -1;
	GameGateServer::GetInstance()->GTSend(GateRoleLogout(roledata));
	return 0;
}

int GTPlatformAgent::OnPlayerLogin(int roleid,const GRoleInfo& info)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::OnPlayerLogin roleid=%d",roleid);
	PlayerInfo* role = UserContainer::GetInstance().FindRoleOnline(roleid);
	int linksid=0;
	int localsid=0;
	GGateRoleInfo roledata(roleid);
	if(role)
	{
		linksid = role->linksid;
		localsid = role->localsid;
		roledata.gameid = role->gameid;
	}
	roledata.occupation = info.occupation;
	roledata.level = info.level;
        roledata.reborntimes = info.reborn_cnt;
        roledata.cultivation = info.cultivation;
        roledata.gender = info.gender;
        roledata.rolename = info.name;	
        GateRoleLogin gmsg(localsid,linksid,roledata);
        GameGateServer::GetInstance()->GTSend(gmsg);
	return 0;
}

int GTPlatformAgent::CreateRole(int roleid, int userid, const GRoleInfo & info)
{
	if(!IsGTOpen())
		return -1;
	DEBUG_PRINT("GTPlatformAgent::CreateRole roleid=%d",roleid);
	GGateRoleInfo roledata(roleid);
	roledata.gender = info.gender;
	roledata.occupation = info.occupation;
	roledata.level = info.level;
	roledata.reborntimes = info.reborn_cnt;
	roledata.cultivation = info.cultivation;
	roledata.rolename = info.name;	
	GateRoleCreate gmsg(roleid, userid, roledata);
	GameGateServer::GetInstance()->GTSend(gmsg);
	return 0;
}

};
