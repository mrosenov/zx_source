#include "gtmanager.h"
#include "gtplatformclient.hpp"
#include "gamedbclient.hpp"
#include "gamegateclient.hpp"
#include "gaterolecache.h"
#include "gtcommon.h"

#include "rolestatusupdate.hpp"
#include "factioninforeq.hpp"
#include "rolestatusreq.hpp"
#include "rolestatusresp.hpp"
#include "rolelistresp.hpp"
#include "factioninforesp.hpp"
#include "roleinfoupdate.hpp"
#include "gateupdatestatus.hpp"
#include "roleinforesp.hpp"
#include "rolerelationresp.hpp"
#include "privatechat.hpp"
#include "rolemsg.hpp"
#include "gatefactionchat.hpp"
#include "factionmsg.hpp"
#include "gateofflinechat.hpp"
#include "dbgateactivate.hrp"
#include "dbgatedeactivate.hrp"
//#include "userqueryagent2.h"
//#include "factionqueryagent.h"
#include "removefaction.hpp"
#include "removerole.hpp"

#include "rolestatusbean"
#include "ggateroleinfo"
#include "ggaterolegtstatus"
#include "gfactioninfo"
#include "gfamily"
#include "factioninfobean"
#include "roleinfobean"
#include "rolebean"
#include "ggroupinfo"
#include "gfriendinfo"
#include "rolegroupbean"
#include "dbgaterolerelationres"
#include "ggaterolegtstatus"
#include "rolemsgbean"
#include "gtcommon.h"
namespace GNET
{
/////////////////////////////////// GTManager  //////////////////////////////////////
int GTManager::OnRoleListReq(int64_t gtuserid,int pt_type)
{
	int userid;
	if(Check_64to32_Cast(gtuserid,userid))
	{
		UserQuery key(userid, pt_type);
		return rolelist_queue.TryRunTask(key);
	}
	else
		return -1;
}

int GTManager::OnRoleInfoReq(int64_t localuid,int64_t gtroleid)
{
	int roleid;
//	DEBUG_PRINT("GTManager::OnRoleInfoReq roleid=%lld",gtroleid);
	if(Check_64to32_Cast(gtroleid,roleid))
	{
		return roleinfo_queue.TryRunTask(RoleInfoTask(localuid,roleid));
	}
	else
		return -1;
}

int GTManager::OnFactionInfoReq(FactionReq req)
{
//	DEBUG_PRINT("GTManager::OnFactionInfoReq");
	return factioninfo_queue.TryRunTask(req);
}

int GTManager::OnFamilyInfoReq(FactionReq req)
{
//	DEBUG_PRINT("GTManager::OnFamilyInfoReq");
	return familyinfo_queue.TryRunTask(FamilyInfoTask(req.localuid,req.fid));
}

int GTManager::OnDBGateGetRoleErr(int userid,int64_t localuid,int roleid,int retcode, int pt_type)
{
	if(userid!=0)//acquire role data for rolelist query
	{
		UserQueryMAP::iterator it;
		it = user_query_map.find(userid);
		if(it==user_query_map.end())
			return -1;
		if(it->second.On_Resp_Two_Err(roleid))
			DEBUG_PRINT("GTManager::OnDBGateGetRoleErr On_Resp_Two_Err error");
		if(it->second.QueryFinish())
		{
			RoleListResp re;
			re.userid=userid;
			it->second.BuildResult(re.roles);
			if(pt_type == GGT_SNS_TYPE)
			{
				SNSPlatformClient::GetInstance()->SendProtocol(re);	
			}
			else
				GTPlatformClient::GetInstance()->SendProtocol(re);
			user_query_map.erase(it);
		}	
	}
	else if(localuid!=0)//roleinfo query
	{
//		RoleInfoResp re;
//		re.roleinfo = GGateRoleInfo2RoleInfoBean(roleinfo);
//		re.localuid = localuid;
//		GTPlatformClient::GetInstance()->SendProtocol(re);
		if(retcode == ERR_DATANOTFIND)
		{
			RemoveRole re;
			re.userid = userid;// 0
			re.roleid = roleid;
			if(pt_type == GGT_SNS_TYPE)
			{
				SNSPlatformClient::GetInstance()->SendProtocol(re);	
			}
			else
				GTPlatformClient::GetInstance()->SendProtocol(re);
		}
	}
	else
	{
		DEBUG_PRINT("GTManager::OnDBGateGetRole argument err,userid=%d,localuid = %lld,pt_type=%d",userid,localuid,pt_type);
		return -1;
	}
	return 0;
}

int GTManager::OnDBGateGetRole(const GGateRoleInfo& roleinfo,int userid,int64_t localuid,int pt_type)
{
	if(userid!=0)//acquire role data for rolelist query
	{
		UserQueryMAP::iterator it;
		it = user_query_map.find(userid);
		if(it==user_query_map.end())
			return -1;
		it->second.On_Resp_Two(roleinfo.roleid,roleinfo);
		if(it->second.QueryFinish())
		{
			RoleListResp re;
			re.userid=userid;
			it->second.BuildResult(re.roles);
			if(pt_type == GGT_SNS_TYPE)
			{
				SNSPlatformClient::GetInstance()->SendProtocol(re);	
			}
			else
				GTPlatformClient::GetInstance()->SendProtocol(re);
			user_query_map.erase(it);
		}	
	}
	else if(localuid!=0)//roleinfo query
	{
		RoleInfoResp re;
		re.roleinfo = GGateRoleInfo2RoleInfoBean(roleinfo);
		re.localuid = localuid;
		if(pt_type == GGT_SNS_TYPE)
		{
			SNSPlatformClient::GetInstance()->SendProtocol(re);	
		}
		else
			GTPlatformClient::GetInstance()->SendProtocol(re);
	}
	else
	{
		DEBUG_PRINT("GTManager::OnDBGateGetRole argument err,userid=%d,localuid = %lld,pt_type=%d",userid,localuid,pt_type);
		return -1;
	}
	return 0;
}

int GTManager::OnDBGateGetUser(int userid, unsigned int rolelist, unsigned int logicuid, int pt_type)
{
	UserQueryMAP::iterator it;
	it = user_query_map.find(userid);
	if(it==user_query_map.end())
		return -1;
	if(it->second.On_Resp_One(RoleList(rolelist,logicuid),0))//no step1result for user query
		return -1;
	if(it->second.QueryFinish())//no role
	{
		RoleListResp re;
		re.userid=userid;
	//	it->second.BuildResult(re.roles);
		if(pt_type == GGT_SNS_TYPE)
		{
			SNSPlatformClient::GetInstance()->SendProtocol(re);	
		}
		else
			GTPlatformClient::GetInstance()->SendProtocol(re);
		user_query_map.erase(it);
	}	
	return 0;	
}

int GTManager::OnDBFamilyGet(int factionid, const GFamily& family,int64_t localuid, int pt_type)
{
	if(factionid!=0)//acquire family data for faction query
	{
		FactionQueryMAP::iterator it;
		it = faction_query_map.find(factionid);
		if(it==faction_query_map.end())
			return -1;
		if(it->second.On_Resp_Two(family.id,family.member))
			DEBUG_PRINT("GTManager::OnDBFamilyGet On_Resp_Two error");
		if(it->second.QueryFinish())
		{
			FactionInfoResp re;
			it->second.BuildResult(re);
			if(pt_type == GGT_SNS_TYPE)
			{
				SNSPlatformClient::GetInstance()->SendProtocol(re);	
			}
			else
				GTPlatformClient::GetInstance()->SendProtocol(re);
			DEBUG_PRINT("GTManager::OnDBFamilyGet BuildResult for faction %d, pt_type=%d",factionid,pt_type);
			faction_query_map.erase(it);
		}
		return 0;
	}
	else if(localuid!=0)//family query
	{
		FactionInfoResp re;
	        re.factioninfo = FactionInfoBean(family.name,family.announce);;
	        re.factionid = FactionIDBean(FAMILYTYPE,family.id);
		re.localuid = localuid;
		if(FamilyMemberSetData(re,family.member,15))
		{
			DEBUG_PRINT("GTManager::OnDBFamilyGet BuildResult for family %d err",family.id);
			return -1;
		}
		FamilyChangeTitle(re);
		if(pt_type == GGT_SNS_TYPE)
		{
			SNSPlatformClient::GetInstance()->SendProtocol(re);	
		}
		else
			GTPlatformClient::GetInstance()->SendProtocol(re);
		return 0;
	}
	else
	{
		DEBUG_PRINT("GTManager::OnDBFamilyGet argument err,factionid=%d,localuid = %lld,pt_type=%d",factionid,localuid,pt_type);
		return -1;
	}
}

int GTManager::OnDBFamilyGetErr(int factionid, int familyid,int64_t localuid,int retcode,int pt_type)
{
	if(factionid!=0)//acquire family data for faction query
	{
		FactionQueryMAP::iterator it;
		it = faction_query_map.find(factionid);
		if(it==faction_query_map.end())
			return -1;
		if(it->second.On_Resp_Two_Err(familyid))
			DEBUG_PRINT("GTManager::OnDBFamilyGetERR On_Resp_Two_Err error");
		if(it->second.QueryFinish())
		{
			FactionInfoResp re;
			it->second.BuildResult(re);
			if(pt_type == GGT_SNS_TYPE)
			{
				SNSPlatformClient::GetInstance()->SendProtocol(re);	
			}
			else
				GTPlatformClient::GetInstance()->SendProtocol(re);
			DEBUG_PRINT("GTManager::OnDBFamilyGetErr BuildResult for faction %d,pt_type=%d",factionid,pt_type);
			faction_query_map.erase(it);
		}
		return 0;
	}
	else if(localuid!=0)//family query
	{
		if(retcode == ERR_DATANOTFIND)
		{
			RemoveFaction re;
			re.factionid.factionid = familyid;
			re.factionid.ftype = FAMILYTYPE;
			if(pt_type == GGT_SNS_TYPE)
			{
				SNSPlatformClient::GetInstance()->SendProtocol(re);	
			}
			else
				GTPlatformClient::GetInstance()->SendProtocol(re);
		}
		return 0;
	}
	else
	{
		DEBUG_PRINT("GTManager::OnDBFamilyGetErr argument err,factionid=%d,localuid = %lld,pt_type=%d",factionid,localuid,pt_type);
		return -1;
	}

}

int GTManager::FamilyChangeTitle(FactionInfoResp & resp)
{
	std::vector<GNET::FactionTitleBean>::iterator vit,vite;
	vit=resp.members.begin();
	vite=resp.members.end();
	for(;vit!=vite;++vit)
	{
		if(vit->titleid==TITLE_MASTER || vit->titleid==TITLE_VICEMASTER || vit->titleid==TITLE_CAPTAIN)
		{
			vit->titleid = TITLE_HEADER;
		}
	}
	return 0;
}

int GTManager::FamilyMemberSetData(FactionInfoResp & resp,const std::vector<GNET::GFolk>& info,int reserve_num)
{
	std::vector<GNET::GFolk>::const_iterator it,ite=info.end();
	std::vector<GNET::FactionTitleBean>::iterator vit,vite;
	RoleStatusBean status;
	for(it=info.begin();it!=ite;++it)
	{
		status = RoleStatusQuery(it->rid);
		vit=resp.members.begin();
		vite=resp.members.end();
		for(;vit!=vite;++vit)
		{
			if(vit->titleid == it->title)
			{
				vit->titlemembers.push_back(RoleBean(RoleInfoBean(it->rid,it->name),status));
				break;
			}
		}
		if(vit==vite)
		{
			resp.members.push_back(FactionTitleBean(it->title));
			resp.members.back().titlemembers.reserve(reserve_num);//only normal folk need
			resp.members.back().titlemembers.push_back(RoleBean(RoleInfoBean(it->rid,it->name),status));
		}
	}
	return 0;

}

int GTManager::FriendSetData(std::vector<RoleGroupBean> & ret,const GFriendInfoVector& friends,const GGroupInfoVector& group)
{
	ret.push_back(RoleGroupBean(FRIENDTYPE,0));//default group
	GGroupInfoVector::const_iterator git,gite=group.end();
	for(git=group.begin();git!=gite;++git)
		ret.push_back(RoleGroupBean(FRIENDTYPE,git->gid,git->name));

	std::vector<RoleGroupBean>::iterator vit,vite;
	GFriendInfoVector::const_iterator it,ite=friends.end();
	vite=ret.end();

	RoleStatusBean status;
	for(it=friends.begin();it!=ite;++it)
	{
		status = RoleStatusQuery(it->rid);
		for(vit=ret.begin();vit!=vite;++vit)
		{
			if(static_cast<char>(vit->groupid) == it->gid)
			{
				//都填成 -1 表示非法值，这样 gt 的数据库就不会用它来覆盖自己本地的正确数据
				vit->friends.push_back(RoleBean(RoleInfoBean(it->rid,it->name,-1,-1,-1,-1),status));
				break;
			}
		}
		if(vit==vite)
		{
			DEBUG_PRINT("FriendSetData:friend roleid=%d cannot find group, gid=%d",it->rid,it->gid);
		}
	}
	return 0;
}

int GTManager::OnDBFactionGet(int factionid, const GFactionInfo& info, int pt_type)
{
	FactionQueryMAP::iterator it;
	it = faction_query_map.find(factionid);
	if(it==faction_query_map.end())
		return -1;
	DEBUG_PRINT("GTManager::OnDBFactionGet facid=%d,membersize=%d",factionid,info.member.size());
	it->second.On_Resp_One(FamilyList(info.member),GFactionInfo2FactionInfoBean(info));
	if(it->second.QueryFinish())//no family,impossible
	{
		FactionInfoResp re;
		it->second.BuildResult(re);//set factioninfo etc..
		if(pt_type == GGT_SNS_TYPE)
		{
			SNSPlatformClient::GetInstance()->SendProtocol(re);	
		}
		else
			GTPlatformClient::GetInstance()->SendProtocol(re);
		Log::log(LOG_ERR,"GTManager::OnDBFactionGet BuildResult for faction %d,pt_type=%d",factionid,pt_type);
		faction_query_map.erase(it);
	}
	return 0;
}

int GTManager::OnDBFactionGetErr(int factionid,int64_t localuid,int retcode, int pt_type)
{
	DEBUG_PRINT("GTManager::OnDBFactionGetERR facid=%d,pt_type=%d",factionid,pt_type);
	FactionQueryMAP::iterator it;
	it = faction_query_map.find(factionid);
	if(it==faction_query_map.end())
		return -1;
	faction_query_map.erase(it);
	if(retcode == ERR_DATANOTFIND)
	{
		RemoveFaction re;
		re.factionid.factionid = factionid;
		re.factionid.ftype = FACTIONTYPE;
		if(pt_type == GGT_SNS_TYPE)
		{
			SNSPlatformClient::GetInstance()->SendProtocol(re);	
		}
		else
			GTPlatformClient::GetInstance()->SendProtocol(re);
	}
	return 0;
}

int GTManager::TryUserQuery(UserQuery req)
{
	int userid = req.userid;
	UserQueryMAP::iterator it;
	it = user_query_map.find(userid);
	if(it!=user_query_map.end())
		return -1;
	std::pair<UserQueryMAP::iterator,bool>pa = user_query_map.insert(std::make_pair(userid,UserQueryAgent(req,false)));
	if( pa.second)
		return pa.first->second.Query_One();
	return -1;
}

int GTManager::TryFactionQuery(FactionReq req)
{
//	DEBUG_PRINT("GTManager::TryFactionQuery,fid=%d",req.fid);
	FactionQueryMAP::iterator it;
	it = faction_query_map.find(req.fid);
	if(it!=faction_query_map.end())
		return -1;
	std::pair<FactionQueryMAP::iterator,bool>pa = faction_query_map.insert(std::make_pair(req.fid,FactionQueryAgent(req,true)));
	if( pa.second)
		return pa.first->second.Query_One();
	return -1;
}

int GTManager::OnRoleRelationReq(int64_t gtuserid,int64_t gtroleid,int pt_type)
{
	int userid,roleid;
	Check_64to32_Cast(gtuserid,userid);//need not check userid
	if(Check_64to32_Cast(gtroleid,roleid))
	{
		return rolerelation_queue.TryRunTask(RoleRelationTask(userid,roleid,pt_type));
	}
	else
		return -1;
}

int GTManager::OnDBGateRoleRelation(RoleRelationResp& re,DBGateRoleRelationRes* res,int pt_type)
{
	re.roleinfo = RoleBean(GGateRoleInfo2RoleInfoBean(res->roledata),RoleStatusQuery(res->roledata.roleid));
	if(res->factionid != 0)
		re.factions.push_back(FactionIDBean(FACTIONTYPE,res->factionid));
	if(res->familyid != 0)
		re.factions.push_back(FactionIDBean(FAMILYTYPE,res->familyid));
	re.userid = res->userid;
	FriendSetData(re.friends,res->friends,res->groups);
	if(pt_type == GGT_SNS_TYPE)
	{
		SNSPlatformClient::GetInstance()->SendProtocol(re);	
	}
	else
		GTPlatformClient::GetInstance()->SendProtocol(re);
	return 0; 
}

int GTManager::OnDBGateRoleRelationErr(int userid,int roleid,int retcode,int pt_type)
{
	if(retcode == ERR_GT_DATANOTFIND)
	{
		RemoveRole re;
		re.userid = userid;
		re.roleid = roleid;
		if(pt_type == GGT_SNS_TYPE)
		{
			SNSPlatformClient::GetInstance()->SendProtocol(re);	
		}
		else
			GTPlatformClient::GetInstance()->SendProtocol(re);
	}
	return 0;
}

int GTManager::OnRoleStatusResp(const std::map<int64_t,RoleStatusBean>& rolestatus)
{
	std::map<int64_t,RoleStatusBean>::const_iterator it,ite=rolestatus.end();
	for(it=rolestatus.begin();it!=ite;++it)
	{
		UpdateRoleStatusCache(it->first,it->second);
	}
	return 0;
}

int GTManager::OnRoleStatusUpdate(const RoleStatusUpdate& tsk)
{
	if(!global_gtstatus_chat_limit.Check())
		return -1;
	UpdateClientRoleStatus(tsk.roleid,tsk.status,tsk.friends);//should be called before UpdateRoleStatusCache
	return UpdateRoleStatusCache(tsk.roleid,tsk.status);
}

int GTManager::UpdateClientRoleStatus(int64_t gtroleid,const RoleStatusBean& status,const std::vector<int64_t>& friends)
{
	int roleid;
	if(!Check_64to32_Cast(gtroleid,roleid))
		return -1;
	RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
	bool newstatus = IS_GTOnline_In_Client_Eyes(status.status);
	bool need_update=false;
	if(role != NULL)
	{
		bool old = IS_GTOnline_In_Client_Eyes(role->GetGTStatus());
		if(old != newstatus)
			need_update=true;
		else if (IS_Hide_To_Offline(status.status,role->GetGTStatus()))
			need_update=true;
	}
	else 
	{
		if(newstatus)
			need_update=true;
	}
	
	if(need_update)
	{
		GateUpdateStatus re;
		char tmp = newstatus;
		re.friendstatus =GGateRoleGTStatus(roleid,tmp);
		std::vector<int64_t>::const_iterator it,ite=friends.end();
		for(it=friends.begin();it!=ite;++it)
		{
			if(Check_64to32_Cast(*it,roleid))
			{
				re.rolelist.push_back(roleid);
			}
		}
		if(re.rolelist.size()>0)
			GameGateClient::GetInstance()->SendProtocol(re);
	}
	return 0;
}

int GTManager::UpdateRoleStatusCache(int64_t gtroleid,const RoleStatusBean& status)
{
	int roleid;
	if(!Check_64to32_Cast(gtroleid,roleid))
		return -1;
	RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
	if(role != NULL)
	{
		role->SetGTStatus(status.status);
		if( role->GameOnline() != (!IS_GAME_OFFLINE(status.status)))
		{
			UpdateStatusTOGT(roleid,role);
		}
	}
	else if(!IS_GT_OFFLINE(status.status))
	{
		if(RoleContainer::GetInstance().AddRole(roleid,false,status.status))
			return -1;
	
		if(!IS_GAME_OFFLINE(status.status))
		{
			role = RoleContainer::GetInstance().FindRole(roleid);
			UpdateStatusTOGT(roleid,role);
		}
	}
	//else both game and gt offline, need not add entry
	return 0;
}

int GTManager::UpdateStatusTOGT(int roleid,const RoleInfo* role)
{
	if(role==NULL)
		return -1;
	RoleStatusBean status(ROLEINTO_TO_GTSTATUS(role->GameOnline(),role->GetGTStatus()));
	RoleStatusUpdate msg(roleid,status);
	GTPlatformClient::GetInstance()->SendProtocol(msg);
	return 0;
}

bool GTManager::GameOnline(int roleid)
{
	RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
	if(role != NULL)
	{
		if(role->GameOnline())
			return true;
	}
	return false;
}

bool GTManager::GTOnline(int roleid)
{
	RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
	if(role != NULL)
	{
		if(!IS_GT_OFFLINE(role->GetGTStatus()))
			return true;
	}
	return false;
}

RoleStatusBean GTManager::RoleStatusQuery(int roleid)
{
	unsigned char status=0;
	RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
	if(role != NULL)
	{
		status= role->GetGTStatus();
		if(role->GameOnline())
			status|=0x1;
	}
	return RoleStatusBean(status);
}

int GTManager::GameLogin(unsigned int roleid,int localsid, int linksid,const GGateRoleInfo& data )
{
	RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
	if(role != NULL)
	{
		role->SetGameStatus(true);
		role->SetRoleData(data);
	}
	else
	{
		if(RoleContainer::GetInstance().AddRole(roleid,true,0,data))
			return -1;
		role = RoleContainer::GetInstance().FindRole(roleid);
	}
	UpdateStatusTOGT(roleid,role);
	return 0;
}

int GTManager::GameLogout(unsigned int roleid, const GGateRoleInfo& data)
{
	RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
	if(role != NULL)
	{
		//有可能传来的等级、姓名是无效的
		if (data.level > 0)
		{
			GGateRoleInfo olddata;
			if(role->GetRoleData(olddata))
			{
				UpdateGTRoleData(roleid,olddata,data);
			}
			else
				SendGTRoleData(roleid,data);
			role->SetRoleData(data);
		}
		role->SetGameStatus(false);
	}
	else
	{
		if(RoleContainer::GetInstance().AddRole(roleid,false,0,data))
			return -1;
		role = RoleContainer::GetInstance().FindRole(roleid);
	}
	UpdateStatusTOGT(roleid,role);
	return 0;

}

int GTManager::GameRoleCreate(int roleid, int userid, const GGateRoleInfo & data)
{
	RoleInfoUpdate msg(GGateRoleInfo2RoleInfoBean2(data, userid), UPDATE_ROLE_CREATE);
	GTPlatformClient::GetInstance()->SendProtocol(msg);
	return 0;
}

int GTManager::UpdateGTRoleData(unsigned int roleid, const GGateRoleInfo& olddata, const GGateRoleInfo& data)
{
	//use "race" field to send cultivation	
	int cflag=0;//change flag
/*	if(olddata.occupation != data.occupation)
	{
		cflag|=UPDATE_OCCUPATION;
	}*/
	if(olddata.level != data.level)
	{
		cflag|=UPDATE_LEVEL;
		cflag|=UPDATE_OCCUPATION;
		cflag|=UPDATE_RACE;
	}
	if(olddata.reborntimes != data.reborntimes)
	{
		cflag|=UPDATE_EXT;
	}
/*	if(olddata.cultivation != data.cultivation)
	{
		cflag|=UPDATE_RACE;
	}*/
	if(olddata.rolename != data.rolename)
	{
		cflag|=UPDATE_NAME;
	}
	if(cflag)
	{
		RoleInfoUpdate msg(GGateRoleInfo2RoleInfoBean(data),cflag);
		GTPlatformClient::GetInstance()->SendProtocol(msg);
	}
	return 0;
}

int GTManager::SendGTRoleData(unsigned int roleid, const GGateRoleInfo& data)
{
	int cflag=0;//change flag
	cflag|=UPDATE_OCCUPATION;
	cflag|=UPDATE_LEVEL;
	cflag|=UPDATE_EXT;
	cflag|=UPDATE_RACE;
	cflag|=UPDATE_NAME;
	RoleInfoUpdate msg(GGateRoleInfo2RoleInfoBean(data),cflag);
	GTPlatformClient::GetInstance()->SendProtocol(msg);
	return 0;
}

int GTManager::OnRoleStatusReq(const std::vector<int64_t>& rolelist,int64_t localrid)
{
	int roleid;
	RoleStatusResp re;
	std::vector<int64_t>::const_iterator it,ite=rolelist.end();
	for(it=rolelist.begin();it!=ite;++it)
	{
		if(Check_64to32_Cast(*it,roleid))
		{
			re.rolestatus.insert(std::make_pair(roleid,RoleStatusQuery(roleid)));
		}
	}
	re.localrid=localrid;
	GTPlatformClient::GetInstance()->SendProtocol(re);
	return 0;
}

int GTManager::OnGetGTRoleStatus(const std::vector<int> & rolelist,std::vector<GGateRoleGTStatus>& statuslist)
{
	std::vector<int>::const_iterator it,ite=rolelist.end();
	unsigned char status;
	int roleid;
	statuslist.reserve(rolelist.size());
	for(it=rolelist.begin();it!=ite;++it)
	{
		roleid=*it;
		RoleInfo* role = RoleContainer::GetInstance().FindRole(roleid);
		if(role != NULL)
			status= IS_GTOnline_In_Client_Eyes(role->GetGTStatus());//set status in client's view
		else
			status=0;
		statuslist.push_back(GGateRoleGTStatus(roleid,status));
	}
	return 0;
}

int GTManager::OnGTRoleMsg(int64_t receiver,const RoleMsgBean& msg)
{
	int receiverid,senderid;
	if(!Check_64to32_Cast(receiver,receiverid) || !Check_64to32_Cast(msg.sender,senderid))
		return -1;
	if(!global_private_chat_limit.Check())
	{
	//	DEBUG_PRINT("OnGTRoleMsg too many global chats roleid=%d",senderid);
		return -1;
	}
	if(!private_chat_limit.Check(senderid))
	{
	//	DEBUG_PRINT("OnGTRoleMsg too many chats roleid=%d",senderid);
		global_private_chat_limit.Rollback();
		return -1;
	}
	PrivateChat chat;
	chat.channel = CHANNEL_NORMAL;
	chat.emotion = msg.emotiongroup;
	chat.src_name = msg.sendername;
	chat.srcroleid = senderid;
	chat.dstroleid = receiverid;
	chat.msg = msg.content;
	chat.route = FROM_GT;
	GameGateClient::GetInstance()->SendProtocol(chat);
	return 0;
}

int GTManager::OnGTOfflineRoleMsg(int64_t receiver,const std::vector<RoleMsgBean>& msgs)
{
	int receiverid;
	if(!Check_64to32_Cast(receiver,receiverid)) 
		return -1;
	if(!global_private_chat_limit.Check())
	{
	//	DEBUG_PRINT("OnGTOfflineMsg too many global offline chats receiver roleid=%d",receiverid);
		return -1;
	}
	GateOfflineChat offchat;
	offchat.dstroleid= receiverid;
	offchat.channel = CHANNEL_NORMAL;

	GGTOfflineMsg msg;
	std::vector<RoleMsgBean>::const_iterator it,ite=msgs.end();
	int senderid;
	for(it=msgs.begin();it!=ite;++it)
	{
		if(!Check_64to32_Cast(it->sender,senderid))
			continue;
		msg.srcroleid=senderid;
		msg.src_name = it->sendername;
		msg.emotion = it->emotiongroup;
		msg.msg = it->content;
		msg.time = it->time;
		offchat.messages.push_back(msg);
	}
	GameGateClient::GetInstance()->SendProtocol(offchat);
	return 0;
}

int GTManager::OnGTFactionMsg(int64_t factionid,int ftype,const RoleMsgBean& msg)
{
	int fid,senderid;
	if(!Check_64to32_Cast(factionid,fid) || !Check_64to32_Cast(msg.sender,senderid))
		return -1;
	if(!global_faction_chat_limit.Check())
	{
	//	DEBUG_PRINT("OnGTFactionMsg too many global chats roleid=%d",senderid);
		return -1;
	}
	if(!faction_chat_limit.Check(senderid))
	{
	//	DEBUG_PRINT("OnGTFactionMsg too many chats roleid=%d",senderid);
		global_faction_chat_limit.Rollback();
		return -1;
	}
	//did not use gt's factionid
	GateFactionChat chat;
	switch(ftype)
	{
	case FACTIONTYPE:
		chat.channel = GP_CHAT_FACTION;
		break;
	case FAMILYTYPE:
		chat.channel = GP_CHAT_FAMILY;
		break;
	default:
		return -1;
		break;
	}

	chat.emotion = msg.emotiongroup;
	chat.src = senderid;
	chat.msg = msg.content;
	chat.fid = fid;
	GameGateClient::GetInstance()->SendProtocol(chat);
	return 0;
}

int GTManager::SyncGameOnlineTOGT()
{
	std::vector<int> rolelist;
	char finish;
	RoleStatusResp msg;
	msg.localrid=0;
	int startrole =0;
	while( RoleContainer::GetInstance().GetOnlineRolelist(startrole,MAX_RECORD_NUM_IN_OUTGOING_ROLESTATUSRESP,rolelist,finish)==0)//modify finish
	{
		std::vector<int>::iterator it,ite=rolelist.end();
		for(it=rolelist.begin();it!=ite;++it)
		{
			msg.rolestatus.insert(std::make_pair(*it,RoleStatusBean(STATUS_ONLINE_GAME)));
		}
		GTPlatformClient::GetInstance()->SendProtocol(msg);
		if(finish == 1)
			break;
		if(msg.rolestatus.size())
			startrole = msg.rolestatus.rbegin()->first;
		msg.rolestatus.clear();
		rolelist.clear();
	}
//	OpenGT();
	return 0;
}

int GTManager::OnGamePrivateChat(const PrivateChat& chat)
{
//	if (chat.channel != CHANNEL_NORMAL || chat.route != FROM_GAME)
	if (chat.route != FROM_GAME)
	{
		DEBUG_PRINT("PrivateChat: channel=%d route=%d err", chat.channel,chat.route);
		return -1;
	}
	if(!GTOnline(chat.dstroleid))
	{
		GateUpdateStatus re;
		char tmp = false;
		re.friendstatus = GGateRoleGTStatus(chat.dstroleid,tmp);
		re.rolelist.push_back(chat.srcroleid);
		GameGateClient::GetInstance()->SendProtocol(re);
		return 0;
	}
	RoleMsg msg;
	msg.receiver = chat.dstroleid;
	RoleMsgBean& mbean= msg.message;
	mbean.sender = chat.srcroleid;
	mbean.sendername = chat.src_name;
	mbean.emotiongroup = chat.emotion;// is this right?
	mbean.content = chat.msg;
	mbean.time = Timer::GetTime();
	GTPlatformClient::GetInstance()->SendProtocol(msg);

	return 0;
}

int GTManager::OnGameFactionChat(const GateFactionChat& chat)
{
	int ftype;
	switch(chat.channel)
	{
	case GP_CHAT_FACTION:
		ftype = FACTIONTYPE;
		break;
	case GP_CHAT_FAMILY:
		ftype = FAMILYTYPE;
		break;
	default:
		DEBUG_PRINT("FactionChat: channel=%d err sender=%d", chat.channel,chat.src);
		return -1;
		break;
	}
	FactionMsg msg;
	msg.factionid = FactionIDBean(ftype,chat.fid);
	RoleMsgBean& mbean= msg.message;
	mbean.sender = chat.src;
	mbean.emotiongroup = chat.emotion;// is this right?
	mbean.content = chat.msg;
	mbean.time = Timer::GetTime();
	GTPlatformClient::GetInstance()->SendProtocol(msg);
	return 0;
}

int GTManager::OnRoleActivation(int64_t gtroleid,unsigned char operation)
{
	int roleid;
	if(!Check_64to32_Cast(gtroleid,roleid))
		return -1;
	Rpc* rpc;
	switch(operation)
	{
	case OP_ACTIVATE://DBGateGTActivate*
		rpc=Rpc::Call(
			RPC_DBGATEACTIVATE,
			DBGateActivateArg(roleid,ACTIVATE_GT)
			);
		break;
	case OP_DEACTIVATE://DBGateGTDeactivate*
		rpc=Rpc::Call(
			RPC_DBGATEDEACTIVATE,
			DBGateActivateArg(roleid,ACTIVATE_GT)
			);
		break;
	default:
		return -1;
		break;
	}
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return 0;
}

int GTManager::OnGameGateDisconnect()
{
	GTPlatformClient::GetInstance()->OnGameGateDisconnect();
	return 0;
}

int GTManager::OnGameGateSync()
{
	GTPlatformClient::GetInstance()->OnGameGateSync();
	return 0;
}

bool GTManager::Update()
{
	return true;
}

};
