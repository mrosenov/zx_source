#include "crossbattlemanager.hpp"
#include "battlemanager.h"
#include "centraldeliveryserver.hpp"
#include "centraldeliveryclient.hpp"
#include "gdeliveryserver.hpp"
#include "crossbattlejointeam.hpp"
#include "battlejoin_re.hpp"
#include "battlegetfield.hpp"
#include "battleleave_re.hpp"
#include "battleenter.hpp"
#include "battleenter_re.hpp"
#include "openbanquetenter.hpp"
#include "copenbanquetjoin.hpp"

namespace GNET
{

void CrossBattleManager::JoinTeam(int battle, int subbattle, int gsid, int mapid, int roleid, int starttime, int team, int zoneid)
{
	InsertCrossPlayer(battle, subbattle, gsid, mapid, roleid, starttime, team);
	_role2zone[roleid] = zoneid;
	// 报名了跨服战场需要通知原服，不允许其在原服再报名战场
	// 但在原服报名战场不需要通知跨服，因为原服战场Logout就会清除
	CrossBattleJoinTeam jointeam(roleid, battle, subbattle, gsid, mapid, starttime, team);
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, jointeam); 
}

void CrossBattleManager::PlayerLogin(int roleid)
{

	LOG_TRACE("CrossBattleManager::PlayerLogin  roleid=%d, _crossroles.size=%d",  roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	/*
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EraseCrossBattle troleid=%d", troleid);
	}*/


	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
	if(info == NULL)
	      	return;

	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	bool iscentralds = dsm->IsCentralDS();
	int now = Timer::GetTime();
	int gsid = 0, mapid = 0, subbattle = 0, starttime = 0, team = 0, zoneid = 0, retcode = 0;
	CrossBattleMap &battles = cpit->second;
	for(CrossBattleMap::iterator cbit = battles.begin(); cbit != battles.end();)
	{
		// 角色登陆，通知其都参加了哪些跨服战场，以便客户端显示战场图标
		gsid = cbit->second._gsid;
		mapid = cbit->second._mapid;
		subbattle = cbit->second._subbattle_type;
		starttime = cbit->second._starttime;
		team = cbit->second._team;
		if(now > starttime) // 如果已经开始，则表明因为某些原因失败了
		{
			battles.erase(cbit++);
		}
		else
		{
			// 在天界时，如果换线看不到战场按钮并且战场开启将不通知
			if(!iscentralds || gsid == info->gameid) 
			{
				if(subbattle != OPENBANQUET_BATTLE) 
				{
					dsm->Send(info->linksid, BattleJoin_Re(ERR_BATTLE_CROSSBATTLE, roleid, gsid, mapid, subbattle, !iscentralds, team, starttime, info->localsid));
					LOG_TRACE("CrossBattleManager::PlayerLogin roleid=%d,gsid=%d,mapid=%d,subbattle=%d",roleid, gsid, mapid, subbattle);
					++cbit;
				}
				else
				{
					// 流水席战场，报名时退出跨服，要在原服显示排队信息
					COpenBanquetJoin join_msg;
					join_msg.roleid = roleid;
					CentralDeliveryClient::GetInstance()->SendProtocol(join_msg);
					++cbit;
				}
			}
			else if(iscentralds && gsid != info->gameid) // 在天界换线则视为退出天界战场, 流水席除外，因为流水席可以换线
			{
				if(subbattle != OPENBANQUET_BATTLE) // 流水席除外，因为流水席报名之后，可以去各个县
				{
					battles.erase(cbit++);
					retcode = BattleManager::GetInstance()->LeaveTeam(roleid, gsid);
					zoneid = GetSrcZoneID(roleid);
					CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, BattleLeave_Re(retcode, roleid, 0, true));
				}
				else
				{
					// 流水席要看是否在报名状态中
					//OpenBanquetManager::GetInstance()->OnQueue(roleid, gsid, mapid);
					++cbit;
				}
			}
			else
			{
				++cbit;
			}
		}
	}
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}

void CrossBattleManager::LeaveBattle(int battle_type, int roleid, int sid)
{
	/*
	LOG_TRACE("LeaveBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("LeaveBattle troleid=%d", troleid);
	}*/

	_role2tag.erase(roleid);
	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	CrossBattleInfo &info = cbit->second;
	int retcode = 0;
	int zoneid = GetSrcZoneID(roleid);
	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	if(dsm->IsCentralDS())
	{
		retcode = BattleManager::GetInstance()->LeaveTeam(roleid, info._gsid);
		CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, BattleLeave_Re(retcode, roleid, 0, true));
	}
	dsm->Send(sid, BattleLeave_Re(retcode, roleid, 0, false));

	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}

void CrossBattleManager::EraseCrossBattle(int roleid, int battle_type)
{
	/*
	LOG_TRACE("EraseCrossBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EraseCrossBattle troleid=%d", troleid);
	}*/

	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
		return;
	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	     	return;
	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		//_role2zone.erase(roleid);
	}
}

void CrossBattleManager::EnterBattle(int battle_type, int roleid)
{
	/*
	LOG_TRACE("EnterBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EnterBattle troleid=%d", troleid);
	}*/

	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	CrossBattleInfo &info = cbit->second;
	CentralDeliveryClient::GetInstance()->SendProtocol(BattleEnter(roleid, info._gsid, info._mapid, battle_type, 0, true));
	InsertFightingTag(info._gsid, roleid); // 为了跨服时能进入指定的GS

	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}

void CrossBattleManager::EnterOpenBanquetBattle(int battle_type, int roleid)
{
	/*
	LOG_TRACE("EnterOpenBanquetBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EnterOpenBanquetBattle troleid=%d", troleid);
	}
	*/
	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	CrossBattleInfo &info = cbit->second;
	CentralDeliveryClient::GetInstance()->SendProtocol(OpenBanquetEnter(roleid, info._gsid, info._mapid, 0, true));
	InsertFightingTag(info._gsid, roleid); // 为了跨服时能进入指定的GS

	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}


void CrossBattleManager::EnterBattle(int battle_type, BattleEnter_Re &res)
{
	int roleid = res.roleid;
	int zoneid = _role2zone[roleid];
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, res);
	/*CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	res.battle_type = cbit->second._subbattle_type;
	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}*/
}

void CrossBattleManager::SetRoleCoolDown(int battle_type, int subbattle_type, int roleid)
{
	if(battle_type == NORMAL_BATTLE)
	{
		BattleManager::GetInstance()->SetRoleCoolDown(subbattle_type, roleid);
	}
}

bool CrossBattleManager::Update()
{
	time_t now = Timer::GetTime();
	std::map<int, FightingTag>::iterator it, ite = _role2tag.end();
	for (it = _role2tag.begin(); it != ite; ++it)
	{
		//active 之后 客户端传天界的过程中可能失败 此处通过超时恢复
		if (it->second._isactive && it->second._active_time > 0
				&& now-it->second._active_time > 60)
		{
			LOG_TRACE("inactive fighting tag, roleid %d tag %d", it->first,
					it->second._tag);
			it->second._isactive = false;
			it->second._active_time = 0;
		}
	}
	return true;
}
}
