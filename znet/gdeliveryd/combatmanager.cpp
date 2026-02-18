#include <algorithm>
#include "log.h"
#include "itimer.h"
#include "combatmanager.h"
#include "factionmanager.h"
#include "combatcontrol.hpp"
#include "combatstatus.hpp"
#include "gproviderserver.hpp"
#include "maplinkserver.h"
#include "mapuser.h"
#include "chatbroadcast.hpp"
#include "localmacro.h"
#include "combatrecord"
#include "battlemanager.h"

namespace GNET
{

CombatController::CombatController(int fid1, int fid2, const Octets& name1, const Octets& name2)
       	: attacker(fid1),defender(fid2),attacker_name(name1),defender_name(name2),attacker_kills(0),defender_kills(0)
{
	mapid = -1;
	updatetime = 0;
	updatecount = 0;
	refcount = 0;
	tick = 0;
	endtime = 0;
	status = COMBAT_CREATING;
	AddRef();
	IntervalTimer::Attach(this,5000000/IntervalTimer::Resolution());
	Broadcast(MSG_COMBATCHALLENGE);
}

CombatController::CombatController(int map,int fid1, int fid2) : attacker(fid1),defender(fid2),attacker_kills(0),defender_kills(0)
{
	mapid = map;
	updatetime = 0;
	updatecount = 0;
	refcount = 0;
	tick = 0;
	endtime = 0;
	status = COMBAT_CREATING;
	AddRef();
	IntervalTimer::Attach(this,5000000/IntervalTimer::Resolution());
}

void CombatController::OnBattleStart()
{
	DEBUG_PRINT("CombatController: start , attacker=%d, defender=%d", attacker, defender);
	if(mapid>=0)
	{
		status = COMBAT_FIGHTING;
		endtime = Timer::GetTime() + 8000;
	}
}

void CombatController::OnBattleEnd()
{
	DEBUG_PRINT("CombatController: end , attacker=%d, defender=%d", attacker, defender);
	if(mapid>=0)
	{
		status = COMBAT_CLOSED;
	}
}

void CombatController::OnAccept(int fid1, int fid2)
{
	if(attacker!=fid1 || defender!=fid2 || status!=COMBAT_CREATING)
		return;
	Log::formatlog("combatstart","attacker=%d:defender=%d", attacker, defender);
	status = COMBAT_FIGHTING;
	endtime = Timer::GetTime() + 3600;
	Broadcast(MSG_COMBATSTART);

	CombatControl cmd;
	cmd.cmd = 1;
	cmd.attacker = attacker;
	cmd.defender = defender;
	cmd.endtime = endtime+5;
	GProviderServer::GetInstance()->BroadcastProtocol(cmd);
	cmd.endtime -= 5;
	FactionManager::Instance()->Broadcast(attacker, cmd, cmd.localsid);
	FactionManager::Instance()->Broadcast(defender, cmd, cmd.localsid);
	tick = 2;
}

void CombatController::OnRefuse(int fid1, int fid2)
{
	if(attacker!=fid1 || defender!=fid2 || status!=COMBAT_CREATING)
		return;
	Broadcast(MSG_COMBATREFUSE);
	status = COMBAT_CLOSED;
}
void CombatController::DecRef() 
{ 
	refcount--; 
	if(refcount<=0)
	{
		DEBUG_PRINT("CombatController: erasing controller, attacker=%d, defender=%d", attacker, defender);
		delete this;
	}
}
void CombatController::UpdateKill(int killer, int victim, int victim_faction)
{
	updatecount++;
	if(victim_faction==attacker)
		defender_kills++;
	else
		attacker_kills++;
	for(RecordVector::iterator it=records.begin(),ie=records.end();it!=ie;++it)
	{
		if(it->roleid==killer)
		{
			it->kills++;
			killer = 0;
		}
		else if(it->roleid==victim)
		{
			it->deaths++;
			victim = 0;
		}
	}
	if(killer)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(killer);
		if(!pinfo)
			return;
		records.push_back(CombatRecord(killer, pinfo->name, pinfo->factionid, 1, 0));
	}
	if(victim)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(victim);
		if(!pinfo)
			return;
		records.push_back(CombatRecord(victim, pinfo->name, pinfo->factionid, 0, 1));
	}
}

void CombatController::GetTop(int& handle, RecordVector& list, int& att, int& def)
{
	time_t now = Timer::GetTime();
	if(updatecount && now-updatetime>15)
	{
		updatetime = now;
		updatecount = 0;
		std::sort(records.begin(), records.end(), CompareRecord());
	}
	int size = records.size();
	if(size==0)
		return;
	if(handle<0 || handle>(size-1)/MAX_PLAYER_PERPAGE)
		handle = (size-1)/MAX_PLAYER_PERPAGE;

	RecordVector::iterator it,ie = records.end();
	it = records.begin();
	std::advance(it, handle*MAX_PLAYER_PERPAGE);
	for(int n=0;it!=ie && n<MAX_PLAYER_PERPAGE;++it,++n)
		list.push_back(*it);

	att = attacker;
	def = defender;
}

bool CombatController::Update()
{
	tick++;
	time_t now = Timer::GetTime();
	DEBUG_PRINT("CombatController::Update, attacker=%d, defender=%d", attacker, defender);
	if(status == COMBAT_CLOSED)
	{
		DEBUG_PRINT("CombatController::Update, closed, attacker=%d, defender=%d", attacker, defender);
		DecRef();
		return false;
	}
	if(status==COMBAT_CREATING)
	{
		if(mapid==-1 && tick==5)
		{
			DEBUG_PRINT("CombatController::Update, timeout, attacker=%d, defender=%d", attacker, defender);
			status = COMBAT_CLOSED;
			Broadcast(MSG_COMBATTIMEOUT);
		}
		return true;
	}
	if(now>endtime)
	{
		if(mapid==-1)
		{
			CombatControl cmd;
			cmd.cmd = 0;
			cmd.attacker = attacker;
			cmd.defender = defender;
			cmd.endtime = endtime;
			FactionManager::Instance()->Broadcast(attacker, cmd, cmd.localsid);
			FactionManager::Instance()->Broadcast(defender, cmd, cmd.localsid);
			BroadcastResult();
		}
		status = COMBAT_CLOSED;
		DecRef();
		return false;
	}
	if((tick%3)==0 || endtime-now<30 )
	{
		if(mapid==-1)
		{
			CombatControl cmd;
			cmd.cmd = 1;
			cmd.attacker = attacker;
			cmd.defender = defender;
			cmd.endtime = endtime;
			GProviderServer::GetInstance()->BroadcastProtocol(cmd);
		}
		BroadcastStatus(now);
	}
	return true;
}

void CombatController::Broadcast(int messageid)
{
	ChatBroadCast cbc;
	cbc.channel = GP_CHAT_SYSTEM;
	cbc.srcroleid = messageid; 
	Marshal::OctetsStream data;
	data<<(int)attacker<<(int)defender<<attacker_name<<defender_name;
	cbc.msg = data;
	LinkServer::GetInstance().BroadcastProtocol(cbc);
}

void CombatController::BroadcastStatus(time_t now)
{
	if(mapid==-1 || (updatecount || now-updatetime<20))
	{
		CombatStatus status;
		status.mapid = mapid;
		status.attacker = attacker;
		status.defender = defender;
		status.attacker_kills = attacker_kills;
		status.defender_kills = defender_kills;
		status.remain_time = mapid>=0 ? -1 : (endtime - now);
		FactionManager::Instance()->Broadcast(attacker, status, status.localsid);
		FactionManager::Instance()->Broadcast(defender, status, status.localsid);
	}
}

void CombatController::BroadcastResult()
{
	Log::formatlog("combatstop","attacker=%d:defender=%d:attacker_kills=%d:defender_kills=%d",
		attacker, defender, attacker_kills, defender_kills);
	ChatBroadCast cbc;
	cbc.channel = GP_CHAT_SYSTEM;
	cbc.srcroleid = MSG_COMBATEND; 
	Marshal::OctetsStream data;
	data<<(int)attacker<<(int)defender<<attacker_name<<defender_name<<(int)attacker_kills<<(int)defender_kills;
	cbc.msg = data;
	LinkServer::GetInstance().BroadcastProtocol(cbc);
}

int CombatManager::CanChallenge(int attacker, int defender)
{
	if(Timer::GetTime()-coolings[attacker]<300)
		return ERR_COMBAT_COOLING;
	Map::iterator it = controllers.find(attacker);
	if(it!=controllers.end() && !it->second->IsClosed())
		return ERR_COMBAT_BUSY;
	it = controllers.find(defender);
	if(it!=controllers.end() && !it->second->IsClosed())
		return ERR_COMBAT_BUSY;
	/*
	if(!BattleManager::GetInstance()->CanCombat(attacker))
		return ERR_COMBAT_INBATTLE;
	if(!BattleManager::GetInstance()->CanCombat(defender))
		return ERR_COMBAT_INBATTLE;
	*/
	return 0;
}


};

