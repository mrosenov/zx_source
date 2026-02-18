#include <list>
#include "battlemanager.h"
#include "hashstring.h"
#include "groleforbid"
#include "groleinventory"
#include "battlestart.hpp"
#include "mapuser.h"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"
#include "factionchat.hpp"
#include "factionid"
#include "localmacro.h"
#include "battlegetfield_re.hpp"
#include "battlegetlist_re.hpp"
#include "battlestartnotify.hpp"
#include "sendbattleenter.hpp"
#include "battlejoin_re.hpp"
#include "dbbattlejoin.hrp"
#include "crossbattlemanager.hpp"
#include "centraldeliveryserver.hpp"

//#define ARENA_DEBUG 
//#define SONJIN_DEBUG
//#define CHALLENGESPACE_DEBUG
namespace GNET
{

enum
{
	BATTLESTART_SUCCESS				= 0,	//Õ½³¡³É¹¦¿ªÆô
	BATTLESTART_JOINNUMBER			= 1,	//Õ½³¡¿ªÆôÒòÎª±¨ÃûÈËÊý²»¹»Ê§°Ü
};

BattleField::STUBMAP BattleField::stubmap;
static CrssvrBattleField _stub_CrssvrBattleField(BATTLEFIELD_TYPE_CRSSVR);
static NormalBattleField _stub_NormalBattleField(BATTLEFIELD_TYPE_NORMAL);
static RebornBattleField _stub_RebornBattleField(BATTLEFIELD_TYPE_REBORN);
static SonjinBattleField _stub_SonjinBattleField(BATTLEFIELD_TYPE_SONJIN); 
static SonjinRebornBattleField _stub_SonjinRebornBattleField(BATTLEFIELD_TYPE_SONJINREBORN); 
static ArenaBattleField _stub_ArenaBattleField(BATTLEFIELD_TYPE_ARENA); 
static ArenaRebornBattleField _stub_ArenaRebornBattleField(BATTLEFIELD_TYPE_ARENAREBORN); 
static CrssvrBattleField _stub_AnonCrssvrBattleField(BATTLEFIELD_TYPE_ANONCRSSVR);
static ChallengeSpaceBattleField _stub_ChallengeSpaceBattleField(BATTLEFIELD_TYPE_CHALLENGESPACE);
static CrssvrTeamBattleField _stub_CrssvrTeamBattleField(BATTLEFIELD_TYPE_CRSSVRTEAM);

static bool b_debug_mode = false;
enum
{
	TEAM_NORMAL_LIMIT 	= 6,
	TEAM_CRSSVR_LIMIT 	= 20,
	TEAM_SONJIN_LIMIT	= 120,
#ifdef ARENA_DEBUG
	TEAM_ARENA_LIMIT 	= 3,
#else
	TEAM_ARENA_LIMIT 	= 100,
#endif
};

CrssvrBattleField::~CrssvrBattleField() { }
CrssvrTeamBattleField::~CrssvrTeamBattleField() { }
bool CrssvrTeamBattleField::IsRoleInTheTeam(int roleid,unsigned char team)
{
	if (fighting_tag)
	{
		if(team  == TEAM_RED)
		{
			TeamType::iterator it = team_red_fighting.find(roleid);
			if (it != team_red_fighting.end())
				return true;
		}
		else if (team  == TEAM_BLUE)
		{
			TeamType::iterator it = team_blue_fighting.find(roleid);
			if (it != team_blue_fighting.end())
				return true;
		}
		return false;
	}
	else
	{
		if(team  == TEAM_RED)
		{
			TeamType::iterator it = team_red.find(roleid);
			if (it != team_red.end())
				return true;
		}
		else if (team  == TEAM_BLUE)
		{
			TeamType::iterator it = team_blue.find(roleid);
			if (it != team_blue.end())
				return true;
		}
		return false;
	}
}

bool CrssvrBattleField::IsRoleInTeam(int roleid)
{
	if (fighting_tag)
	{
		TeamType::iterator it = team_red_fighting.find(roleid);
		if (it != team_red_fighting.end())
			return true;
		it = team_blue_fighting.find(roleid);
		if (it != team_blue_fighting.end())
			return true;
		return false;
	}
	else
	{
		TeamType::iterator it = team_red.find(roleid);
		if (it != team_red.end())
			return true;
		it = team_blue.find(roleid);
		if (it != team_blue.end())
			return true;
		return false;
	}
}
bool NormalBattleField::IsRoleInTeam(int roleid)
{
	//if (fighting_tag) ÒòÎªÆÕÍ¨Õ½³¡FIGHTING×´Ì¬ÏÂ Ò²¿ÉÒÔ±¨Ãû£(²Î¼ÓÏÂÒ»ÂÖ)ËùÒÔ´Ë´¦²»¿ÉÒÔÒÀÀµfighting_tag
	TeamType::iterator it = team_red_fighting.find(roleid);
	if (it != team_red_fighting.end())
		return true;
	it = team_blue_fighting.find(roleid);
	if (it != team_blue_fighting.end())
		return true;
	it = team_red.find(roleid);
	if (it != team_red.end())
		return true;
	it = team_blue.find(roleid);
	if (it != team_blue.end())
		return true;
	return false;
}
bool ChallengeSpaceBattleField::IsRoleInTeam(int roleid)
{
	//ÒòÎªFIGHTING×´Ì¬ÏÂÒ²¿ÉÒÔ±¨Ãû²Î¼ÓÏÂÒ»ÂÖ£¬ËùÒÔ´Ë´¦²»ÒÀÀµÓÚfighting_tag
	//ÔÚÌôÕ½¿Õ¼äÖÐÖ»ÓÐÒ»·½£¬ËùÒÔÖ»¼ì²éºì·½¼´¿É
	TeamType::iterator it = team_red_fighting.find(roleid);
	if(it != team_red_fighting.end())
	      	return true;
	it = team_red.find(roleid);
	if(it != team_red.end())
	      	return true;
	return false;
}
bool RebornBattleField::IsRoleInTeam(int roleid)
{
	TeamType::iterator it = team_red_fighting.find(roleid);
	if (it != team_red_fighting.end())
		return true;
	it = team_blue_fighting.find(roleid);
	if (it != team_blue_fighting.end())
		return true;
	it = team_red.find(roleid);
	if (it != team_red.end())
		return true;
	it = team_blue.find(roleid);
	if (it != team_blue.end())
		return true;
	return false;
}

bool ArenaBattleField::IsRoleInTeam(int roleid)
{
	TeamType::iterator it = team_red_fighting.find(roleid);
	if (it != team_red_fighting.end())
		return true;
	it = team_blue_fighting.find(roleid);
	if (it != team_blue_fighting.end())
		return true;
	it = team_red.find(roleid);
	if (it != team_red.end())
		return true;
	it = team_blue.find(roleid);
	if (it != team_blue.end())
		return true;
	it = potential_team_red.find(roleid);
	if (it != potential_team_red.end())
		return true;
	return false;
}

int CrssvrBattleField::GetBlueNumber() 
{ 
	if (fighting_tag)
	{
		return team_blue_fighting.size();
	}
	return team_blue.size(); 
}
int NormalBattleField::GetBlueNumber() 
{ 
	return team_blue.size(); 
}
int ChallengeSpaceBattleField::GetBlueNumber()
{
	return team_blue.size(); //team_blueÓ¦¸ÃÃ»ÓÐÍæ¼Ò£¬¸Ãº¯ÊýÓ¦¸ÃÒ»Ö±·µ»Ø0
}
int RebornBattleField::GetBlueNumber() 
{ 
	return team_blue.size(); 
}
int CrssvrBattleField::GetRedNumber()
{ 
	if (fighting_tag)
	{
		return team_red_fighting.size();
	}
	return team_red.size(); 
}
int NormalBattleField::GetRedNumber()
{ 
	return team_red.size(); 
}
int ChallengeSpaceBattleField::GetRedNumber()
{
	return team_red.size();
}
int RebornBattleField::GetRedNumber()
{ 
	return team_red.size(); 
}
void BattleField::Broadcast(BattleStartNotify & data, unsigned int & localsid, int tag) 
{
	int endtime = data.starttime + GetFightingTime();
	for (TeamType::const_iterator it = team_red.begin(); it != team_red.end(); ++it)
	{
		localsid = it->second.localsid;
		SendStartNotify(it->second.roleid, data, TEAM_ATTACKER, endtime, tag);
	}
	for (TeamType::const_iterator it = team_blue.begin(); it != team_blue.end(); ++it)
	{
		localsid = it->second.localsid;
		SendStartNotify(it->second.roleid, data, TEAM_DEFENDER, endtime, tag);
	}
}

void BattleField::StartBattleError()
{
	CrossBattleManager *crossbattle = CrossBattleManager::GetInstance();
	for (TeamType::const_iterator it = team_red.begin(); it != team_red.end(); ++it)
	{
		crossbattle->EraseCrossBattle(it->second.roleid, NORMAL_BATTLE);
	}
	for (TeamType::const_iterator it = team_blue.begin(); it != team_blue.end(); ++it)
	{
		crossbattle->EraseCrossBattle(it->second.roleid, NORMAL_BATTLE);
	}
}

void BattleField::SendStartNotify(int roleid, BattleStartNotify &notify, int team, int endtime, int tag)
{
	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	CrossBattleManager *manager = CrossBattleManager::GetInstance();
	PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
	if(dsm->IsCentralDS() && info == NULL) // ¿ç·þÕ½³¡¿ªÆôµ«ÊÇ±¨ÃûÕß²»ÔÚ¿ç·þ£¬Í¨ÖªÔ­·þ
	{
		notify.roleid = roleid;
		notify.iscross = true;
		notify.localsid = 0;
		int src_zoneid = manager->GetSrcZoneID(roleid);
		if(src_zoneid != 0)
		{
			manager->InsertFightingTag(notify.gs_id, roleid, tag, team, endtime, GetFieldType());
			manager->EraseCrossBattle(roleid, NORMAL_BATTLE);
			CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, notify);
			LOG_TRACE("SendStartNotify::Send to NormalDS roleid=%d", notify.roleid);
		}
	}
	else if(info != NULL && info->ingame && info->gameid == notify.gs_id) // ±¨ÃûÕßÔÚ±¨ÃûµÄGSÉÏ
	{
		notify.roleid = roleid;
		notify.iscross = false;
		notify.localsid = 0;
		dsm->Send(info->linksid, notify);
		LOG_TRACE("SendStartNotify::Send to Client roleid=%d", notify.roleid);
	}
	else
	{
		LOG_TRACE("BattleField::SendStartNotify roleid=%d is not in game", roleid);
		manager->EraseCrossBattle(roleid, NORMAL_BATTLE);
	}
}

struct  LevelGreater
{
	bool operator()(const BattleField::TeamType::value_type &r1, const BattleField::TeamType::value_type &r2)
	{
		return r1.second.level > r2.second.level;
	}
};

void RebornBattleField::ServerAppointTeam(TeamType &candidates)
{
	std::sort(candidates.begin(), candidates.end(), LevelGreater());
	size_t size = candidates.size(), i = 0;
	if (size&0x1)
		--size;
	TeamType::const_iterator it=candidates.begin();

	for (; i < size; ++i, ++it)
	{
		if (i & 0x1)
			team_blue.insert(std::make_pair(it->second.roleid, it->second));
		else
			team_red.insert(std::make_pair(it->second.roleid, it->second));
	}

	if (it != candidates.end())
		team_blue.insert(std::make_pair(it->second.roleid, it->second));
}

void BattleField::OnBattleStart(int retcode)
{
	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		if (it->second.GetStatus() == BattleField::ST_SENDSTART)
		{
			if (retcode == ERR_SUCCESS)
			{
				if (IsServerAppointTeam())
				{
					TeamType temp;
					temp.swap(team_red);
					ServerAppointTeam(temp);
				}

				time_t now = BattleManager::GetInstance()->GetTime();
				BattleStartNotify notify(BATTLESTART_SUCCESS, 0, server->gs_id, map_id, now, 0, 0);
				Broadcast(notify, notify.localsid, it->second.tag);

				team_red_fighting.swap(team_red);
				team_blue_fighting.swap(team_blue);
				team_looker_fighting.clear();
				team_red.clear();
				team_blue.clear();
				//potential_team_red.clear();
				//LOG_TRACE("BattleField::OnBattleStart, red=%d,blue=%d,red_fighting=%d,blue_fighting=%d",team_red.size(),team_blue.size(),team_red_fighting.size(),team_blue_fighting.size());
				it->second.SetStatus(BattleField::ST_FIGHTING);
				it->second.SetTimeout(now, BattleField::ENTER_TIMEOUT);
				fighting_tag = it->second.tag;
			}
			else
			{
				it->second.ClearTimeout();
				it->second.SetStatus(BattleField::ST_NONE);
				//LOG_TRACE("BattleField::OnBattleStart StartError retcode=%d", retcode);
				StartBattleError();
			}
			return;
		}
	}
}

int CrssvrBattleField::OnBattleEnter(int roleid, bool b_gm)
{
	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for (; tit != tie; ++ tit)
	{
		//LOG_TRACE("CrssvrBattleField::OnBattleEnter, status=%d, ST_FIGHTING=%d, map_id=%d", tit->second.GetStatus(), ST_FIGHTING, map_id);
		if (tit->second.GetStatus() == ST_FIGHTING)
		{
			if (b_gm)
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_NONE, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				return ERR_SUCCESS;
			}

			TeamType::iterator it = team_red_fighting.find(roleid);
			if (it != team_red_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_RED, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				return ERR_SUCCESS;
			}
			it = team_blue_fighting.find(roleid);
			if (it != team_blue_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_BLUE, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				return ERR_SUCCESS;
			}
			it = team_looker_fighting.find(roleid);
			if (it != team_looker_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_LOOKER, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				return ERR_SUCCESS;
			}
			return ERR_BATTLE_NOT_INTEAM;
		}
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

void CrssvrBattleField::InsertReEnter(int roleid, bool reenter, TEAM team)
{
	if(!reenter)
	{
		// Èç¹û²»ÊÇÖØ¸´½øÈë,¼ÓÈëmapÖÐ
		LOG_TRACE("CrssvrBattleField InsertReEnter:gs_id=%d,map_id=%d,roleid=%d,team=%d,reenter=%d", server->gs_id,map_id,roleid,team,reenter);
		reenter_map[roleid] = team;
	}
	LOG_TRACE("CrssvrBattleField InsertReEnter:gs_id=%d,map_id=%d,roleid=%d,team=%d,reenter=%d", server->gs_id,map_id,roleid,team,reenter);
}

bool CrssvrBattleField::CheckReEnter(int roleid, bool reenter, TEAM team)
{
	if(reenter)
	{
		// Èç¹ûÊÇÖØ¸´½øÈë,ÔÚmapÖÐ¿ÉÒÔ½ø£¬·ñÔò²»ÈÃ½ø
		return IsReEnter(roleid, team);
	}
	else
	{
		// ´Ó±¨Ãû·¿¼ä½øÈëµÄ£¬ÓÀÔ¶¿ÉÒÔ½øÈë
		return true;
	}
}

bool CrssvrBattleField::IsReEnter(int roleid, TEAM team)
{
	TReEnterMap::iterator it = reenter_map.find(roleid);
	if(it != reenter_map.end())
	{
		if(it->second == team)
		{
			return true;
		}
		else
		{

			LOG_TRACE("CrssvrBattleField IsReEnter:gs_id=%d,map_id=%d,roleid=%d find in reenter_map, but team=%d not equal it->second=%d", server->gs_id,map_id,roleid, team, it->second);
			return false;
		}
	}

	LOG_TRACE("CrssvrBattleField IsReEnter:gs_id=%d,map_id=%d,roleid=%d not find in reenter_map", server->gs_id,map_id,roleid);
	return false;
}

int NormalBattleField::OnBattleEnter(int roleid, bool b_gm)
{
	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for (; tit != tie; ++ tit)
	{
		if (tit->second.GetStatus() == ST_FIGHTING)
		{
			TeamType::iterator it = team_red_fighting.find(roleid);
			if (it != team_red_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_RED, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			it = team_blue_fighting.find(roleid);
			if (it != team_blue_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_BLUE, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			return ERR_BATTLE_NOT_INTEAM;
		}
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int ChallengeSpaceBattleField::OnBattleEnter(int roleid, bool b_gm)
{
	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for(; tit != tie; ++tit)
	{
		if(tit->second.GetStatus() == ST_FIGHTING)
		{
			TeamType::iterator it = team_red_fighting.find(roleid);
			if(it != team_red_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_RED, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			return ERR_BATTLE_NOT_INTEAM;
		}
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int RebornBattleField::OnBattleEnter(int roleid, bool b_gm)
{
	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for (; tit != tie; ++ tit)
	{
		if (tit->second.GetStatus() == ST_FIGHTING)
		{
			TeamType::iterator it = team_red_fighting.find(roleid);
			if (it != team_red_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_RED, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			it = team_blue_fighting.find(roleid);
			if (it != team_blue_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_BLUE, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			return ERR_BATTLE_NOT_INTEAM;
		}
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int SonjinBattleField::OnBattleEnter(int roleid, bool b_gm)
{
	time_t now = BattleManager::GetInstance()->GetTime();	
	//¿ªÆô10·ÖÖÓºó²»ÄÜÔÙ½øÈëÕ½³¡
	if (now-starttime > 600) return ERR_BATTLE_STARTED10;

	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for (; tit != tie; ++ tit)
	{
		if (tit->second.GetStatus() == ST_FIGHTING)
		{
			TeamType::iterator it = team_red_fighting.find(roleid);
			if (it != team_red_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_RED, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			it = team_blue_fighting.find(roleid);
			if (it != team_blue_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_BLUE, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			return ERR_BATTLE_NOT_INTEAM;
		}
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int ArenaBattleField::OnBattleEnter(int roleid, bool b_gm)
{
	time_t now = BattleManager::GetInstance()->GetTime();	
	//¿ªÆô90sºó²»ÄÜÔÙ½øÈëÕ½³¡
	if (now-starttime > 90) return ERR_BATTLE_STARTED10;

	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for (; tit != tie; ++ tit)
	{
		if (tit->second.GetStatus() == ST_FIGHTING)
		{
			TeamType::iterator it = team_red_fighting.find(roleid);
			if (it != team_red_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_MELEE, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			it = team_blue_fighting.find(roleid);
			if (it != team_blue_fighting.end())
			{
				SendBattleEnter req(roleid, tit->second.tag, map_id, TEAM_MELEE, GetFieldType());
				GProviderServer::GetInstance()->Send(server->sid, req);
				BattleManager::GetInstance()->SetRoleCoolDown(GetFieldType(), roleid);
				return ERR_SUCCESS;
			}
			return ERR_BATTLE_NOT_INTEAM;
		}
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

void CrssvrBattleField::OnBattleEnd(int tag)
{
	LOG_TRACE("CrssvrBattleField OnBattleEnd:gs_id=%d,map_id=%d", server->gs_id,map_id);
	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		if (tag == it->second.tag)
		{
			LOG_TRACE("OnBattleEnd:gs_id=%d,map_id=%d,tag=%d,status=%d"
					, server->gs_id,map_id,it->second.tag,it->second.status);
			if (it->second.GetStatus() == ST_WAITINGEND || it->second.GetStatus() == ST_FIGHTING)
			{
				if (fighting_tag == tag) fighting_tag = 0;
				struct tm dt;
				time_t now = BattleManager::GetInstance()->GetTime();
				localtime_r(&now, &dt);
				dt.tm_sec = 0;
				dt.tm_min = 0;
				dt.tm_hour += 1;
				starttime = mktime(&dt);
				it->second.SetStatus(ST_NONE);
				if (TryBeginQueue())
				{
					it->second.SetStatus(ST_QUEUEING);
				}

				reenter_map.clear();
			}
			break;
		}
	}
}

void CrssvrTeamBattleField::OnBattleEnd(int tag)
{
	LOG_TRACE("CrssvrTeamBattleField OnBattleEnd:gs_id=%d,map_id=%d", server->gs_id,map_id);
	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		if (tag == it->second.tag)
		{
			LOG_TRACE("OnBattleEnd:gs_id=%d,map_id=%d,tag=%d,status=%d"
					, server->gs_id,map_id,it->second.tag,it->second.status);
			if (it->second.GetStatus() == ST_WAITINGEND || it->second.GetStatus() == ST_FIGHTING)
			{
				if (fighting_tag == tag) fighting_tag = 0;
					it->second.SetStatus(ST_NONE);
				if (TryBeginQueue())
				{
					it->second.SetStatus(ST_QUEUEING);
				}
			}
			break;
		}
	}
}

void NormalBattleField::OnBattleEnd(int tag)
{
	LOG_TRACE("OnBattleEnd:gs_id=%d,map_id=%d", server->gs_id,map_id);
	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		if (tag == it->second.tag)
		{
			LOG_TRACE("OnBattleEnd:gs_id=%d,map_id=%d,tag=%d,status=%d"
					, server->gs_id,map_id,it->second.tag,it->second.status);
			if (it->second.GetStatus() == ST_WAITINGEND || it->second.GetStatus() == ST_FIGHTING)
			{
				if (fighting_tag == tag) fighting_tag = 0;
				it->second.SetStatus(ST_NONE);
				if (TryBeginQueue())
				{
					//LOG_TRACE("BattleManager:gs_id=%d, map_id=%d, tag=%d try begin queue"
					//		, server->gs_id, map_id, it->second.tag);
					it->second.SetStatus(ST_QUEUEING);
				}
			}
			break;
		}
	}
}

void ChallengeSpaceBattleField::OnBattleEnd(int tag)
{
	LOG_TRACE("OnBattleEnd:gs_id=%d, map_id=%d", server->gs_id, map_id);
	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for(; it != ie; ++it)
	{
		if(tag == it->second.tag)
		{
			LOG_TRACE("OnBattleEnd:gs_id=%d,map_id=%d,tag=%d,status=%d", server->gs_id, map_id, it->second.tag, it->second.status);
			if(it->second.GetStatus() == ST_WAITINGEND || it->second.GetStatus() == ST_FIGHTING)
			{
				if(fighting_tag == tag)
				{
					fighting_tag = 0;
				}
				it->second.SetStatus(ST_NONE);
				if(TryBeginQueue())
				{
					it->second.SetStatus(ST_QUEUEING);
				}
			}
			break;
		}
	}
}

void RebornBattleField::OnBattleEnd(int tag)
{
	LOG_TRACE("OnBattleEnd:gs_id=%d,map_id=%d", server->gs_id,map_id);
	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		if (tag == it->second.tag)
		{
			LOG_TRACE("OnBattleEnd:gs_id=%d,map_id=%d,tag=%d,status=%d"
					, server->gs_id,map_id,it->second.tag,it->second.status);
			if (it->second.GetStatus() == ST_WAITINGEND || it->second.GetStatus() == ST_FIGHTING)
			{
				if (fighting_tag == tag) fighting_tag = 0;
				it->second.SetStatus(ST_NONE);
				if (TryBeginQueue())
				{
					//LOG_TRACE("BattleManager:gs_id=%d, map_id=%d, tag=%d try begin queue"
					//		, server->gs_id, map_id, it->second.tag);
					it->second.SetStatus(ST_QUEUEING);
				}
			}
			break;
		}
	}
}

size_t CrssvrBattleField::GetTeamLimit()
{
	return TEAM_CRSSVR_LIMIT;	
}

size_t NormalBattleField::GetTeamLimit()
{
	return TEAM_NORMAL_LIMIT;	
}

size_t RebornBattleField::GetTeamLimit()
{
	return 2*TEAM_NORMAL_LIMIT;	
}

size_t SonjinBattleField::GetTeamLimit()
{
	return TEAM_SONJIN_LIMIT;	
}

size_t ArenaBattleField::GetTeamLimit()
{
	return TEAM_ARENA_LIMIT;	
}

int CrssvrBattleField::JoinTeam(BattleRole & role, TeamType & team, unsigned char tid, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	LOG_TRACE("CrssvrBattleField::JoinTeam roleid=%d,gsid=%d,mapid=%d,is_reenter=%d, tid=%d", role.roleid, server->gs_id, map_id, role.is_reenter, tid);

	if (!b_gm && tid!=TEAM_LOOKER)
	{
		if (!JoinPolicy(role.level))	
			return ERR_BATTLE_LEVEL_LIMIT;
		if (team.size() >= GetTeamLimit())
			return ERR_BATTLE_TEAM_FULL;
		team.insert(std::make_pair(role.roleid, role));
	}
	else if (tid == TEAM_LOOKER)
		team.insert(std::make_pair(role.roleid, role));

	InsertReEnter(role.roleid, role.is_reenter, (TEAM)tid);

	BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
	GDeliveryServer::GetInstance()->Send(role.linksid, re);
	//¶ÔÓÚÕýÔÚÕ½¶·µÄ¿ç·þÕ½³¡, Íæ¼ÒÐèÒªÁ¢¼´½ø³¡
	if (fighting_tag)
	{
		BattleStartNotify notify(BATTLESTART_SUCCESS, role.roleid, server->gs_id, map_id, starttime, role.localsid, 0);
		SendStartNotify(role.roleid, notify, tid, starttime + GetFightingTime(), fighting_tag);
	}
	_starttime = starttime;
	return ERR_SUCCESS;
}

int NormalBattleField::JoinTeam(BattleRole & role, TeamType & team, unsigned char tid, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	if (!JoinPolicy(role.level))	
		return ERR_BATTLE_LEVEL_LIMIT;
	if (team.size() >= GetTeamLimit())
		return ERR_BATTLE_TEAM_FULL;
	team.insert(std::make_pair(role.roleid, role));

	BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
	LOG_TRACE("Send BattleJoin_Re roleid=%d,gsid=%d,mapid=%d,battle_type=%d,iscross=%d,team=%d,starttime=%d,localsid=%d", re.roleid, re.gs_id, re.map_id, re.battle_type, re.iscross, re.team, re.starttime, re.localsid);
	GDeliveryServer::GetInstance()->Send(role.linksid, re);

	_starttime = starttime;
	return ERR_SUCCESS;
}

int ChallengeSpaceBattleField::JoinTeam(BattleRole &role, TeamType &team, unsigned char tid, int &_starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData &syncdata)
{
	if(!JoinPolicy(role.level))
	      	return ERR_BATTLE_LEVEL_LIMIT;
	if((reborn_count_need == 1) && (role.reborn_cnt < 1))
	      	return ERR_BATTLE_REBORN_NEED;
	if(team.size() >= GetTeamLimit())
	      	return ERR_BATTLE_TEAM_FULL;
	team.insert(std::make_pair(role.roleid, role));

	BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
	GDeliveryServer::GetInstance()->Send(role.linksid, re);

	_starttime = starttime;
	return ERR_SUCCESS;
}

int RebornBattleField::JoinTeam(BattleRole & role, TeamType & team, unsigned char tid, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	if (!JoinPolicy(role.level))	
		return ERR_BATTLE_LEVEL_LIMIT;
	if (role.reborn_cnt<1)
		return ERR_BATTLE_REBORN_NEED;
	if (team.size() >= GetTeamLimit())
		return ERR_BATTLE_TEAM_FULL;
	team.insert(std::make_pair(role.roleid, role));

	BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
	GDeliveryServer::GetInstance()->Send(role.linksid, re);

	_starttime = starttime;
	return ERR_SUCCESS;
}

int SonjinBattleField::JoinTeam(BattleRole & role, TeamType & team, unsigned char tid, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	if (fighting_tag)
		return ERR_BATTLE_STARTED;
	if (!JoinPolicy(role.level))	
		return ERR_BATTLE_LEVEL_LIMIT;
	if (team.size() >= GetTeamLimit())
		return ERR_BATTLE_TEAM_FULL;
	team.insert(std::make_pair(role.roleid, role));

	BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
	GDeliveryServer::GetInstance()->Send(role.linksid, re);

	_starttime = starttime;
	return ERR_SUCCESS;
}

int ArenaBattleField::JoinTeam(BattleRole & role, TeamType & team, unsigned char tid, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	if (fighting_tag)
		return ERR_BATTLE_STARTED;
	if (!JoinPolicy(role.level))	
		return ERR_BATTLE_LEVEL_LIMIT;
	if (team.size()+potential_team_red.size() >= GetTeamLimit())
		return ERR_BATTLE_TEAM_FULL;
	if (itemid == -1)
	{
		//¿Í»§¶Ë·¢À´µÄÐ­Òé ¾º¼¼Õ½±¨ÃûÐ­Òé²»Ó¦¸Ã´Ó¿Í»§¶Ë·¢À´
		return -1;
	}
	else if (itemid == 0)
	{//²»ÐèÒª¿ÛÎïÆ·	
		team.insert(std::make_pair(role.roleid, role));
		BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
		GDeliveryServer::GetInstance()->Send(role.linksid, re);
		LOG_TRACE("ArenaBattleField Join success! roleid %d  no need item", role.roleid);
	}
	else
	{
		potential_team_red.insert(std::make_pair(role.roleid, role));
		DBBattleJoinArg arg(role.roleid, server->gs_id, map_id, itemid, item_pos, syncdata);
		DBBattleJoin * rpc = (DBBattleJoin *) Rpc::Call(RPC_DBBATTLEJOIN, arg);
		GameDBClient::GetInstance()->SendProtocol(rpc); 
		LOG_TRACE("ArenaBattleField Join send to DB. roleid %d itemid %d item_pos %d", role.roleid, itemid, item_pos);
	}
	_starttime = starttime;
	return ERR_SUCCESS;
}

void ArenaBattleField::OnDBBattleJoin(int retcode, int roleid)
{
	TeamType::iterator it = potential_team_red.find(roleid);
	if (it == potential_team_red.end())
	{
		Log::log(LOG_ERR, "OnDBBattleJoin role %d leave. retcode %d", roleid, retcode);	
		return;
	}
	BattleJoin_Re re(retcode, roleid, server->gs_id, map_id, field_type, 0, 0, starttime, it->second.localsid);
	GDeliveryServer::GetInstance()->Send(it->second.linksid, re);
	LOG_TRACE("ArenaBattleField OnDBBattleJoin! roleid %d retcode %d", roleid, retcode);
	if (retcode != ERR_SUCCESS)
	{
		potential_team_red.erase(roleid);
		return;
	}
	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for (; tit != tie; ++ tit)
	{
		if (tit->second.GetStatus() == ST_FIGHTING)
		{
			team_red_fighting.insert(std::make_pair(roleid, it->second));
			potential_team_red.erase(roleid);
			return;
		}
	}
	team_red.insert(std::make_pair(roleid, it->second));
	potential_team_red.erase(roleid);
}

int SonjinRebornBattleField::JoinTeam(BattleRole & role, TeamType & team, unsigned char tid, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	//Èç¹ûÕ½³¡ÒÑ¾­¿ªÆôÍæ¼Ò²»ÄÜÔÙ±¨Ãû
	if (fighting_tag)
		return ERR_BATTLE_STARTED;
	if (!JoinPolicy(role.level))	
		return ERR_BATTLE_LEVEL_LIMIT;
	if (role.reborn_cnt<1)
		return ERR_BATTLE_REBORN_NEED;
	if (team.size() >= GetTeamLimit())
		return ERR_BATTLE_TEAM_FULL;
	team.insert(std::make_pair(role.roleid, role));

	BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
	GDeliveryServer::GetInstance()->Send(role.linksid, re);

	_starttime = starttime;
	return ERR_SUCCESS;
}

int ArenaRebornBattleField::JoinTeam(BattleRole & role, TeamType & team, unsigned char tid, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	//Èç¹ûÕ½³¡ÒÑ¾­¿ªÆôÍæ¼Ò²»ÄÜÔÙ±¨Ãû
	if (fighting_tag)
		return ERR_BATTLE_STARTED;
	if (!JoinPolicy(role.level))	
		return ERR_BATTLE_LEVEL_LIMIT;
	if (role.reborn_cnt<1)
		return ERR_BATTLE_REBORN_NEED;
	if (team.size()+potential_team_red.size() >= GetTeamLimit())
		return ERR_BATTLE_TEAM_FULL;
	if (itemid == -1)
		return -1;
	else if (itemid == 0)
	{//²»ÐèÒª¿ÛÎïÆ·	
		team.insert(std::make_pair(role.roleid, role));
		BattleJoin_Re re(ERR_SUCCESS, role.roleid, server->gs_id, map_id, field_type, 0, tid, starttime, role.localsid);
		GDeliveryServer::GetInstance()->Send(role.linksid, re);
		LOG_TRACE("ArenaRebornBattleField Join success! roleid %d  no need item", role.roleid);
	}
	else
	{
		potential_team_red.insert(std::make_pair(role.roleid, role));
		DBBattleJoinArg arg(role.roleid, server->gs_id, map_id, itemid, item_pos, syncdata);
		DBBattleJoin * rpc = (DBBattleJoin *) Rpc::Call(RPC_DBBATTLEJOIN, arg);
		GameDBClient::GetInstance()->SendProtocol(rpc); 
		LOG_TRACE("ArenaRebornBattleField Join send to DB. roleid %d itemid %d item_pos %d", role.roleid, itemid, item_pos);
	}
	_starttime = starttime;
	return ERR_SUCCESS;
}

int CrssvrBattleField::JoinTeam(BattleRole & role, unsigned char team, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
//	LOG_TRACE("CrssvrBattleField JoinTeam roleid %d,team=%d,b_gm=%d fighting_tag=%d", role.roleid,team,b_gm,fighting_tag);
//	DEBUG_PRINT("CrssvrBattleField::JoinTeam fighting_tag=%d team=%d starttime=%d",fighting_tag,team,starttime);
	if (b_gm && !fighting_tag)
		return ERR_SUCCESS;

	// Èç¹ûÔÚÌæ²¹·¿¼ä²¢ÇÒÊÇ±¨Ãû½×¶Î£¬²»ÄÜ±¨Ãû
	if(role.is_reenter && !fighting_tag)
	{
		return ERR_BATTLE_ENTER_NOT_IN_JOINROOM;
	}

	if (fighting_tag)
	{
		//¿ç·þÕ½³¡¶ÓÎé£ºÈç¹ûÕýÔÚÕ½¶·£¬Ó¦¸Ã½øÈëÕ½¶·¶ÓÎé
		if(!CheckReEnter(role.roleid, role.is_reenter,(TEAM)team))
		{
			return ERR_BATTLE_ENTER_NOT_IN_JOINROOM;
		}

		if (team == TEAM_RED)
			return JoinTeam(role, team_red_fighting, team, _starttime, b_gm, itemid, item_pos, syncdata);
		else if (team == TEAM_BLUE)
			return JoinTeam(role, team_blue_fighting, team, _starttime, b_gm, itemid, item_pos, syncdata);
		else if (team == TEAM_LOOKER)
		{
			//×î¶àÔÊÐí10¸öÈË¹Û²ì
			//if (team_looker_fighting.size() >= 10)
			//	return ERR_BATTLE_TEAM_FULL;
			return JoinTeam(role, team_looker_fighting, team, _starttime, b_gm, itemid, item_pos, syncdata);
		}
	}
	
	if (team == TEAM_RED)
		return JoinTeam(role, team_red, team, _starttime, b_gm, itemid, item_pos, syncdata);
	else if (team == TEAM_BLUE)
		return JoinTeam(role, team_blue, team, _starttime, b_gm, itemid, item_pos, syncdata);

	return ERR_BATTLE_MAP_NOTEXIST;
}

int NormalBattleField::JoinTeam(BattleRole & role, unsigned char team, int & _starttime, bool b_gm, int itemid, int itempos, const GMailSyncData & syncdata)
{
	if (team == TEAM_RED)
		return JoinTeam(role, team_red, team, _starttime, b_gm, itemid, itempos, syncdata);
	else if (team == TEAM_BLUE)
		return JoinTeam(role, team_blue, team, _starttime, b_gm, itemid, itempos, syncdata);

	return ERR_BATTLE_MAP_NOTEXIST;
}

int ChallengeSpaceBattleField::JoinTeam(BattleRole &role, unsigned char team, int &_starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData &syncdata)
{
	// ÌôÕ½¿Õ¼äÖ»´æÔÚÒ»·½£¬È«¶¼·ÅÈëºì·½ÖÐ
	if(team == TEAM_RED)
	      	return JoinTeam(role, team_red, team, _starttime, b_gm, itemid, item_pos, syncdata);
	return ERR_BATTLE_MAP_NOTEXIST;
}

//·ÉÉýÕ½³¡ÔÚ±¨ÃûµÄÊ±ºòÏÈ¶¼·ÅÈëºì·½£¬ÔÚÕ½³¡¿ªÆôÊ±ÔÚÓÉ·þÎñÆ÷Ëæ¼´·ÖÅäÕóÓª
int RebornBattleField::JoinTeam(BattleRole & role, unsigned char team, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	return JoinTeam(role, team_red, team, _starttime, b_gm, itemid, item_pos, syncdata);
}


inline bool TeamErase(BattleField::TeamType & team, int roleid)
{
	BattleField::TeamType::iterator it = team.find(roleid);
	if (it != team.end())
	{
		team.erase(it);
		return true;
	}
	return false;
}

//Àë¿ªÕýÔÚÅÅ¶ÓµÄ¶ÓÎé
int CrssvrBattleField::LeaveTeam(int roleid)
{
//	DEBUG_PRINT("CrssvrBattleField::LeaveTeam roleid=%d",roleid);
	if (fighting_tag)
		return (TeamErase(team_red_fighting, roleid) || TeamErase(team_blue_fighting, roleid) || TeamErase(team_looker_fighting, roleid)) ? ERR_SUCCESS: -1;
	else
	 	return (TeamErase(team_red, roleid) || TeamErase(team_blue, roleid)) ? ERR_SUCCESS: -1;
}

int NormalBattleField::LeaveTeam(int roleid)
{
	return (TeamErase(team_red, roleid) || TeamErase(team_blue, roleid)) ? ERR_SUCCESS: -1;
}

int ChallengeSpaceBattleField::LeaveTeam(int roleid)
{
	return TeamErase(team_red, roleid) ? ERR_SUCCESS : -1;
}

int RebornBattleField::LeaveTeam(int roleid)
{
	return (TeamErase(team_red, roleid) || TeamErase(team_blue, roleid)) ? ERR_SUCCESS: -1;
}

int ArenaBattleField::LeaveTeam(int roleid)
{
	return (TeamErase(team_red, roleid) || TeamErase(team_blue, roleid)) || TeamErase(potential_team_red, roleid) ? ERR_SUCCESS: -1;
}

void CrssvrBattleField::GetMember(unsigned char team, std::vector<GBattleRole> & members)
{
	if (team != TEAM_RED && team != TEAM_BLUE)
		return;

	TeamType::iterator it, ie;
	if (fighting_tag)
	{
		if (team == TEAM_RED)
		{
			it = team_red_fighting.begin();
			ie = team_red_fighting.end();
		}
		else
		{
			it = team_blue_fighting.begin();
			ie = team_blue_fighting.end();
		}
	}
	else
	{
		if (team == TEAM_RED)
		{
			it = team_red.begin();
			ie = team_red.end();
		}
		else
		{
			it = team_blue.begin();
			ie = team_blue.end();
		}
	}
	for (; it != ie; ++ it)
		members.push_back(it->second);
}

void NormalBattleField::GetMember(unsigned char team, std::vector<GBattleRole> & members)
{
	if (team != TEAM_RED && team != TEAM_BLUE)
		return;

	TeamType::iterator it, ie;
	if (team == TEAM_RED)
	{
		it = team_red.begin();
		ie = team_red.end();
	}
	else
	{
		it = team_blue.begin();
		ie = team_blue.end();
	}
	for (; it != ie; ++ it)
		members.push_back(it->second);
}

void ChallengeSpaceBattleField::GetMember(unsigned char team, std::vector<GBattleRole> &members)
{
	if(team != TEAM_RED)
	      	return;
	TeamType::iterator it = team_red.begin(), ie = team_red.end();
	for(; it != ie; ++it)
	      	members.push_back(it->second);
}

void RebornBattleField::GetMember(unsigned char team, std::vector<GBattleRole> & members)
{
	if (team != TEAM_RED && team != TEAM_BLUE)
		return;

	TeamType::iterator it, ie;
	if (team == TEAM_RED)
	{
		it = team_red.begin();
		ie = team_red.end();
	}
	else
	{
		it = team_blue.begin();
		ie = team_blue.end();
	}

	for (; it != ie; ++ it)
		members.push_back(it->second);
}

void CrssvrBattleField::OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify)
{
	b_notify = false;
	if (now >= starttime) 
	{
		BattleStart start(0, 0, 0);
		start.end_time = starttime + GetFightingTime();
		endtime = starttime + GetFightingTime();
		start.tag = tag.tag;
		start.map_id = map_id;
		//LOG_TRACE("CrssvrBattleField Send BattleStart, tag=%d,map_id=%d,type=%d,start_time=%d,end_time=%d", start.tag,start.map_id,field_type,starttime,start.end_time);
		GProviderServer::GetInstance()->Send(server->sid, start);
		tag.SetStatus(ST_SENDSTART);
		tag.SetTimeout(now, SWITCH_TIMEOUT);
	}
}

void CrssvrTeamBattleField::OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify)
{
	b_notify = true;
	if (now >= starttime) 
	{
		starttime += GetFightingTime();
		endtime = starttime;
		BattleStart start(0, 0, 0);
		start.end_time = endtime - GetQueueingTime(GetFightingTime());
		start.tag = tag.tag;
		start.map_id = map_id;
		//LOG_TRACE("Send BattleStart, tag=%d,map_id=%d,type=%d,newstart_time=%d,end_time=%d", start.tag,start.map_id,field_type,starttime,start.end_time);
		GProviderServer::GetInstance()->Send(server->sid, start);
		tag.SetStatus(ST_SENDSTART);
		tag.SetTimeout(now, SWITCH_TIMEOUT);
	}
}

void NormalBattleField::OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify)
{
	b_notify = true;
	if (now >= starttime) 
	{
		BattleStart start(0, 0, 0);
		start.end_time = starttime + GetFightingTime();
		starttime += GetFightingTime();
		endtime = starttime;
		start.tag = tag.tag;
		start.map_id = map_id;
		//LOG_TRACE("Send BattleStart, tag=%d,map_id=%d,type=%d,start_time=%d,end_time=%d", start.tag,start.map_id,field_type,starttime,start.end_time);
		GProviderServer::GetInstance()->Send(server->sid, start);
		tag.SetStatus(ST_SENDSTART);
		tag.SetTimeout(now, SWITCH_TIMEOUT);
	}
}

void ChallengeSpaceBattleField::OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify)
{
	b_notify = true;
	if(now >= starttime)
	{
		BattleStart start(0, 0, 0);
		//ÓÉÓÚÖ»ÓÐÒ»¸öÕ½³¡½øÐÐÂÖ×ª£ Í¨ÖªGS½áÊøÊ±¼äÊ±ÉÔÎ¢ÍùÇ°ÌáÇ°Ò»·ÖÖÓ£¬ÒÔ±£Ö¤GSÄÜ¹»½«×ÊÔ´ÇåÀíÍê±Ï
		start.end_time = starttime + GetFightingTime() - TIMEOFFSET_BETWEEN_GS_DELI;
		starttime += GetFightingTime(); //´ËÊ±starttime¼ÇÂ¼µÄÎªÏÂÒ»³¡Õ½³¡¿ªÆôµÄÊ±¼ä
		endtime = starttime;
		if(!TimePolicy()) //¼ì²éÏÂÒ»³¡Õ½³¡µÄ¿ªÆôÊ±¼äÊÇ·ñ»¹ÔÚÕ½³¡Ã¿ÌìµÄ¿ª·ÅÊ±¼ä¶ÎÄÚ
		{
			b_open = false;
		}
		start.tag = tag.tag;
		start.map_id = map_id;
		//LOG_TRACE("Send BattleStart, tag=%d,map_id=%d,type=%d,start_time=%d,end_time=%d", start.tag,start.map_id,field_type,starttime,start.end_time);
		GProviderServer::GetInstance()->Send(server->sid, start);
		tag.SetStatus(ST_SENDSTART);
		tag.SetTimeout(now, SWITCH_TIMEOUT);
	}
}

void RebornBattleField::OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify)
{
	b_notify = true;
	if (now >= starttime) 
	{
		BattleStart start(0, 0, 0);
		start.end_time = starttime + GetFightingTime();
		starttime += GetFightingTime();
		endtime = starttime;
		start.tag = tag.tag;
		start.map_id = map_id;
		//LOG_TRACE("Send BattleStart, tag=%d,map_id=%d,type=%d,start_time=%d,end_time=%d", start.tag,start.map_id,field_type,starttime,start.end_time);
		GProviderServer::GetInstance()->Send(server->sid, start);
		tag.SetStatus(ST_SENDSTART);
		tag.SetTimeout(now, SWITCH_TIMEOUT);
	}
}

void SonjinBattleField::OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify)
{
	b_notify = true;
	if (now >= starttime) 
	{
		//ËÎ½ðÕ½³¡ÒªÅÐ¶ÏÕýÊ½¿ªÆôÌõ¼þ
		if (StartPolicy())
		{
			BattleStart start(0, 0, 0);
#if (defined ARENA_DEBUG || defined SONJIN_DEBUG)
			endtime = starttime + 1800;
#else
			endtime = starttime + GetFightingTime();
#endif
			start.end_time = endtime;
			start.tag = tag.tag;
			start.map_id = map_id;
			start.apply_count = GetRedNumber();
			//LOG_TRACE("Send BattleStart, tag=%d,map_id=%d,type=%d,start_time=%d,end_time=%d", start.tag,start.map_id,field_type,starttime,start.end_time);
			GProviderServer::GetInstance()->Send(server->sid, start);
			tag.SetStatus(ST_SENDSTART);
			tag.SetTimeout(now, SWITCH_TIMEOUT);
		}
		else
		{
			time_t now = BattleManager::GetInstance()->GetTime();
			BattleStartNotify notify(BATTLESTART_JOINNUMBER, 0, server->gs_id, map_id, now, 0, 0);
			Broadcast(notify, notify.localsid);
			team_red.clear();
			team_blue.clear();
			tag.ClearTimeout();
			tag.SetStatus(ST_NONE);
			if (fighting_tag == tag.tag) fighting_tag = 0;
		}
	}
}

bool SonjinBattleField::StartPolicy()
{
#ifdef SONJIN_DEBUG
	return (GetRedNumber() >= 2);
#else
	return (GetRedNumber() >= 30);
#endif
}

bool ArenaBattleField::StartPolicy()
{
#ifdef ARENA_DEBUG
	return (GetRedNumber() >= 2);
#else
	return (GetRedNumber() >= 20);
#endif
}

void BattleField::Update(time_t now, bool & b_notify)
{
	for (TagInfos::iterator it = taginfos.begin(); it != taginfos.end(); ++ it)
	{
		char status = it->second.GetStatus();
		switch (status)
		{
		case BattleField::ST_QUEUEING:
			OnQueueingUpdate(now, it->second, b_notify);
			break;

		case BattleField::ST_FIGHTING:
			{
				if (it->second.IsTimeout(now))
				{
					it->second.ClearTimeout();
				}
				if (now >= endtime)
				{
			//		LOG_TRACE("BattleField::Update ,ST_FIGHTING overtime start_time=%d,end_time=%d", starttime,endtime);
					it->second.SetStatus(BattleField::ST_WAITINGEND);
					it->second.SetTimeout(now, BattleField::SWITCH_TIMEOUT);
				}
			}
			break;
		case BattleField::ST_SENDSTART:
			if (it->second.IsTimeout(now))
			{
				//OnBattleEnd(it->first);
				OnBattleStart(ERR_BATTLE_TIMEOUT);
			}
			break;
		case BattleField::ST_WAITINGEND:
			if (it->second.IsTimeout(now))
			{
				it->second.ClearTimeout();
				it->second.SetStatus(BattleField::ST_NONE);
			}
			break;
		}

	}
	for (TagInfos::iterator it = taginfos.begin(); it != taginfos.end(); ++ it)
	{
		char status = it->second.GetStatus();
		if (status == ST_NONE && TryBeginQueue())
		{
			it->second.SetStatus(BattleField::ST_QUEUEING);
		}
	}
	// update user info per 5 minutes
	counter ++;
	if (counter >= 15)  
	{
		counter = 0;
		UserContainer& container = UserContainer::GetInstance();
		Thread::RWLock::RDScoped l(container.GetLocker());
		PlayerInfo * pinfo;
		for (TeamType::iterator it = team_blue.begin(); it != team_blue.end(); ++it)
		{
			pinfo = container.FindRoleOnline(it->second.roleid);
			if (pinfo)
			{
				it->second.level = pinfo->level;
				it->second.occupation = pinfo->occupation;
				it->second.score = pinfo->battlescore;
			}
		}
		for (TeamType::iterator it = team_red.begin(); it != team_red.end(); ++it)
		{
			pinfo = container.FindRoleOnline(it->second.roleid);
			if (pinfo)
			{
				it->second.level = pinfo->level;
				it->second.occupation = pinfo->occupation;
				it->second.score = pinfo->battlescore;
			}
		}
	}
}

void CrssvrBattleField::Init(const BattleFieldInfo &info, BattleServer * _server)
{
	map_id = info.map_id;
	level_min = info.level_min;
	level_max = info.level_max;
	server = _server;
	b_open = true;
	struct tm dt;
	time_t now = BattleManager::GetInstance()->GetTime();
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour += 1;
	starttime = mktime(&dt);
}

void CrssvrTeamBattleField::Init(const BattleFieldInfo &info, BattleServer * _server)
{
	map_id = info.map_id;
	level_min = info.level_min;
	level_max = info.level_max;
	server = _server;
	b_open = true;
	struct tm dt;
	time_t now = BattleManager::GetInstance()->GetTime();
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour += 1;
//	dt.tm_sec = 0;
//	dt.tm_min += 3;
	starttime = mktime(&dt);
	fighting_time = info.fighting_time;
	maxplayer_perround = info.maxplayer_perround;
}

void NormalBattleField::Init(const BattleFieldInfo &info, BattleServer * _server)
{
	map_id = info.map_id;
	level_min = info.level_min;
	level_max = info.level_max;
	server = _server;
	starttime = server->starttime;
}

void ChallengeSpaceBattleField::Init(const BattleFieldInfo &info, BattleServer *_server)
{
	map_id = info.map_id;
	level_min = info.level_min;
	level_max = info.level_max;
	server = _server;
	starttime = server->challenge_starttime;
	reborn_count_need = info.reborn_count_need;
	fighting_time = info.fighting_time;
	entermax_perday = info.entermax_perday;
	cooldown_time = info.cooldown_time;
	maxplayer_perround = info.maxplayer_perround;
}

void RebornBattleField::Init(const BattleFieldInfo &info, BattleServer * _server)
{
	map_id = info.map_id;
	level_min = info.level_min;
	level_max = info.level_max;
	server = _server;
	starttime = server->starttime;
}

void BattleField::AddTag(int tag)
{
	TagInfos::iterator it = taginfos.find(tag);
	if (it != taginfos.end())
	{
		if (it->second.GetStatus() == ST_FIGHTING)
		{
			it->second.SetStatus(ST_NONE);
		}
		return;
	}
	taginfos.insert(std::make_pair(tag, TagInfo(tag)));
}

enum
{
	NORMAL_BATTLEFIELD_START_TIME = 64800,
	NORMAL_BATTLEFIELD_END_TIME = 79200, 	

	REBORN_BATTLEFIELD_FORMAL_START_TIME = 0,	//·ÉÉýÕ½³¡ÕýÊ½¿ªÆôºó±¨ÃûÊ±¼ä¶Î¸ÄÎª00:00-24:00
	REBORN_BATTLEFIELD_FORMAL_END_TIME=86400,

	SONJIN_BATTLEFIELD_START_TIME1 = 46800, //13:00
	SONJIN_BATTLEFIELD_START_TIME2 = 57600,//16:00
	SONJIN_QUEUEING_TIME   = 1800,	//ËÎ½ðÕ½³¡±¨Ãû0.5Ð¡Ê±

	ARENA_BATTLEFIELD_START_TIME = 70200,//¾º¼¼³¡Ã¿ÖÜÈÕ19:30µã¿ªÆô
	ARENA_QUEUEING_TIME = 1800,		//¾º¼¼³¡±¨Ãû°ë¸öÐ¡Ê±

	CHALLENGESPACE_BATTLEFIELD_START_TIME = 0,
	CHALLENGESPACE_BATTLEFIELD_END_TIME = 86400,
};

void SonjinBattleField::Init(const BattleFieldInfo &info, BattleServer * _server)
{
	map_id = info.map_id;
	level_min = info.level_min;
	level_max = info.level_max;
	server = _server;
	starttime = 0;
}

bool NormalBattleField::TimePolicy()
{
	time_t now = BattleManager::GetInstance()->GetTime();

	time_t start_time = now - BattleManager::GetInstance()->GetTimeBase() + GetFightingTime();

	if (start_time >= NORMAL_BATTLEFIELD_START_TIME && start_time < NORMAL_BATTLEFIELD_END_TIME)
		return true;
	return false;
}

bool ChallengeSpaceBattleField::TimePolicy()
{
	time_t now = BattleManager::GetInstance()->GetTime();
	time_t start_time = now - BattleManager::GetInstance()->GetTimeBase() + GetFightingTime();
	if(start_time >= CHALLENGESPACE_BATTLEFIELD_START_TIME && start_time < CHALLENGESPACE_BATTLEFIELD_END_TIME)
	      	return true;
	return false;
}

bool RebornBattleField::TimePolicy()
{
	time_t now = BattleManager::GetInstance()->GetTime();

	time_t start_time = now - BattleManager::GetInstance()->GetTimeBase() + GetFightingTime();

	if (open_status==OPEN_UNFORMAL 
			&& start_time >= NORMAL_BATTLEFIELD_START_TIME 
			&& start_time < NORMAL_BATTLEFIELD_END_TIME)
		return true;

	if (open_status==OPEN_FORMAL
			&& start_time >= REBORN_BATTLEFIELD_FORMAL_START_TIME
			&& start_time < REBORN_BATTLEFIELD_FORMAL_END_TIME)
		return true;

	return false;
}

bool SonjinBattleField::TimePolicy()
{
	time_t now = BattleManager::GetInstance()->GetTime();
#ifdef SONJIN_DEBUG
	time_t start_time = now - BattleManager::GetInstance()->GetTimeBase();
	if (start_time >= 0 && start_time < 900)
	{
		starttime = BattleManager::GetInstance()->GetTimeBase()+900;
		return true;
	}
#else
	struct tm dt;
	localtime_r(&now, &dt);
	if (dt.tm_wday==0 || dt.tm_wday==6)
	{
		time_t start_time = now - BattleManager::GetInstance()->GetTimeBase();

		if (start_time >= SONJIN_BATTLEFIELD_START_TIME1 && start_time < SONJIN_BATTLEFIELD_START_TIME1+SONJIN_QUEUEING_TIME)
		{
			starttime = BattleManager::GetInstance()->GetTimeBase()+SONJIN_BATTLEFIELD_START_TIME1+SONJIN_QUEUEING_TIME;
			return true;
		}
		if (start_time >= SONJIN_BATTLEFIELD_START_TIME2 && start_time < SONJIN_BATTLEFIELD_START_TIME2+SONJIN_QUEUEING_TIME)
		{
			starttime = BattleManager::GetInstance()->GetTimeBase()+SONJIN_BATTLEFIELD_START_TIME2+SONJIN_QUEUEING_TIME;
			return true;
		}
	}
#endif
	return false;
}

bool ArenaBattleField::TimePolicy()
{
	time_t now = BattleManager::GetInstance()->GetTime();	
#ifdef ARENA_DEBUG
	time_t start_time = now - BattleManager::GetInstance()->GetTimeBase();
	if (start_time >= 0 && start_time < 900)
	{
		starttime = BattleManager::GetInstance()->GetTimeBase()+900;
		return true;
	}
#else
	struct tm dt;
	localtime_r(&now, &dt);
	if (dt.tm_wday == 0)
	{
		time_t start_time = now - BattleManager::GetInstance()->GetTimeBase();

		if (start_time >= ARENA_BATTLEFIELD_START_TIME && start_time < ARENA_BATTLEFIELD_START_TIME+ARENA_QUEUEING_TIME)
		{
			starttime = BattleManager::GetInstance()->GetTimeBase()+ARENA_BATTLEFIELD_START_TIME+ARENA_QUEUEING_TIME;
			return true;
		}
	}
#endif
	return false;
}

bool CrssvrBattleField::CheckFormalOpenCond(const GTopTable & table, int level_min)
{
	if (table.items.size() < 500)
		return false;
	std::vector<GTopItem>::const_iterator it = table.items.begin();
	std::advance(it, 499);

	int64_t tail_level = it->value;

	if (level_min>= 135)
		if (tail_level >= 136) return true; else return false;
	if (level_min >= 120)
		if (tail_level >= 125) return true; else return false;

	if (level_min >= 105)
		if (tail_level >= 110) return true; else return false;
	if (level_min >= 90)
		if (tail_level >= 95) return true; else return false;
	if (level_min >= 75)
		if (tail_level >= 80) return true; else return false;
	return false;		
}

bool NormalBattleField::CheckFormalOpenCond(const GTopTable & table, int level_min)
{
	if (table.items.size() < 500)
		return false;
	std::vector<GTopItem>::const_iterator it = table.items.begin();
	std::advance(it, 499);

	int64_t tail_level = it->value;

	if (level_min>= 135)
		if (tail_level >= 136) return true; else return false;
	if (level_min >= 120)
		if (tail_level >= 125) return true; else return false;

	if (level_min >= 105)
		if (tail_level >= 110) return true; else return false;
	if (level_min >= 90)
		if (tail_level >= 95) return true; else return false;
	if (level_min >= 75)
		if (tail_level >= 80) return true; else return false;
	return false; 
}

bool ChallengeSpaceBattleField::CheckFormalOpenCond(const GTopTable &table, int level_min)
{
#ifdef CHALLENGESPACE_DEBUG
	int64_t tail_level = 0;
	std::vector<GTopItem>::const_reverse_iterator rit = table.items.rbegin();
	if(rit != table.items.rend())
	{
		tail_level = rit->value;
	}
	else
	{
		return false;
	}
	if(reborn_count_need == 1) // Èç¹ûÎª·ÉÉýµØÍ¼
	{
		//¹æ¶¨ÐèÒªÅÅÐÐ°ñ×îºóÒ»Ãû´ïµ½·ÉÉý90¼¶²ÅÄÜ¿ªÆôÌôÕ½¿Õ¼ä
		return (tail_level >= (200 + 90)) ? true : false;
	}
	return false;
#else
	return true;
#endif
}

bool RebornBattleField::CheckFormalOpenCond(const GTopTable & table, int level_min)
{
	if (table.items.size() < 500)
		return false;
	std::vector<GTopItem>::const_iterator it = table.items.begin();
	std::advance(it, 499);

	int64_t tail_level = it->value;

	if (level_min>= 135)
		if (tail_level >= 336) return true; else return false;
	if (level_min >= 120)
		if (tail_level >= 325) return true; else return false;

	if (level_min >= 105)
		if (tail_level >= 310) return true; else return false;
	if (level_min >= 90)
		if (tail_level >= 295) return true; else return false;
	return false;		
}

bool CrssvrBattleField::CheckUnFormalOpenCond(const GTopTable & table, int level_min)
{
	if (table.items.size() < 30)
		return false;
	std::vector<GTopItem>::const_iterator it = table.items.begin();
	std::advance(it, 29);
	int64_t tail_level = it->value;

	if (level_min >= 135)
		if (tail_level >= 135) return true; else return false;
	if (level_min >= 120)
		if (tail_level >= 120) return true; else return false;
	if (level_min >= 105)
		if (tail_level >= 105) return true; else return false;
	if (level_min >= 90)
		if (tail_level >= 90) return true; else return false;
	if (level_min >= 75)
		if (tail_level >= 75) return true; else return false;
	return false;
}

bool NormalBattleField::CheckUnFormalOpenCond(const GTopTable & table, int level_min)
{
	if (table.items.size() < 30)
		return false;
	std::vector<GTopItem>::const_iterator it = table.items.begin();
	std::advance(it, 29);
	int64_t tail_level = it->value;

	if (level_min >= 135)
		if (tail_level >= 135) return true; else return false;
	if (level_min >= 120)
		if (tail_level >= 120) return true; else return false;
	if (level_min >= 105)
		if (tail_level >= 105) return true; else return false;
	if (level_min >= 90)
		if (tail_level >= 90) return true; else return false;
	if (level_min >= 75)
		if (tail_level >= 75) return true; else return false;
	return false;
}

bool ChallengeSpaceBattleField::CheckUnFormalOpenCond(const GTopTable &table, int level_min)
{
	return false;
}

bool RebornBattleField::CheckUnFormalOpenCond(const GTopTable & table, int level_min)
{
	if (table.items.size() < 30)
		return false;
	std::vector<GTopItem>::const_iterator it = table.items.begin();
	std::advance(it, 29);
	int64_t tail_level = it->value;

	if (level_min >= 135)
		if (tail_level >= 335) return true; else return false;
	if (level_min >= 120)
		if (tail_level >= 320) return true; else return false;
	if (level_min >= 105)
		if (tail_level >= 305) return true; else return false;
	if (level_min >= 90)
		if (tail_level >= 290) return true; else return false;
	return false;
}

/*
return:	 0 : ²»Âú×ãÅÅÃû
	¡1 : Âú×ãÊÔÑé¿ªÆôÅÅÃû
	 2 : Âú×ãÕýÊ½¿ªÆôÅÅÃû
*/
int BattleField::TopPolicy()
{
	// ¿ç·þÕ½³¡Ã»ÓÐÅÅÐÐ°ñÒªÇó
	if(GDeliveryServer::GetInstance()->IsCentralDS())
		return 2;

	if (b_debug_mode)
		return 2;
#ifdef CHALLENGESPACE_DEBUG
	GTopTable table;
	GTopItem item;
	item.value = 290;
	table.items.push_back(item);
#else
	GTopTable table;
	if (0 != TopManager::Instance()->GetDailyTable(TOP_PERSONAL_LEVEL, table))
		return 0;
#endif
	if (CheckFormalOpenCond(table, level_min))
		return 2;
	if (CheckUnFormalOpenCond(table, level_min))
		return 1;
	return 0;

	return 2;
}

bool CrssvrBattleField::TryBeginQueue()
{
	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		char status = it->second.GetStatus();
		if (status != ST_NONE) 
			return false;
	}
	return true;
}

bool NormalBattleField::TryBeginQueue()
{
	if (b_open)
	{
		if (open_status == OPEN_UNFORMAL && !TimePolicy())
		{
			b_open = false;
			open_status = OPEN_NONE;
			return false;
		}
	}
	else 
	{
		int top = TopPolicy();
		if (0 == top)
			return false;
		if (1 == top)
		{
			if (!TimePolicy())
				return false;

			b_open = true;
			open_status = OPEN_UNFORMAL;
			LOG_TRACE("BattleField open not formally! mapid=%d, gs_id=%d", map_id, server->gs_id);  
		}
		else // 2 == top
		{
			b_open = true;
			open_status = OPEN_FORMAL;
			LOG_TRACE("BattleField open formally! mapid=%d, gs_id=%d", map_id, server->gs_id);  

		}
	}

	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		char status = it->second.GetStatus();
		if (status == ST_QUEUEING)
			return false;
	}
	return true;
}

bool ChallengeSpaceBattleField::TryBeginQueue()
{
	if(b_open)
	{
		if(!TimePolicy())
		{
			b_open = false;
			open_status = OPEN_NONE;
			return false;
		}
	}
	else
	{
		int top = TopPolicy();
		if(0 == top)
		{
		      	return false;
		}
		else
		{
			if(!TimePolicy())
			      	return false;
			b_open = true;
			open_status = OPEN_FORMAL;
			LOG_TRACE("BattleField open formally! mapid=%d, gs_id=%d", map_id, server->gs_id);
		}
	}

	return true;
}

bool RebornBattleField::TryBeginQueue()
{
	//·ÉÉýÕ½³¡ÕýÊ½¿ªÆôµÄ±¨ÃûÊ±¼äÎª00:00-24:00£¬·ÇÕýÊ½¿ªÆôµÄ±¨ÃûÊ±¼äÎª18:00-22:00
	//remove TimePolicy check in OPEN_FORMAL status
	if (b_open)
	{
		if (open_status == OPEN_UNFORMAL && !TimePolicy())
		{
			b_open = false;
			open_status = OPEN_NONE;
			return false;
		}
	}
	else 
	{
		int top = TopPolicy();
		if (0 == top)
			return false;
		if (1 == top)
		{
			open_status = OPEN_UNFORMAL;		//ÔÚTimePolicyÖÐ¸ù¾Ý´Ë±êÊ¶ÅÐ¶Ï²»Í¬µÄ±¨ÃûÊ±¼ä¶Î
			if (!TimePolicy())
			{
				open_status = OPEN_NONE;
				return false;
			}
			b_open = true;
			LOG_TRACE("Reborn BattleField open not formally! mapid=%d, gs_id=%d", map_id, server->gs_id);  
		}
		else // 2 == top
		{
			open_status = OPEN_FORMAL;
			b_open = true;
			LOG_TRACE("Reborn BattleField open formally! mapid=%d, gs_id=%d", map_id, server->gs_id);  
		}
	}

	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		char status = it->second.GetStatus();
		if (status == ST_QUEUEING)
			return false;
	}
	return true;
}

bool SonjinBattleField::TryBeginQueue()
{
	if (b_open)
	{
		if (!TimePolicy())
		{
			b_open = false;
			open_status = OPEN_NONE;
			return false;
		}
	}
	else 
	{
		if (!TimePolicy())
			return false;

		b_open = true;
		open_status = OPEN_FORMAL;
		LOG_TRACE("BattleField open ! mapid=%d, gs_id=%d", map_id, server->gs_id);  
	}

	TagInfos::iterator it = taginfos.begin(), ie = taginfos.end();
	for (; it != ie; ++ it)
	{
		char status = it->second.GetStatus();
		if (status == ST_QUEUEING)
			return false;
	}
	return true;
}

void BattleField::OnDisconnect()
{
	for (TagInfos::iterator it = taginfos.begin(); it != taginfos.end(); ++ it)
	{
		if (it->second.GetStatus() == ST_WAITINGEND || it->second.GetStatus() == ST_FIGHTING)
		{
			OnBattleEnd(it->first);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void BattleServer::Init(int _gs_id, int _sid, time_t _starttime, const std::vector<BattleFieldInfo> &info, time_t _challenge_starttime)
{
	gs_id = _gs_id;
	sid = _sid;
	if (0 == starttime)
		starttime = _starttime;
	if(0 == challenge_starttime)
	      	challenge_starttime = _challenge_starttime;
	LOG_TRACE("BattleServer Init: gs_id=%d, sid=%d, starttime=%s, challenge_starttime=%s", gs_id, sid, ctime(&starttime), ctime(&challenge_starttime));
	for (BattleMap::iterator it = battlemap.begin(); it !=  battlemap.end(); ++it)
	{
		it->second->Destroy();
	}
	battlemap.clear();

	for (std::vector<BattleFieldInfo>::const_iterator it = info.begin(); it != info.end(); ++it)
	{
		LOG_TRACE("BattleServer Init:gs_id=%d,map_id=%d,tag=%d,level_min=%d,level_max=%d,reborn_count_need=%d,fighting_time=%d,entermax_perday=%d, cooldown_time=%d, maxplayer_perround=%d"
				, gs_id, it->map_id, it->tag, it->level_min, it->level_max, it->reborn_count_need, it->fighting_time, it->entermax_perday, it->cooldown_time, it->maxplayer_perround);
		BattleMap::iterator bit = battlemap.find(it->map_id);
		BattleField *field = NULL;
		if (bit == battlemap.end())
		{
			field = BattleField::Create(it->battle_type);
			if (field != NULL) battlemap[it->map_id] = field;
		}
		else
			field = bit->second;

		if (field != NULL)
		{
			field->Init(*it, this);
			field->AddTag(it->tag);
		}
	}
}

void BattleServer::Update(time_t now)
{
	bool b_notify = false;
	for (BattleMap::iterator it = battlemap.begin(); it !=  battlemap.end(); ++it)
	{
		it->second->Update(now, b_notify);
	}

	if (b_notify && (starttime - now) < ONE_MINUTE && (starttime - now) >= (ONE_MINUTE - UPDATE_INTERVAL))
	{
		ChatBroadCast cbc;
		cbc.channel = GP_CHAT_SYSTEM;
		cbc.srcroleid = MSG_BATTLEWILLSTART;
		Marshal::OctetsStream data;
		data << (int)gs_id << (int)starttime;
		cbc.msg = data;
		LinkServer::GetInstance().BroadcastProtocol(cbc);
	}

	if (now > starttime) starttime += NORMAL_FIGHTING_TIME;
	if (now > challenge_starttime) challenge_starttime += CHALLENGESPACE_FIGHTING_TIME;
}

void BattleServer::OnBattleStart(int map_id, int retcode)
{
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
		it->second->OnBattleStart(retcode);
}

void BattleServer::OnBattleEnd(int map_id, int tag)
{
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		it->second->OnBattleEnd(tag);
		return;
	}
}

int BattleServer::OnBattleEnter(int roleid, int map_id, int & endtime, char &battle_type, bool b_gm)
{
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		endtime = it->second->endtime;
		battle_type = it->second->GetFieldType();
		LOG_TRACE("BattleServer::OnBattleEnter, battle_type=%d, starttime=%d, endtime=%d, map_id=%d", battle_type, it->second->starttime, it->second->endtime, it->second->map_id);
		return it->second->OnBattleEnter(roleid, b_gm);
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int BattleServer::OnBattleEnterFail(int roleid, int map_id)
{
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
		return it->second->OnBattleEnterFail(roleid);
	return ERR_BATTLE_MAP_NOTEXIST;
}

void BattleServer::OnDBBattleJoin(int retcode, int roleid, int map_id)
{
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
		return it->second->OnDBBattleJoin(retcode, roleid);
}

void BattleServer::OnLogout(int roleid)
{
	for (BattleMap::iterator it = battlemap.begin(); it != battlemap.end(); ++ it)
	{
		if (it->second->LeaveTeam(roleid) == ERR_SUCCESS || it->second->LeaveGame(roleid) == ERR_SUCCESS)
			return;
	}
}

void BattleServer::GetMap(BattleGetList_Re & re, char battle_type)
{
	for (BattleMap::iterator it = battlemap.begin(); it !=  battlemap.end(); ++it)
	{
		BattleField & field = *it->second;
		if (!field.b_open)
			continue;
		if (battle_type == BATTLEFIELD_TYPE_CRSSVR)
		{
			if (field.field_type!=BATTLEFIELD_TYPE_CRSSVR && field.field_type!=BATTLEFIELD_TYPE_ANONCRSSVR)
				continue;
		}
		else if (field.field_type != battle_type)
			continue;
		re.battle_type = field.field_type;
		re.fields.push_back(GFieldInfo(gs_id, field.map_id, 0, 0, field.starttime));
		GFieldInfo & info = re.fields.back();

		info.red_number = field.GetRedNumber();
		info.blue_number = field.GetBlueNumber();
	}
}

void BattleServer::GetFieldInfo(int map_id, int roleid ,BattleGetField_Re & re)
{
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end() && it->second->b_open)
	{
		re.retcode = ERR_SUCCESS;
		re.starttime = it->second->starttime;
		re.battle_type = it->second->GetFieldType();
		if(re.battle_type != BATTLEFIELD_TYPE_CRSSVRTEAM)
		{
			it->second->GetMember(TEAM_RED, re.red_team);
			it->second->GetMember(TEAM_BLUE, re.blue_team);
		}
		else
		{
			CrssvrTeamBattleField* pf = dynamic_cast<CrssvrTeamBattleField*>(it->second);
			if(pf!=NULL)
			{
				if(pf->IsRoleInTheTeam(roleid,TEAM_RED))
					pf->GetMember(TEAM_RED, re.red_team);
				else if(pf->IsRoleInTheTeam(roleid,TEAM_BLUE))
					pf->GetMember(TEAM_BLUE, re.blue_team);
			}
		}
	}
}

int BattleServer::JoinTeam(BattleRole & role, int map_id, unsigned char team, int & _starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata, char &battle_type)
{
	if (IsRoleInTeam(role.roleid))
		return ERR_BATTLE_JOIN_ALREADY;
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		return it->second->JoinTeam(role, team, _starttime, b_gm, itemid, item_pos, syncdata);
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int BattleServer::LeaveTeam(int roleid)
{
	int retcode;
	for (BattleMap::iterator it = battlemap.begin(); it != battlemap.end(); ++it)
	{
		retcode = it->second->LeaveTeam(roleid);
		if (retcode == ERR_SUCCESS)
			return retcode;
	}
	return ERR_BATTLE_NOT_INTEAM;
}

bool BattleServer::IsRoleInTeam(int roleid)
{
	for (BattleMap::iterator it = battlemap.begin(); it != battlemap.end(); ++it)
	{
		if (it->second->IsRoleInTeam(roleid))
			return true;
	}
	return false;
}
void BattleServer::OnLeaveGame(int map_id, int tag, int roleid, int reason)
{
	BattleMap::iterator it = battlemap.find(map_id);
	if (it != battlemap.end())
	{
		it->second->LeaveGame(roleid);
	}
}
void BattleServer::OnDisconnect()
{
	for (BattleMap::iterator it = battlemap.begin(); it != battlemap.end(); ++it)
	{
		it->second->OnDisconnect();
	}
}

/////////////////////////////////////////////////////////////////////////
time_t BattleManager::AllocStartTime()
{
	std::set<time_t> slots;
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		slots.insert(it->second.GetStartTime());
	}
	if (slots.size() == 0)
	{
		return GetTime() + NORMAL_FIGHTING_TIME;
	}
	time_t lasttime = 0;
	for (std::set<time_t>::iterator it = slots.begin(); it != slots.end(); ++ it)
	{
		if (lasttime == 0)
		{
			lasttime = *it;
			continue;
		}
		if (*it - lasttime > NORMAL_BATTLEFIELD_START_DURATION)
			return lasttime + NORMAL_BATTLEFIELD_START_DURATION;
		lasttime = *it;
	}
	time_t time;
	
	if (lasttime == 0)
		time = GetTime() + NORMAL_FIGHTING_TIME;
	else
		time =  lasttime + NORMAL_BATTLEFIELD_START_DURATION;
	return time;
}

time_t BattleManager::AllocChallengeTime()
{
	std::set<time_t> slots;
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		slots.insert(it->second.GetChallengeTime());
	}
	if (slots.size() == 0)
	{
#ifdef CHALLENGESPACE_DEBUG
		return GetTime() + CHALLENGESPACE_BATTLEFIELD_START_DURATION * 2;
#else
		return GetTime() + CHALLENGESPACE_FIGHTING_TIME;
#endif
	}
	time_t lasttime = 0;
	for (std::set<time_t>::iterator it = slots.begin(); it != slots.end(); ++ it)
	{
		if (lasttime == 0)
		{
			lasttime = *it;
			continue;
		}
		if (*it - lasttime > CHALLENGESPACE_BATTLEFIELD_START_DURATION)
			return lasttime + CHALLENGESPACE_BATTLEFIELD_START_DURATION;
		lasttime = *it;
	}
	time_t time;
	
	if (lasttime == 0)
		time = GetTime() + CHALLENGESPACE_FIGHTING_TIME;
	else
		time =  lasttime + CHALLENGESPACE_BATTLEFIELD_START_DURATION;
	return time;
}

void BattleManager::RegisterServer(int sid, int gs_id, const std::vector<BattleFieldInfo> & info)
{
	time_t starttime = AllocStartTime();
	time_t challenge_starttime = AllocChallengeTime();
	LOG_TRACE("alloc time=%s, starttime=%d, gs_id=%d", ctime(&starttime), starttime, gs_id);
	BattleServer & server = servers[gs_id];
	server.Init(gs_id, sid, starttime, info, challenge_starttime);
}
void BattleManager::GetMap(int roleid, char battle_type, int linksid, int localsid)
{
	BattleGetList_Re re;
	re.retcode = ERR_SUCCESS;
	re.localsid = localsid;
	re.battle_type = battle_type;
	//CRSSVR type enter_times always 0
	if (battle_type != BATTLEFIELD_TYPE_CRSSVR && battle_type != BATTLEFIELD_TYPE_CRSSVRTEAM)
	{
		FieldRoleTimer::iterator fit = rolemap.find(battle_type);
		if (fit != rolemap.end())
		{
			RoleTimer::iterator it = fit->second.find(roleid);
			if (it != fit->second.end())
			{
				re.enter_times =  it->second.second;
			}
			else
			{
				re.enter_times = 0;
			}
		}
		else
			re.enter_times = 0;
	}
	else
	{
			re.enter_times = 0;
	}

	for (ServerMap::iterator it = servers.begin(); it != servers.end(); ++ it)
	{
		it->second.GetMap(re, battle_type);
	}
	LOG_TRACE("BattleManager::GetMap: battle_type=%d, fields.size=%d", battle_type, re.fields.size());
	GDeliveryServer::GetInstance()->Send(linksid, re);
}

void BattleManager::GetFieldInfo(int gs_id, int map_id, int roleid, BattleGetField_Re & re)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.GetFieldInfo(map_id, roleid, re);
}

int BattleManager::JoinTeam(BattleRole & role, int gs_id, int map_id, unsigned char team, int & starttime, int & cooltime, char &battle_type,bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata)
{
	//if(!GDeliveryServer::GetInstance()->IsCentralDS() && CrossBattleManager::GetInstance()->IsRoleInTeam(role.roleid, NORMAL_BATTLE))
	//      	return ERR_BATTLE_JOIN_ALREADY;
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	/*char field_type = it->second.GetFieldType(map_id);
	if (field_type== -1)
		return ERR_BATTLE_MAP_NOTEXIST;*/
	BattleField *field = it->second.GetBattleField(map_id);
	if(field == NULL)
	      	return ERR_BATTLE_MAP_NOTEXIST;
	int ret = IsRoleInCoolDown(field->GetFieldType(), role.roleid, field, cooltime);
	if (ret != ERR_SUCCESS)
		return ret;
	ret = it->second.JoinTeam(role, map_id, team, starttime, b_gm, itemid, item_pos, syncdata, battle_type);
	if(ret == ERR_SUCCESS && GDeliveryServer::GetInstance()->IsCentralDS())
	{
		CrossBattleManager::GetInstance()->JoinTeam(NORMAL_BATTLE, battle_type, gs_id, map_id, role.roleid, starttime, team, role.src_zoneid);
	}
	return ret;
}

//Àë¿ªÕýÔÚÕ½¶·µÄ¶ÓÎé
int BattleField::LeaveGame(int roleid)
{
	return (TeamErase(team_red_fighting, roleid) || TeamErase(team_blue_fighting, roleid)) ? ERR_SUCCESS: -1;
}

int BattleField::OnBattleEnterFail(int roleid)
{
	TagInfos::iterator tit = taginfos.begin(), tie = taginfos.end();
	for (; tit != tie; ++ tit)
	{
		if (tit->second.GetStatus() == ST_FIGHTING)
		{
			BattleManager::GetInstance()->DecRoleEnterCnt(GetFieldType(), roleid);
			return ERR_SUCCESS;
		}
	}
	return ERR_BATTLE_MAP_NOTEXIST;
}

int BattleManager::LeaveTeam(int roleid, int gs_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	return it->second.LeaveTeam(roleid);
}

int BattleManager::IsRoleInCoolDown(char field_type, int roleid, BattleField *field, int & remain_cooltime)
{
	time_t now = GetTime();
	FieldRoleTimer::iterator fit = rolemap.find(field_type);
	if (fit == rolemap.end())
		return ERR_SUCCESS;
	RoleTimer::iterator it = fit->second.find(roleid);
	if (it == fit->second.end())
	      return ERR_SUCCESS;
	int cooltime = field->GetCooldownTime();
	int enter_max_perday = field->GetEnterMaxPerDay();
	if (it->second.first + cooltime >= now)
	{
		remain_cooltime = it->second.first + cooltime - now;
		return ERR_BATTLE_COOLDOWN;
	}
	time_t lasttime = it->second.first;
	if (lasttime%86400 > now%86400)
	{
		fit->second.erase(it);
		return ERR_SUCCESS;	
	}
	if (enter_max_perday!=-1 && it->second.second >= enter_max_perday)
		return ERR_BATTLE_ENTER_MAXTIMES;
	return ERR_SUCCESS;
}

void BattleManager::SetRoleCoolDown(char field_type, int roleid)
{
	time_t now = GetTime();
	RoleTimer *prt = NULL;
	FieldRoleTimer::iterator fit = rolemap.find(field_type);
	if (fit == rolemap.end())
		prt = &rolemap[field_type];
	else
		prt = &fit->second;

	RoleTimer &roletimer = *prt;
	RoleTimer::iterator it = roletimer.find(roleid);
	if (it == roletimer.end())
	{
		roletimer[roleid] = std::make_pair(now, 1);
	}
	else
	{
		time_t lasttime = it->second.first;
		short times = it->second.second;
		if (lasttime%86400 > now%86400)
			it->second = RoleInfo(now, 1);
		else
			it->second = RoleInfo(now, ++ times);
	}
}

void BattleManager::DecRoleEnterCnt(char field_type, int roleid)
{
	FieldRoleTimer::iterator fit = rolemap.find(field_type);
	if (fit == rolemap.end())
		return;
	RoleTimer & roletimer = fit->second;
	RoleTimer::iterator it = roletimer.find(roleid);
	if (it == roletimer.end())
		return;
	if (it->second.second > 0)
		it->second.second--;
}

bool BattleManager::Update()
{
	time_t now = GetTime();
#if (defined ARENA_DEBUG || defined SONJIN_DEBUG)
//	if (now - t_base > 5400) t_base += 5400;
	if (now - t_base > 3600) t_base += 3600;
#else
	if (now - t_base > 86400 || now < t_base)
	{
		struct tm dt;
		localtime_r (&now, &dt);
		dt.tm_sec = 0;
		dt.tm_min = 0;
		dt.tm_hour = 0;
		t_base = mktime(&dt);
	}
#endif
	//LOG_TRACE("Battle Timer update: (%d) %s", t_forged, ctime(&now));
	//time_t base = t_base;
	//LOG_TRACE("Battle Timer update t_base %s", ctime(&base));
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); ++ it)
	{
		it->second.Update(now);
	}

	FieldRoleTimer::iterator fit = rolemap.lower_bound(cursor_fieldtype);
	if (fit == rolemap.end())
	{
		cursor_fieldtype = 0;
		cursor_roleid = 0;
		return true;
	}
	RoleTimer &roletimer = fit->second;
	RoleTimer::iterator it = roletimer.lower_bound(cursor_roleid);
	for (int count = 0; count < COOLDOWN_CHECK_MAX && it != roletimer.end(); count++)
	{
		time_t lasttime = it->second.first;
		if (lasttime%86400 > now%86400)
			roletimer.erase(it++);
		else
			++it;
	}
	if (it != roletimer.end())
		cursor_roleid = it->first;
	else
	{
		cursor_roleid = 0;
		++fit;
		if (fit != rolemap.end())
			cursor_fieldtype = fit->first;
		else
			cursor_fieldtype = 0;
	}
	return true;
}

inline time_t BattleManager::GetTime()
{
	time_t now = Timer::GetTime();
	if (t_forged)
		now += t_forged;
	return now;
}

void BattleManager::OnBattleStart(int gs_id, int tag, int mapid, int retcode)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnBattleStart(mapid, retcode);
}

void BattleManager::OnBattleEnd(int gs_id, int tag, int mapid)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnBattleEnd(mapid, tag);
}

int BattleManager::OnBattleEnter(int roleid, int gs_id, int map_id, int & endtime, char &battle_type, bool b_gm)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	return it->second.OnBattleEnter(roleid, map_id, endtime, battle_type, b_gm);
}

int BattleManager::OnBattleEnterFail(int roleid, int gs_id, int map_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return ERR_BATTLE_MAP_NOTEXIST;
	return it->second.OnBattleEnterFail(roleid, map_id);
}

void BattleManager::OnDBBattleJoin(int retcode, int roleid, int gs_id, int map_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return;
	it->second.OnDBBattleJoin(retcode, roleid, map_id);
}

void BattleManager::OnLogout(int roleid, int gs_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnLogout(roleid);
}

bool BattleManager::Initialize()
{
	struct tm dt;
	time_t now = Timer::GetTime();
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
#if !(defined ARENA_DEBUG || defined SONJIN_DEBUG)
	dt.tm_hour = 0;
#endif
	t_base = mktime(&dt);

	IntervalTimer::Attach( this, UPDATE_INTERVAL * 1000000 /IntervalTimer::Resolution());
	return true;
}

void BattleManager::DebugList(int gs_id)
{
	LOG_TRACE("debug battle list request:gs_id=%d", gs_id);
	ServerMap::iterator sit = servers.find(gs_id);
	if (sit == servers.end())
		return;

	BattleServer & server = sit->second;
	for(BattleMap::iterator it = server.battlemap.begin(); it != server.battlemap.end(); ++ it)
	{
		LOG_TRACE("info: gs_id=%d, starttime=%s" , gs_id, ctime(&server.starttime));
		for (BattleField::TagInfos::iterator tit = it->second->taginfos.begin(); tit != it->second->taginfos.end(); ++ tit)
		{
			LOG_TRACE("info: map_id=%d, tag=%d, status=%d, level_min=%d, level_max=%d, battle_type=%d", 
			it->second->map_id, tit->second.tag, tit->second.GetStatus(), it->second->level_min, it->second->level_max, it->second->GetFieldType());

		}
		BattleField & field = *it->second;
		LOG_TRACE("info: TEAM_RED list:");
		for (BattleField::TeamType::iterator tit = field.team_red.begin(); tit != field.team_red.end(); ++ tit)
			LOG_TRACE("\troleid=%d", tit->second.roleid);
		LOG_TRACE("info: TEAM_BLUE list:");
		for (BattleField::TeamType::iterator tit = field.team_blue.begin(); tit != field.team_blue.end(); ++ tit)
			LOG_TRACE("\troleid=%d", tit->second.roleid);
	}
}
void BattleManager::DebugStartBattle(int gs_id, int map_id)
{
	int starttime = 0;
	LOG_TRACE("debug battle start request:gs_id=%d, map_id=%d", gs_id, map_id);
	ServerMap::iterator it = servers.find(gs_id);
	if (it == servers.end())
		return;
	BattleServer & server = it->second;
	BattleMap::iterator bit = server.battlemap.find(map_id);
	if (bit == server.battlemap.end())
		return;
	BattleField & field = *bit->second;
	for (BattleField::TagInfos::iterator tit = field.taginfos.begin(); tit != field.taginfos.end(); ++ tit)
	{
		if (tit->second.GetStatus() == BattleField::ST_QUEUEING)
		{
			//LOG_TRACE("debug battle start:gs_id=%d, map_id=%d, tag=%d", gs_id, map_id, tit->second.tag);
			BattleStart start(0, 0, 0);
			int now = Timer::GetTime();
			switch(field.field_type)
			{
				case BATTLEFIELD_TYPE_CHALLENGESPACE:
				{
					starttime = field.starttime;
					start.end_time = field.starttime + field.GetFightingTime() - TIMEOFFSET_BETWEEN_GS_DELI;
					field.starttime += field.GetFightingTime();
					field.endtime = field.starttime;
					break;
				}
				case BATTLEFIELD_TYPE_CRSSVR:
				{
					field.starttime = now;
	                                start.end_time = field.starttime + field.GetFightingTime();
	                                field.endtime = field.starttime + field.GetFightingTime();
					break;
				}
				case BATTLEFIELD_TYPE_CRSSVRTEAM:
				{
					field.starttime = now + field.GetFightingTime();
	                                start.end_time = field.starttime -  CrssvrTeamBattleField::GetQueueingTime(field.GetFightingTime());
	                                field.endtime = field.starttime;
					break;	
				}
				default: 
				{
					start.end_time = field.starttime + NORMAL_FIGHTING_TIME;
					// ÉèÖÃ±¾´ÎÕ½³¡µÄ½áÊøÊ±¼ä
					field.starttime += NORMAL_FIGHTING_TIME;
					field.endtime = field.starttime;
					break;
				}
			}
			start.tag = tit->second.tag;
			start.map_id = map_id;
			LOG_TRACE("debug battle start:gs_id=%d, map_id=%d, tag=%d, start.endtime=%d starttime=%d endtime=%d", gs_id, map_id, tit->second.tag, start.end_time, field.starttime, field.endtime);
			GProviderServer::GetInstance()->Send(server.sid, start);
			tit->second.SetStatus(BattleField::ST_SENDSTART);
			tit->second.SetTimeout(GetTime(), BattleField::SWITCH_TIMEOUT);
			break;
		}
	}
}
void BattleManager::DebugBattleEnter(int gs_id, int map_id, int roleid)
{
	//int endtime;
	//int retcode = OnBattleEnter(roleid, gs_id, map_id, endtime);
	//LOG_TRACE("debug battle enter:gs_id=%d, map_id=%d, roleid=%d, retcode=%d" , gs_id, map_id, roleid, retcode);
}

void BattleManager::DebugBattleJoin(int gs_id, int map_id, int roleid, unsigned char team)
{
	LOG_TRACE("debug battle join: gs_id=%d,map_id=%d,roleid=%d,team=%d", gs_id, map_id, roleid, team);	
	BattleRole role;
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL==pinfo )
		{
			LOG_TRACE("debug battle join: role not ongame");	
			return;
		}
		GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
		if(prole == NULL)
		{
			LOG_TRACE("debug battle join: roleinfo not find");
			return;
		}
		if (gs_id != pinfo->gameid)
		{
			LOG_TRACE("debug battle join: gs_id != pinfo->gs_id, gs_id=%d, pinfo->gs_id=%d",
				gs_id, pinfo->gameid);	
			return;
		}
		role.roleid = pinfo->roleid;
		role.level = pinfo->level;
		role.name = pinfo->name;
		role.occupation = pinfo->occupation;
		role.score = 0; //pinfo->battlescore;
		role.reborn_cnt = prole->reborn_cnt;
		role.localsid = pinfo->localsid;
		role.linksid = pinfo->linksid;
	}
	int starttime = 0;
	char battle_type = 0;
	int cooltime = -1;
	int retcode = JoinTeam(role, gs_id, map_id, team, starttime, cooltime, battle_type);
	LOG_TRACE("debug battle join: gs_id=%d,map_id=%d,roleid=%d,team=%d,retcode=%d",
		       gs_id, map_id, roleid, team, retcode);	

}
void BattleManager::DebugBattleLeave(int gs_id, int roleid)
{
	int retcode = LeaveTeam(roleid, gs_id);
	LOG_TRACE("debug battle leave: gs_id=%d,roleid=%d,retcode=%d", gs_id, roleid, retcode);	
}

void BattleManager::OnLeaveGame(int gs_id, int map_id, int tag, int roleid, int reason)
{
	LOG_TRACE("BattleManager::OnLeaveGame: gs_id=%d,roleid=%d,mapid=%d,tag=%d", gs_id, roleid, map_id, tag);	
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnLeaveGame(map_id, tag, roleid, reason);
}

void BattleManager::OnDisconnect(int gs_id)
{
	ServerMap::iterator it = servers.find(gs_id);
	if (it != servers.end())
		it->second.OnDisconnect();
}
void BattleManager::SetDebugMode()
{
	b_debug_mode = true;
}
};

