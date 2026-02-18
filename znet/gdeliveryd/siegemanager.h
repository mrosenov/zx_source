#ifndef __SIEGE_MANAGER_HPP_INCLUDE__
#define __SIEGE_MANAGER_HPP_INCLUDE__

#include "gcity"
#include "gchallenger"
#include "sendsiegestart.hpp"
#include "localmacro.h"
#include "gproviderserver.hpp"
#include "siegetoprecord"

namespace GNET
{

class GSiegeServerRegister;
class CityDetail;
class CityTag
{
public:
	CityTag(CityDetail * _city): city(_city), b_register(false), gs_id(_GAMESERVER_ID_INVALID) {}
	void Init(int _tag, int _gs_id)
	{
		b_register = true;
		tag = _tag; 
		gs_id = _gs_id;
	}
	bool IsUseable() { return b_register; }
	CityDetail * city;	
	bool b_register;
	int tag;
	int gs_id;
};
class CityDetail
{
public:
	class CompareRecord
	{
		public:
			bool operator() (const SiegeTopRecord& lhs, const SiegeTopRecord& rhs)
			{
				return lhs.kills>rhs.kills || (lhs.kills==rhs.kills && lhs.deaths<rhs.deaths);
			}
	};
	typedef std::vector<SiegeTopRecord> RecordVector;
	enum TIME
	{
		T_BGN_SET_ASSISTANT = 20*3600,
		T_END_SET_ASSISTANT = 6*3600,
	};
	enum STATUS
	{
		STATUS_INIT = 0,
		STATUS_WAITING = 1,
		STATUS_SENDSTART = 2,
		STATUS_FIGHTING = 3,
		STATUS_SAVING = 4,
	};

	enum
	{
		//BATTLE_TIME = 2000,
		BATTLE_TIME = 7200,
	};

	CityDetail() : status(STATUS_INIT),owner(0),owner_master(0),attacker(0), assistant(0), tag_common(this), tag_battle(this)
		       , update_count(0), update_time(0), b_loaded(false), b_registed(false), synctime(0) {}
	void Init(GCity & city);
	
	void Register(int gs_id, GSiegeServerRegister & info);
	
	void Update(int now)
	{
		switch (status)
		{
		case STATUS_INIT:
			return;
		case STATUS_WAITING:
			{
			if (timeout && now > timeout)
			{
				owner_top.clear();
				attacker_top.clear();
				assistant_top.clear();
				timeout = 0;
			}
			if (now > synctime)
			{
				int b_time = SendNormalStart();
				if (b_time)
					synctime = b_time - 1800;
			}
			if (challengers.size() == 0)
				return;
			GChallenger & challenger = challengers.front();
			if (challenger.begin_time <= now)
			{
				if (owner == attacker)
				{
					Log::formatlog("CityDetail", "battle canceled because owner equals challenger! battle_id=%d,owner=%d",battle_id,owner);
					OnSiegeEnd(GetBattleTag(), SIEGE_CANCEL);
				}
				else
				{
					if (SendStart()) 
					{
						status = STATUS_SENDSTART;
						timeout = now + 120;
					}
					else
					{
						Log::formatlog("CityDetail", "sendstart failed, battle_id=%d, gs_id=%d", battle_id, GetBattleServerID());
						OnSiegeEnd(GetBattleTag(), SIEGE_CANCEL);
					}
				}
			}
			}
			break;
		case STATUS_SENDSTART:
			{
				if (now > timeout)
				{
					Log::formatlog("CityDetail", "sendstart timeout, battle_id=%d, gs_id=%d", battle_id, GetBattleServerID());
					OnSiegeEnd(GetBattleTag(), SIEGE_CANCEL);
				}
			}
			break;
		case STATUS_FIGHTING:
			{
				if (now > timeout)
				{
					Log::formatlog("CityDetail", "fighting timeout, battle_id=%d, gs_id=%d", battle_id, GetBattleServerID());
					OnSiegeEnd(GetBattleTag(), SIEGE_TIMEOUT);
				}
			}
			break;
		case STATUS_SAVING:
			{
				if (now > timeout)
				{
					Log::formatlog("CityDetail", "status saving timeout, battle_id=%d", battle_id);
					status = STATUS_WAITING;
				}
			}
			break;
		}
	}

	void DebugStart()
	{
		LOG_TRACE("SiegeManager DebugStart: battle_id=%d,gs_id=%d,map_id=%d,status=%d"
			,battle_id,GetBattleServerID(),GetBattleTag(),status);
		if (status == STATUS_WAITING)
		{
			if (challengers.size() == 0)
				return;
			if (SendStart()) 
			{
				status = STATUS_SENDSTART;
				timeout = Timer::GetTime() + 120;
			}
		}
	}

	bool SendStart()
	{
		LOG_TRACE("SiegeManager SendStart: battle_id=%d,gs_id=%d,map_id=%d", battle_id, GetBattleServerID(),GetBattleTag());
		return GProviderServer::GetInstance()->DispatchProtocol(GetBattleServerID(), 
			SendSiegeStart(battle_id, GetBattleTag(), owner, attacker, assistant, Timer::GetTime() + BATTLE_TIME));
	}
	int SendNormalStart() ;
	

	int GetCommonServerID() { return tag_common.gs_id; }
	int GetBattleServerID() { return tag_battle.gs_id; }
	int GetCommonTag() { return tag_common.tag; }
	int GetBattleTag() { return tag_battle.tag; }
	int GetOwner() { return owner;}
	int GetAttacker() { return attacker; }
	int GetAssistant() { return assistant; }

	bool IsFighting() { return status != STATUS_INIT && status != STATUS_WAITING; }
	bool IsWaiting() { return status == STATUS_WAITING; }


	void OnSiegeEnd(int world_tag, int result);
	void AfterSiegeEnd(int retcode, int result);

	int  CanChallenge(int fid, int & begin_time);
	void OnChallenge(std::vector<GChallenger> & challengers);

	int CanSetAssistant(int fid);
	void OnSetAssistant(int attacker, int assistant, int master);

	void OnStart(int retcode, int world_tag);
	int  AllocBeginTime();

	int  	GetTop(class SiegeTop_Re & re);
	void 	GetTop(const RecordVector & vec, RecordVector & res);
	void 	UpdateTop(int roleid, int faction, int action);
	void 	UpdateKill(int killer, int killer_faction, int victim, int victim_faction);

	void  	OnLogin(int roleid, int factionid, int sid, int localsid);
	typedef std::vector<GChallenger> Challengers;
	void 	OnDisconnect(int gs_id);
	void 	GetFactionInfo(class SiegeEnter_Re & re);
	void 	SyncFaction(int factionid, int master);

	int battle_id;
	int status;
	int owner;
	int owner_master;
	int attacker;
	int assistant;
	int occupy_time;
	int timeout;
	Challengers challengers;
	CityTag tag_common;
	CityTag tag_battle;
	RecordVector owner_top;
	RecordVector attacker_top;
	RecordVector assistant_top;
	int update_count;
	int update_time;
	bool b_loaded;
	bool b_registed;
	int synctime;

};
class SiegeManager : public IntervalTimer::Observer
{
	enum {CHALLENGE_COOLTIME = 600};

public:
	SiegeManager():cooldown_cursor(0), open(false)
	{
	}
	CityDetail * Find(int battle_id)
	{
		Cities::iterator it = cities.find(battle_id);
		if (it != cities.end() && it->second.status != CityDetail::STATUS_INIT)
			return &(it->second);
		return NULL;
	}
	void OnLoad(std::vector<GCity> & dbdata)  
	{
		std::vector<GCity>::iterator it = dbdata.begin();
		for (; it != dbdata.end(); ++it)
		{
			CityDetail & city = cities[it->info.battle_id];
			city.Init(*it);
		}
	}
	void Register(int gs_id, GSiegeServerRegister & info);
	
	bool Initialize();
	void OnDBConnect();
	bool Update()
	{
		int now = Timer::GetTime();
		for(Cities::iterator it = cities.begin(); it != cities.end(); ++it)
		{
			it->second.Update(now);
		}

		CooldownMap::iterator it = cooldown.lower_bound(cooldown_cursor);
		CooldownMap::iterator ie = cooldown.end();
		for (int i = 0; i<10 && it!=ie; ++i)
		{
			if (it->second+CHALLENGE_COOLTIME < now)
				cooldown.erase(it++);
			else
				++it;
		}
		if (it == ie)
			cooldown_cursor = 0;
		else
			cooldown_cursor = it->first;

		return true;
	}
	
	static SiegeManager * GetInstance()
	{
		static SiegeManager inst;
		return & inst;	
	}

	void 	SyncFaction(int factionid, int master);
	void 	SendStatus(int sid);
	int  	CanChallenge(int battle_id, int fid, int &begin_time);
	void 	OnChallenge(int battle_id, std::vector<GChallenger> & challengers);

	void 	OnSiegeEnd(int battle_id, int world_tag, int result);
	void 	AfterSiegeEnd(int battle_id, int retcode, int result);

	int 	CanSetAssistant(int battle_id, int fid);
	void 	OnSetAssistant(int battle_id, int attacker, int assistant, int master);

	void 	OnStart(int retcode, int battle_id, int world_tag);

	int	GetInfo(int battle_id, GCityInfo & cityinfo, std::vector<GChallenger> & challengers)
	{
		CityDetail * city = Find(battle_id);
		if (city)
		{
			cityinfo.battle_id = battle_id;
		       	cityinfo.owner = GFactionMaster(city->owner, city->owner_master);
			cityinfo.occupy_time = city->occupy_time;	
			challengers = city->challengers;
			return ERR_SUCCESS;
		}
		return ERR_SIEGE_NOTFOUND;
	}
	void 	UpdateKill(int battle_id, int killer, int killer_faction, int victim, int victim_faction);
	int  	GetTop(int battle_id, class SiegeTop_Re & re);
	void  	OnLogin(int roleid, int factionid, int sid, int localsid);
	void 	OnDisconnect(int gs_id);
	typedef std::map<int, CityDetail> Cities;
	Cities cities;
	void 	SetCooldown(int fid)
	{
		cooldown[fid] = Timer::GetTime();
	}
	bool	IsInCooldown(int fid)
	{
		CooldownMap::iterator it = cooldown.find(fid);
		if (it == cooldown.end()) 
			return false;
		else
			return it->second+CHALLENGE_COOLTIME>=Timer::GetTime();
	}
	typedef std::map<int, time_t> CooldownMap;
	CooldownMap cooldown;
	int cooldown_cursor;
private:
	bool open;
	friend class DebugCommand;
	void 	DebugStart()
	{
		for (Cities::iterator it = cities.begin(); it != cities.end(); ++it)
		{
			it->second.DebugStart();
		}
	}

}; // class Siegemanager
}; // namespace GNET
#endif //__SIEGE_MANAGER_HPP_INCLUDE__
