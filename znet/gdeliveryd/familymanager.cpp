#include "roleid"
#include "factionid"
#include "log.h"
#include "familymanager.h"
#include "dbfactionget.hrp"
#include "dbfamilysync.hrp"
#include "dbfamilyappoint.hrp"
#include "dbfactionget.hrp"
#include "dbfamilyget.hrp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "mapuser.h"
#include "maplinkserver.h"
#include "factionmanager.h"
#include "factionrecruit_re.hpp"
#include "factionannounce_re.hpp"
#include "factiondismiss_re.hpp"
#include "postdeletefamily.hrp"
#include "uniquenameclient.hpp"
#include "factionexpel_re.hpp"
#include "factionleave_re.hpp"
#include "factionnickname_re.hpp"
#include "factionappoint_re.hpp"
#include "syncplayerfaction.hpp"
#include "syncfamilydata.hpp"
#include "sendasyncdata.hpp"
#include "familyexpshare.hpp"
#include "familyrecord.hpp"
#include "factionchat.hpp"
#include "dbundeletefamily.hrp"
#include "gtplatformagent.h"
//#include "factionnamechange.hpp"
#include "gchangefactionname_re.hpp"
#include "factionmultiexpsync.hpp"
#include "tsplatformagent.h"
namespace GNET
{
char FamilyInfo::Packer::version = 0;

extern double GetExpShare(int64_t exp, int self_level, int peer_level, float factor);

void ObsoleteFactionCache(FactionDetailInfo *faction)
{
	if (faction)
	{
		faction->ObsoleteCache();
	}
}

void SendExpShare(int gameid, int familyid, int roleid, int64_t exp)
{
//	LOG_TRACE("FamilyExpShare: gameid=%d, exp=%d, roleid=%d, familyid=%d", gameid, exp, roleid, familyid );
	if(exp>40000)
		exp = 40000;
	GProviderServer::GetInstance()->DispatchProtocol(gameid, FamilyExpShare(roleid, familyid,0,exp));
}

int GetMaxAbility(int level)
{
	static int _max[] = { 0, 100, 400, 1000, 2000, 4000, 6000, 9000, 12000, 17000, 22000, 29000, 37000, 50000, 72000 }; 
	return _max[level%16];
}

const Marshal::OctetsStream& operator >> (const Marshal::OctetsStream & os, const TaskPairData &x) 
{
	TaskPairData & ref = remove_const(x);
	return os >> ref.key >> ref.value1 >> ref.value2;
}
Marshal::OctetsStream& operator << (Marshal::OctetsStream &os, const TaskPairData &x) 
{
	return os << x.key << x.value1 << x.value2; 
}

void FamilyInfo::Packer::Pack(FamilyInfo & info, Octets& dst)
{
	Marshal::OctetsStream os;
	try
	{
		switch (version)
		{
		case 0:
			os << version << info.task_done << info.task_doing << info.task_counter;
			break;
		}
	}
	catch (...)
	{
		Log::log(LOG_ERR, "Pack data err, familyid=%d", info.info.id);
	}
	dst = os;
}

void FamilyInfo::Packer::Unpack(const FamilyInfo & info, Octets & src)
{
	if (src.size() <= 13)
		return;
	Marshal::OctetsStream os(src);
	try
	{
		char ver;
		os >> ver;
		switch (ver)
		{
		case 0:
			os >> info.task_done >> info.task_doing >> info.task_counter;
			break;
		}
	}
	catch (...)
	{
		Log::log(LOG_ERR, "Unpack data err, familyid=%d", info.info.id);
	}
}

/////////////////////////////////// FamilyInfo  //////////////////////////////////////
void FamilyInfo::Update(bool &needsync, bool isbatch)
{
	DBFamilySyncArg arg;
        arg.fid = info.id;
	bool isdirty = dirty;
	{       
                UserContainer& container = UserContainer::GetInstance();
                Thread::RWLock::RDScoped l(container.GetLocker());
                PlayerInfo * pinfo;
                for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
                {
                        if(it->gameid>=0)
                        {
                                pinfo = container.FindRoleOnline(it->role.rid);
                                if (pinfo)
                                {
                                        if(it->level != pinfo->level)
                                               it->level = pinfo->level;
                                        if(it->occupation != pinfo->occupation)
                                               it->occupation = pinfo->occupation;
                                }
                        }
                        if(it->dirty || it->role.level != it->level || it->role.occupation != it->occupation)
                        {
				isdirty = true;
                                it->dirty = false;
                                it->role.level = it->level;
                                it->role.occupation = it->occupation;
                        }
			arg.member.push_back(it->role);
                }
        }

	if(isdirty)
	{
		needsync = true;
		dirty = false;
		arg.skills = info.skills;
		Packer::Pack(*this, arg.task_data);
		LOG_TRACE("Family: sync member info to db, family=%d, size=%d, task_data=%d", 
			info.id, arg.member.size(), arg.task_data.size());
		arg.task_record = info.task_record;
		DBFamilySync* rpc = (DBFamilySync*) Rpc::Call( RPC_DBFAMILYSYNC,arg);
		rpc->isbatch = isbatch;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}

	time_t now = Timer::GetTime();
	if ((now%86400) < (updatetime%86400))
		expel_count = 0;
	updatetime = now;
}

void FamilyInfo::JoinFaction(FactionDetailInfo * pf)
{
	if (info.deletetime > 0)
	{
		Log::log(LOG_ERR, "family %d join faction %d, but already marked delete", info.id, pf->info.fid);
		DBUndeleteFamily* rpc = (DBUndeleteFamily*) Rpc::Call( RPC_DBUNDELETEFAMILY, FamilyId(info.id));
		rpc->isclient = false;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	info.factionid = pf->info.fid;
	info.jointime = Timer::GetTime();
	RegFaction(pf);

	ObsoleteFactionCache(pfaction);

	RoleContainer::iterator it = list.begin();
        for (; it != list.end(); ++ it)
        {
		it->role.nickname.clear();
		if (info.master == it->role.rid)
		{
			if (pf->info.master == it->role.rid)
			{
				it->role.title = TITLE_MASTER;
				UpdatePlayerFaction(it->role.rid, pf->info.fid, info.id, TITLE_MASTER, pf->info.name);
			}
			else
			{
				it->role.title = TITLE_HEADER;
				UpdatePlayerFaction(it->role.rid, pf->info.fid, info.id, TITLE_HEADER, pf->info.name);
			}
		}
		else
			UpdatePlayerFaction(it->role.rid, pf->info.fid, info.id, TITLE_MEMBER, pf->info.name);
        }
}

void FamilyInfo::OnFactionDelete()
{
        FactionDismiss_Re re(ERR_SUCCESS, info.factionid, 0, 0, 1);

	info.factionid = 0;
	UnregFaction(pfaction);

	RoleContainer::iterator it = list.begin();
        for (; it != list.end(); ++ it)
        {
		it->role.contribution = 0;
		if(it->role.title != TITLE_MEMBER) 
			it->role.title = TITLE_HEADER;
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(it->role.rid);
		if (pinfo)
		{
			pinfo->factionid = 0;
			if(pinfo->factiontitle != TITLE_MEMBER)
				pinfo->factiontitle = TITLE_HEADER;
			re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, re);
			UpdatePlayerFaction(pinfo, it->role.rid, 0, pinfo->familyid, pinfo->factiontitle, Octets());
		}
        }
	contribution_cache = -1;
}

void FamilyInfo::LeaveFaction()
{
	info.factionid = 0;
	info.jointime = Timer::GetTime();
	ObsoleteFactionCache(pfaction);
	UnregFaction(pfaction);

	RoleContainer::iterator it = list.begin();
        for (; it != list.end(); ++ it)
        {
		it->role.contribution = 0;
		if (it->role.rid == info.master)
		{
			it->role.title = TITLE_HEADER;
			UpdatePlayerFaction(it->role.rid, 0, info.id, TITLE_HEADER, Octets());
		}
		else
		{
			it->role.title = TITLE_MEMBER;
			UpdatePlayerFaction(it->role.rid, 0, info.id, TITLE_MEMBER, Octets());
		}
        }
	contribution_cache = -1;
}

void FamilyInfo::OnDelete()
{
	if (info.factionid)
	{
		Log::log(LOG_ERR, "family %d send delete request, but still in faction %d", info.id, info.factionid);
		if (pfaction)
		{

			ObsoleteFactionCache(pfaction);
			pfaction->OnFamilyDismiss(this);
			UnregFaction(pfaction);
		}
	}
	info.factionid = 0;

        FactionDismiss_Re re(ERR_SUCCESS, info.id, 0, 1, 1);
        Broadcast(re, re.localsid);

	RoleContainer::iterator it = list.begin();
        for (; it != list.end(); ++ it)
		UpdatePlayerFaction(it->role.rid, 0, 0, 0, Octets());
}

void FamilyInfo::Broadcast(Protocol& data, unsigned int &localsid)
{
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	access_num++;
        for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
        {
		if(it->gameid<0)
			break;
                localsid = it->localsid;
                dsm->Send(it->linksid,data);
        }
}

void FamilyInfo::BroadcastGS(Protocol& data, int & roleid)
{
	GProviderServer* psm = GProviderServer::GetInstance();
	access_num++;
        for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
        {
		if (it->IsOnline())
		{
			roleid = it->role.rid;
			psm->DispatchProtocol(it->gameid, data);
		}
        }
}

void FamilyInfo::SendTaskRecord(int sid, int localsid)
{
	FamilyRecord rec(info.id, 0, star, info.task_record, localsid);
	GDeliveryServer::GetInstance()->Send(sid, rec);
}

bool FamilyManager::GetTaskRecord(int fid,int& star, std::vector<int>& list)
{
	Map::iterator ic = families.find(fid);
        if(ic==families.end())
                return false;

	star = ic->second->star;
	list = ic->second->info.task_record;
	return true;
}

bool FamilyInfo::IsSilent()
{
	LOG_TRACE("FamilyManager: issilent fid= %d online=%d, access_num=%d", info.id, online, access_num);
	return !online && !access_num;
}
////////////////////////////////// FamilyManager ///////////////////////////////////

FamilyManager::~FamilyManager()
{
	for(Map::iterator it=families.begin(),ie=families.end();it!=ie;++it)
		delete it->second;
	families.clear();
}

bool FamilyManager::Initialize()
{
	IntervalTimer::Attach(this,500000/IntervalTimer::Resolution());
	open = true;
	return true;
}
FamilyInfo * FamilyManager::Find(unsigned int familyid)
{
	if (familyid == 0)
		return NULL;
	Map::iterator it = families.find(familyid);
	if (it != families.end())
		return it->second;
	return NULL;
}

unsigned int FamilyManager::GetFactionId(unsigned int familyid)
{
	if (familyid == 0)
		return 0;
	Map::iterator it = families.find(familyid);
	if (it == families.end())
		return 0;
	return it->second->info.factionid;
}

bool FamilyManager::Update()
{
	time_t now = Timer::GetTime();
	Map::iterator it = families.upper_bound(cursor);
	if(it==families.end())
	{
		cursor = 0;
		return true;
	}
	cursor = it->second->info.id;
	
	if (NULL == it->second->pfaction)
	{
		if(now - it->second->updatetime > UPDATE_INTERVAL)
		{
			bool needsync = false;
			it->second->Update(needsync);
			if(it->second->IsSilent())
			{
				if (it->second->DeleteCheck())
				{
					delete it->second;
					families.erase(it);
					LOG_TRACE("FamilyManager: remove timeout family, familyid=%d, mapsize=%d", 
						cursor, families.size());
					return true;
				}
				else
				{
					LOG_TRACE("FamilyManager: delete check failed, familyid=%d", cursor);
				}
			}
			it->second->access_num = 0;
		}
	}
	return true;
}

void FamilyManager::OnCreate(GFamily& info)
{
	FamilyInfo*  detail = new FamilyInfo(info);
	families[info.id] = detail;
	detail->IncOnline();
	std::list<FamilyRole>::iterator it = detail->list.begin();
	Log::formatlog("FamilyManager OnCreate", "familyid=%d", info.id);
	if(it!=detail->list.end())
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(it->role.rid);
                if ( !pinfo)
		{
			Log::log(LOG_ERR, "FamilyManager: err, pinfo=%p,rid=%d,status=%d",pinfo,it->role.rid,pinfo?pinfo->user->status:0);
			return;
		}
		GRoleInfo *prole = RoleInfoCache::Instance().Get(it->role.rid);
		if (prole == NULL)
		{
			Log::log(LOG_ERR, "FamilyManager: err, roleinfo not in cache rid=%d",it->role.rid);
			return;
		}
		it->SetOnline(pinfo->gameid, pinfo->linksid, pinfo->localsid, prole->reborn_cnt, prole->cultivation, prole->fac_coupon_add);
		pinfo->familyid = info.id;
		pinfo->factiontitle = TITLE_HEADER;
		UpdatePlayerFaction(pinfo, pinfo->roleid, 0, info.id, TITLE_HEADER, Octets());

		SyncFamilyData data;
		data.roleid = pinfo->roleid;
		data.familyid = info.id;
		data.skills = info.skills;
		for(std::vector<GFamilySkill>::iterator it=data.skills.begin(),ie=data.skills.end();it!=ie;++it)
		{
			it->id += FAMILYSKILL_BASE;
		}
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,data);
	        GFolk user(pinfo->roleid, prole->name, Octets(), prole->level, TITLE_HEADER, prole->occupation, 0, 0, 0);
		GTPlatformAgent::FamilyAddMember(info.id,user,prole->reborn_cnt,prole->cultivation,prole->gender,0);
		//TSPlatformAgent::AddFamily(user.rid, info.id);
	}
}

void FamilyManager::OnLoad(GFamily& family)
{
	//name2id[family.name] = family.id;
	Map::iterator it = families.find(family.id);
	if (it != families.end())
	{
		DEBUG_PRINT("family found familyid=%d",family.id);
		return;
	}
	unsigned char title = TITLE_MEMBER;
        GFolkVector::iterator im = family.member.begin();
        for(GFolkVector::iterator it=im,ie=family.member.end();it!=ie;++it)
        {       
                if(it->title<title)
                {
                        im = it;
                        title = it->title; 
                }
        }               
        if(im!=family.member.end() && family.master!=im->rid)
        {               
                Log::log(LOG_ERR,"Assign family master, familyid=%d,predecessor=%d,roleid=%d",family.id,family.master,im->rid);
                SyncMaster(family.id, im->rid, *im);
        }     

	FamilyInfo * pfamily = new FamilyInfo(family);
	families.insert(std::make_pair(family.id, pfamily));
	pfamily->access_num = 1;
	if (family.factionid)
	{
		FactionDetailInfo * faction = FactionManager::Instance()->Find(family.factionid);
		if (!faction)
		{
			DEBUG_PRINT("familymanager onload, factionid=%d, info not found, familyid=%d",family.factionid, family.id);
			DBFactionGet* rpc = (DBFactionGet*) Rpc::Call( RPC_DBFACTIONGET,FactionId(family.factionid));
			rpc->roleid = 0;
			rpc->listener = NULL;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			return;
		}

		ObsoleteFactionCache(faction);

		pfamily->RegFaction(faction);	
		faction->RegFamily(pfamily);
	}

}

bool FamilyManager::GetBaseInfo(int fid, GFactionBaseInfo& info)
{
	Map::iterator it = families.find(fid);
        if(it==families.end())
                return false;
        info.fid = it->second->info.id;
        info.name = it->second->info.name;
        info.level = it->second->star;
        it->second->access_num++;
        return true;
}

void FamilyManager::OnLogin(int roleid,int factionid,char title,int familyid,
		int contribution,int devotion,int gameid,int linksid,int localsid )
{
	if (!open)
		return;
	if (!familyid)
		return;

	FamilyContainer::iterator ic = families.find(familyid);
	if (ic == families.end())
	{
		LOG_TRACE("FamilyManager::OnLogin:familyid=%d not found", familyid);
		DBFamilyGet * rpc = (DBFamilyGet*) Rpc::Call(RPC_DBFAMILYGET, FamilyId(familyid));
		rpc->roleid = roleid;
		rpc->sendclient = false;
		rpc->logining = true;
		rpc->contribution = contribution;
		rpc->devotion = devotion;
		rpc->factionid = factionid;
		rpc->factiontitle = title;
		GameDBClient::GetInstance()->SendProtocol(rpc);	
		return;
	}
	LOG_TRACE("FamilyManager::OnLogin: roleid=%d,faction=%d,family=%d,title=%d,contr=%d,"\
			"devotion=%d,gameid=%d,linksid=%d,localsid=%d",
			roleid,factionid,familyid,title,contribution,devotion,gameid,linksid,localsid);

	ObsoleteFactionCache(ic->second->pfaction);

	RoleContainer & list = ic->second->list;
	for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
	{
		if(it->role.rid==(unsigned int)roleid && it->gameid!=gameid)
		{
			if (it->gameid == -1) ic->second->IncOnline();
			if(contribution>0 && it->role.contribution!=contribution)
			{
				it->dirty = true;
				it->role.contribution = contribution;
				ic->second->contribution_cache = -1;
			}
			if (devotion>0 && it->role.devotion!=devotion)
			{
				it->dirty = true;
				it->role.devotion = devotion;
			}
			int reborn = -1;
			short cult = -1;
			int64_t fac_coupon_add = 0;
			GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
			if (prole)
			{
				reborn = prole->reborn_cnt;
				cult = prole->cultivation;
				fac_coupon_add = prole->fac_coupon_add;
			}
			list.insert(list.begin(),*it)->SetOnline(gameid,linksid,localsid,reborn,cult,fac_coupon_add);
			list.erase(it);
			if(roleid==(int)ic->second->info.master && title==TITLE_MEMBER)
			{
				Log::log(LOG_ERR,"Assign family master, familyid=%d,predecessor=%d,roleid=%d",
						familyid, roleid, roleid);
				SyncMaster(familyid, roleid, it->role); 
			}
			break;
		}
	}

}
void FamilyManager::OnLogout(int roleid, int factionid, int familyid, int level, int occupation)
{
	if (!open)
		return;
	Map::iterator ic = families.find(familyid);
        if(ic==families.end())
                return;

	ObsoleteFactionCache(ic->second->pfaction);

        RoleContainer& list = ic->second->list;
        for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
        {
                if(it->role.rid==(unsigned int)roleid && it->gameid>=0)
                {
                        ic->second->DecOnline();
                        it->level = level;
                        it->occupation = occupation;
                        list.insert(list.end(),*it)->SetOffline();
                        list.erase(it);
                        break;
                }
        }
}

int FamilyManager::OnRecruit(int familyid, Octets& familyname)
{
	Map::iterator it = families.find(familyid);
        if(it==families.end())
                return 2;
	if(it->second->list.size() >= MAX_FAMILY_MEMBER_NUMBER)
		return 1;
        familyname = it->second->info.name;
        return 0;
}

void FamilyManager::OnJoin(int fid, int inviter, int invitee, int level, int occupation, Octets& name)
{
        Map::iterator ic = families.find(fid);
        if(ic==families.end())
                return;

	ObsoleteFactionCache(ic->second->pfaction);

	int gameid, linksid, localsid, reborn;
	unsigned char gender;
	short cult;
	int64_t fac_coupon_add = 0;
        {
                Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
                PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(invitee);
		GRoleInfo *prole = RoleInfoCache::Instance().Get(invitee);
                if ( !pinfo || pinfo->familyid || !prole)
                        return;
		Octets facname = ic->second->pfaction != NULL ? ic->second->pfaction->info.name : Octets();
		::UpdatePlayerFaction(pinfo, invitee, ic->second->info.factionid, fid, TITLE_MEMBER, facname);
                gameid = pinfo->gameid;
                linksid = pinfo->linksid;
                localsid = pinfo->localsid;
		reborn = prole->reborn_cnt;
		cult = prole->cultivation;
		gender = prole->gender;
		fac_coupon_add = prole->fac_coupon_add;

		SyncFamilyData data;
		data.roleid = invitee;
		data.familyid = fid;
		data.skills = ic->second->info.skills;
		for(std::vector<GFamilySkill>::iterator it=data.skills.begin(),ie=data.skills.end();it!=ie;++it)
		{
			it->id += FAMILYSKILL_BASE;
		}
		GProviderServer::GetInstance()->DispatchProtocol(gameid,data);
		ic->second->SendTaskRecord(pinfo->linksid, pinfo->localsid);

		int multi_exp = 0, end_time = 0;
		if (ic->second->pfaction != NULL &&
				ic->second->pfaction->GetMultiExp(multi_exp, end_time))
		{
			FactionMultiExpSync sync(invitee, multi_exp, end_time);
			GProviderServer::GetInstance()->DispatchProtocol(gameid, sync);
		}
	}

        GFolk user(invitee, name, Octets(), level, TITLE_MEMBER, occupation, 0, Timer::GetTime(), 0);
	GTPlatformAgent::FamilyAddMember(fid,user,reborn,cult,gender,ic->second->info.factionid);
	//TSPlatformAgent::AddFamily(user.rid, fid);
	std::list<FamilyRole>& list = ic->second->list;
        list.insert(list.begin(),FamilyRole(user))->SetOnline(gameid,linksid,localsid,reborn,cult,fac_coupon_add);
        ic->second->IncOnline();

        FactionRecruit_Re re(0, fid, invitee, inviter, level, occupation, name, 1, 0);
        ic->second->Broadcast(re, re.localsid);

	if (ic->second->info.factionid)
	{
		FAC_DYNAMIC::member_change log = {FAC_DYNAMIC::member_change::MEMBER_JOIN, 20, {}, 20, {} };
		FactionDynamic::GetName(name, log.rolename, log.rolenamesize);
		FactionDynamic::GetName(ic->second->info.name, log.familyname, log.familynamesize);
		FactionManager::Instance()->RecordDynamic(ic->second->info.factionid, FAC_DYNAMIC::MEMBER_CHANGE, log);
	}
}

int FamilyManager::CanDismiss(int fid)
{
	Map::iterator ic = families.find(fid);
        if(ic==families.end())
		return ERR_PERMISSION_DENIED;
        if(ic->second->info.deletetime) 
                return ERR_FC_DISMISSWAITING;
        return 0;;
}

bool FamilyManager::CanLeave(int roleid, int familyid)
{
	Map::iterator ic = families.find(familyid);
	if(ic==families.end())
		return false;

	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pinfo || pinfo->familyid != (unsigned int)familyid || pinfo->factiontitle!=TITLE_MEMBER)
		return false;
	return true;
}

void FamilyManager::OnDelete(int fid)
{
	Map::iterator ic = families.find(fid);
        if (ic == families.end())
                return;
        FamilyInfo* family = ic->second;
	family->OnDelete();

	//name2id.erase(family->info.name);
	/*
	PostDeleteFamilyArg arg(GDeliveryServer::GetInstance()->zoneid,fid,family->info.name);
        PostDeleteFamily* rpc = (PostDeleteFamily*) Rpc::Call(RPC_POSTDELETEFAMILY, arg);
        UniqueNameClient::GetInstance()->SendProtocol(rpc);
	*/
	delete family;
	families.erase(ic);
}

void FamilyManager::OnLeave(int fid, int roleid, int master)
{
	::UpdatePlayerFaction(roleid, 0, 0, 0, Octets());


	Map::iterator ic = families.find(fid);
        if(ic==families.end())
                return;

	ObsoleteFactionCache(ic->second->pfaction);
	GTPlatformAgent::FamilyDelMember(fid,roleid,ic->second->info.factionid);
	//TSPlatformAgent::LeaveFamily(roleid, fid);

        RoleContainer& list = ic->second->list;
        for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
        {
                if(it->role.rid==(unsigned int)roleid)
                {
			if(master)
			{
				ic->second->expel_count++;
				ic->second->contribution_cache = -1;
				FactionExpel_Re re(0, master, fid, roleid, it->role.name, 1, 0);
				ic->second->Broadcast(re, re.localsid);
			}
			else
			{
				ic->second->contribution_cache = -1;
				FactionLeave_Re re(0, roleid, fid, it->role.name, 1,0);
				ic->second->Broadcast(re, re.localsid);
			}
                        if(it->gameid>=0)
                                ic->second->DecOnline();
			if (ic->second->pfaction)
			{
				ic->second->pfaction->OnRoleLeaveFamily(roleid, it->role.contribution);
			}
			if (ic->second->info.factionid)
			{
				FAC_DYNAMIC::member_change log = {FAC_DYNAMIC::member_change::MEMBER_LEAVE, 20, {}, 20, {} };
				FactionDynamic::GetName(it->role.name, log.rolename, log.rolenamesize);
				FactionDynamic::GetName(ic->second->info.name, log.familyname, log.familynamesize);
				FactionManager::Instance()->RecordDynamic(ic->second->info.factionid, FAC_DYNAMIC::MEMBER_CHANGE, log);
			}
			int multi_exp = 0, end_time = 0;
			if (ic->second->pfaction != NULL &&
					ic->second->pfaction->GetMultiExp(multi_exp, end_time))
			{
				FactionMultiExpSync sync(roleid, 0, 0);
				GProviderServer::GetInstance()->DispatchProtocol(it->gameid, sync);
			}

                        list.erase(it);
                        break;
                }
        }
        return;
}

void FamilyManager::SetAnnounce(int fid, int roleid, Octets & announce)
{
	Map::iterator ic = families.find(fid);
	if (ic == families.end())
		return;
	ic->second->info.announce = announce;
	FactionAnnounce_Re re(0,roleid,announce,0,1);
	GTPlatformAgent::FactionUpdate(FAMILYTYPE,fid,announce,FAC_UPDATE_ANNOUNCE);
	ic->second->Broadcast(re, re.localsid);
}
void FamilyManager::OnAppoint(int fid, int appointer, int appointee, char title)
{
	Map::iterator ic = families.find(fid);
        if(ic==families.end())
                return;
	ic->second->OnAppoint(appointer, appointee, title);
}
void FamilyInfo::OnAppoint(int appointer, int appointee, char title)
{
        int sum = 2;
        for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie&&sum;++it)
        {       
                if(it->role.rid==(unsigned int)appointee)
                {       
                        sum--;
			it->role.title = TITLE_HEADER;
                }else if(it->role.rid==(unsigned int)appointer)
                {       
                        sum--;
                        it->role.title = TITLE_MEMBER;
                }
        }
	Octets facname = pfaction != NULL ? pfaction->info.name : Octets();
	::UpdatePlayerFaction(appointer, info.factionid, info.id, TITLE_MEMBER, facname);	
	::UpdatePlayerFaction(appointee, info.factionid, info.id, TITLE_HEADER, facname);	
	GTPlatformAgent::FactionModMember(FAMILYTYPE,info.id,appointer,TITLE_MEMBER);
	GTPlatformAgent::FactionModMember(FAMILYTYPE,info.id,appointee,TITLE_HEADER);
	if(info.factionid)
	{
		GTPlatformAgent::FactionModMember(FACTIONTYPE,info.factionid,appointer,TITLE_MEMBER);
		GTPlatformAgent::FactionModMember(FACTIONTYPE,info.factionid,appointee,TITLE_HEADER);
	}

	info.master = appointee;

	FactionAppoint_Re re(0, appointer, appointee, title, 0, 1);
	Broadcast(re, re.localsid);

	SyncFamilyData data;
	data.familyid = info.id;
	for(std::vector<GFamilySkill>::iterator is=info.skills.begin(),ie=info.skills.end();is!=ie;++is)
	{
		if(is->level==1)
			is->ability = 0;
		else if(is->level>1)
			is->ability = GetMaxAbility(is->level-2);
		if(is->level>1)
		{
			is->level--;
			data.skills.push_back(GFamilySkill(is->id+FAMILYSKILL_BASE, is->level, is->ability, 0));
		}
	}
	if(data.skills.size())
	{
		for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
		{
			if(it->IsOnline())
			{
				data.roleid = it->role.rid;
				GProviderServer::GetInstance()->DispatchProtocol(it->gameid,data);
			}
		}
		dirty = true;
	}
}

void FamilyManager::OnDeleteRole(int fid, int roleid)
{
	Map::iterator ic = families.find(fid);
        if(ic==families.end())
                return;

	ObsoleteFactionCache(ic->second->pfaction);

        RoleContainer & list = ic->second->list;
        for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
        {
                if(it->role.rid==(unsigned int)roleid)
                {
                        if(it->gameid>=0)
                                ic->second->DecOnline();
			if (ic->second->pfaction)
			{
				ic->second->pfaction->OnDeleteRole(roleid, it->role.contribution);
			}
                        list.erase(it);
                        break;
                }
        }
        return;
}

bool FamilyManager::SetNickname(int fid, int roleid, int receiver, Octets & nickname, GFolk & info)
{
	Map::iterator ic = families.find(fid);
        if(ic==families.end())
                return false;

        std::list<FamilyRole>& list = ic->second->list;
        std::list<FamilyRole>::iterator it=list.begin(), ie=list.end();
        for(;it!=ie;++it)
        {
                if(it->role.rid==(unsigned int)receiver)
                {
                        it->role.nickname = nickname;
			it->dirty = true;
                        info = it->role;
                        FactionNickname_Re  re(0,roleid,receiver,nickname,0);
                        FactionManager::Instance()->Broadcast(ic->second->info.factionid, re, re.localsid);
                        return true;
                }
        }
        return false;
}
void FamilyManager::Sync2Game(int fid, int roleid, int contribution, int devotion)
{
	Map::iterator ic = families.find(fid);
	if(ic==families.end())
		return ;
        std::list<FamilyRole>& list = ic->second->list;
        std::list<FamilyRole>::iterator it=list.begin(), ie=list.end();
	for(;it!=ie;++it)
        {
                if(it->role.rid==(unsigned int)roleid)
                {
                        if(it->role.contribution!=contribution)
                        {
                                it->dirty = true;
                                it->role.contribution = contribution;
				ic->second->contribution_cache = -1;
                        }
			if(it->role.devotion != devotion)
			{
				it->dirty = true;
				it->role.devotion = devotion;
			}
                        break;
                }
        }
        return;
}
void FamilyManager::SyncMaster(int fid, int roleid, GFolk& info)
{
	DBFamilyAppointArg arg(fid, roleid, TITLE_HEADER) ;
        DBFamilyAppoint* rpc = (DBFamilyAppoint*) Rpc::Call( RPC_DBFAMILYAPPOINT,arg);
        rpc->roleid = roleid;
        GameDBClient::GetInstance()->SendProtocol(rpc);
}

void FamilyManager::GetMember(int fid, FactionList_Re & list, char init)
{
	Map::iterator ic = families.find(fid);
	if (ic == families.end())
		return;
	
	list.familyid = fid;
	list.online = ic->second->GetOnline();
	list.population = ic->second->list.size();
	list.prosperity = 0;
	if (ic->second->info.deletetime != 0)
		list.deletetime = ic->second->info.deletetime + 7*86400;
	else
		list.deletetime = 0;

	if(init)
		list.announce = ic->second->info.announce;
	std::list<FamilyRole>& roles = ic->second->list;
	std::list<FamilyRole>::iterator it = roles.begin(), ie = roles.end();
	for (; it != ie; ++it)
	{
		list.members.add(MemberInfo(it->role.rid, it->role.level, it->role.occupation, it->role.title, 
			it->gameid, fid, it->role.contribution, it->role.devotion, it->role.name, it->role.nickname, it->reborn_cnt, it->cultivation, it->fac_coupon_add));
	}
	std::vector<GFamilySkill>& skills = ic->second->info.skills;
	std::vector<GFamilySkill>::iterator sit = skills.begin(), sie = skills.end();
	for (; sit != sie; ++sit)
		list.familyskill.push_back(FamilySkill(sit->id+FAMILYSKILL_BASE, sit->ability));
}

bool FamilyManager::CanExpel(int roleid, int familyid, int expellee)
{
	if(!familyid || roleid==expellee)
		return false;
	Map::iterator ic = families.find(familyid);
	if(ic==families.end())
		return false;
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( !pinfo )
			return false;
		if (pinfo->familyid!=(unsigned int)familyid || pinfo->factiontitle == TITLE_MEMBER )
			return false;
	}
	std::list<FamilyRole>& roles = ic->second->list;
	std::list<FamilyRole>::iterator it = roles.begin(), ie = roles.end();
	for (; it != ie; ++it)
	{
		if(it->role.rid==(unsigned int)expellee)
			return true;
	}
	return false;
}
bool FamilyManager::IsMember(int familyid, unsigned int roleid)
{
	Map::iterator ic = families.find(familyid);
	if(ic==families.end())
		return false;
	std::list<FamilyRole>& roles = ic->second->list;
	std::list<FamilyRole>::iterator it = roles.begin(), ie = roles.end();
	for (; it != ie; ++it)
	{
		if(it->role.rid==roleid)
			return true;
	}
	return false;
}
void FamilyManager::SetDelete(int fid, time_t deletetime)
{
	Map::iterator ic = families.find(fid);
	if(ic==families.end())
		return;
	ic->second->info.deletetime = deletetime;

	if (deletetime > 0)  // delete family 
	{       
		FactionDismiss_Re re(ERR_FC_PREDELSUCCESS, fid, 0, 1, 1);
		ic->second->Broadcast(re, re.localsid);
	}               
	else // undelete family 
	{       
		FactionDismiss_Re re(ERR_SUCCESS, fid, 0, 1, 0);
		ic->second->Broadcast(re, re.localsid);
	}
}
bool FamilyManager::SyncGameOnLogin(int fid, std::vector<GFamilySkill>& list, int linksid, int localsid)
{
	Map::iterator ic = families.find(fid);
	if(ic==families.end())
		return false;
	list = ic->second->info.skills;
	for(std::vector<GFamilySkill>::iterator it=list.begin(),ie=list.end();it!=ie;++it)
	{
		it->id += FAMILYSKILL_BASE;
	}
	ic->second->SendTaskRecord(linksid, localsid);
	return true;
}

void FamilySaver::DoSave()
{
	if (!isWork)
		return;
	FamilyContainer & families = FamilyManager::Instance()->families;
	FamilyContainer::iterator it = families.upper_bound(curcor);
	while (it != families.end())
	{
		if (NULL == it->second->pfaction)
		{
			curcor = it->first;
			bool needsync = false;	
			it->second->Update(needsync, true);
			if (needsync) return;
		}
		++it;

	}
	isWork = false;
}

void FamilyManager::Save2DB()
{
	saver.Reset();
	saver.Begin();
}

void FamilyManager::OnSync(int fid) { saver.OnSync(fid);}

void FamilyManager::UseSkill(int fid, int skill, int ability)
{
	skill -= FAMILYSKILL_BASE;
	Map::iterator ic = families.find(fid);
	if(ic==families.end())
		return;
	std::vector<GFamilySkill>& list = ic->second->info.skills;
	for(std::vector<GFamilySkill>::iterator it=list.begin(),ie=list.end();it!=ie;++it)
	{
		if(it->id==(unsigned int)skill)
		{
			it->ability += ability;
			int max = GetMaxAbility(it->level);
			if(it->ability > max)
				it->ability = max;
			ic->second->dirty = true;
			return;
		}
	}
}
void FamilyManager::ExpShare(int fid, int roleid,  int64_t exp, int level)
{
	Map::iterator ic = families.find(fid);
	if(ic==families.end())
		return;
	RoleContainer & list = ic->second->list;
	ic->second->expcounter++;
	for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
	{
		if(it->IsOnline())
		{
			if(it->role.rid!=(unsigned int)roleid)
				it->exp += GetExpShare(exp, it->level, level, ic->second->expfactor);
			if(it->exp<0)
				it->exp = 0;
			if(ic->second->expcounter>30 && it->exp>1.0)
			{
				SendExpShare(it->gameid, ic->second->info.id, it->role.rid, (int64_t)it->exp);
				it->exp -= (int64_t)it->exp;
			}
		}
	}
	if(ic->second->expcounter>30)
		ic->second->expcounter = 0;
}

void FamilyInfo::TaskGetSkill(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec)
{
	std::vector<GFamilySkill>& skills = info.skills;
	std::vector<GFamilySkill>::iterator sit, sie = skills.end();
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		for (sit=skills.begin(); sit != sie; ++sit)
		{
			if(sit->id==(unsigned int)pair.key+1)
			{
				vec.push_back(TaskPairData(pair.key, sit->level, sit->ability));
				break;
			}
		}
	}
}

void FamilyInfo::TaskSetSkill(TaskFamilyAsyncData& request)
{
	SyncFamilyData data;
	data.familyid = info.id;

	std::vector<GFamilySkill>& skills = info.skills;
	std::vector<GFamilySkill>::iterator sit, sie = skills.end();
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		LOG_TRACE("FamilyInfo::TaskSetSkill:familyid=%d,key=%d,value1=%d,value2=%d", 
			info.id, pair.key, pair.value1, pair.value2 );
		for (sit=skills.begin(); sit != sie; ++sit)
		{
			if(sit->id==(unsigned int)pair.key+1)
			{
				if(pair.value2)
				{
					sit->ability += pair.value2;
					int max = GetMaxAbility(sit->level);
					if(sit->ability > max)
						sit->ability = max;
					SendMessage(MSG_FAMILYSKILLABILITY, sit->id-1, pair.value2);
				}
				if(pair.value1 && pair.value1==sit->level+1)
				{
					sit->level = pair.value1;
					data.skills.push_back(GFamilySkill(sit->id+FAMILYSKILL_BASE, sit->level, sit->ability, 0));
					UpdateExpFactor();
					SendMessage(MSG_FAMILYSKILLEVEL, sit->id-1, sit->level);
				}
				dirty = true;
				break;
			}
		}
	}
	if(data.skills.size())
	{
		for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
		{
			if(it->IsOnline())
			{
				data.roleid = it->role.rid;
				GProviderServer::GetInstance()->DispatchProtocol(it->gameid,data);
			}
		}
	}
}

void FamilyManager::OnAsyncData(AsyncData* arg, AsyncData* res)
{
	TaskFamilyAsyncData request;
	std::vector<TaskPairData> vec;
	request.Unmarshal(arg->data.begin(), arg->data.size());
	LOG_TRACE("AsyncData: familyid=%d:reason=%d:mainkey=%d:count=%d",
		request.GetHeader()->nFamilyId, 
		request.GetHeader()->nReason, 
		request.GetHeader()->nMainKey, 
		request.GetHeader()->nPairCount
		);

	Map::iterator ic = families.find(request.GetHeader()->nFamilyId);
	if (ic == families.end())
		return;
	ic->second->OnAsyncData(request, vec);

	TaskPairData* array = NULL;
	if(vec.size())
	{
		array = new TaskPairData[vec.size()];
		int i=0;
		for(std::vector<TaskPairData>::iterator it=vec.begin(),ie=vec.end();it!=ie;++it,++i)
			array[i] = *it;
	}

	TaskFamilyAsyncData response(
		request.GetHeader()->nFamilyId,
		request.GetHeader()->nReason,
		request.GetHeader()->nTask,
		request.GetHeader()->nTaskReason,
		request.GetHeader()->nMainKey,
		array,
		vec.size()
	);
	if(array)
		delete array;
	res->roleid = arg->roleid;
	res->result = 0;
	res->data.replace(response.GetBuffer(), response.GetSize());
}

void FamilyManager::OnRolenameChange(int fid, int roleid, const Octets & newname)
{
	Map::iterator ic = families.find(fid);
	if (ic == families.end())
		return;
	std::list<FamilyRole> & roles = ic->second->list;
	std::list<FamilyRole>::iterator it = roles.begin(), ie = roles.end();
	for (; it != ie; ++it)
	{
		if((int)it->role.rid == roleid)
		{
			it->role.name = newname;
			ic->second->dirty = true;
			break;
		}
	}
}

int FamilyManager::CanChangeName(int fid, int roleid, const Octets & newname)
{
	Map::iterator ic = families.find(fid);
	if (ic == families.end())
		return ERR_FC_FACTION_NOTEXIST;
	if (ic->second->info.master != (unsigned int)roleid)
		return ERR_FC_NO_PRIVILEGE;
	if (Timer::GetTime()-ic->second->info.changenametime < 604800)
		return ERR_FC_CHANGENAME_CD;
	if (newname == ic->second->info.name)
		return ERR_FC_CREATE_DUP;
	if (!FactionManager::Instance()->ValidName(newname))
		return ERR_FC_INVALIDNAME;
	return ERR_SUCCESS;
}

void FamilyManager::OnNameChange(int fid, const Octets & newname, char scale)
{
	Map::iterator ic = families.find(fid);
	if (ic == families.end())
		return;
	ic->second->info.name = newname;
	ic->second->info.changenametime = Timer::GetTime();
	//FactionNameChange notice(newname, scale);
	//ic->second->Broadcast(notice, notice.localsid);
	GTPlatformAgent::FactionUpdate(FAMILYTYPE,fid,newname,FAC_UPDATE_NAME);
	GChangeFactionName_Re re(ERR_SUCCESS, 0, fid, newname, scale);
	ic->second->BroadcastGS(re, re.roleid);
}

void FamilyManager::DebugClearCD(int fid)
{
	Map::iterator ic = families.find(fid);
	if (ic == families.end())
		return;
	ic->second->info.changenametime = 0;
	LOG_TRACE("Clear family %d cooldown", fid);
}

void FamilyInfo::TaskAdd(TaskFamilyAsyncData& request)
{
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		LOG_TRACE("FamilyInfo::TaskAdd:familyid=%d,key=%d,value1=%d,value2=%d", 
			info.id, pair.key, pair.value1, pair.value2 );
		task_doing[pair.key] = pair;
	}
	dirty = true;
}
void FamilyInfo::TaskGet(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec)
{
	for(FamilyInfo::PAIRMAP::iterator it=task_doing.begin(),ie=task_doing.end();it!=ie;++it)
		vec.push_back(it->second);
}
void FamilyInfo::TaskRemove(TaskFamilyAsyncData& request)
{
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		LOG_TRACE("FamilyInfo::TaskRemove:familyid=%d,key=%d,value1=%d,value2=%d", 
			info.id, pair.key, pair.value1, pair.value2 );
		task_doing.erase(pair.key);
		task_counter.erase(pair.key);
	}
	dirty = true;
}

void FamilyInfo::TaskDone(TaskFamilyAsyncData& request)
{
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		LOG_TRACE("FamilyInfo::TaskDone:familyid=%d,key=%d,value1=%d,value2=%d", 
			info.id, pair.key, pair.value1, pair.value2 );
		task_done[pair.key] = pair;
	}
	dirty = true;
}

void FamilyInfo::UpdateExpFactor()
{
	expfactor = 0.0;
	for(std::vector<GFamilySkill>::iterator it=info.skills.begin(),ie=info.skills.end();it!=ie;++it)
	{
		if(it->id==4)
		{
			if(it->level<=15)
				expfactor = 0.005 + 0.005 * it->level;
		}
	}
}
void FamilyInfo::TaskGetRecord(TaskFamilyAsyncData& request,  std::vector<TaskPairData>& vec)
{
	int i = 0;
	for(std::vector<int>::iterator it=info.task_record.begin(),ie=info.task_record.end();it!=ie;++it,++i)
		vec.push_back(TaskPairData(i,*it,0));
}
void FamilyInfo::TaskGetFinished(TaskFamilyAsyncData& request,  std::vector<TaskPairData>& vec)
{
	for(FamilyInfo::PAIRMAP::iterator it=task_done.begin(),ie=task_done.end();it!=ie;++it)
		vec.push_back(it->second);
}
void FamilyInfo::TaskUpdateRecord(TaskFamilyAsyncData& request)
{
	std::vector<int>& v = info.task_record;
	if(v.size()<9)
		v.insert(v.begin(),9,0);

	int kill;
	bool changed = false;
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		LOG_TRACE("FamilyInfo::TaskUpdateRecord:familyid=%d,key=%d,value1=%d,value2=%d", 
			info.id, pair.key, pair.value1, pair.value2 );
		if(pair.key>=(int)v.size() || pair.key<0)
			return;
		kill = pair.value1;
		if(v[pair.key]<pair.value1)
		{
			v[pair.key] = pair.value1;
			changed = true;
		}
	}
	int newstar = 0;
	for(std::vector<int>::iterator it=v.begin(),ie=v.end();it!=ie;++it)
		if(*it>TASK_KILLTHRESHOLD)
			newstar++;
	if(star != newstar)
		star = newstar;

	if(changed)
	{
		// todo star 改变广播，否则家族内广播即可
		FamilyRecord rec(info.id, 0,  star, v, 0);
		LinkServer::GetInstance().BroadcastProtocol( rec );
	}
	dirty = true;
}
void FamilyInfo::TaskGetKill(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec)
{
	int taskid =  request.GetHeader()->nMainKey;
	FamilyInfo::PAIRLISTMAP::iterator it = task_counter.find(taskid);
	if(it!=task_counter.end())
	{
		for(std::vector<TaskPairData>::iterator is=it->second.begin(),ie=it->second.end();is!=ie;++is)
			vec.push_back(*is);
	}
}
void FamilyInfo::TaskUpdateKill(TaskFamilyAsyncData& request)
{
	int taskid =  request.GetHeader()->nMainKey;
	FamilyInfo::PAIRLISTMAP::iterator it = task_counter.find(taskid);
	if(it==task_counter.end())
		it = task_counter.insert(std::make_pair(taskid, std::vector<TaskPairData>())).first;

	std::vector<TaskPairData>& list = it->second;
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		std::vector<TaskPairData>::iterator iv = list.begin(),ie=list.end();
		for(;iv!=ie;++iv)
		{
			if(iv->key==pair.key)
			{
				iv->value1 += pair.value1;
				pair.value1 = iv->value1;
				break;
			}
		}
		if(iv==ie)
			list.push_back(pair);
	}
	dirty = true;
}
void FamilyInfo::BroadAsyncData(TaskFamilyAsyncData& data)
{
	Octets oct(data.GetBuffer(),data.GetSize());
	for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
	{
		if(it->IsOnline())
		{
			GProviderServer::GetInstance()->DispatchProtocol(it->gameid, SendAsyncData(it->role.rid, oct));
		}
	}
}
void FamilyInfo::SendMessage(int msgid, int data1, int data2)
{
	FactionChat msg;
	msg.src = msgid;
	msg.channel = GP_CHAT_SYSTEM;
	Marshal::OctetsStream data;
	data<<(int)data1<<(int)data2;
	msg.msg = data;
	for(RoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
	{
		if(it->IsOnline())
		{
			msg.localsid = it->localsid;
			GDeliveryServer::GetInstance()->Send(it->linksid, msg);
		}
	}
}
void FamilyInfo::TaskGetCommon(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec)
{
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		if(pair.key==enumTaskFamilyValue1)
		{
			int nimbus = FactionManager::Instance()->GetNimbus(info.factionid);
			vec.push_back(TaskPairData(pair.key, nimbus, 0));
		}
	}
}
void FamilyInfo::TaskAddCommon(TaskFamilyAsyncData& request)
{
	for(int i=0;i<request.GetHeader()->nPairCount;i++)
	{
		TaskPairData& pair = request.GetPair(i);
		LOG_TRACE("FamilyInfo::TaskAddCommon:familyid=%d,key=%d,value1=%d,value2=%d", 
			info.id, pair.key, pair.value1, pair.value2 );
		if(pair.key==enumTaskFamilyValue1)
		{
			FactionManager::Instance()->UpdateNimbus(info.factionid, pair.value1);
		}
	}
}

void FamilyInfo::OnAsyncData(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec)
{
	switch(request.GetHeader()->nReason)
	{
	case enumTaskFamilyGetSkillInfo:
		/*
			获得家族技能级别熟练度
			arg:
				pair.key    技能id(0-3)
			res: 
				pair.key    技能id(0-3)
				pair.value1 技能级别
				pair.value2 技能熟练度

		*/
		TaskGetSkill(request, vec);
		break;
	case enumTaskFamilyUpdateSkillInfo:
		/*
			更新家族技能级别熟练度
			arg:
				pair.key    技能id(0-3)
				pair.value1 技能级别
				pair.value2 技能熟练度

		*/
		TaskSetSkill(request);
		break;
	case enumTaskFamilyGetMonsterInfo:
		/*
			获得家族任务杀怪记录
			arg:
				nMainKey    任务编号
			res:
				pair.key    怪物编号
				pair.value1 杀怪数目

		*/
		TaskGetKill(request,vec);
		break;
	case enumTaskFamilyUpdateMonsterInfo:
		/*
			更新家族任务杀怪记录
			arg:
				nMainKey    任务编号
				pair.key    怪物编号
				pair.value1 新增杀怪数目

		*/
		TaskUpdateKill(request);
		BroadAsyncData(request);
		break;
	case enumTaskFamilyGetSharedTasks:
		/*
			获得当前家族任务列表
			res:
				pair        任务数据
		*/
		TaskGet(request,vec);
		break;
	case enumTaskFamilyAddSharedTask:
		/*
			添加家族任务
			arg:
				pair        任务数据
		*/
		TaskAdd(request);
		BroadAsyncData(request);
		break;
	case enumTaskFamilyRemoveSharedTask:
		/*
			删除家族任务
			arg:
				pair        任务数据
		*/
		TaskRemove(request);
		break;
	case enumTaskFamilyGetFinishedTasks:
		/*
			获得已完成的家族任务列表
			res:
				pair        已完成任务数据
		*/
		TaskGetFinished(request,vec);
		break;
	case enumTaskFamilyAddFinishedTask:
		/*
			添加已完成的家族任务
			arg:
				pair        已完成任务数据
		*/
		TaskDone(request);
		break;
	case enumTaskFamilyNotifyFinish:
		/*
			通知所有在线家族成员任务已经完成
			arg:
				pair        已完成任务数据
		*/
		BroadAsyncData(request);
		break;
	case enumTaskFamilyGetMonsterRecord:
		/*
			查询家族挑战任务记录
			res:
				pair.key    记录索引0-8
				pair.value1 记录数值	
		*/
		TaskGetRecord(request,vec);
		break;
	case enumTaskFamilyGetCommonValue:
		/*
			查询通用存储变量
			arg:
				pair.key    变量索引
			res:
				pair.key    变量索引
				pair.value1 记录数值	
		*/
		TaskGetCommon(request,vec);
		break;
	case enumTaskFamilyAddCommonValue:
		/*
			更新通用存储变量
			arg:
				pair.key    变量索引
				pair.value1 增量
		*/
		TaskAddCommon(request);
		break;
	case enumTaskFamilyUpdateMonsterRecord:
		/*
			更新家族挑战任务记录
			arg:
				pair.key    记录索引0-8
				pair.value1 记录数值    
		*/
		TaskUpdateRecord(request);
		break;
	}
}

bool FamilyInfo::DeleteCheck()
{
	if (!info.factionid) 
		return true;
	FactionDetailInfo * faction = FactionManager::Instance()->Find(info.factionid);
	if (NULL == faction)
		return true;
	faction->RegFamily(this);	
	RegFaction(faction);
	return false;
}

};

