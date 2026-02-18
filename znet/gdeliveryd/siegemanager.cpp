#include "siegemanager.h"
#include "dbsiegeload.hrp"
#include "dbsiegeend.hrp"
#include "gamedbclient.hpp"
#include "siegetop_re.hpp"
#include "gdeliveryserver.hpp"
#include "chatsinglecast.hpp"
#include "factionmanager.h"
#include "gsiegeserverregister.hpp"
#include "factionmanager.h"
#include "dbfactionget.hrp"
#include "siegeenter_re.hpp"
#include "gsiegestatus.hpp"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"
#include "siegeinfoget_re.hpp"
#include "glog.h"

namespace GNET
{
void LoadFaction(int fid)
{
	FactionDetailInfo * faction = FactionManager::Instance()->Find(fid);
	if (!faction)
	{
		DBFactionGet* rpc = (DBFactionGet*) Rpc::Call( RPC_DBFACTIONGET,FactionId(fid));
		rpc->listener = new SiegeListener();
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return;
	}
	faction->SetKeepAlive();
}

int CityDetail::SendNormalStart()
{
	if (b_registed && b_loaded)
	{
		if (challengers.size())
		{
			GChallenger & challenger = challengers.front();
			int btime =  challenger.begin_time-450;
			GProviderServer::GetInstance()->DispatchProtocol(GetCommonServerID(),
					SendSiegeStart(battle_id, GetCommonTag(), owner, 0, 0, btime, 1));
			LOG_TRACE("Start Normal city, owner=%d, close_time=%s", owner, ctime((time_t*)&btime));
			return challenger.begin_time - 450;

			/*
			   if (clear_time > Timer::GetTime())
			   {
			   GProviderServer::GetInstance()->DispatchProtocol(GetCommonServerID(),
			   SendSiegeStart(battle_id, GetCommonTag(), owner, 0, 0, clear_time, 1));
			   return;
			   }
			 */
		}
		int btime =  AllocBeginTime() - 450;
		GProviderServer::GetInstance()->DispatchProtocol(GetCommonServerID(),
				SendSiegeStart(battle_id, GetCommonTag(), owner, 0, 0, btime, 1));
		LOG_TRACE("Start Normal city, owner=%d, close_time=%s", owner, ctime((time_t*)&btime));
		return btime;
	}
	return 0;
}

void CityDetail::Init(GCity & city)
{
	LOG_TRACE("CityDetail Init: battle_id=%d,owner=%d, challengers.size=%d", city.info.battle_id
			, city.info.owner.fid, city.challengers.size());
	battle_id = city.info.battle_id;
	status = STATUS_WAITING;
	owner = city.info.owner.fid;
	owner_master = city.info.owner.master;
	occupy_time = city.info.occupy_time;
	challengers = city.challengers;
	b_loaded = true;
	if (challengers.size())
	{
		GChallenger & challenger = challengers.front();
		attacker = challenger.challenger.fid;	
		assistant = challenger.assistant.fid;
	}
	if (owner) LoadFaction(owner);
	Challengers::iterator it = challengers.begin();
	for (; it != challengers.end(); ++it)
	{
		if (it->challenger.fid) LoadFaction(it->challenger.fid);
		if (it->assistant.fid) LoadFaction(it->assistant.fid);
	}
	SendNormalStart();
}

void CityDetail::Register(int gs_id, GSiegeServerRegister & info)
{
	battle_id = info.battle_id;
	if (!info.use_for_battle)
	{
		tag_common.Init(info.world_tag, gs_id);
		b_registed = true;
		SendNormalStart();
	}
	else
		tag_battle.Init(info.world_tag, gs_id);
	LOG_TRACE("CityDetail Register: battle_id=%d world_tag=%d, gs_id=%d, b_for_battle=%d", battle_id 
			, info.world_tag, gs_id, info.use_for_battle);
}
void CityDetail::OnSiegeEnd(int world_tag, int result)
{
	Log::formatlog("Siege","OnSiegeEnd battle_id=%d,owner=%d,attacker=%d,assistant=%d,result=%d"
		, battle_id, owner, attacker, assistant, result);
	if (world_tag != GetBattleTag())
		return;
	DBSiegeEndArg arg(battle_id, result, owner, attacker);
	DBSiegeEnd * rpc = (DBSiegeEnd *) Rpc::Call(RPC_DBSIEGEEND, arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
	status = STATUS_SAVING;
	timeout = Timer::GetTime() + 120;
}

void CityDetail::AfterSiegeEnd(int retcode, int result)
{
	/*
	owner_top.clear();
	attacker_top.clear();
	assistant_top.clear();
	*/
	status = STATUS_WAITING;
	timeout = Timer::GetTime() + 500;
	if (retcode != ERR_SUCCESS) 
	{
		Log::formatlog("Siege","AfterSiegeEnd retcode=%d,battle_id=%d,owner=%d,attacker=%d,assistant=%d,result=%d"
		, retcode, battle_id, owner, attacker, assistant, result);
		return;
	}
	GLog::action("waroff, cid=%d:def=%d:att=%d:time=%d:res=%d",
			battle_id,owner,attacker,(int)(Timer::GetTime()-(challengers.size()?challengers.front().begin_time:0)),result);
	if (result == SIEGE_ATTACKER_WIN)
	{
		owner = attacker;
		if (challengers.size())
			owner_master = challengers.front().challenger.master;
		occupy_time = Timer::GetTime();
		challengers.erase(challengers.begin());
		/*
		for (std::vector<GChallenger>::iterator it = challengers.begin(); it != challengers.end(); )
		{
			if (it->challenger.fid != owner)
				break;
			it = challengers.erase(it);
		}
		*/
		GProviderServer::GetInstance()->BroadcastProtocol(GSiegeStatus(battle_id, owner, occupy_time));
		SiegeInfoGet_Re re(ERR_SUCCESS);
		re.city.battle_id = battle_id;
		re.city.owner = GFactionMaster(owner, owner_master);
		re.city.occupy_time = occupy_time;
		LinkServer::GetInstance().BroadcastProtocol(re);
	}
	else
		challengers.erase(challengers.begin());
	attacker = assistant = 0;
	if (challengers.size())
	{
		GChallenger & challenger = challengers.front();
		attacker = challenger.challenger.fid;
		assistant = challenger.assistant.fid;
		
		Marshal::OctetsStream data;
		data << challengers.front().begin_time;

		//FactionManager::Instance()->SendSiegeMessage(owner, MSG_SIEGEOWNER, data);
		//FactionManager::Instance()->SendSiegeMessage(attacker, MSG_SIEGEATTACKER, data);
		//FactionManager::Instance()->SendSiegeMessage(assistant, MSG_SIEGEASSISTANT, data);
	}
	SendNormalStart();
}

void CityDetail::OnChallenge(std::vector<GChallenger> & _challengers)
{
	std::vector<GChallenger>::iterator it = _challengers.begin();
	for (; it != _challengers.end(); ++it)
	{
		///
		FAC_DYNAMIC::siege_ready log = {it->begin_time, 0, 20, {}, 20, {}, 20, {}};
		Octets def_name, att_name, ass_name;
		FactionManager::Instance()->GetName(owner, def_name);
		FactionDynamic::GetName(def_name, log.defendername, log.defendernamesize);
		FactionManager::Instance()->GetName(it->challenger.fid, att_name);
		FactionDynamic::GetName(att_name, log.attackername, log.attackernamesize);
		FactionManager::Instance()->GetName(it->assistant.fid, ass_name);
		FactionDynamic::GetName(ass_name, log.assistname, log.assistnamesize);
		if (owner)
		{
			log.self_role = FAC_DYNAMIC::siege_ready::DEFENDER;
			FactionManager::Instance()->RecordDynamic(owner, FAC_DYNAMIC::SIEGE_READY, log);
		}
		log.self_role = FAC_DYNAMIC::siege_ready::ATTACKER;
		FactionManager::Instance()->RecordDynamic(it->challenger.fid, FAC_DYNAMIC::SIEGE_READY, log);
		if (it->assistant.fid)
		{
			log.self_role = FAC_DYNAMIC::siege_ready::ASSISTANT;
			FactionManager::Instance()->RecordDynamic(it->assistant.fid, FAC_DYNAMIC::SIEGE_READY, log);
		}
		////
		challengers.push_back(*it);
		Log::formatlog("Siege","Challenge battle_id=%d,attacker=%d,begin_time=%d"
			, battle_id, it->challenger.fid, it->begin_time);
	}
	GChallenger & challenger = challengers.front();
	attacker = challenger.challenger.fid; 
	assistant = challenger.assistant.fid;

	LoadFaction(challengers.back().challenger.fid);
}

int CityDetail::CanChallenge(int fid, int &begin_time)
{
	if (challengers.size() >= SIEGE_QUEUE_MAX)
		return ERR_SIEGE_QUEUEFULL;
	if (fid == owner)
		return ERR_SIEGE_SELF;
	Challengers::iterator it = challengers.begin();
	for (; it != challengers.end(); ++it)
	{
		if (it->challenger.fid == fid)
			return ERR_SIEGE_CHALLENGED;
	}
	begin_time = AllocBeginTime();
	return ERR_SUCCESS;
}

int CityDetail::AllocBeginTime()
{
	if (challengers.size())
	{
		return challengers.back().begin_time + 86400;
	}
	else
	{
		time_t now = Timer::GetTime();
		struct tm dt;
		localtime_r(&now, &dt);
		dt.tm_sec = 0;
		dt.tm_min = 0;
		dt.tm_hour = 20;
		// 提前十分钟开，让gs准备
		return mktime(&dt)+86400-600;
	}
}
  
int CityDetail::CanSetAssistant(int _attacker)
{
	if (_attacker != attacker)
		return ERR_SIEGE_CHECKFAILED;

	int begin_time = challengers.front().begin_time;	
	int now = Timer::GetTime();
	if (now >= begin_time - T_BGN_SET_ASSISTANT && now <= begin_time - T_END_SET_ASSISTANT)
		return ERR_SUCCESS;
	else
		return ERR_SIEGE_TIMEERROR;
}

void CityDetail::OnSetAssistant(int _attacker, int _assistant, int _master)
{
	if (attacker != _attacker)
	{
		Log::formatlog("CityDetail","SetAssistant battle_id=%d,attacker=%d,faction=%d,assistant=%d"
				, battle_id, attacker, _attacker, _assistant);
	}
	Log::formatlog("Siege","SetAssistant battle_id=%d,attacker=%d,assistant=%d", battle_id, attacker, _assistant);
	challengers.front().assistant = GFactionMaster(_assistant, _master); 
	assistant = _assistant;
	LoadFaction(assistant);
}

void CityDetail::OnStart(int retcode, int world_tag)
{
	if (world_tag == GetCommonTag())
	{
		if (retcode != ERR_SUCCESS)
		{
			Log::formatlog("CityDetail","Start CommonTag failed battle_id=%d,world_tag=%d", battle_id, world_tag);
		}
		return;
	}
	if (retcode == ERR_SUCCESS)
	{
		status = STATUS_FIGHTING; 
		timeout = Timer::GetTime() + BATTLE_TIME + 120;
		update_count = 0;
		update_time = Timer::GetTime();
		attacker_top.clear();
		owner_top.clear();
		assistant_top.clear();
		Octets msg;
		ChatBroadCast cbc;
		cbc.channel = GP_CHAT_SYSTEM;
		cbc.srcroleid = MSG_SIEGEBROADCAST;
		LinkServer::GetInstance().BroadcastProtocol(cbc);
		if (owner != 0)
			FactionManager::Instance()->SendSiegeMessage(owner, MSG_SIEGEFACTIONCAST, msg);
		if (attacker != 0)
			FactionManager::Instance()->SendSiegeMessage(attacker, MSG_SIEGEFACTIONCAST, msg);
		if (assistant != 0)
			FactionManager::Instance()->SendSiegeMessage(assistant, MSG_SIEGEFACTIONCAST, msg);
		GLog::action("waron, cid=%d:def=%d:att=%d",battle_id,owner,attacker);
	} 
	else
	{
		OnSiegeEnd(GetBattleTag(), SIEGE_CANCEL);
	}
}

void CityDetail::OnLogin(int roleid, int factionid, int sid, int localsid)
{
	unsigned char type;
	if (factionid == owner)
	{
		type = MSG_SIEGEOWNER;
	} 
	else if (factionid == attacker)
	{
		type = MSG_SIEGEATTACKER;
	}
	else if (factionid == assistant)
	{
		type = MSG_SIEGEASSISTANT;
	}
	else
		return;

	ChatSingleCast csc;
       	csc.channel = GP_CHAT_BROADCAST;
	csc.srcroleid = type;
	csc.dstroleid = roleid;	
	csc.dstlocalsid = localsid;
	Marshal::OctetsStream data;
	if (challengers.size())
		data << challengers.front().begin_time;
	else
		data << 0;
	csc.msg = data;
	GDeliveryServer::GetInstance()->Send(sid, csc);
}

void CityDetail::OnDisconnect(int gs_id)
{
	LOG_TRACE("SiegeManager OnDisconnect: gs_id=%d, status=%d",gs_id,status);
	if (GetBattleServerID() == gs_id && 
		(status == STATUS_SENDSTART || status == STATUS_FIGHTING))
	{
		OnSiegeEnd(GetBattleTag(), SIEGE_CANCEL);
		//tag_battle.gs_id = _GAMESERVER_ID_INVALID;	
	}
	/*
	if (GetCommonServerID() == gs_id && status == STATUS_WAITING)
	{
		tag_common.gs_id = _GAMESERVER_ID_INVALID;
	}
	*/
}
void CityDetail::GetTop(const RecordVector & vec, RecordVector & res)
{
	RecordVector::const_iterator ie = vec.end();
	if (vec.size() > 10)
		ie = vec.begin() + 10;
	std::copy(vec.begin(), ie, std::back_inserter(res));
}

int CityDetail::GetTop(SiegeTop_Re &re)
{
	int now = Timer::GetTime();
	if(update_count&& now - update_time > 15)
	{
		update_time = now;
		update_count = 0;
		std::sort(owner_top.begin(), owner_top.end(), CompareRecord());
		std::sort(attacker_top.begin(), attacker_top.end(), CompareRecord());
		std::sort(assistant_top.begin(), assistant_top.end(), CompareRecord());
	}
	GetTop(owner_top, re.owner_top);	
	GetTop(attacker_top, re.attacker_top);	
	GetTop(assistant_top, re.assistant_top);	
	return ERR_SUCCESS;
}
enum
{
	ACTION_BEKILL = 0,
	ACTION_KILL = 1,
};

void UpdateKiller(CityDetail::RecordVector & vec, int roleid) 
{
	CityDetail::RecordVector::iterator it = vec.begin();
	for(; it != vec.end(); ++it)
	{
		if (it->roleid == roleid)
		{
			it->kills++;
			break;
		}
	}
	if (it == vec.end())
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(!pinfo)
			return;
		vec.push_back(SiegeTopRecord(roleid, pinfo->name, 1, 0));
	}
}
void UpdateVictim(CityDetail::RecordVector & vec, int roleid) 
{
	CityDetail::RecordVector::iterator it = vec.begin();
	for(; it != vec.end(); ++it)
	{
		if (it->roleid == roleid)
		{
			it->deaths++;
			break;
		}
	}
	if (it == vec.end())
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(!pinfo)
			return;
		vec.push_back(SiegeTopRecord(roleid, pinfo->name, 0, 1));
	}

}

void CityDetail::UpdateTop(int roleid, int faction, int action)
{
	RecordVector * ptop = NULL;
	if (faction == owner)
	{
		ptop = &owner_top;
	}
	else if (faction == attacker)
	{
		ptop = &attacker_top;
	}
	else if (faction == assistant) 
	{
		ptop = &assistant_top;
	}
	if (ptop == NULL)
		return;
	if (action == ACTION_KILL)
		UpdateKiller(*ptop, roleid);
	else if (action == ACTION_BEKILL)
		UpdateVictim(*ptop, roleid);
	
}
void CityDetail::UpdateKill(int killer, int killer_faction, int victim, int victim_faction)
{
	update_count ++;
	UpdateTop(killer, killer_faction, ACTION_KILL);
	UpdateTop(victim, victim_faction, ACTION_BEKILL);
}

void CityDetail::GetFactionInfo(SiegeEnter_Re & re)
{
	re.owner.fid = owner;
	re.owner.master = owner_master;
	if (challengers.size())
	{
		GChallenger & challenger = challengers.front();
		re.attacker = challenger.challenger;
		re.assistant = challenger.assistant;
	}
	if (status == STATUS_FIGHTING)
		re.end_time = timeout - 120;
	else
		re.end_time = -1;
}
void CityDetail::SyncFaction(int factionid, int master)
{
	if (owner == factionid)
		owner_master = master;

	for (std::vector<GChallenger>::iterator it = challengers.begin(); it != challengers.end(); ++it)
	{
		if (it->challenger.fid == factionid)
		{
			it->challenger.master = master;
		}
		if (it->assistant.fid == factionid)
		{
			it->assistant.master = master;
		}
	}

}
///////////////////////////////////////////////////////////////////////////////

void SiegeManager::UpdateKill(int battle_id, int killer, int killer_faction, int victim, int victim_faction)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		city->UpdateKill(killer, killer_faction, victim, victim_faction);
	}
}
void SiegeManager::OnSiegeEnd(int battle_id, int world_tag, int result)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		city->OnSiegeEnd(world_tag, result);
	}
}

void SiegeManager::AfterSiegeEnd(int battle_id, int retcode, int result)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		city->AfterSiegeEnd(retcode, result);
	}
}

int SiegeManager::CanChallenge(int battle_id, int fid, int &begin_time)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		return city->CanChallenge(fid, begin_time);
	}
	return ERR_SIEGE_NOTFOUND;
}

void SiegeManager::OnChallenge(int battle_id, std::vector<GChallenger> & challengers)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		city->OnChallenge(challengers);
	}
}

int SiegeManager::CanSetAssistant(int battle_id, int attacker)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		return city->CanSetAssistant(attacker);
	}
	return ERR_SIEGE_NOTFOUND;
}

void SiegeManager::OnSetAssistant(int battle_id, int attacker, int assistant, int master)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		city->OnSetAssistant(attacker, assistant, master);
	}
}

void SiegeManager::OnStart(int retcode, int battle_id, int world_tag)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		city->OnStart(retcode, world_tag);
	}
}

void SiegeManager::OnDBConnect()
{
	if (!open)
		return;
	DBSiegeLoad * rpc = (DBSiegeLoad*) Rpc::Call(RPC_DBSIEGELOAD, DBSiegeLoadArg());
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

bool SiegeManager::Initialize()
{
	IntervalTimer::Attach(this, 1000000/IntervalTimer::Resolution());
	open = true;
	return true;
}

void SiegeManager::OnLogin(int roleid, int factionid, int sid, int localsid)
{
	if (!open)
		return;
	if (factionid == 0)
		return;
	Cities::iterator it = cities.begin();
	for (; it != cities.end(); ++it)
	{
		it->second.OnLogin(roleid, factionid, sid, localsid);
	}
}
void SiegeManager::Register(int gs_id, GSiegeServerRegister & info)
{
	cities[info.battle_id].Register(gs_id, info);
}
void SiegeManager::OnDisconnect(int gs_id)
{
	for (Cities::iterator it = cities.begin(); it != cities.end(); ++it)
	{
		it->second.OnDisconnect(gs_id);
	}
}

int SiegeManager::GetTop(int battle_id, SiegeTop_Re &re)
{
	CityDetail * city = Find(battle_id);
	if (city)
	{
		city->GetTop(re);
	}
	return ERR_SIEGE_NOTFOUND;
}
void SiegeManager::SyncFaction(int factionid, int master)
{
	Cities::iterator it = cities.begin();
	for (; it != cities.end(); ++it)
	{
		it->second.SyncFaction(factionid, master);
	}
}
void SiegeManager::SendStatus(int sid)
{
	Cities::iterator it = cities.begin();
	for (; it != cities.end(); ++it)
	{
		GSiegeStatus status(it->first);
		status.owner = it->second.owner;
		status.occupy_time = it->second.occupy_time;
		GProviderServer::GetInstance()->Send(sid, status);
	}
}
} // namespace GNET
