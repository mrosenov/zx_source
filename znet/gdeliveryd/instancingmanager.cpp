#include <list>
#include <string.h>
#include "instancingmanager.h"
#include "mapuser.h"
#include "hashstring.h"
#include "groleforbid"
#include "groleinventory"
#include "instancingstart.hpp"
#include "sendinstancingstart.hpp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "factionchat.hpp"
#include "factionid"
#include "localmacro.h"
#include "topmanager.h"
#include "instancinggetfield_re.hpp"
#include "instancinggetlist_re.hpp"
#include "instancingstartnotify.hpp"
#include "instancingclosenotify.hpp"
#include "sendinstancingenter.hpp"
#include "instancingjoin_re.hpp"
#include "instancingjoinreq.hpp"
#include "dbinstancingactivate.hrp"
#include "gmailendsync.hpp"

namespace GNET
{
class RoleControl 
{
	enum
	{
		ACTIVATE_COUNT_LIMIT = 3,
		ENTER_COUNT_LIMIT = 3,
	};
	enum
	{
		ACTIVATE_COOLDOWN_INDEX = 0,
		ENTER_COOLDOWN_INDEX,
		MAX_COOLDOWN_INDEX,
	};
public:
	RoleControl() 
	{
		memset(count, 0, sizeof(count));
		memset(lasttime, 0, sizeof(lasttime));
	}
	void SetActivateCoolDown()
	{
		int now = Timer::GetTime();
		if (lasttime[ACTIVATE_COOLDOWN_INDEX]%86400 > now%86400)
			count[ACTIVATE_COOLDOWN_INDEX] = 1;
		else
			++count[ACTIVATE_COOLDOWN_INDEX];
		lasttime[ACTIVATE_COOLDOWN_INDEX] = now;
	}
	bool IsActivateCoolDown()
	{
		return count[ACTIVATE_COOLDOWN_INDEX]>=ACTIVATE_COUNT_LIMIT;
	}
	void SetEnterCoolDown()
	{
		int now = Timer::GetTime();
		if (lasttime[ENTER_COOLDOWN_INDEX]%86400 > now%86400)
			count[ENTER_COOLDOWN_INDEX] = 1;
		else
			++count[ENTER_COOLDOWN_INDEX];
		lasttime[ENTER_COOLDOWN_INDEX] = now;
	}
	bool IsEnterCoolDown()
	{
		return count[ENTER_COOLDOWN_INDEX]>=ENTER_COUNT_LIMIT;
	}
	int GetEnterCount()
	{
		return count[ENTER_COOLDOWN_INDEX];
	}
	void DecEnterCount()
	{
		if (count[ENTER_COOLDOWN_INDEX] > 0)
			count[ENTER_COOLDOWN_INDEX]--;
	}
private:
	int	count[MAX_COOLDOWN_INDEX];
	int lasttime[MAX_COOLDOWN_INDEX];
};

class RoleControlManager
{
public:
	RoleControlManager() : update_time(0){}
	void Init(int basetime)
	{
		update_time = basetime;
	}
	
	bool IsRoleActivateCoolDown(int roleid, int tid)
	{
		TidControlMap::iterator tit = control.find(tid);
		if (tit != control.end())
		{
			RoleControlMap::iterator it = tit->second.find(roleid);
			if (it != tit->second.end())
			{
				return it->second.IsActivateCoolDown();
			}
		}
		return false;
	}
	void SetRoleActivateCoolDown(int roleid, int tid)
	{
		control[tid][roleid].SetActivateCoolDown();
	}

	bool IsRoleEnterCoolDown(int roleid, int tid)
	{
		TidControlMap::iterator tit = control.find(tid);
		if (tit != control.end())
		{
			RoleControlMap::iterator it = tit->second.find(roleid);
			if (it != tit->second.end())
			{
				return it->second.IsEnterCoolDown();
			}
		}
		return false;
	}
	void DecRoleEnterCnt(int roleid, int tid)
	{
		TidControlMap::iterator tit = control.find(tid);
		if (tit == control.end())
			return;
		RoleControlMap::iterator it = tit->second.find(roleid);
		if (it == tit->second.end())
			return;
		it->second.DecEnterCount();
	}

	void SetRoleEnterCoolDown(int roleid, int tid)
	{
		control[tid][roleid].SetEnterCoolDown();
	}

	int GetRoleEnterCount(int roleid, int tid)
	{
		TidControlMap::iterator tit = control.find(tid);
		if (tit != control.end())
		{
			RoleControlMap::iterator it = tit->second.find(roleid);
			if (it != tit->second.end())
			{
				return it->second.GetEnterCount();
			}
		}
		return 0;
	}
	
	void Update(int now)
	{
		if (now - update_time >= 86400) 
		{
			update_time += 86400;
			control.clear();
		}
	}
private:
	typedef std::map<int, RoleControl> RoleControlMap;
	typedef std::map<int, RoleControlMap> TidControlMap;
	TidControlMap control;
	int update_time;
};
	
RoleControlManager control;
inline RoleControlManager & GetControl() { return control; }
////////////////////////////////////////////////////////////////////////////////////////////////

void InstancingField::BroadcastTeam(TeamType & team, Protocol & data, unsigned int & localsid)
{
	GDeliveryServer * dsm = GDeliveryServer::GetInstance();
	for (TeamType::const_iterator it = team.begin(); it != team.end(); ++it)
	{
		localsid = it->localsid;
		dsm->Send(it->linksid, data); 
	}
}
void InstancingField::ClearTeam(TeamType & team)
{
	for (TeamType::const_iterator it = team.begin(); it != team.end(); ++it)
	{
		server->EraseMapID(map_id,it->roleid);
	}
	team.clear();
}
void InstancingField::Broadcast(Protocol & data, unsigned int & localsid) 
{
	BroadcastTeam(team_attacker, data, localsid);
	BroadcastTeam(team_attacker_req, data, localsid);
	BroadcastTeam(team_defender, data, localsid);
	BroadcastTeam(team_defender_req, data, localsid);
	BroadcastTeam(team_looker, data, localsid);
	BroadcastTeam(team_looker_req, data, localsid);
}

void InstancingField::OnBattleStart(int retcode)
{
	LOG_TRACE("OnBattleStart:gs_id=%d,retcode=%d battle_id=%d",server->gs_id,retcode,battle_id);
	if (retcode != ERR_SUCCESS)
	{
		tag.SetStatus(BS_READY);
		status = ST_QUEUEING;
		Log::formatlog("OnBattleStart", "retcode=%d,gs_id=%d,map_id=%d,battle_id=%d", retcode,server->gs_id,map_id,battle_id);
		return;
	}
	Log::formatlog("OnBattleStart", "retcode=%d,gs_id=%d,map_id=%d,battle_id=%d", retcode,server->gs_id,map_id,battle_id);
	tag.SetStatus(BS_WAITRUNNING);
	status = ST_FIGHTING;
	//start_time = Timer::GetTime();已经在TryStart中设置了
	check_time = start_time;
	InstancingStartNotify notify(server->gs_id, map_id, start_time+fighting_time);
	Broadcast(notify, notify.localsid);
}
bool InstancingField::OnBattleEnterTeam(int roleid, TeamType & team, unsigned char team_id)
{
	for (TeamType::iterator it = team.begin(); it != team.end(); ++it)
	{
		if  (roleid == it->roleid)
		{
			it->role_status = ROLE_FIGHTING;
			LOG_TRACE("instancingmanager::OnBattleEnter:roleid=%d map_id=%d team=%d", roleid, map_id, team_id);
			Log::formatlog("instancing_stat","OnBattleEnter map_id=%d roleid=%d level=%d",map_id,roleid,it->level);
			SendInstancingEnter req(roleid, tag.tag, map_id, team_id);
			GProviderServer::GetInstance()->Send(server->sid, req);
			GetControl().SetRoleEnterCoolDown(roleid, tid);
			return true;
		}
	}
	return false;
}
int InstancingField::OnBattleEnter(int roleid, int &endtime)
{
	if (status == ST_QUEUEING) 
		return ERR_BATTLE_MAP_NOTEXIST;

	endtime = start_time+fighting_time;
	if (OnBattleEnterTeam(roleid, team_attacker, TEAM_ATTACKER)
		|| OnBattleEnterTeam(roleid, team_defender, TEAM_DEFENDER) 
		|| OnBattleEnterTeam(roleid, team_looker, TEAM_LOOKER)
		) 
		return ERR_SUCCESS;
	return ERR_BATTLE_NOT_INTEAM;
}

bool InstancingField::OnBattleEnd(int tag_id)
{
	LOG_TRACE("OnBattleEnd: gs_id=%d,map_id=%d",server->gs_id,map_id);
	status = ST_PREPARING;
	start_time = Timer::GetTime();
	server->OnBattleEnd(map_id, *this);
	team_attacker.clear();
	team_defender.clear();
	team_looker.clear();
	return true;
}

int InstancingField::JoinReqTeam(InstancingRole & role, unsigned char teamid, TeamType & team_req, TeamType & team_queue, InstancingSideInfo & sideinfo, GMailSyncData & syncdata)
{
	int retcode = JoinPolicy(role, sideinfo, team_queue);
	if (retcode != ERR_SUCCESS)
		return retcode;

	LOG_TRACE("InstancingField JoinReqTeam:roleid=%d,map_id=%d,team=%d,status=%d",role.roleid,map_id,teamid,status);
	team_req.push_back(role);
	if (team_queue.size() > 0)
	{
		InstancingRole & master = *team_queue.begin();
		InstancingJoinReq req(master.roleid, map_id, role.roleid, master.localsid);
		GDeliveryServer::GetInstance()->Send(master.linksid, req);
	}

	GProviderServer::GetInstance()->Send(server->sid, GMailEndSync(0, ERR_SUCCESS, role.roleid, syncdata));
	return ERR_SUCCESS;
}

void InstancingField::OnActivate(int retcode, InstancingRole & role, unsigned char teamid, TeamType & team_req, TeamType & team_queue, InstancingSideInfo & sideinfo, GMailSyncData & syncdata)
{
	LOG_TRACE("InstancingField OnActivate:retcode=%d, roleid=%d,map_id=%d,team=%d,status=%d",retcode, role.roleid,map_id,teamid,status);
	if (retcode == ERR_SUCCESS)
	{
		status = ST_QUEUEING;
		start_time = Timer::GetTime();
		role.jointime = Timer::GetTime();
		role.role_status = ROLE_QUEUEING;
		team_queue.push_back(role);
		GetControl().SetRoleActivateCoolDown(role.roleid, tid);
	}

	InstancingJoin_Re re(retcode, server->gs_id, map_id, teamid, start_time+queuing_time, role.localsid);
	GDeliveryServer::GetInstance()->Send(role.linksid, re);

	GProviderServer::GetInstance()->Send(server->sid, GMailEndSync(0, retcode, role.roleid, syncdata));
}

int InstancingField::Activate(InstancingRole & role, unsigned char teamid, TeamType & team_req, TeamType & team_queue, InstancingSideInfo & sideinfo, GMailSyncData & syncdata)
{

	LOG_TRACE("InstancingField Activate:roleid=%d,map_id=%d,team=%d,status=%d",role.roleid,map_id,teamid,status);

	int retcode = JoinPolicy(role, sideinfo, team_queue);
	if (retcode != ERR_SUCCESS)
		return retcode;

	// 激活次数，或者进入次数超过限制都不能激活
	if (GetControl().IsRoleActivateCoolDown(role.roleid, tid))
		return ERR_BATTLE_ACTIVATECOOLDOWN;
	if (sideinfo.required_money || sideinfo.required_item)
	{
		DBInstancingActivateArg arg(role.roleid, server->gs_id, map_id, teamid, sideinfo.required_money, sideinfo.required_item, syncdata);
		DBInstancingActivate *rpc = (DBInstancingActivate *)Rpc::Call(RPC_DBINSTANCINGACTIVATE, arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	else
	{
		syncdata.inventory.items.clear();
		OnActivate(ERR_SUCCESS, role, teamid, team_req, team_queue, sideinfo, syncdata);
	}
	return ERR_SUCCESS;
}

int InstancingField::JoinTeam(InstancingRole & role, unsigned char team, GMailSyncData & syncdata)
{
	if (status!=ST_WAITQUEUE && status!=ST_QUEUEING)
	{
		if (status == ST_PREPARING)
			return ERR_BATTLE_CLOSE;
		else
			return ERR_BATTLE_STARTED;
	}
	if (GetControl().IsRoleEnterCoolDown(role.roleid, tid))
		return ERR_BATTLE_COOLDOWN;

	int retcode = ERR_BATTLE_MAP_NOTEXIST;
	if (status == ST_QUEUEING)
	{
		switch (team)
		{
			case TEAM_ATTACKER:
				retcode = JoinReqTeam(role, team, team_attacker_req, team_attacker, attacker_info, syncdata);
				break;
			case TEAM_DEFENDER:
				retcode = JoinReqTeam(role, team, team_defender_req, team_defender, defender_info, syncdata);
				break;
			case TEAM_LOOKER:
				retcode = JoinReqTeam(role, team, team_looker_req, team_looker, looker_info, syncdata);
				break;
			default: break;
		}
	}
	else
	{
		switch (team)
		{
			case TEAM_ATTACKER:
				retcode = Activate(role, team, team_attacker_req, team_attacker, attacker_info, syncdata);
				break;
			case TEAM_DEFENDER:
				retcode = Activate(role, team, team_defender_req, team_defender, defender_info, syncdata);
				break;
			case TEAM_LOOKER:
				retcode = Activate(role, team, team_looker_req, team_looker, looker_info, syncdata);
				break;
			default: break;
		}
	}

	return retcode;
}

void InstancingField::OnActivate(int retcode, InstancingRole & role, unsigned char team, GMailSyncData & syncdata)
{
	switch (team)
	{
		case TEAM_ATTACKER:
			OnActivate(retcode, role, team, team_attacker_req, team_attacker, attacker_info, syncdata);
			break;
		case TEAM_DEFENDER:
			OnActivate(retcode, role, team, team_defender_req, team_defender, defender_info, syncdata);
			break;
		case TEAM_LOOKER:
			OnActivate(retcode, role, team, team_looker_req, team_looker, looker_info, syncdata);
			break;
		default: break;
	}
}

bool InstancingField::LeaveTeam(InstancingField::TeamType & team, int roleid, bool force = true)
{
	InstancingField::TeamType::iterator it = team.begin();
	for (; it != team.end(); ++it)
	{
		if (it->roleid == roleid)
		{
			if (force || (it->role_status != ROLE_FIGHTING))
			{
				team.erase(it);
				return true;
			}
		}
	}
	return false;
}
int InstancingField::LeaveTeam(int roleid, bool force = true)
{
	int retcode = LeaveTeam(team_attacker, roleid, force) || LeaveTeam(team_defender, roleid, force) 
	|| 	LeaveTeam(team_looker, roleid, force)  || LeaveTeam(team_attacker_req, roleid, force)
	|| 	LeaveTeam(team_defender_req, roleid, force) || LeaveTeam(team_looker_req, roleid, force) 
	? ERR_SUCCESS : -1;

	//如果战场报名为空，则重新回到等待激活
	if (status==ST_QUEUEING && team_attacker.empty() && team_defender.empty()) status = ST_WAITQUEUE;
	return retcode;
}

void InstancingField::GetMember(int roleid, unsigned char team, std::vector<GBattleRole> & members, std::vector<GBattleRole> &req_members)
{
	if (team != TEAM_ATTACKER && team != TEAM_DEFENDER && team != TEAM_LOOKER)
		return;

	TeamType::iterator it, ie;
	TeamType::iterator it_req, ie_req;
	if (team == TEAM_ATTACKER)
	{
		it = team_attacker.begin();
		ie = team_attacker.end();
		it_req = team_attacker_req.begin();
		ie_req = team_attacker_req.end();
	}
	else if (team == TEAM_DEFENDER)
	{
		it = team_defender.begin();
		ie = team_defender.end();
		it_req = team_defender_req.begin();
		ie_req = team_defender_req.end();
	}
	else if (team == TEAM_LOOKER)
	{
		it = team_looker.begin();
		ie = team_looker.end();
		it_req = team_looker_req.begin();
		ie_req = team_looker_req.end();
	}
	for (; it != ie; ++ it)
	{
		if (it->roleid == roleid) it->jointime = Timer::GetTime(); // 用户查看队列，刷新访问时间
		members.push_back(*it);
	}
	for (; it_req != ie_req; ++it_req)
	{
		if (it_req->roleid == roleid) it_req->jointime = Timer::GetTime(); // 用户查看队列，刷新访问时间
		req_members.push_back(*it_req);
	}
}

void InstancingField::Update(time_t now, bool & b_notify)
{
	if (status == ST_QUEUEING)
	{
		if (now-start_time >= queuing_time)
		{
			int retcode = TryStart(false);
			if (retcode != ERR_SUCCESS)
			{
				InstancingCloseNotify notify(map_id, retcode);
				Broadcast(notify, notify.localsid);

				ClearTeam(team_attacker);
				ClearTeam(team_attacker_req);
				ClearTeam(team_defender);
				ClearTeam(team_defender_req);
				ClearTeam(team_looker);
				ClearTeam(team_looker_req);
				status = ST_WAITQUEUE;
				start_time = now;
			}
		}
	}
	else if (status == ST_PREPARING)
	{
		if (now-start_time>=preparing_time && tag.GetStatus()==BS_READY)
		{
			status = ST_WAITQUEUE;
			start_time = now;
		}
	}
	else if (status == ST_FIGHTING)
	{
		if (check_time && ((check_time + 120) < now))
		{
			for (TeamType::iterator it = team_defender.begin(); it != team_defender.end();)
			{
				if (it->role_status == ROLE_QUEUEING && it->jointime < check_time)
				{
					server->EraseMapID(map_id,it->roleid);
					it = team_defender.erase(it);
				}
				else
					++it;
			}
			for (TeamType::iterator it = team_attacker.begin(); it != team_attacker.end();)
			{
				if (it->role_status == ROLE_QUEUEING && it->jointime < check_time)
				{
					server->EraseMapID(map_id,it->roleid);
					it = team_attacker.erase(it);
				}
				else
					++it;
			}
			for (TeamType::iterator it = team_looker.begin(); it != team_looker.end();)
			{
				if (it->role_status == ROLE_QUEUEING && it->jointime < check_time)
				{
					server->EraseMapID(map_id,it->roleid);
					it = team_looker.erase(it);
				}
				else
					++it;
			}
			check_time = now + 120; // 2分钟检查一次
		}
	}
}

void InstancingField::Init(const InstancingFieldInfo &_info, InstancingServer* _server)
{
	map_id = _info.world_tag;
	battle_id = _info.battle_id;
	tid = _info.tid;
	attacker_info = _info.attacker;
	defender_info = _info.defender;
	looker_info = _info.looker;
	queuing_time = _info.queuing_time;
	fighting_time = _info.fighting_time;
	preparing_time = _info.preparing_time;
	server = _server;
}

void InstancingField::AddTag(int tag_id)
{
	tag = TagInfo(tag_id);
	tag.SetStatus(BS_READY);
}

void InstancingField::OnChangeStatus(int tag_id, int tag_status)
{
	tag.SetStatus(tag_status);
	switch (tag_status)
	{
		/*case BS_READY: 
			if (status == ST_PREPARING) 
				status = ST_WAITQUEUE;
			break;*/
		case BS_CLOSING:
			if (status == ST_FIGHTING)
			{
				OnBattleEnd(tag_id);
			}
			break;
		default:
			break;

	}
}

int InstancingField::TryStart(bool manual)
{
	if (status != ST_QUEUEING)
		return ERR_BATTLE_NOT_INTEAM;

	if (!SizePolicy(manual))
		return ERR_BATTLE_TEAM_SIZE;

	if (tag.GetStatus() == BS_READY)
	{
		team_attacker_req.clear();
		team_defender_req.clear();
		team_looker_req.clear();

		tag.SetStatus(BS_SENDSTART);
		status = ST_SENDSTART;
		start_time = Timer::GetTime();
		SendInstancingStart start(server->gs_id, tag.tag, map_id, start_time+fighting_time);
		LOG_TRACE("InstancingField: sendstart, gs_id=%d,map_id=%d",server->gs_id, map_id);
		GProviderServer::GetInstance()->Send(server->sid, start);
		return ERR_SUCCESS;
	}
	return ERR_BATTLE_NOT_INTEAM;
}
bool InstancingField::TeamSizePolicy(TeamType &team, InstancingSideInfo & info)
{
	return (team.size() < (unsigned int)info.max_player_limit);
}

bool TeamErase(InstancingField::TeamType & team, int roleid)
{
	for (InstancingField::TeamType::iterator it = team.begin(); it != team.end(); ++it)
	{
		if (it->roleid == roleid)
		{
			team.erase(it);
			return true;
		}
	}
	return false;
}

bool IsMaster(InstancingField::TeamType & team, int roleid)
{
	return team.size() && team.begin()->roleid == roleid;
}
int InstancingField::Accept(TeamType & req_team, TeamType & queue_team, unsigned char teamid, InstancingSideInfo & sideinfo, short accept, int target)
{
	int retcode = ERR_BATTLE_NOT_INTEAM;
	for (TeamType::iterator it = req_team.begin(); it != req_team.end(); ++it)
	{
		if (it->roleid == target)
		{
			int localsid = it->localsid;
			int linksid = it->linksid;

			if (accept)
			{
				if (TeamSizePolicy(queue_team, sideinfo))
				{
					it->jointime = Timer::GetTime();
					it->role_status = ROLE_QUEUEING;
					queue_team.push_back(*it);			
					retcode = ERR_SUCCESS;
					req_team.erase(it);
				}
				else
					retcode = ERR_BATTLE_TEAM_FULL;
			}
			else
			{
				retcode = ERR_BATTLE_JOIN_REFUSED;
				req_team.erase(it);
				server->EraseMapID(map_id, target);
			}

			InstancingJoin_Re re(retcode, server->gs_id, map_id, teamid, start_time+queuing_time, localsid);
			GDeliveryServer::GetInstance()->Send(linksid, re);
			break;
		}
	}
	return retcode;
}
int	InstancingField::Appoint(TeamType &team_queue, unsigned char teamid, int target)
{
	int retcode = ERR_BATTLE_NOT_INTEAM;
	for (TeamType::iterator it = team_queue.begin(); it != team_queue.end(); ++it)
	{
		if (it->roleid == target)
		{
			InstancingRole role = *it;
			team_queue.erase(it);
			team_queue.insert(team_queue.begin(), role);
			retcode = ERR_SUCCESS;
			break;
		}
	}
	return retcode;
}
int InstancingField::Accept(int roleid, short accept, int target)
{
	if (status != ST_QUEUEING) return ERR_BATTLE_NOT_INTEAM;
	if (IsMaster(team_attacker, roleid))
	{
		return Accept(team_attacker_req, team_attacker, TEAM_ATTACKER, attacker_info, accept, target);
	}
	if (IsMaster(team_defender, roleid))
	{
		return Accept(team_defender_req, team_defender, TEAM_DEFENDER, defender_info, accept, target);
	}
	return ERR_BATTLE_NOT_INTEAM;
}

int InstancingField::Appoint(int roleid, int target)
{
	if (status != ST_QUEUEING) return ERR_BATTLE_NOT_INTEAM;
	if (IsMaster(team_attacker, roleid))
	{
		return Appoint(team_attacker, TEAM_ATTACKER, target);
	}
	if (IsMaster(team_defender, roleid))
	{
		return Appoint(team_defender, TEAM_DEFENDER, target);
	}
	return ERR_BATTLE_NOT_INTEAM;
}
int InstancingField::Kick(int roleid, int target)
{
	if (status != ST_QUEUEING) return ERR_BATTLE_NOT_INTEAM;
	if (IsMaster(team_attacker, roleid))
	{
		if (TeamErase(team_attacker, target))
		{
			server->EraseMapID(map_id, target);
			return ERR_SUCCESS;
		}
		return ERR_BATTLE_NOT_INTEAM;
	}
	if (IsMaster(team_defender, roleid))
	{
		if (TeamErase(team_defender, target))
		{
			server->EraseMapID(map_id, target);
			return ERR_SUCCESS;
		}
		return ERR_BATTLE_NOT_INTEAM;
	}
	return ERR_BATTLE_NOT_INTEAM;
}
int InstancingField::BattleStart(int roleid)
{
	if (status != ST_QUEUEING) return ERR_BATTLE_NOT_INTEAM;
	
	if (IsMaster(team_attacker, roleid))
		return TryStart(true);
	if (IsMaster(team_defender, roleid))
		return TryStart(true);
	return ERR_BATTLE_NOT_MASTER;
}

void InstancingField::GetFieldInfo(int roleid, class InstancingGetField_Re & re)
{
	int now = Timer::GetTime();
	re.retcode = ERR_SUCCESS;
	GetMember(roleid, TEAM_ATTACKER, re.attacker, re.req_attacker);
	GetMember(roleid, TEAM_DEFENDER, re.defender, re.req_defender);
	re.looker = team_looker.size();
	re.time_left = start_time+queuing_time-now;
}
//////////////////////////////////////////////////////////////////////////

void InstancingServer::Init(int _gs_id, int _sid, const std::vector<InstancingFieldInfo> &info)
{
	gs_id = _gs_id;
	sid = _sid;
	LOG_TRACE("InstancingServer Init: gs_id=%d, sid=%d", gs_id, sid);

	battlemap.clear();
	tidmap.clear();
	queue_roles.clear();

	for (std::vector<InstancingFieldInfo>::const_iterator it = info.begin(); it != info.end(); ++it)
	{
		LOG_TRACE("InstancingServer Init: gs_id=%d, battle_id=%d, tag=%d", gs_id, it->battle_id, it->world_tag);
		InstancingField & field = battlemap[it->world_tag];
		field.Init(*it, this);
		field.AddTag(it->world_tag);
		tidmap[it->tid].push_back(&field);
	}
}

void InstancingServer::Update(time_t now)
{
	bool b_notify = false;
	for (InstancingMap::iterator it = battlemap.begin(); it !=  battlemap.end(); ++it)
	{
		it->second.Update(now, b_notify);
	}
}

void InstancingServer::OnBattleStart(int map_id, int retcode)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
		it->second.OnBattleStart(retcode);
}

void InstancingServer::EraseMapID(int map_id, int roleid)
{
	QueueRoles::iterator it = queue_roles.find(roleid);
	if (it == queue_roles.end())
		return;

	RoleMaps & maps = it->second;	
	for (RoleMaps::iterator rit = maps.begin(); rit != maps.end();)
	{
		if (*rit == map_id)
		{
			rit = maps.erase(rit);
		}
		else
			++ rit;
	}
	if (maps.size() == 0)
		queue_roles.erase(it);

}

void  InstancingServer::OnBattleEnd(int map_id, const InstancingField &field)
{
	InstancingField::TeamType::const_iterator it = field.team_attacker.begin();
	for (; it != field.team_attacker.end(); ++it)
		EraseMapID(map_id, it->roleid);

	it = field.team_attacker_req.begin();
	for (; it != field.team_attacker_req.end(); ++it)
		EraseMapID(map_id, it->roleid);

	it = field.team_defender.begin();
	for (; it != field.team_defender.end(); ++it)
		EraseMapID(map_id, it->roleid);

	it = field.team_defender_req.begin();
	for (; it != field.team_defender_req.end(); ++it)
		EraseMapID(map_id, it->roleid);

	it = field.team_looker.begin();
	for (; it != field.team_looker.end(); ++it)
		EraseMapID(map_id, it->roleid);

	it = field.team_looker_req.begin();
	for (; it != field.team_looker_req.end(); ++it)
		EraseMapID(map_id, it->roleid);
}

int InstancingServer::OnBattleEnter(int roleid, int map_id, int &endtime)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		int retcode =  it->second.OnBattleEnter(roleid, endtime);
		if (retcode == ERR_SUCCESS)
		{
			RoleMaps & maps = queue_roles[roleid];
			for (RoleMaps::iterator rit = maps.begin(); rit != maps.end();)
			{
				if (*rit != map_id)
				{
					InstancingField & field = battlemap[*rit];
					field.LeaveTeam(roleid);
					rit = maps.erase(rit);
				}
				else
				{
					++rit;
				}
			}
		}
		return retcode;
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int InstancingServer::OnBattleEnterFail(int roleid, int map_id)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		GetControl().DecRoleEnterCnt(roleid, it->second.tid);
		/*
		if (it->second.LeaveTeam(roleid) == ERR_SUCCESS)
			EraseMapID(map_id, roleid);
			*/
		return ERR_SUCCESS;
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

void InstancingServer::OnLogout(int roleid)
{
	//LOG_TRACE("InstancingServer: onlogout: roleid=%d", roleid);
	QueueRoles::iterator qit = queue_roles.find(roleid);
	if (qit == queue_roles.end())
		return;
	RoleMaps & maps = qit->second;
	for (RoleMaps::iterator it = maps.begin(); it != maps.end(); ++it)
	{
		InstancingField & field = battlemap[*it];
		field.LeaveTeam(roleid);
	}
	queue_roles.erase(qit);
}

void InstancingServer::GetMap(int battle_tid, std::vector<GInstancingFieldInfo> & fields)
{
	TidMap::iterator it = tidmap.find(battle_tid);
	if (it == tidmap.end())
		return;

	std::vector<InstancingField *>::iterator field_it = it->second.begin(), field_ie = it->second.end();
	for (; field_it != field_ie; ++field_it)
	{
		InstancingField * pf = *field_it;
		if (pf != NULL)
		{
			fields.push_back(GInstancingFieldInfo());
			GInstancingFieldInfo &info = fields.back();
			info.tid = pf->tid;
			info.gs_id = gs_id;
			info.map_id = pf->map_id;
			info.status = pf->status;
			if (info.status==InstancingField::ST_QUEUEING || info.status==InstancingField::ST_FIGHTING)
			{
				int now = Timer::GetTime();
				if (info.status == InstancingField::ST_QUEUEING)
					info.time_left = pf->start_time+pf->queuing_time-now;
				else
					info.time_left = pf->start_time+pf->fighting_time-now;
			}
			else
				info.time_left = -1;
			info.player_number = pf->GetAttackerNumber();
			info.player_limit = pf->attacker_info.max_player_limit;
			if (pf->team_attacker.size())
				info.leader = pf->team_attacker.begin()->name;
		}
	}
}

void InstancingServer::GetFieldInfo(int roleid, int map_id, InstancingGetField_Re & re)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		it->second.GetFieldInfo(roleid, re);
	}
}

int InstancingServer::JoinTeam(InstancingRole & role, int map_id, unsigned char team, GMailSyncData & syncdata)
{
	// 排的队伍过多
	QueueRoles::iterator qit = queue_roles.find(role.roleid);
	if (qit != queue_roles.end() && qit->second.size() >= QUEUE_MAX)
		return ERR_BATTLE_QUEUELIMIT;

	//正在战斗，不能排队
	if (qit != queue_roles.end() && qit->second.size() == 1)
	{
		int queued_map = qit->second[0];
		InstancingField & field = battlemap[queued_map];
		if (field.status != InstancingField::ST_QUEUEING && field.status != InstancingField::ST_WAITQUEUE)
			return ERR_BATTLE_QUEUELIMIT;
	}
	if (qit != queue_roles.end())
		for (RoleMaps::iterator rit = qit->second.begin(); rit != qit->second.end(); ++ rit)
		{
			// 不能重复排同一个队
			if (*rit == map_id) return ERR_BATTLE_JOIN_ALREADY;
		}

	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		int retcode = it->second.JoinTeam(role, team, syncdata);
		if (retcode == ERR_SUCCESS)
		{
			if (qit == queue_roles.end())
			{
				RoleMaps & vec = queue_roles[role.roleid];
				vec.push_back(map_id);
			}
			else
				qit->second.push_back(map_id);
		}
		return retcode;
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

void InstancingServer::OnActivate(int retcode, InstancingRole & role, int map_id, unsigned char team, GMailSyncData & syncdata)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
		it->second.OnActivate(retcode, role, team, syncdata);
}

int InstancingServer::LeaveTeam(int roleid, int map_id)
{
	int retcode;
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		retcode = it->second.LeaveTeam(roleid, false);
		if (retcode == ERR_SUCCESS)
			EraseMapID(map_id, roleid);
		return retcode;
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

void  InstancingServer::OnChangeStatus(int battle_id, int map_id, int status)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		return it->second.OnChangeStatus(map_id, status);
	}
}
void  InstancingServer::OnLeaveGame(int battle_id, int map_id, int roleid, int reason)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		int retcode = it->second.LeaveTeam(roleid);
		if (retcode == ERR_SUCCESS)
		{
			// XXX 全部删除是因为进入战场后，用户不可能再在其他战场排队
			queue_roles.erase(roleid);
		}
	}
}
void InstancingServer::OnDisconnect()
{
	battlemap.clear();
	queue_roles.clear();
	tidmap.clear();
}

int InstancingServer::Accept(int roleid, short accept, int map_id, int target)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		return it->second.Accept(roleid, accept, target);
	}
	return ERR_BATTLE_MAP_NOTEXIST;

}
int InstancingServer::Appoint(int roleid, int map_id, int target)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		return it->second.Appoint(roleid, target);
	}
	return ERR_BATTLE_MAP_NOTEXIST;

}
int InstancingServer::BattleStart(int roleid, int map_id)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		return it->second.BattleStart(roleid);
	}
	return ERR_BATTLE_MAP_NOTEXIST;

}
int InstancingServer::Kick(int roleid, int map_id, int target)
{
	InstancingMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		return it->second.Kick(roleid, target);
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}
/////////////////////////////////////////////////////////////////////////

void InstancingManager::RegisterServer(int sid, int gs_id, const std::vector<InstancingFieldInfo> & info)
{
	InstancingServer& server = servers[gs_id];
	server.Init(gs_id, sid, info);
}
void InstancingManager::GetMap(int roleid, int battle_tid, std::vector<GInstancingFieldInfo> & fields, int &enter_count)
{
	enter_count = GetControl().GetRoleEnterCount(roleid, battle_tid);
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); ++ it)
		it->second.GetMap(battle_tid, fields);
}

void InstancingManager::GetFieldInfo(int roleid, int gs_id, int map_id, InstancingGetField_Re & re)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.GetFieldInfo(roleid, map_id, re);
}

int InstancingManager::JoinTeam(InstancingRole & role, int gs_id, int map_id, unsigned char team, GMailSyncData & syncdata)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	return it->second.JoinTeam(role, map_id, team, syncdata);
}

void InstancingManager::OnActivate(int retcode, InstancingRole & role, int gs_id, int map_id, unsigned char team, GMailSyncData & syncdata)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnActivate(retcode, role, map_id, team, syncdata);
}

int InstancingManager::LeaveTeam(int roleid, int gs_id, int map_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	return it->second.LeaveTeam(roleid, map_id);
}

bool InstancingManager::Update()
{
	time_t now = GetTime();
	if (now - t_base > 86400) t_base += 86400;
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); ++ it)
	{
		it->second.Update(now);
	}
	GetControl().Update(now);
	return true;
}

inline time_t InstancingManager::GetTime()
{
	time_t now = Timer::GetTime();
	if (t_forged)
		now += t_forged;
	return now;
}

int InstancingManager::Kick(int roleid, int gs_id, int map_id, int target)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		return it->second.Kick(roleid, map_id, target);
	return ERR_BATTLE_MAP_NOTEXIST;
}
void InstancingManager::OnLeaveGame(int gs_id, int battle_id, int map_id, int roleid, int reason)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnLeaveGame(battle_id, map_id, roleid, reason);
}
void InstancingManager::OnChangeStatus(int gs_id, int battle_id, int map_id, int status)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnChangeStatus(battle_id, map_id, status);
}

int InstancingManager::BattleStart(int roleid, int gs_id, int mapid)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		return it->second.BattleStart(roleid, mapid);
	return ERR_BATTLE_MAP_NOTEXIST;
}
void InstancingManager::OnBattleStart(int gs_id, int mapid, int retcode)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnBattleStart(mapid, retcode);
}

int InstancingManager::OnBattleEnter(int roleid, int gs_id, int map_id, int &endtime)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	return it->second.OnBattleEnter(roleid, map_id, endtime);
}

int InstancingManager::OnBattleEnterFail(int roleid, int gs_id, int map_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	return it->second.OnBattleEnterFail(roleid, map_id);
}

void InstancingManager::OnLogout(int roleid, int gs_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnLogout(roleid);
}

bool InstancingManager::Initialize()
{
	struct tm dt;
	time_t now = Timer::GetTime();
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 0;
	t_base = mktime(&dt);

	GetControl().Init(t_base);
	IntervalTimer::Attach( this, UPDATE_INTERVAL * 1000000 /IntervalTimer::Resolution());
	return true;
}

void InstancingManager::OnDisconnect(int gs_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
	{
		it->second.OnDisconnect();
		servers.erase(it);
	}

}
int InstancingManager::Accept(int roleid, short accept, int gs_id, int map_id, int target)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
	{
		return it->second.Accept(roleid, accept, map_id, target);
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int	InstancingManager::Appoint(int roleid, int gs_id, int map_id, int target)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
	{
		return it->second.Appoint(roleid, map_id, target);
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}
};

