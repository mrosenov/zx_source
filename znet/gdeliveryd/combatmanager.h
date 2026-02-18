#ifndef __GNET_COMBATMANAGER
#define __GNET_COMBATMANAGER

#include <set>
#include "thread.h"
#include "combatrecord"
#include "localmacro.h"

namespace GNET
{
class CombatController : public IntervalTimer::Observer
{
	class CompareRecord
	{
	public:
		bool operator() (const CombatRecord& lhs, const CombatRecord& rhs)
		{
			return lhs.kills>rhs.kills || (lhs.kills==rhs.kills && lhs.deaths<rhs.deaths);
		}
	};
	enum
	{
		COMBAT_CREATING,
		COMBAT_FIGHTING,
		COMBAT_CLOSED,
	};
	enum
	{
		MAX_PLAYER_PERPAGE = 19,
	};
public:
	typedef std::vector<CombatRecord> RecordVector;
	RecordVector records;
	int updatetime;
	int updatecount;

	int status;
	int refcount;
	int endtime;
	int tick;

	int mapid; // -1 for normal combat, >=0 for battle
	int attacker;
	int defender;
	Octets attacker_name;
	Octets defender_name;
	int attacker_kills;
	int defender_kills;

	CombatController(int fid1, int fid2, const Octets& name1, const Octets& name2);
	CombatController(int map, int fid1, int fid2);

	bool Update();
	void AddRef() { refcount++; }
	void DecRef();
	void UpdateKill(int killer, int victim, int victim_faction);
	void Broadcast(int messageid);
	void BroadcastResult();
	void BroadcastStatus(time_t now);
	void GetTop(int &handle, RecordVector& list, int& att, int& def);
	void OnAccept(int fid1, int fid2);
	void OnRefuse(int fid1, int fid2);
	void OnBattleStart();
	void OnBattleEnd();
	bool IsClosed() { return status==COMBAT_CLOSED; }
};

class CombatManager
{
public:
	typedef std::map<int,CombatController*> Map;
	std::map<int,int> coolings;
	Map controllers;
	Map battles;

	void UpdateKill(int mapid, int killer, int victim, int victim_faction)
	{
		if(mapid>=0)
		{
			Map::iterator it = battles.find(mapid);
			if(it==battles.end())
				return;
			it->second->UpdateKill(killer, victim, victim_faction);
		}
		else
		{
			Map::iterator it = controllers.find(victim_faction);
			if(it==controllers.end())
				return;
			it->second->UpdateKill(killer, victim, victim_faction);
		}
	}

	void CreateController(int fid1, int fid2, const Octets& name1, const Octets& name2)
	{
		CombatController* controller = new CombatController(fid1, fid2, name1, name2);
		UpdateRef(fid1, fid2, controller);
	}

	void UpdateRef(int fid1, int fid2, CombatController* controller)
	{
		Map::iterator it = controllers.find(fid1);
		if(it!=controllers.end())
			it->second->DecRef();
		it = controllers.find(fid2);
		if(it!=controllers.end())
			it->second->DecRef();
		controllers[fid1] = controller;
		controller->AddRef();
		controllers[fid2] = controller;
		controller->AddRef();
		SetCooling(fid1);
	}

	bool GetTop(int factionid, int mapid, int& handle, std::vector<CombatRecord>& list, int& attacker, int& defender)
	{
		if(mapid<=0)
		{
			Map::iterator it = controllers.find(factionid);
			if(it==controllers.end())
				return false;
			it->second->GetTop(handle, list, attacker, defender);
		}
		else
		{
			Map::iterator it = battles.find(mapid);
			if(it==battles.end())
				return false;
			it->second->GetTop(handle, list, attacker, defender);
		}
		return true;
	}

	int CanChallenge(int attacker, int defender);

	void SetCooling(int factionid)
	{
		coolings[factionid] = Timer::GetTime();
	}

	void AcceptChallenge(int attacker, int defender)
	{
		Map::iterator it = controllers.find(attacker);
		if(it!=controllers.end())
			it->second->OnAccept(attacker, defender);
	}

	void RefuseChallenge(int attacker, int defender)
	{
		Map::iterator it = controllers.find(attacker);
		if(it!=controllers.end())
			it->second->OnRefuse(attacker, defender);
	}

	void BattleStart(int mapid,int fid1,int fid2)
	{
		CombatController* controller = new CombatController(mapid, fid1, fid2);
		Map::iterator it = battles.find(mapid);
		if(it!=battles.end())
			it->second->DecRef();
		battles[mapid] = controller;
		controller->AddRef();
		controller->OnBattleStart();
	}

	void BattleEnd(int attacker, int defender)
	{
		Map::iterator it = controllers.find(attacker);
		if(it!=controllers.end())
			it->second->OnBattleEnd();
	}

	static CombatManager* Instance() { static CombatManager instance; return &instance;}
};


};
#endif

