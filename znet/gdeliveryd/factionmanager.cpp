#include "roleid"
#include "factionid"
#include "log.h"
#include "matcher.h"
#include "factionmanager.h"
#include "dbfactionget.hrp"
#include "dbfactionsync.hrp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "factionannounce_re.hpp"
#include "factionexpel_re.hpp"
#include "factionleave_re.hpp"
#include "factionrecruit_re.hpp"
#include "factionappoint_re.hpp"
#include "factionresign_re.hpp"
#include "factionupgrade_re.hpp"
#include "factionannounce_re.hpp"
#include "factiondismiss_re.hpp"
#include "factionnickname_re.hpp"
#include "syncplayerfaction.hpp"
#include "syncfactionlevel.hpp"
#include "uniquenameclient.hpp"
#include "dbfactionappoint.hrp"
#include "dbfactionupdate.hrp"
#include "dbfactionleave.hrp"
#include "postdeletefaction.hrp"
#include "playerfactioninfo_re.hpp"
#include "factionchat.hpp"
#include "battlemanager.h"
#include "familymanager.h"
#include "mapuser.h"
#include "worldchat.hpp"
#include "maplinkserver.h"
#include "dbfamilyget.hrp"
#include "localmacro.h"
#include "hostileadd_re.hpp"
#include "hostiledelete_re.hpp"
#include "dbhostiledelete.hrp"
#include "dbhostileupdate.hrp"
#include "chatmulticast.hpp"
#include "siegemanager.h"
#include "snsplayerinfomanager.h"
#include "territorymanager.h"
#include "gtplatformagent.h"
//#include "factionnamechange.hpp"
#include "gchangefactionname_re.hpp"
#include "raidmanager.h"
#include "dbfactionaddmoney.hrp"
#include "dbfactionwithdrawmoney.hrp"
#include "kingdommanager.h"
#include "dbcreatefacbase.hrp"
#include "createfacbase_re.hpp"
#include "facbasemanager.h"
#include "factionmultiexpsync.hpp"
#include "tsplatformagent.h"
#include "facbasestopnotice.hpp"
namespace GNET
{

inline time_t GetUpdateBaseTime(time_t time)
{
	time_t basetime;
	struct tm dt;
	localtime_r(&time, &dt);
	dt.tm_wday = 1;
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 0;
	basetime = mktime(&dt);
	return (basetime > time)?  basetime - 604800 : basetime;
}

inline bool HostileProtected(unsigned char status)
{
	return status & HOSTILE_PROTECTED != 0;
}
void UpdatePlayerFaction(int roleid, int factionid, int familyid, int title, const Octets & facname)
{
	int gameid;
	int oldfaction = 0;
        {
                Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
                PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
                if ( !pinfo )
                        return;
		oldfaction = pinfo->factionid;
                pinfo->factionid = factionid;
                pinfo->factiontitle = title;
		if (pinfo->familyid != (unsigned int)familyid)
			pinfo->jointime = Timer::GetTime();
                pinfo->familyid = familyid;
                gameid = pinfo->gameid;
        }
	if (oldfaction != factionid)
		KingdomManager::GetInstance()->OnFactionChange(oldfaction, factionid, roleid);
        GProviderServer::GetInstance()->DispatchProtocol(gameid, SyncPlayerFaction(roleid,factionid,familyid,title,0,0,0,0,0,0,facname));
}
void UpdatePlayerFaction(PlayerInfo* pinfo, int roleid, int factionid, int familyid, int title, const Octets & facname)
{
	if ( !pinfo)
		return;
	int oldfaction = pinfo->factionid;
	pinfo->factionid = factionid;
	pinfo->factiontitle = title;
	if (pinfo->familyid != (unsigned int)familyid)
		pinfo->jointime = Timer::GetTime();
	pinfo->familyid = familyid;
	if (oldfaction != factionid)
		KingdomManager::GetInstance()->OnFactionChange(oldfaction, factionid, roleid);
        GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, SyncPlayerFaction(roleid,factionid,familyid,title,0,0,0,0,0,0,facname));
}

static unsigned int _pros[9] = {15000, 167000, 738000, 2216000, 11551000, 22736000, 43927000, 85119000, 166469000};
bool CanUpgrade(GFactionInfo& info, int roleid)
{

	if(roleid!=(int)info.master)
		return false;
	int level = info.level;
	if(level<0 || level>4)
		return false;
	return info.prosperity >= _pros[level];
}
bool CanDegrade(int level, unsigned int prosperity)
{
	if (level < 1) return false;
	return prosperity < _pros[level-1];
}

int GetLevel(unsigned int prosperity)
{
	int i = 9;
        while (_pros[i-1] > prosperity)
        {
                if (i-- == 1) break;
        }
        return i;
}

int  GetTitleLimit(char title)
{
	static int _max[7] = {0, 0, 1, 1, 3, 32, 500};
	return _max[((unsigned int)title)%7];
}


///////////////////////////////////////////////////////////////////////////////

FactionDetailInfo::FactionDetailInfo(GFactionInfo& _info) 
		: dirty_mask(0), activity(0), act_uptime(0), base_status(0), creating_base(false), access_num(0), expel_count(0), family_count(0), ref_cnt(0)
{
	std::set<int> counter;
	info = _info;
	time_t now = Timer::GetTime();
	updatetime = loadtime = now;
	money_updatetime = updatetime;
	for(FamilyIdVector::iterator it=_info.member.begin();it!=_info.member.end();)
	{
		FamilyInfo * finfo = FamilyManager::Instance()->Find(it->fid);
		if (finfo)
		{
			if (finfo->RegFaction(this))
			{
				RegFamily(finfo);
				counter.insert(it->fid);
				++ it;
			}
			else
			{
				Log::log(LOG_ERR, "FactionDetailInfo: RegFaction failed! factionid=%d, familyid=%d", 
					info.fid, it->fid);
				DBFactionLeaveArg arg(_info.fid, it->fid, 0);
				DBFactionLeave* rpc = (DBFactionLeave*) Rpc::Call( RPC_DBFACTIONLEAVE,arg);
				rpc->sendclient = false;
				GameDBClient::GetInstance()->SendProtocol(rpc);
				it = _info.member.erase(it);

			}
		}
		else
		{
			DBFamilyGet * rpc = (DBFamilyGet*) Rpc::Call(RPC_DBFAMILYGET, FamilyId(it->fid));
			rpc->roleid = -1;
			rpc->factionid = info.fid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			counter.insert(it->fid);
			++ it;
		}
	}
	info.member.swap(_info.member);
	family_count = counter.size();
	dynamic.LoadData(_info.dynamic);
	/*
	   int warning;
	std::vector<FactionMsgItem>::const_iterator mit, mite = info.msgs.end();
	for (mit = info.msgs.begin(); mit != mite; ++mit)
	{
		if (now - mit->timestamp < DAY_SECONDS) 
		{
			int & tmp = rolemsg[mit->roleid];
			if (mit->timestamp > tmp)
				tmp = mit->timestamp;
		}
	}
	*/
}

FactionDetailInfo::~FactionDetailInfo()
{
	for (FamilyContainer::iterator it = families.begin(); it != families.end(); ++ it)
	{
		it->second->UnregFaction(this);
	}
	families.clear();
	for (ListenerContainer::iterator it = listeners.begin(); it != listeners.end(); ++ it)
	{
		delete *it;
	}
	listeners.clear();
}

bool FactionDetailInfo::IsLoadFinish()
{
	return family_count == families.size();
}

bool FactionDetailInfo::RegFamily(FamilyInfo * finfo)
{
	families[finfo->info.id] = finfo;
	if (IsLoadFinish())
	{
		CheckIntegration();
		Event(E_LOADFINISH);
	}
	return true;
}
void FactionDetailInfo::OnDBFactionAddMoney(unsigned int new_money)
{
	info.pk_bonus = new_money;
	money_updatetime = Timer::GetTime();
}
void FactionDetailInfo::OnDBFactionWithDrawMoney(unsigned int new_money)
{
	info.pk_bonus = new_money;
	money_updatetime = Timer::GetTime();
}
int FactionDetailInfo::WithdrawFactionMoney(int roleid)
{
	if(static_cast<unsigned int>(roleid) != info.master)
	{
		LOG_TRACE("FactionDetailInfo::WithdrawFactionMoney err master=%d roleid=%d",info.master,roleid);
		return ERR_FC_NO_PRIVILEGE;
	}
	DBFactionWithDrawMoneyArg arg(info.fid, roleid);
	DBFactionWithDrawMoney* rpc = (DBFactionWithDrawMoney*) Rpc::Call( RPC_DBFACTIONWITHDRAWMONEY,arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return ERR_SUCCESS;
}

void FactionDetailInfo::OnFamilyDismiss(FamilyInfo * finfo)
{
	// 正常不会出现，如果出现就改一下faction的数据算了
	FamilyContainer::iterator it = families.find(finfo->info.id);
	if(it != families.end())
	{
		family_count --;
		families.erase(it);	
	}

	//FactionDismiss_Re re(ERR_SUCCESS, finfo->info.id, 0, 1, 1);
	//Broadcast(re, re.localsid);	
}


void FactionDetailInfo::Broadcast(Protocol& data, unsigned int &localsid)
{
	access_num++;
	for(FamilyContainer::iterator it=families.begin(),ie=families.end();it!=ie;++it)
	{
		it->second->Broadcast(data, localsid);
	}
}

void FactionDetailInfo::BroadcastGS(Protocol& data, int & roleid)
{
	access_num++;
	for(FamilyContainer::iterator it=families.begin(),ie=families.end();it!=ie;++it)
	{
		it->second->BroadcastGS(data, roleid);
	}
}

void FactionDetailInfo::CheckIntegration()
{
	int title = TITLE_MEMBER + 1;
	RoleContainer::iterator rp;
	FamilyContainer::iterator fit = families.begin();
	for(; fit != families.end(); ++ fit)
	{
		RoleContainer::iterator rit = fit->second->list.begin();
		for (; rit != fit->second->list.end(); ++ rit)
		{
			if (rit->role.title < title)
			{
				rp = rit;
				title = rit->role.title;
			}
		}
	}
	if (title < TITLE_MEMBER + 1 && info.master != rp->role.rid)
	{
		Log::log(LOG_ERR,"Assign faction master, factionid=%d,predecessor=%d,roleid=%d",info.fid,info.master,rp->role.rid);
                SyncMaster(info.fid, rp->role.rid,true);
	}	
}

bool FactionDetailInfo::IsSilent()
{
	if (access_num)
		return false;
	for(FamilyContainer::iterator it = families.begin(); it != families.end(); ++ it)
	{
		if (it->second && (it->second->online || it->second->access_num))	
			return false;
	}
	if(RaidManager::GetInstance()->IsPkFaction(info.fid))
		return false;
	return true;
}

void FactionDetailInfo::OnDeleteRole(int roleid, int contribution)
{
	info.contribution += contribution;
}

void FactionDetailInfo::OnRoleLeaveFamily(int roleid, int contribution)
{
	info.contribution += contribution;
}
void FactionDetailInfo::Save2DB(bool &isSilent, bool isbatch)
{
	DBFactionSyncArg arg;
	arg.fid = info.fid;
	arg.population = families.size();
	int prosperity = info.contribution;
	for(FamilyContainer::iterator it=families.begin(),ie=families.end();it!=ie;++it)
	{
		if (!it->second)
			continue;
		
		FamilyInfo* finfo = it->second;
		bool needsync = false;
		finfo->Update(needsync);
		if (isSilent)
			isSilent = finfo->IsSilent();
		finfo->access_num = 0;

		prosperity += finfo->GetContribution();
	}
	if(prosperity<0)
		prosperity = 0;
	if(dirty_mask || dynamic.IsDirty() || (int)info.prosperity != prosperity)
	{
		arg.prosperity = info.prosperity = prosperity;
		arg.contribution = info.contribution;
		arg.nimbus = info.nimbus;
		arg.charm = info.charm;
		arg.syncmask = dirty_mask;
		arg.multi_exp = info.datagroup[FAC_DATA_MULTI_EXP];
		arg.multi_exp_endtime = info.datagroup[FAC_DATA_MULTI_EXP_ENDTIME];
		/*
		   int warning;
		if (dirty_mask & FAC_DIRTY_MSGS)
			arg.msgs = info.msgs;
			*/
		if (dynamic.IsDirty())
		{
			arg.syncmask |= FAC_DIRTY_DYNAMIC;
			arg.dynamic = dynamic.GetData();
		}
		if (dirty_mask & FAC_DIRTY_ACT)
		{
			arg.activity = activity;
			arg.act_uptime = act_uptime;
		}
		DBFactionSync* rpc = (DBFactionSync*) Rpc::Call( RPC_DBFACTIONSYNC,arg);
		rpc->isbatch = isbatch;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		//dirty = false;
		ClearDirty();
		dynamic.SetDirty(false);
	}
	//CheckHostile();
}

bool FactionDetailInfo::NeedUpdateHostile(time_t now, unsigned char & actionpoint)
{
	int basetime = FactionManager::Instance()->actiontime;
	unsigned char currentpoint = GetHostileInfo(info).actionpoint;
	if (GetHostileInfo(info).updatetime < basetime && now > basetime)
	{
		if (currentpoint >= 5)
		{
			actionpoint = 5;
		}
		else
		{
			time_t oldtime = GetHostileInfo(info).updatetime;
			time_t oldbasetime = GetUpdateBaseTime(oldtime);
			unsigned char delta = (basetime - oldbasetime)/604800;
			actionpoint = (currentpoint + delta) >= 5 ? 5 : currentpoint + delta;
		}
		return true;
	}
	return false;
}

void FactionDetailInfo::CheckHostile()
{
	DBHostileUpdateArg arg;
	arg.factionid = info.fid;
	time_t now = Timer::GetTime() ; //+ FactionManager::Instance()->GetForged();
	HostileInfo & hinfo = GetHostileInfo(info);
	// check update actionpoint
	if (hinfo.updatetime == 0)
	{
		LOG_TRACE("updattime=0, need update actionpoint");
		hinfo.updatetime = now;
		arg.actionpoint = 1;
		arg.datamask |= OP_ACTIONPOINT;
	}
	else
	{
		if (NeedUpdateHostile(now, arg.actionpoint))
		{
			LOG_TRACE("need update actionpoint, actionpoint=%d", arg.actionpoint);
			hinfo.updatetime = now;
			arg.datamask |= OP_ACTIONPOINT;
		}
	}
	// check protect timeout
	if (HostileProtected(hinfo.status) && hinfo.protecttime + PROTECT_TIMEOUT <= now)
	{
		LOG_TRACE("need update protect, protecttime=%s, now=%s", ctime((time_t*)&hinfo.protecttime), ctime(&now));
		arg.datamask |= OP_PROTECT;	
	}
	if (arg.datamask)
	{
		DBHostileUpdate * rpc = (DBHostileUpdate*) Rpc::Call( RPC_DBHOSTILEUPDATE, arg);
		LOG_TRACE("send hostile update, factionid=%d,datamask=%d,actionpoint=%d"
				, arg.factionid, arg.datamask, arg.actionpoint);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}

	// check hostile timeout
	std::vector<unsigned int> todel;
	std::vector<HostileFaction>::iterator it = Hostiles(info).begin(), ie = Hostiles(info).end();
	for (; it != ie; ++it)
	{
		if (now >= it->addtime + HOSTILE_TIMEOUT) 
			todel.push_back(it->fid);
	}
	if (todel.size())
	{
		DBHostileDeleteArg arg(info.fid);
		arg.hostile.swap(todel);
		DBHostileDelete* rpc = (DBHostileDelete*) Rpc::Call( RPC_DBHOSTILEDELETE, arg);
		rpc->sendclient = false;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
}

void FactionDetailInfo::Update(bool &isSilent)
{
	if (!IsLoadFinish())
	{
		Log::log(LOG_ERR, "Faction update, still loading.  factionid=%d, expect=%d, current=%d", 
			info.fid, family_count, families.size());
		time_t now = Timer::GetTime();
		if (now - loadtime > LOAD_TIMEOUT)
		{
			LoadFamilies();
			loadtime = now;
		}
		return;
	}	
	isSilent = (ref_cnt<=0) && (access_num == 0);
	Save2DB(isSilent);
}

void FactionDetailInfo::SyncMaster(int fid, int roleid, bool set)
{
	DBFactionAppointArg arg(fid, roleid, TITLE_MASTER) ;
	if(!set)
		arg.title = TITLE_HEADER;
	DBFactionAppoint* rpc = (DBFactionAppoint*) Rpc::Call( RPC_DBFACTIONAPPOINT,arg);
	rpc->roleid = roleid;
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

int FactionDetailInfo::GetMasterID()
{
	return info.master;
}

void FactionDetailInfo::UpdateCharm(int charm)
{
	info.charm += charm;
	if (info.charm < 0)
		info.charm = 0;
	SetNormalDirty();
}

void FactionDetailInfo::GetOnlineMember(std::vector<int> &roles, int gsid)
{
	GetMember();
	roles.clear();
	for(RoleList::const_iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		const FamilyRole *familyrole = it->role;
		if(familyrole == NULL || familyrole->gameid != gsid || familyrole->linksid == 0)
		{
			continue;
		}
		roles.push_back(familyrole->role.rid);
	}
}

void FactionDetailInfo::GetMember()
{
	if (rolecache.size())
		return;

	online = 0;
	int masterfamilyid;
	FamilyRole * master = NULL;
	FamilyContainer::iterator it = families.begin();
	for(; it != families.end(); ++ it)
	{
		FamilyInfo * family = it->second;
		online += family->GetOnline();
		for(std::list<FamilyRole>::iterator iit = family->list.begin(); iit != family->list.end(); ++ iit)
		{
			if (info.master == iit->role.rid)
			{
				if (master)
					Log::log(LOG_ERR, "master duplicate %d %d ", master->role.rid, iit->role.rid);
				masterfamilyid = it->first;
				master = &*iit;
				continue;
			}
			if (iit->gameid >= 0)
				rolecache.push_front(FactionRole(it->first, *iit));
			else
				rolecache.push_back(FactionRole(it->first, *iit));	
		}	
	}
	if (master)
		rolecache.push_front(FactionRole(masterfamilyid, master));
}


void FactionDetailInfo::GetMemberTree(FactionList_Re& ret)
{
	GetMemberFlat(0, ret);

	ret.announce = info.announce;
	FamilyContainer::iterator it = families.begin(), ie = families.end();	
	for(;it!=ie;++it)
	{
		FamilyInfo * info = it->second;
		ret.familylist.push_back(GFamilyBrief(it->first, info->info.name));
	}
	ret.base_status = base_status;
	ret.activity = activity;
}

int FactionDetailInfo::GetMemberCount()
{
	GetMember();
	return rolecache.size();
}

void FactionDetailInfo::GetMemberFlat(int page, FactionList_Re& ret)
{
	GetMember();

	ret.page = page;
	if(page<0)
	{
		ret.announce = info.announce;
		page = 0;
	}

        RoleList::iterator it, ie;
	int size = rolecache.size();
	ie = rolecache.end();
	if(size<=MAX_MEMBER_PERPAGE)
		it = rolecache.begin();
	else if(page*MAX_MEMBER_PERPAGE>=size)
		it = rolecache.end();
	else{
		it = rolecache.begin();
		std::advance(it, page*MAX_MEMBER_PERPAGE);
		if((page+1)*MAX_MEMBER_PERPAGE<size)
		{
			ie = it;
			std::advance(ie, (int)MAX_MEMBER_PERPAGE);
		}
	} 

	for(;it!=ie;++it)
	{
		FamilyRole *info = it->role;
		ret.members.add(MemberInfo(info->role.rid, info->role.level, info->role.occupation, info->role.title, 
			info->gameid,it->familyid, info->role.contribution, info->role.devotion, 
			info->role.name, info->role.nickname, info->reborn_cnt, info->cultivation, info->fac_coupon_add));
	}
	ret.online = online;
	ret.population = size;
	ret.money = info.pk_bonus;
	ret.prosperity = info.prosperity;
	ret.base_status = base_status;
	ret.activity = activity;
	if (info.deletetime != 0)
	{
		ret.deletetime = info.deletetime + 7*86400;
	}
	else
		ret.deletetime = 0;
	/*
	ret.actionpoint = GetHostileInfo(info).actionpoint;
	if (page == 0)
		ret.hostiles = Hostiles(info);
	*/
}

void FactionDetailInfo::UpdateRole(int roleid, char title)
{
	GetMember();
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		FamilyRole * role = it->role;
		if (role->role.rid == (unsigned int)roleid)
		{
			role->role.title = title;
			::UpdatePlayerFaction(roleid, info.fid, it->familyid, title, info.name);
			break;
		}
	}
}

void FactionDetailInfo::OnAppoint(int rid, int candidate, char title, char oldtitle)
{
	UpdateRole(candidate, title);
	GTPlatformAgent::FactionModMember(FACTIONTYPE,info.fid,candidate,title);
	if (title == TITLE_MASTER)
	{
		info.master = candidate;
		UpdateRole(rid, TITLE_HEADER);
		GTPlatformAgent::FactionModMember(FACTIONTYPE,info.fid,rid,TITLE_HEADER);
		SNSPlayerInfoManager::GetInstance()->OnFactionAppointMaster(this, rid, candidate);
		SiegeManager::GetInstance()->SyncFaction(info.fid, info.master);
	}

	if(title==TITLE_MASTER && rid==candidate)
		return;

	if (rid != candidate) // appoint	
	{
		FactionAppoint_Re re(0, rid, candidate, title, 0, 0);
		Broadcast(re, re.localsid);
	}
	else // resign
	{
		FactionResign_Re re(0, rid, oldtitle, 0, 0);
		Broadcast(re, re.localsid);
	}
}

void FactionDetailInfo::LoadFamilies()
{
	for(FamilyIdVector::iterator it=info.member.begin(),ie=info.member.end();it!=ie;++it)
	{
		if (families.find(it->fid) == families.end())
		{
			DBFamilyGet * rpc = (DBFamilyGet*) Rpc::Call(RPC_DBFAMILYGET, FamilyId(it->fid));
			rpc->roleid = -1;
			rpc->factionid = info.fid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
	}

}

void FactionDetailInfo::OnLoadErrFamily(int familyid)
{
	LOG_TRACE("FactionDetailInfo OnLoadErrFamily: factionid=%d, familyid=%d", info.fid, familyid);
	FamilyIdVector::iterator it = info.member.begin();
	for(; it!=info.member.end(); )
	{
		if (it->fid == (unsigned int)familyid)
		{
			if (families.find(familyid) == families.end())
			{
				DBFactionLeaveArg arg(info.fid, familyid, 0);
				DBFactionLeave* rpc = (DBFactionLeave*) Rpc::Call( RPC_DBFACTIONLEAVE,arg);
				rpc->sendclient = false;
				GameDBClient::GetInstance()->SendProtocol(rpc);

				family_count --;
				it = info.member.erase(it);
				continue;
			}
		}
		++it;
	}
}
void FactionDetailInfo::OnDeleteHostile(unsigned int fid) 
{ 
	for (std::vector<HostileFaction>::iterator hit = Hostiles(info).begin(); hit != Hostiles(info).end(); ++ hit)
	{
		if (hit->fid == fid)
		{
			Hostiles(info).erase(hit);
			HostileDelete_Re re(ERR_SUCCESS, info.fid, fid, 0);
			Broadcast(re, re.localsid);
			GProviderServer::GetInstance()->BroadcastProtocol(re);
			break;
		}
	}
}

void FactionDetailInfo::OnDeleteHostile(std::vector<unsigned int> & fids) 
{ 
	for (std::vector<unsigned int>::iterator it = fids.begin(); it != fids.end(); ++ it)
		OnDeleteHostile(*it);
}

struct NotifyGSHostileAdd : public Thread::Runnable
{
	NotifyGSHostileAdd (int fid, int hostile)
		: factionid(fid), hostile_id(hostile)
	{
	}

	void Run()
	{
		HostileAdd_Re re(ERR_SUCCESS, factionid, hostile_id, 0);
		GProviderServer::GetInstance()->BroadcastProtocol(re);
	}
private:
	int factionid, hostile_id;
};

void FactionDetailInfo::OnAddHostile(int hostile, Octets & name, bool active) 
{
	if (active)
	{
		GetHostileInfo(info).actionpoint --;
	}
	Hostiles(info).push_back(HostileFaction(hostile,  name, Timer::GetTime()));
	HostileAdd_Re re(ERR_SUCCESS, info.fid, hostile, 0);
	Broadcast(re, re.localsid);
	IntervalTimer::Schedule(new NotifyGSHostileAdd(info.fid, hostile), HOSTILE_WAIT_TIME);
}

void FactionDetailInfo::GetFactionHostiles(SyncFactionHostiles & sync)
{
	if (Hostiles(info).size() == 0)
		return;

	sync.list.push_back(FactionHostiles(info.fid));
	FactionHostiles & data = sync.list.back();
	for (std::vector<HostileFaction>::iterator hit = Hostiles(info).begin(); hit != Hostiles(info).end(); ++ hit)
		data.hostiles.push_back(hit->fid);
}

void FactionDetailInfo::SendSiegeMessage(int type, Octets & msg)
{
	GetMember();
	std::map<int, PlayerVector> player_line;
	RoleList::iterator it, ie;
	it = rolecache.begin();
	ie = rolecache.end();
	int count = 0;
	for (; it != ie; ++it, count ++)
	{
		FamilyRole * role = it->role;
		if (role->gameid >= 0)
		{
			std::map<int, PlayerVector>::iterator map_it = player_line.find(role->linksid);
			if (map_it == player_line.end())
				map_it = player_line.insert(std::make_pair(role->linksid, PlayerVector())).first;
			map_it->second.push_back(Player(role->role.rid, role->localsid));
		}
		else if (count > 1)
			break;
	}
			                		
	ChatMultiCast csc;
	csc.channel = GP_CHAT_SYSTEM;
	csc.srcroleid = type;
	csc.msg = msg;
	for (std::map<int, PlayerVector>::iterator it = player_line.begin(); it != player_line.end(); ++ it)
	{
		csc.playerlist = it->second;
		GDeliveryServer::GetInstance()->Send(it->first, csc);
	}
}
bool FactionDetailInfo::GetPostTime(int roleid, int & time) const
{
	std::map<int, int>::const_iterator tit = rolemsg.find(roleid);
	if (tit == rolemsg.end())
		return false;
	time = tit->second;
	return true;
}
bool FactionDetailInfo::RoleHasPost(int roleid) const
{
	int time = 0;
	if (!GetPostTime(roleid, time))
		return false;
	return (GDeliveryServer::GetInstance()->IsSameDay(time, Timer::GetTime()));
}
	/*
void FactionDetailInfo::GetMsg(int roleid, int pageid, int & totalsize, std::vector<FactionMsgItem> & msgs, char & haspost) const
{
	   int warning;
	if (pageid < 0 || (int)(pageid*MSG_PAGE_SIZE) < 0 ||
			pageid*MSG_PAGE_SIZE >= (int)info.msgs.size())
		return;
	totalsize = info.msgs.size();
	std::vector<FactionMsgItem>::const_iterator it = info.msgs.begin(), ie = info.msgs.end();
	std::advance(it, pageid*MSG_PAGE_SIZE);
	for (int i = 0; i < MSG_PAGE_SIZE && it != ie; i++,++it)
		msgs.push_back(*it);
	if (pageid == 0)
		haspost = RoleHasPost(roleid);
}
		*/
	/*
int FactionDetailInfo::PostMsg(int roleid, const Octets & rolename, const Octets & msg, char repost)
{
	   int warning;
	int post_time = 0;
	if (GetPostTime(roleid, post_time))
	{
		if (Timer::GetTime() - post_time < POST_MSG_CD)
			return ERR_FC_MSG_COOLDOWN;
	}
	if ((bool)repost != RoleHasPost(roleid))
		return ERR_FC_MSG_ARG;
	if (msg.size() > MAX_MSG_LENGTH)
		return ERR_FC_MSG_LENGTH;
	if (repost)
	{
		int warning; //判断贡献度并扣除贡献度
	}
	int now = Timer::GetTime();
	FactionMsgItem item(roleid, rolename, now, msg);
	info.msgs.push_back(item);
	if (info.msgs.size() > MAX_MSG_SIZE)
		info.msgs.erase(info.msgs.begin());
	rolemsg[roleid] = now;
	SetMsgsDirty();
	return ERR_SUCCESS;
}
	*/

void FactionDetailInfo::SetAct(int act, int act_time, int clear_time)
{
	activity = act;
	act_uptime = act_time;
	CheckClearAct(clear_time);
}

void FactionDetailInfo::UpdateAct(int delta)
{
	if (info.level < FAC_ACT_START_LEV)
	{
		Log::log(LOG_ERR, "updateactivity, but faction level %d invalid", info.level);
		return;
	}
	activity += delta;
	act_uptime = Timer::GetTime();
	SetActDirty();
}
void FactionDetailInfo::CheckClearAct(int clear_time)
{
	if (clear_time > 0 && act_uptime <= clear_time && activity > 0 )
	{
		LOG_TRACE("set and clear faction act from %d time %d, fid=%d", activity, act_uptime, info.fid);
		activity = 0;
		act_uptime = Timer::GetTime();
		SetActDirty();
	}
}

void FactionDetailInfo::SetBaseStatus(int s)
{
	base_status = s;
}

bool FactionDetailInfo::IsBaseOpen()
{
	return (base_status & ST_FAC_BASE_OPEN);
}
bool FactionDetailInfo::IsBaseStart()
{
	return (base_status & ST_FAC_BASE_START);
}

int FactionDetailInfo::CanCreateBase(int roleid)
{
	if (IsBaseOpen())
		return ERR_FAC_BASE_ALREADY_OPEN;
	if (creating_base)
		return ERR_SUCCESS;
	if (IsDeleting())
		return ERR_FAC_BASE_CREATE_DELETING;
	if (info.master != (unsigned int)roleid)
		return ERR_FAC_BASE_LOGIC;
	if (info.level < FAC_ACT_START_LEV)
		return ERR_FAC_BASE_CREATE_LEV;
	return ERR_SUCCESS;
}

int FactionDetailInfo::CreateBase(int roleid)
{
	int ret = CanCreateBase(roleid);
	if (ret != ERR_SUCCESS)
		return ret;
	DBCreateFacBase * rpc = (DBCreateFacBase *)Rpc::Call(RPC_DBCREATEFACBASE, DBCreateFacBaseArg(info.fid, roleid));
	GameDBClient::GetInstance()->SendProtocol(rpc);
	creating_base = true;
	return ERR_SUCCESS;
}

void FactionDetailInfo::OnBaseCreate(int roleid, int ret)
{
	if (!creating_base)
	{
		Log::log(LOG_ERR, "faction %d OnBaseCreate, ret %d but creating is false",
				info.fid, ret);
		return;
	}
	creating_base = false;
	if (ret != ERR_SUCCESS)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, CreateFacBase_Re(ERR_FAC_BASE_CREATE_DB, roleid, pinfo->localsid, 0));
		return;
	}
	base_status |= ST_FAC_BASE_OPEN;
	if (!IsDeleting())
	{
		ret = FacBaseManager::GetInstance()->TryStartBase(info.fid, roleid); 
		if (ret != ERR_SUCCESS)
		{
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (pinfo)
				GDeliveryServer::GetInstance()->Send(pinfo->linksid, CreateFacBase_Re(ERR_SUCCESS, roleid, pinfo->localsid, 0));
			return;
		}
	}
}
void FactionDetailInfo::OnBaseStart()
{
	base_status |= ST_FAC_BASE_START;
}

void FactionDetailInfo::OnBaseStop()
{
	base_status &= (~ST_FAC_BASE_START);
	//通知客户端基地关闭
	FacBaseStopNotice notice;
	Broadcast(notice, notice.localsid);
}

void FactionDetailInfo::OnStartMultiExp(int multi, int end_time)
{
	info.datagroup[FAC_DATA_MULTI_EXP] = multi;
	info.datagroup[FAC_DATA_MULTI_EXP_ENDTIME] = end_time;
	SetNormalDirty();

	if (multi > 1 && end_time > Timer::GetTime())
	{
		FactionMultiExpSync sync(0, multi, end_time);
		BroadcastGS(sync, sync.roleid);
	}
}

bool FactionDetailInfo::GetMultiExp(int & multi, int & endtime)
{
	multi = info.datagroup[FAC_DATA_MULTI_EXP];
	endtime = info.datagroup[FAC_DATA_MULTI_EXP_ENDTIME];
	return (multi > 0 && endtime > Timer::GetTime());
}
/////////////////////////////////////////////////////////////////////////////////
bool FactionManager::Initialize()
{
	IntervalTimer::Attach(this,500000/IntervalTimer::Resolution());
	open = true;
	return true;
}

unsigned int FactionDetailInfo::ComputeProsperity()
{
	unsigned int prosperity = 0;	
	for(FamilyContainer::iterator it = families.begin(); it != families.end(); ++it)
	{
		if (!it->second)
			continue;
		prosperity += it->second->GetContribution(); 
	}
	prosperity += info.contribution;
	if(prosperity<0)
		prosperity = 0;
	info.prosperity = prosperity;
	return prosperity;
}

bool FactionManager::Update()
{
	time_t now = Timer::GetTime(); //+ t_forged;
	if (0 == actiontime)
		actiontime = GetUpdateBaseTime(now);
	else
		if (now - actiontime >= 604800)
			actiontime += 604800;

	Map::iterator it = factions.upper_bound(cursor);
	if(it==factions.end())
	{
		cursor = 0;
		return true;
	}
	cursor = it->second->info.fid;
	if(now - it->second->updatetime > UPDATE_INTERVAL)
	{
		LOG_TRACE("Faction: update, factionid=%d, mapsize=%d", cursor, factions.size());
		bool isSilent = false;
		it->second->Update(isSilent);
		if(isSilent)
		{
			delete it->second;
			it->second = NULL;
			factions.erase(it);
			LOG_TRACE("Faction: remove timeout faction, factionid=%d, mapsize=%d", cursor, factions.size());
		}
		else
		{
			if((now%86400)<(it->second->updatetime%86400))
				it->second->expel_count = 0;
			it->second->updatetime = now;
			it->second->access_num = 0;
		}
	}
	return true;
}

void FactionManager::OnLoad(GFactionInfo& info, int act, int act_uptime, int bstatus)
{
	Map::iterator it = factions.find(info.fid);
	if (it != factions.end())
		return;
	FactionDetailInfo * faction = new FactionDetailInfo(info);
	factions.insert(std::make_pair(info.fid, faction));
	faction->access_num = 1;
	faction->SetAct(act, act_uptime, act_clear_time);
	faction->SetBaseStatus(bstatus);
//	if (info.level >= FAC_ACT_START_LEV)
	if (faction->IsBaseOpen() && !faction->IsDeleting()) //基地已经创建 直接开启 分配 gs
		FacBaseManager::GetInstance()->TryStartBase(info.fid); 
	SyncFactionLevel data;
	data.list.push_back(FactionLevel(info.fid,info.level));
	GProviderServer::GetInstance()->BroadcastProtocol(data);
	/*
	faction->CheckHostile();
	SyncFactionHostiles sync;
	faction->GetFactionHostiles(sync);
	GProviderServer::GetInstance()->BroadcastProtocol(sync);
	*/
}

int FactionManager::OnRecruit(int fid, Octets& factionname)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return 2;
	if(FactionHelper::GetMemberCapacity(it->second->info.level)<=it->second->families.size())
		return 1;
	factionname = it->second->info.name;
	return 0;
}

bool FactionManager::GetBaseInfo(int fid, GFactionBaseInfo& info)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return false;
	GFactionInfo& data = it->second->info;
	info.fid = data.fid;
	info.name = data.name;
	info.level = data.level;
	info.nimbus = data.nimbus;
	info.territoryscore = TerritoryManager::GetInstance()->GetScore(fid);
	it->second->access_num++;
	return true;
}

bool FactionManager::SetAnnounce(int fid, int roleid, Octets& announce)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	ic->second->info.announce = announce;
	FactionAnnounce_Re re(0,roleid,announce,0);
	GTPlatformAgent::FactionUpdate(FACTIONTYPE,fid,announce,FAC_UPDATE_ANNOUNCE);
	ic->second->Broadcast(re, re.localsid);
	return true;
}
/*
bool FactionManager::FillSyncArg(int fid, DBFactionSyncArg& arg)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return false;
	GFactionInfo& info = it->second->info;
	arg.fid = fid;
	arg.population = info.population;
	arg.prosperity = info.prosperity;
	arg.contribution = info.contribution;
	arg.charm = info.charm;
	return true;
}
*/
void FactionManager::GetOnlineMember(int fid, std::vector<int> &roles, int gsid)
{
	Map::iterator ic = factions.find(fid);
	if(ic == factions.end())
	      return;
	ic->second->GetOnlineMember(roles, gsid);
}

void FactionManager::GetMemberFlat(int fid, int page, FactionList_Re& ret)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;

	ic->second->GetMemberFlat(page, ret);
}

void FactionManager::GetMemberTree(int fid, FactionList_Re& ret)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;

	ic->second->GetMemberTree(ret);
}

int FactionManager::GetMemberCount(int fid)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return 0;
	else
		return ic->second->GetMemberCount();
}

int FactionManager::FillAppointArg(DBFactionAppointArg& arg, unsigned char mytitle)
{
	if(arg.title>TITLE_MEMBER||arg.title<TITLE_MASTER)
		return 2;
	Map::iterator ic = factions.find(arg.fid);
	if(ic==factions.end())
		return 2;
	FamilyContainer & families = ic->second->families;
	int sum = 0;
	bool found = false;
	for(FamilyContainer::iterator it=families.begin(),ie=families.end();it!=ie;++it)
        {
		if (!it->second)
			continue;
                std::list<FamilyRole>& list = it->second->list;
                for(std::list<FamilyRole>::iterator iit=list.begin(),iie=list.end();iit!=iie; ++iit)
                {
			if (iit->role.title == (char)arg.title)
				sum++;
			if (iit->role.rid == arg.roleid)
			{
				if (iit->role.title <= mytitle)
					return 2;
				found = true;
			}
                }
        }
	if(arg.title!=TITLE_MASTER && sum>=GetTitleLimit(arg.title))
		return 1;
	return found?0:2;
}

void FactionManager::OnLogin(int roleid,int factionid,char title,int familyid,
			int contribution,int devotion,int gameid,int linksid,int localsid)
{
	LOG_TRACE("FactionManager::OnLogin, roleid=%d, factionid=%d, familyid=%d, title=%d", roleid, factionid, familyid, title); 
	if (!open)
		return;
	if(!factionid)
	{
		FamilyManager::Instance()->OnLogin(roleid,0,title,familyid,contribution,devotion,gameid,linksid,localsid);
		return;
	}
	Map::iterator ic = factions.find(factionid);
	if(ic==factions.end())  // not found
	{
		DBFactionGet* rpc = (DBFactionGet*) Rpc::Call( RPC_DBFACTIONGET,FactionId(factionid));

		LoginListener * listener = new LoginListener();
		listener->roleid = roleid;
		listener->familyid = (unsigned int)familyid;
		listener->familytitle = title;
		listener->contribution = contribution;
		listener->devotion = devotion;
		listener->factiontitle = title;

		rpc->listener = listener;
		rpc->roleid = roleid;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return;
	}
	else if (!ic->second->IsLoadFinish()) // found but in loading 
	{
		LoginListener * listener = new LoginListener();
		listener->roleid = roleid;
		listener->familyid = (unsigned int)familyid;
		listener->familytitle = title;
		listener->contribution = contribution;
		listener->devotion = devotion;
		listener->factiontitle = title;
		ic->second->AddListener(listener);
	}
	else // found and load finish
	{
		FamilyManager::Instance()->OnLogin(roleid,factionid,title,familyid,contribution,devotion,gameid,linksid,localsid);
		FactionDetailInfo * detail = ic->second;
		if (detail && !detail->IsDeleting() && detail->IsBaseOpen() && !detail->IsBaseStart())
			FacBaseManager::GetInstance()->TryStartBase(factionid); 
	}
	if(ic->second->info.master==(unsigned int)roleid && title!=TITLE_MASTER)
                ic->second->SyncMaster(factionid, roleid,true);
	if(title==TITLE_MASTER && (int)ic->second->info.master!=roleid)
                ic->second->SyncMaster(factionid, roleid,false);
	//通知 gs 帮派名称
	::UpdatePlayerFaction(roleid, factionid, familyid, title, ic->second->info.name);
	int multi_exp = 0, end_time = 0;
	if (ic->second->GetMultiExp(multi_exp, end_time))
	{
		FactionMultiExpSync sync(roleid, multi_exp, end_time);
		GProviderServer::GetInstance()->DispatchProtocol(gameid, sync);
	}
}
void FactionManager::OnLogout(int roleid, int factionid, int familyid, int level, int occupation)
{
	if (!open)
		return;
	FamilyManager::Instance()->OnLogout(roleid, factionid, familyid, level, occupation);
}

bool FactionManager::Broadcast(int fid, Protocol& data, unsigned int &localsid)
{
	if(!fid)
		return false;
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	ic->second->Broadcast(data, localsid);
	return true;
}

bool FactionManager::BroadcastGS(int fid, Protocol& data, int & roleid)
{
	if(!fid)
		return false;
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	ic->second->BroadcastGS(data, roleid);
	return true;
}

int FactionManager::CanDismiss(int fid, int master)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return ERR_PERMISSION_DENIED;
	if(ic->second->info.deletetime)
		return ERR_FC_DISMISSWAITING;
	if(ic->second->GetMasterID()!=master)
		return ERR_PERMISSION_DENIED;
	if(fid == KingdomManager::GetInstance()->GetKingFaction())
		return ERR_FC_CHANGE_KINGDOM;
	return 0;
}

bool FactionManager::CanExpel(int roleid, int fid, int expellee, int& contribution)
{
	if(!fid)
		return false;
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( !pinfo || !pinfo->factionid || pinfo->factionid != (unsigned int)fid)
			return false;
		if (pinfo->factiontitle != TITLE_VICEMASTER && pinfo->factiontitle != TITLE_MASTER)
			return false;
		if (pinfo->familyid == (unsigned int)expellee)
			return false; 
	}
	FamilyContainer & families = ic->second->families;
	FamilyContainer::iterator it = families.begin(), ie = families.end();
	for (; it!=ie; ++it)
	{
		if(it->second->info.id==(unsigned int)expellee)
		{
			contribution = it->second->GetContribution();
			for (RoleContainer::iterator rit=it->second->list.begin(); rit!=it->second->list.end(); ++rit)
				if (rit->role.title < TITLE_HEADER && rit->role.title >= TITLE_MASTER)
					return false;
			break;
		}
	}
	return it != ie;
}

bool FactionManager::ValidName(const Octets& name)
{
	int maxlen = atoi(Conf::GetInstance()->find(GDeliveryServer::GetInstance()->Identification(), "max_name_len").c_str());
	if (maxlen <= 0) maxlen = 16;
	return (name.size()<=(size_t)maxlen &&  Matcher::GetInstance()->Match((char*)name.begin(),name.size())==0);
}

void FactionManager::OnJoin(int inviter, unsigned int fid, unsigned int familyid, Octets& name)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;

	FamilyInfo * finfo = FamilyManager::Instance()->Find(familyid);
	if (!finfo || finfo->info.factionid)
		return;


	finfo->JoinFaction(ic->second); 
	ic->second->families.insert(std::make_pair(familyid, finfo));	
	ic->second->family_count ++;
	int multi_exp = 0, end_time = 0;
	if (ic->second->GetMultiExp(multi_exp, end_time))
	{
		FactionMultiExpSync sync(0, multi_exp, end_time);
		finfo->BroadcastGS(sync, sync.roleid);
	}
	//called after JoinFaction
	GTPlatformAgent::FactionAddMember(ic->second->info,finfo->info,finfo->list);
	//TSPlatformAgent::AddFaction(finfo->info.id, ic->second->info.fid);

	FactionRecruit_Re re(0, fid, familyid, inviter, 0, 0, name, 0, 0);
	ic->second->Broadcast(re, re.localsid);

	FAC_DYNAMIC::member_change log = {FAC_DYNAMIC::member_change::FAMILY_JOIN, 0, {}, 20, {} };
	FactionDynamic::GetName(name, log.familyname, log.familynamesize);
	ic->second->GetDynamic().RecordDynamic(Timer::GetTime(), FAC_DYNAMIC::MEMBER_CHANGE, log);
}
void FactionManager::OnAppoint(int fid, int rid, int candidate, char title, char oldtitle)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;
	ic->second->OnAppoint(rid, candidate, title, oldtitle);
}

void FactionManager::SetDelete(int fid, time_t deletetime)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;
//	ic->second->info.deletetime = deletetime;

	if (deletetime > 0)  // delete faction
	{
		FactionDismiss_Re re(ERR_FC_PREDELSUCCESS, fid, 0, 0, 1);
		ic->second->Broadcast(re, re.localsid);

		ic->second->OnDel(deletetime);
		FacBaseManager::GetInstance()->OnFactionDel(fid);
	}
	else // undelete faction
	{
		FactionDismiss_Re re(ERR_SUCCESS, fid, 0, 0, 0);
		ic->second->Broadcast(re, re.localsid);

		ic->second->OnUnDel();
	}
}

void FactionManager::OnDelete(int fid)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;

	FactionDetailInfo* detail = ic->second;

	SNSPlayerInfoManager::GetInstance()->OnFactionDelete(detail);

	for(FamilyContainer::iterator it=detail->families.begin(),ie=detail->families.end();it!=ie;++it)
	{
		if (!it->second)
			continue;
		it->second->OnFactionDelete();
	}
	detail->families.clear();

	/*
	PostDeleteFactionArg arg(GDeliveryServer::GetInstance()->zoneid,fid,ic->second->info.name);
	PostDeleteFaction* rpc = (PostDeleteFaction*) Rpc::Call(RPC_POSTDELETEFACTION, arg);
	UniqueNameClient::GetInstance()->SendProtocol(rpc);
	*/
	delete detail;
	factions.erase(ic);
}

void FactionManager::OnCreate(GFactionInfo& info)
{
	FactionDetailInfo*  detail = new FactionDetailInfo(info);
	factions[info.fid] = detail;
	FamilyContainer::iterator it = detail->families.begin();
	if (it!=detail->families.end())
	{
		it->second->JoinFaction(detail);
		//called after JoinFaction
		GTPlatformAgent::FactionAddMember(info,it->second->info,it->second->list);
		//TSPlatformAgent::AddFaction(it->second->info.id, info.fid);
	}

	SyncFactionLevel data;
	data.list.push_back(FactionLevel(info.fid,0));
	GProviderServer::GetInstance()->BroadcastProtocol(data);
}

int FactionManager::GetLevels(std::vector<FactionLevel>& list)
{
	for(Map::iterator it=factions.begin(),ie=factions.end();it!=ie;++it)
		list.push_back(FactionLevel(it->first,it->second->info.level));
	return list.size();
}

int FactionManager::GetHostiles(SyncFactionHostiles& sync)
{
	for(Map::iterator it=factions.begin(),ie=factions.end();it!=ie;++it)
		it->second->GetFactionHostiles(sync);
	return sync.list.size();
}

int FactionManager::GetLevel(int fid) const
{
	Map::const_iterator ic = factions.find(fid);
	if(ic==factions.end())
		return 0;
	return ic->second->info.level;
}

void FactionManager::OnGradeChg(int fid, int roleid, int level)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;
	ic->second->info.level = level;;
	/*
	if (level == FAC_ACT_START_LEV)
		FacBaseManager::GetInstance()->TryStartBase(fid); 
		*/
	FactionUpgrade_Re re(ERR_SUCCESS,level,0,0);
	ic->second->Broadcast(re, re.localsid);

	SyncFactionLevel data;
	data.list.push_back(FactionLevel(fid,level));
	GProviderServer::GetInstance()->BroadcastProtocol(data);
}
bool FactionManager::GetName(int fid, Octets& name)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	name = ic->second->info.name;
	return true;
}

bool FactionManager::GetBrief(int fid, Octets& name, int& level)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	name = ic->second->info.name;
	level = ic->second->info.level;
	return true;
}

bool FactionManager::FindMaster(int fid, int &roleid)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	FactionDetailInfo* detail = ic->second;
	for(FamilyContainer::iterator it=detail->families.begin(),ie=detail->families.end();it!=ie;++it)
	{
		FamilyInfo * family = it->second;
		if (!family)
			continue;
		for(RoleContainer::iterator rit=family->list.begin(); rit!=family->list.end()&&rit->gameid>=0; ++rit)
		{
			if(rit->role.title==TITLE_MASTER)
			{
				roleid = rit->role.rid;
				return true;
			}
		}
	}
	return false;
}
bool FactionManager::FindMaster(int fid, unsigned int& linksid, unsigned int &localsid)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	FactionDetailInfo* detail = ic->second;
	for(FamilyContainer::iterator it=detail->families.begin(),ie=detail->families.end();it!=ie;++it)
	{
		FamilyInfo * family = it->second;
		if (!family)
			continue;
		for(RoleContainer::iterator rit=family->list.begin(); rit!=family->list.end()&&rit->gameid>=0; ++rit)
		{
			if(rit->role.title==TITLE_MASTER)
			{
				linksid = rit->linksid;
				localsid = rit->localsid;
				return true;
			}
		}
	}
	return false;
}

bool FactionManager::FindVice(int fid, int& roleid)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	FactionDetailInfo* detail = ic->second;
	for(FamilyContainer::iterator it=detail->families.begin(),ie=detail->families.end();it!=ie;++it)
	{
		FamilyInfo * family = it->second;
		if (!family)
			continue;
		for(RoleContainer::iterator rit=family->list.begin(); rit!=family->list.end()&&rit->gameid>=0; ++rit)
		{
			if(rit->role.title==TITLE_VICEMASTER)
			{
				roleid = rit->role.rid;
				return true;
			}
		}
	}
	return false;
}
bool FactionManager::FindVice(int fid, unsigned int& linksid, unsigned int &localsid)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	FactionDetailInfo* detail = ic->second;
	for(FamilyContainer::iterator it=detail->families.begin(),ie=detail->families.end();it!=ie;++it)
	{
		FamilyInfo * family = it->second;
		if (!family)
			continue;
		for(RoleContainer::iterator rit=family->list.begin(); rit!=family->list.end()&&rit->gameid>=0; ++rit)
		{
			if(rit->role.title==TITLE_VICEMASTER)
			{
				linksid = rit->linksid;
				localsid = rit->localsid;
				return true;
			}
		}
	}
	return false;
}

bool FactionManager::HasProsperity(int fid, unsigned int dec)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	FactionDetailInfo* detail = ic->second;
	if(detail->info.prosperity<dec || dec<0)
		return false;
	return true;
}

bool FactionManager::DecProsperity(int fid, unsigned int dec)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return false;
	FactionDetailInfo* detail = ic->second;
	if(detail->info.prosperity<dec || dec<0)
		return false;
	detail->info.prosperity -= dec;
	detail->info.contribution -= dec;
	return true;
}

bool FactionManager::OnProsperityChange(int fid, int pros_delta)
{
	Map::const_iterator ic = factions.find(fid);
	if (ic == factions.end())
		return false;
	FactionDetailInfo * info = ic->second;

	info->info.prosperity += pros_delta;
	info->info.contribution += pros_delta;

	if (CanDegrade(info->info.level, info->info.prosperity))
	{
		int new_level = ::GetLevel(info->info.prosperity);
		DBFactionUpdateArg arg;
		arg.fid = fid;
		arg.level = new_level;
		arg.prosperity = info->info.prosperity;
		arg.announce = info->info.announce;
		arg.reason = 1;
		DBFactionUpdate* rpc = (DBFactionUpdate*) Rpc::Call( RPC_DBFACTIONUPDATE,arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	return true;
}
/*
void FactionManager::ChatBroadCast(int fid, unsigned char channel, const Octets &msg)
{
	WorldChat chat;
	chat.msg = msg;
	chat.channel = GP_CHAT_SYSTEM;
	chat.roleid = channel;
	GetName(fid, chat.name);
	LinkServer::GetInstance().BroadcastProtocol(chat);
}
*/
bool FactionManager::CanLeave(int familyid, int fid, int& contribution)
{
	FamilyInfo * family = FamilyManager::Instance()->Find(familyid);
	if (!family || family->info.factionid != (unsigned int)fid)
		return false;
	contribution = family->GetContribution();
	return true;
}
void FactionManager::OnLeave(int fid, int familyid, int master, int contribution)
{
	Map::iterator ic = factions.find(fid);
	if(ic==factions.end())
		return;
		
	FamilyContainer::iterator it = ic->second->families.find(familyid);
	if (it == ic->second->families.end() || it->second == NULL)
		return;	

	int multi_exp = 0, end_time = 0;
	if (ic->second->GetMultiExp(multi_exp, end_time))
	{
		FactionMultiExpSync sync(0, 0, 0);
		it->second->BroadcastGS(sync, sync.roleid);
	}

	FAC_DYNAMIC::member_change log = {FAC_DYNAMIC::member_change::FAMILY_LEAVE, 0, {}, 20, {} };
	FactionDynamic::GetName(it->second->info.name, log.familyname, log.familynamesize);
	ic->second->GetDynamic().RecordDynamic(Timer::GetTime(), FAC_DYNAMIC::MEMBER_CHANGE, log);
	
	GTPlatformAgent::FactionDelMember(fid,it->second->info,it->second->list);
	//TSPlatformAgent::LeaveFaction(it->second->info.id, fid);

	ic->second->family_count--;
	if(master)
	{
		ic->second->expel_count++;
		FactionExpel_Re re(0, master, familyid, familyid, it->second->info.name, 0, 0);
		ic->second->Broadcast(re, re.localsid);
	}
	else
	{
		FactionLeave_Re re(0, familyid, familyid, it->second->info.name, 0, 0);
		ic->second->Broadcast(re, re.localsid);
	}

	ic->second->info.contribution += contribution;
	ic->second->SetNormalDirty();
	it->second->LeaveFaction();
	ic->second->families.erase(it);
	return;
}
bool FactionManager::FillUpdateArg(int fid, int roleid, DBFactionUpdateArg& arg)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return false;
	GFactionInfo& info = it->second->info;
	arg.fid = fid;
	arg.level = info.level;
	arg.prosperity = info.prosperity;;
	arg.announce = info.announce;
	arg.rid = roleid;
	if(arg.reason)
	{
		if(!CanUpgrade(info, roleid))
			return false;
		arg.level++;
	}
	return true;
}
int FactionManager::GetNimbus(int fid)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return 0;
	return it->second->info.nimbus;
}

void FactionManager::UpdateNimbus(int fid, int addend)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return;
	it->second->info.nimbus += addend;
	it->second->SetNormalDirty();

	FactionChat msg;
	msg.src = MSG_FACTIONNIMBUS;
	msg.channel = GP_CHAT_SYSTEM;
	Marshal::OctetsStream data;
	data<<(int)it->second->info.nimbus<<(int)0;
	msg.msg = data;
	it->second->Broadcast(msg, msg.localsid);
}

bool FactionManager::GetMemberinfo(int fid, int rid, int& familyid, char& title)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return false;
	return it->second->GetMemberinfo(rid, familyid, title);
}

void FactionManager::Sync2Game(int rid,int factionid,int familyid,int contribution, int devotion)
{
	FamilyManager::Instance()->Sync2Game(familyid, rid, contribution,devotion);
}

bool  FactionDetailInfo::GetMemberinfo(int rid, int& familyid, char& title)
{
	GetMember();

        RoleList::iterator it, ie;
	ie = rolecache.end();
	for(it=rolecache.begin();it!=ie;++it)
	{
		if((int)it->role->role.rid==rid)
		{
			title = it->role->role.title;
		       	familyid = (int)it->familyid;
			return true;
		}
	}
	return false;
}

void FactionSaver::DoSave()
{
	if (!isWork)
		return;
	FactionManager::Map& factions = FactionManager::Instance()->factions;
	FactionManager::Map::iterator it = factions.upper_bound(curcor);
	if(it != factions.end())
	{
		curcor = it->first;
		bool needsync = false;
		it->second->Save2DB(needsync, true);
		return;
	}
	isWork = false;
}

void FactionManager::Save2DB()
{
	saver.Reset();
	saver.Begin();
}

void FactionManager::OnSync(int fid) { saver.OnSync(fid);}

void FactionManager::OnDeleteHostile(int factionid, std::vector<unsigned int> & hostile) 
{ 
	Map::iterator it = factions.find(factionid);
	if(it==factions.end()) return;
	it->second->OnDeleteHostile(hostile);

	std::vector<unsigned int> vec;
	vec.push_back(factionid);
	for (std::vector<unsigned int>::iterator hit = hostile.begin(); hit != hostile.end(); ++ hit)
	{
		Map::iterator it = factions.find(*hit);
		if (it != factions.end())
			it->second->OnDeleteHostile(vec);
	}
}

void FactionManager::OnAddHostile(int factionid, int hostile, Octets & name) 
{ 
	Map::iterator it = factions.find(factionid);
	if(it==factions.end())
		return;
	it->second->OnAddHostile(hostile, name, true);
	Octets fname = it->second->info.name;
	
	it = factions.find(hostile);
	if (it != factions.end())
	{
		it->second->OnAddHostile(factionid, fname);
	}
}

bool FactionManager::GetHostileInfo(int fid, HostileInfo &hostileinfo)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return false;
	hostileinfo = ::GetHostileInfo(it->second->info);
	return true;
}

int FactionManager::CanAddHostile(int fid, int hostile)
{
	Map::iterator it = factions.find(fid);
	if (it==factions.end())
		return -1;
	FactionDetailInfo * faction = it->second;
	if (::GetHostileInfo(faction->info).actionpoint < 1)
		return ERR_HOSTILE_ITEM;
	if (HostileProtected(::GetHostileInfo(faction->info).status))
		return ERR_HOSTILE_PROTECTED;
	if (Hostiles(faction->info).size() >= FactionDetailInfo::HOSTILE_MAX)
		return ERR_HOSTILE_FULL;
	std::vector<HostileFaction>::iterator hit = Hostiles(faction->info).begin(), hie = Hostiles(faction->info).end();
	for (; hit != hie; ++ hit)
	{
		if (hit->fid == (unsigned int)hostile)
			return ERR_HOSTILE_ALREADY;
	}
	
	it = factions.find(hostile);
	if (it == factions.end())
		return ERR_SUCCESS;
	FactionDetailInfo * peer = it->second;
	if (HostileProtected(::GetHostileInfo(peer->info).status))
		return ERR_HOSTILE_PEER_PROTECTED;
	if (Hostiles(peer->info).size() >= FactionDetailInfo::HOSTILE_MAX)
		return ERR_HOSTILE_PEER_FULL;

	hit = Hostiles(peer->info).begin(); 
	hie = Hostiles(peer->info).end();
	for (; hit != hie; ++ hit)
	{
		if (hit->fid == (unsigned int)fid)
			return ERR_HOSTILE_ALREADY;
	}
	if (faction->info.level == 0 || peer->info.level == 0 || std::abs(faction->info.level - peer->info.level) > 1)
		return ERR_HOSTILE_LEVEL_LIMIT;
	return ERR_SUCCESS;
}
int FactionManager::CanDeleteHostile(int fid, int hostile)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return -1;
	FactionDetailInfo * faction = it->second;
	std::vector<HostileFaction>::iterator hit = Hostiles(faction->info).begin(), hie = Hostiles(faction->info).end();
	for (; hit != hie; ++ hit)
	{
		if (hit->fid == (unsigned int)hostile)
		{
			if (Timer::GetTime() > hit->addtime + HOSTILE_MINIMUM)
				return ERR_SUCCESS;
			else
				return ERR_HOSTILE_COOLING;
			
		}

	}
	return -1;
}
void FactionManager::OnHostileUpdate(DBHostileUpdateArg & arg)
{
	Map::iterator it = factions.find(arg.factionid);
	if(it==factions.end())
		return;
	FactionDetailInfo * faction = it->second;
	if (arg.datamask & OP_PROTECT)
	{
		::GetHostileInfo(faction->info).status &= ~HOSTILE_PROTECTED;
	}
	if (arg.datamask & OP_ACTIONPOINT)
	{
		::GetHostileInfo(faction->info).actionpoint = arg.actionpoint;
	}
}

void FactionManager::OnHostileProtect(DBHostileProtectArg & arg)
{
	Map::iterator it = factions.find(arg.factionid);
	if(it==factions.end())
		return;
	FactionDetailInfo * faction = it->second;
	::GetHostileInfo(faction->info).status |= HOSTILE_PROTECTED;
	::GetHostileInfo(faction->info).protecttime = Timer::GetTime();
	std::vector<HostileFaction>::iterator hit = Hostiles(faction->info).begin(), hie = Hostiles(faction->info).end();
	for (; hit != hie; ++ hit)
	{
		faction->OnDeleteHostile(hit->fid);

		it = factions.find(hit->fid);
		if (it != factions.end())
		{
			it->second->OnDeleteHostile(arg.factionid);
		}

	}
	Hostiles(faction->info).clear();
}
bool FactionManager::IsMaster(int fid, int roleid)
{
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return false;
	return it->second->GetMasterID()==roleid;
}
void FactionManager::SendSiegeMessage(int fid, int type, Octets & msg)
{
	if (fid == 0)
		return;
	Map::iterator it = factions.find(fid);
	if(it==factions.end())
		return;
	return it->second->SendSiegeMessage(type, msg);
}

int FactionManager::CanChangeName(int fid, int roleid, const Octets & newname)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return ERR_FC_FACTION_NOTEXIST;
	if (it->second->info.master != (unsigned int)roleid)
		return ERR_FC_NO_PRIVILEGE;
	if (Timer::GetTime()-it->second->info.changenametime < 604800)
		return ERR_FC_CHANGENAME_CD;
	if (newname == it->second->info.name)
		return ERR_FC_CREATE_DUP;
	if (!ValidName(newname))
		return ERR_FC_INVALIDNAME;
	return ERR_SUCCESS;
}

void FactionManager::OnNameChange(int fid, const Octets & newname, char scale)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->info.name = newname;
	it->second->info.changenametime = Timer::GetTime();
//	FactionNameChange notice(newname, scale);
//	it->second->Broadcast(notice, notice.localsid);
	GTPlatformAgent::FactionUpdate(FACTIONTYPE,fid,newname,FAC_UPDATE_NAME);
	RaidManager::GetInstance()->OnFactionNameChange(fid,newname);
	GChangeFactionName_Re re(ERR_SUCCESS, 0, fid, newname, scale);
	it->second->BroadcastGS(re, re.roleid);
}

void FactionManager::DebugClearCD(int fid)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->info.changenametime = 0;
	LOG_TRACE("clear faction %d cooldown", fid);
}

void FactionManager::AddFactionMoney(int fid, unsigned int delta_money)
{
	//save to db
	LOG_TRACE("FactionManager::AddFactionMoney fid=%d delta_money=%d",fid,delta_money);
	DBFactionAddMoneyArg arg(fid, delta_money);
	DBFactionAddMoney* rpc = (DBFactionAddMoney*) Rpc::Call( RPC_DBFACTIONADDMONEY,arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
}
void FactionManager::OnDBFactionAddMoney(int fid, unsigned int new_money)
{
	LOG_TRACE("FactionManager::OnDbAddFactionMoney fid=%d new_money=%d",fid,new_money);
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->OnDBFactionAddMoney(new_money);
}
void FactionManager::OnDBFactionWithDrawMoney(int fid, unsigned int new_money)
{
	LOG_TRACE("FactionManager::OnDbWithdrawFactionMoney fid=%d new_money=%d",fid,new_money);
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->OnDBFactionWithDrawMoney(new_money);
}
int FactionManager::WithdrawFactionMoney(int fid, int roleid)
{
	LOG_TRACE("FactionManager::WithdrawFactionMoney fid=%d roleid=%d",fid,roleid);
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return ERR_FC_FACTION_INEXIST;
	return it->second->WithdrawFactionMoney(roleid);
}
void FactionManager::GetDynamic(int fid, int pageid, int & totalsize, std::vector<Octets> & dynamic)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->GetDynamic().GetData(pageid, totalsize, dynamic);
}
/*
void FactionManager::GetMsg(int fid, int roleid, int pageid, int & totalsize, std::vector<FactionMsgItem> & msgs, char & haspost) const
{
	Map::const_iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->GetMsg(roleid, pageid, totalsize, msgs, haspost);
}
*/
/*
int FactionManager::PostMsg(int fid, int roleid, const Octets & rolename, const Octets & msg, char repost)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return ERR_FC_FACTION_INEXIST;
	return it->second->PostMsg(roleid, rolename, msg, repost);
}
*/
void FactionManager::OnActTopUpdate(int fac_act_toptime)
{
	act_clear_time = fac_act_toptime;
	Map::iterator it, ite = factions.end();
	for (it = factions.begin(); it != ite; ++it)
		it->second->CheckClearAct(act_clear_time);
}

int FactionManager::CreateBase(int fid, int roleid)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return ERR_FC_FACTION_INEXIST;
	return it->second->CreateBase(roleid);
}

void FactionManager::OnBaseCreate(int fid, int roleid, int ret)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->OnBaseCreate(roleid, ret);
}

void FactionManager::OnBaseStart(int fid)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->OnBaseStart();
}

void FactionManager::OnBaseStop(int fid)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->OnBaseStop();
}

int FactionManager::CanCreateBase(int fid, int roleid)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return ERR_FC_FACTION_INEXIST;
	return it->second->CanCreateBase(roleid);
}

void FactionManager::OnStartMultiExp(int fid, int multi, int end_time)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
	{
		Log::log(LOG_ERR, "fid %d start multi %d exp end_time %d, but not load", fid, multi, end_time);
		return;
	}
	it->second->OnStartMultiExp(multi, end_time);
}

void FactionManager::DebugClearDynamic(int fid)
{
	Map::iterator it = factions.find(fid);
	if (it == factions.end())
		return;
	it->second->GetDynamic().Clear();
}
};

