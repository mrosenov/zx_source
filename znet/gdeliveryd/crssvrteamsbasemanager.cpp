#include "crssvrteamsmanager.h"
#include "maplinkserver.h"
#include "gproviderserver.hpp"
#include "centraldeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "matcher.h"
#include "dbcrssvrteamsget.hrp"
#include "dbcrssvrteamsseasontopsave.hrp"
#include "dbcrssvrteamsgetseasontop.hrp"

namespace GNET
{

bool CrssvrTeamsBaseManager::Initialize()
{
	// 每年1.4.7.10月的1号开始比赛 3，6，9，12月25日结束比赛
	seasonperiod.push_back(std::make_pair(0,2));// 1-3月
	seasonperiod.push_back(std::make_pair(3,5));// 4-6月
	seasonperiod.push_back(std::make_pair(6,8));// 7-9月
	seasonperiod.push_back(std::make_pair(9,11));// 10-12月

	mon_seasonperiod.push_back(2);// 3月25
	mon_seasonperiod.push_back(5);// 6月25
	mon_seasonperiod.push_back(8);// 9月25
	mon_seasonperiod.push_back(11);// 12月25

	punishperiod[std::make_pair(0      , 1000)] = 0;
	punishperiod[std::make_pair(1001   , 1500)] = 100;
	punishperiod[std::make_pair(1501   , 1900)] = 50;
	punishperiod[std::make_pair(1901   , 2200)] = 25;
	punishperiod[std::make_pair(2201   , 9900)] = 15;

	t_status_update = GetTime() + STATUS_UPDATE_INTERVAL;
	BuildStatus();
	BuildLastWeekTopTime();

	LoadSeasonTop();
	return true;
}

void CrssvrTeamsBaseManager::Update(time_t now)
{
	// 如果现在时间减去t_lastweektoptime超过一星期，则t_lastweektoptime + 一周
	if(now -  t_lastweektoptime> ONEWEEK_SECOND)
	{
		t_lastweektoptime = t_lastweektoptime + ONEWEEK_SECOND;
		LOG_TRACE("CrssvrTeamsBaseManager::Update, now -  t_lastweektoptime=%d, t_lastweektoptime=%d", now -  t_lastweektoptime, t_lastweektoptime);
	}
}

int CrssvrTeamsBaseManager::GetWeekBeginTime(time_t now)
{
	time_t now_day_begin;
	struct tm now_dt;

	localtime_r(&now, &now_dt);
	now_dt.tm_sec = 0;
	now_dt.tm_min = 0;
	now_dt.tm_hour = 0;
	now_day_begin= mktime(&now_dt);

	LOG_TRACE("GetWeekBeginTime, now_day_begin=%d, tm_wday=%d", now_day_begin, now_dt.tm_wday);
	return now_day_begin - (now_dt.tm_wday - 1) * 24 * 3600;
}

int CrssvrTeamsBaseManager::GetTuesMorningTime(time_t now)
{
	time_t now_day_begin;
	struct tm now_dt;

	localtime_r(&now, &now_dt);
	now_dt.tm_sec = 0;
	now_dt.tm_min = 0;
	now_dt.tm_hour = 6;
	now_day_begin= mktime(&now_dt);

	LOG_TRACE("GetTuesMorningTime, now_day_begin=%d, tm_wday=%d", now_day_begin, now_dt.tm_wday);
	return now_day_begin - (now_dt.tm_wday - 2) * 24 * 3600;
}

void CrssvrTeamsBaseManager::BuildLastWeekTopTime()
{
	// 首先得当前周的起始时间，weekbegintime和当前周的周二早上6点的时间tues_morning
	// 如果当前时间 < tues_morning ，t_lastweektoptime = tues_morning - 一周
	// 如果当前时间 >= tues_morning ,t_lastweektoptime = tues_morning
	time_t now = GetTime();
	time_t weekbegintime, tues_morning_time;

	weekbegintime = GetWeekBeginTime(now);
	tues_morning_time = GetTuesMorningTime(now);

	if(now < tues_morning_time)
	{
		t_lastweektoptime = tues_morning_time - ONEWEEK_SECOND;
	}
	else
	{
		t_lastweektoptime = tues_morning_time;
	}

	LOG_TRACE("BuildLastWeekTopTime, now=%d, weekbegintime=%d, tues_morning_time=%d, t_lastweektoptime=%d", 
			now, weekbegintime, tues_morning_time, t_lastweektoptime);
}

// 计算从a到b这段时间内，经历了几次赛季间隔
int CrssvrTeamsBaseManager::CalSeasonInterval(time_t a, time_t b)
{
	struct tm dt_a;
      	localtime_r(&a, &dt_a);
	int mon_a = dt_a.tm_mon;
	int day_a = dt_a.tm_mday;

	struct tm dt_b;
      	localtime_r(&b, &dt_b);
	int mon_b = dt_b.tm_mon;
	int day_b = dt_b.tm_mday;

	//LOG_TRACE("CalSeasonInterval, a=%d, b=%d, mon_a=%d, day_a=%d, mon_b=%d, day_b=%d", a, b, mon_a, day_a, mon_b, day_b);

	int interval = 0;
	OpenPeriodsVec::const_iterator it, ite = mon_seasonperiod.end();
	for (it = mon_seasonperiod.begin(); it != ite; ++it)
	{
		int t_mon = *it;
		//LOG_TRACE("CalSeasonInterval, t_mon=%d, interval=%d", t_mon, interval);
		if(mon_a <= t_mon && day_a < CRSSVRTEAMS_SEASON_CLOSE_DAY && mon_b > t_mon && day_b >= CRSSVRTEAMS_SEASON_OPEN_DAY)
		{
			interval++;
		}
	}
	
	return interval;
}

// 现在是否赛季开启
bool CrssvrTeamsBaseManager::IsOpenTime(time_t now, time_t & start_time)
{
	struct tm dt;
      	localtime_r(&now, &dt);
	int t_mon = dt.tm_mon;
	int t_day = dt.tm_mday;

	OpenPairPeriodsVec::const_iterator it, ite = seasonperiod.end();
	for (it = seasonperiod.begin(); it != ite; ++it)
	{

		//LOG_TRACE("IsOpenTime, now=%d, t_mon=%d, t_day=%d, it->first=%d, it->second=%d", now, t_mon, t_day, it->first, it->second);
		if((t_mon >= it->first && t_mon < it->second ) || (t_mon == it->second && t_day >= CRSSVRTEAMS_SEASON_OPEN_DAY && t_day < CRSSVRTEAMS_SEASON_CLOSE_DAY))
		{
			// 赛季开始的时间
			dt.tm_sec = 0;
			dt.tm_min = 0;
			dt.tm_hour = 0;
			dt.tm_mday = CRSSVRTEAMS_SEASON_OPEN_DAY;
			dt.tm_mon = it->first;
			start_time = mktime(&dt);
			//LOG_TRACE("IsOpenTime, now=%d, status=%d, it->first=%d, it->second=%d, start_time=%d", now, status, it->first, it->second, start_time);
			return true;
		}
	}
	
	return false;
}

// 得到赛季结束时间, 使用时注意：必须在赛季结束时间段才可以使用
bool CrssvrTeamsBaseManager::IsCloseTime(time_t now, time_t & close_time)
{
	struct tm dt;
      	localtime_r(&now, &dt);
	int t_mon = dt.tm_mon;
	int t_day = dt.tm_mday;

	//time_t close_time = 0;
	OpenPairPeriodsVec::const_iterator it, ite = seasonperiod.end();
	for (it = seasonperiod.begin(); it != ite; ++it)
	{
		//LOG_TRACE("IsCloseTime, now=%d, t_mon=%d, t_day=%d, it->first=%d, it->second=%d", now, t_mon, t_day, it->first, it->second);
		if(t_mon == it->second && t_day >= CRSSVRTEAMS_SEASON_CLOSE_DAY && t_day <= CRSSVRTEAMS_SEASON_REOPEN_DAY)
		{
			// 赛季结束时间
			dt.tm_sec = 0;
			dt.tm_min = 0;
			dt.tm_hour = 0;
			dt.tm_mday = CRSSVRTEAMS_SEASON_CLOSE_DAY;
			dt.tm_mon = it->second;
			close_time = mktime(&dt);
			//LOG_TRACE("IsCloseTime, now=%d, status=%d, it->first=%d, it->second=%d, close_time=%d", now, status, it->first, it->second, close_time);
			return true;
		}
	}
	
	return false;
}

void CrssvrTeamsBaseManager::BuildStatus()
{
	time_t start_time = 0;
	time_t season_close_time = 0;
	time_t now = GetTime();
	
	IsCloseTime(now, season_close_time);
	if (IsOpenTime(now, start_time))
	{
		status = CT_STATUS_OPENING;
		LOG_TRACE("BuildStatus, now %d is opentime, status=%d, season_start=%d, season_close=%d", now, status, start_time, season_close_time);
	}
	else
	{
		status = CT_STATUS_CLOSING;
		LOG_TRACE("BuildStatus, now %d is closingtime, status=%d, season_start=%d, season_close=%d", now, status, start_time, season_close_time);
	}
}

void CrssvrTeamsBaseManager::OnOpened(time_t now)
{
	LOG_TRACE("CrssvrTeamsBaseManager OnOpened");
	
	status = CT_STATUS_OPENED;
	// 赛季开始，清空上赛季排行榜
	GTopTable top_null;
	CrssvrTeamsTop pos_null;
	SendSeasonTopSave(0, top_null, pos_null);
}
void CrssvrTeamsBaseManager::OnClosed(time_t now)
{
	LOG_TRACE("CrssvrTeamsBaseManager OnClosed");
	
	status = CT_STATUS_CLOSED;
	//Save2DB();
}
void CrssvrTeamsBaseManager::CheckClose(time_t now)
{
	//LOG_TRACE("CrssvrTeamsBaseManager CheckClose");
	time_t start_time;
	if (!IsOpenTime(now, start_time))
	{
		status = CT_STATUS_CLOSING;
		LOG_TRACE("CheckClose, now=%d is closetime, status=%d", now, status);
	}

}
void CrssvrTeamsBaseManager::CheckOpen(time_t now)
{
	//LOG_TRACE("CrssvrTeamsBaseManager CheckOpen");
	time_t start_time;
	if (IsOpenTime(now, start_time))
	{
		status = CT_STATUS_OPENING;
		LOG_TRACE("CheckOpen, now=%d is opentime, status=%d", now, status);
	}
}

void CrssvrTeamsBaseManager::UpdateStatus(time_t now)
{
	//LOG_TRACE("CrssvrTeamsBaseManager : UpdateStatus status %d ", status);
	
	switch(status)
	{
		case CT_STATUS_OPENING:
			OnOpened(now);
			break;
		case CT_STATUS_OPENED:
			CheckClose(now);
			break;
		case CT_STATUS_CLOSING:
			OnClosed(now);
			break;
		case CT_STATUS_CLOSED:
			CheckOpen(now);
			break;
		default:
			Log::log(LOG_INFO, "CrssvrTeamsBaseManager unknow status %d", status);
			break;
	}
}

bool CrssvrTeamsBaseManager::ClearTeamByRoleId(int roleid)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::ClearTeamByRoleId, roleid=%d, role_teamid=%d", roleid, role_teamid); 
		return false; 
	}	

	return ClearTeamByTeamId(role_teamid);
}

bool CrssvrTeamsBaseManager::ClearTeamByTeamId(int teamid)
{
	LOG_TRACE("CrssvrTeamsBaseManager::ClearTeamByTeamId teamid=%d", teamid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::ClearTeam teamid=%d team not find", teamid);
		return false;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	detail->Save2DB(true);
	delete detail;
	detail = NULL;
	teams.erase(ic);

	return true;

}

time_t CrssvrTeamsBaseManager::GetTime()
{       
	time_t now = Timer::GetTime();
	return now + t_forged;
}

void CrssvrTeamsBaseManager::SetForgedTime(time_t forge)
{
	t_forged = forge;
	time_t now = GetTime();
	t_status_update = now + STATUS_UPDATE_INTERVAL;
	BuildStatus();
}

int CrssvrTeamsBaseManager::GetRoleList(int roleid, int teamid, CrssvrTeamsRoleList_Re& re)
{
	LOG_TRACE("CrssvrTeamsBaseManager : GetList roleid=%d, teamid=%d", roleid, teamid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : GetList teamid=%d team not find", teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->GetRoleList(re);

	return ERR_SUCCESS;
}

int  CrssvrTeamsBaseManager::GetTeamBase(int roleid, int teamid, CrssvrTeamsTeamBase_Re& re)
{
	LOG_TRACE("CrssvrTeamsBaseManager : GetTeamBase roleid=%d, teamid=%d", roleid, teamid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : GetTeamBase teamid=%d team not find", teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	// team_pos ：赛季中是日榜排名，赛季结束到可以领奖期间是-1，可以领奖时间是赛季榜排名
	if(IsSeasonClose() && IsSeasonReward())
	{
		re.team_pos = GetSeasonTopPos(roleid);
	}
	else
	{
		re.team_pos = GetDailyTopPos(roleid, teamid);
	}

	detail->GetTeamBase(re);

	return ERR_SUCCESS;
}

int CrssvrTeamsBaseManager::GetRoleNotify(int roleid, CrssvrTeamsRoleNotify_Re& re)
{
	LOG_TRACE("CrssvrTeamsBaseManager : GetRoleNotity roleid=%d", roleid);
	SCTDataGroup sdatagroup;
	bool bdata = GetRoleDataGroup(roleid, sdatagroup);
	if(!bdata)
	{
		LOG_TRACE("CrssvrTeamsBaseManager : GetRoleNotity roleid=%d bdata is false", roleid);
		return ERR_CRSSVRTEAMS_OFFLINE;
	}

	re.roleid = roleid;
	re.teamid = sdatagroup.ct_teamid;
	re.ct_last_max_score = sdatagroup.ct_last_max_score;
	re.ct_last_battle_count = sdatagroup.ct_last_battle_count;
	re.role_score = sdatagroup.ct_score;
	return ERR_SUCCESS;
}

int  CrssvrTeamsBaseManager::UpdateRoleName(int roleid, const Octets new_name)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::UpdateRoleName, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return ERR_CRSSVRTEAMS_NOTEXIST; 
	}	
	
	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::UpdateRoleName, roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->UpdateRoleName(roleid, new_name);

	return ERR_SUCCESS;
}

int  CrssvrTeamsBaseManager::UpdateRoleLvl(int roleid, int level)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		//LOG_TRACE("CrssvrTeamsBaseManager::UpdateRoleLvl, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return ERR_CRSSVRTEAMS_NOTEXIST; 
	}	
	
	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::UpdateRoleLvl, roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->UpdateRoleLvl(roleid, level);

	return ERR_SUCCESS;
}

bool CrssvrTeamsBaseManager::GetCrssvrTeamsData(int teamid, GCrssvrTeamsData& info,  TTeamRaidRoleBrief& rolebrief)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return false;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	detail->GetCrssvrTeamsData(info, rolebrief);

	return true;

}

bool CrssvrTeamsBaseManager::GetCrssvrTeamsData(int teamid, const TeamRoleList& rolelist, GCrssvrTeamsData& info,  TTeamRaidRoleBrief& rolebrief)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return false;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	detail->GetCrssvrTeamsData(rolelist, info, rolebrief);

	return true;

}


int CrssvrTeamsBaseManager::GetMemberCount(int teamid)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return 0;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return 0;

	return detail->GetMemberCount();
}

bool CrssvrTeamsBaseManager::GetName(int teamid, Octets& name)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return false;

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	name = ic->second->info.name;
	return true;
}

int CrssvrTeamsBaseManager::GetTeamScore(int teamid)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return 0;

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return 0;

	return detail->info.score;
}

int CrssvrTeamsBaseManager::GetRoleTeamId(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	
	GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
	if(!info)
		return -1;

	return info->ct_teamid;
}

int CrssvrTeamsBaseManager::GetRoleLastWeekMaxScore(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	GRoleInfo* pinfo = RoleInfoCache::Instance().Get(roleid);
	if(!pinfo)
		return -1;

	return pinfo->ct_last_max_score;
}

int CrssvrTeamsBaseManager::GetRoleJoinBattleTime(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	GRoleInfo* pinfo = RoleInfoCache::Instance().Get(roleid);
	if(!pinfo)
		return -1;

	return pinfo->ct_last_punish_time;
}

int CrssvrTeamsBaseManager::GetRoleBattleTimes(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	GRoleInfo* pinfo = RoleInfoCache::Instance().Get(roleid);
	if(!pinfo)
		return -1;

	return pinfo->ct_last_battle_count;
}

bool CrssvrTeamsBaseManager::GetRoleDataGroup(int roleid, SCTDataGroup& data)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	GRoleInfo* pinfo = RoleInfoCache::Instance().Get(roleid);
	if(!pinfo)
		return false;

	data.ct_leave_cool = pinfo->ct_leave_cool;
	data.ct_teamid = pinfo->ct_teamid;
	data.ct_score = pinfo->ct_score;
	data.ct_last_max_score = pinfo->ct_last_max_score;
	data.ct_last_punish_time = pinfo->ct_last_punish_time;
	data.ct_last_battle_count = pinfo->ct_last_battle_count;

	return true;
}

bool CrssvrTeamsBaseManager::SetRoleDataGroup(int roleid, int type, int value)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	GRoleInfo* pinfo = RoleInfoCache::Instance().Get(roleid);
	if(!pinfo)
		return false;

	switch(type)
	{
	case KEY_DATAGOURP_ROLE_CT_COOLTIME:
		{
			pinfo->ct_leave_cool = value;
		}
		break;
	case KEY_DATAGOURP_ROLE_CT_TEAMID:
		{
			pinfo->ct_teamid = value;
		}
		break;
	case KEY_DATAGOURP_ROLE_CT_SCORE:
		{
			pinfo->ct_score = value;
		}
		break;
	case KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE:
		{
			pinfo->ct_last_max_score = value;
		}
		break;
	case KEY_DATAGOURP_ROLE_CT_LAST_PUNISH_TIME:
		{
			pinfo->ct_last_punish_time = value;
		}
		break;
	case KEY_DATAGOURP_ROLE_CT_LAST_BATTLE_COUNT:
		{
			pinfo->ct_last_battle_count = value;
		}
		break;
	default:
		{
			Log::log(LOG_ERR, "SetRoleDataGroup roleid %d status %d", roleid, type);
			return false;
		}
		break;
	}

	return true;
}

int CrssvrTeamsBaseManager::GetGsPostScore(int roleid, int award_type, GCrssvrTeamsPostScore& re)
{
	// from gs
	// 0: 每周分档奖励 (需要个人积分)
	// 1: 每周兑换比奖励 (需要战队上周排名)
	// 2: 战队赛季奖励 (需要战队赛季排名)
	if(award_type == 0)
	{
		re.rolescore = GetRoleLastWeekMaxScore(roleid);
		re.weektoptime = ct_act_weektoptime;
		Log::formatlog("CrssvrTeamsBaseManager", "GetGsPostScore roleid=%d, award_type=%d, rolescore=%d, weektoptime=%d", roleid, award_type,re.rolescore,re.weektoptime);
	}
	else if(award_type == 1)
	{
		if(!IsWeekReward())
		{
			re.toppos = -1;
			return -1;
		}

		re.toppos = GetWeekTopPos(roleid);
		re.weektoptime = ct_act_weektoptime;
		Log::formatlog("CrssvrTeamsBaseManager", "GetGsPostScore lastweekpos roleid=%d, award_type=%d, toppos=%d, weektoptime=%d", roleid,award_type,re.toppos,re.weektoptime);
	}
	else if(award_type == 2)
	{
		if(!IsSeasonReward())
		{
			re.toppos = -1;
			return -2;
		}

		re.toppos = GetSeasonTopPos(roleid);
		re.weektoptime = ct_act_seasontoptime;
		Log::formatlog("CrssvrTeamsBaseManager", "GetGsPostScore SeasonPos roleid=%d, award_type=%d, toppos=%d, weektoptime=%d", roleid,award_type,re.toppos,re.weektoptime);
	}

	return ERR_SUCCESS;
}

int CrssvrTeamsBaseManager::GetRoleLeaveCooldown(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	
	GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
	if(!info)
		return -1;

	return info->ct_leave_cool;

}

int CrssvrTeamsBaseManager::GetWeekTopPos(int roleid)
{
	LOG_TRACE("CrssvrTeamsBaseManager : GetWeekTopPos, roleid=%d", roleid);
	CrssvrTeamsTop::const_iterator it = lastweektop.find(roleid);
	if(it != lastweektop.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : GetWeekTopPos, roleid=%d, pos=%d", roleid, it->second);
		return it->second + 1;
	}

	return -1;
}

int  CrssvrTeamsBaseManager::GetSeasonTopPos(int roleid)
{
	LOG_TRACE("CrssvrTeamsBaseManager : GetSeasonTopPos, roleid=%d", roleid);
	CrssvrTeamsTop::const_iterator it = lastseasontop.find(roleid);
	if(it != lastseasontop.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : GetSeasonTopPos, roleid=%d, pos=%d", roleid, it->second + 1);
		return it->second + 1;
	}

	return -1;
}

int  CrssvrTeamsBaseManager::GetDailyTopPosByRoleId(int roleid)
{
	LOG_TRACE("CrssvrTeamsBaseManager : GetDailyTopPosByRoleId, roleid=%d", roleid);
	int role_teamid = GetRoleTeamId(roleid);
	if( role_teamid <= 0)
	{
		LOG_TRACE("CrssvrTeamsManager::GetDailyTopPosByRoleId, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid);
		return -1;
	}
	return GetDailyTopPos(roleid, role_teamid);
}

int  CrssvrTeamsBaseManager::GetDailyTopPos(int roleid, int teamid)
{
	//LOG_TRACE("CrssvrTeamsBaseManager : GetDailyTopPos, teamid=%d", teamid);
	CrssvrTeamsTop::const_iterator it = dailytop.find(teamid);
	if(it != dailytop.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : GetDailyTopPos roleid=%d, teamid=%d, pos=%d", roleid, teamid, it->second + 1);
		return it->second + 1;
	}

	return -1;
}

bool CrssvrTeamsBaseManager::FindMaster(int teamid, int &roleid)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return false;
	CrssvrTeams_Info* detail = ic->second;
	
	if(!detail)
		return false;

	roleid = detail->GetMasterID();

	return true;
}

bool CrssvrTeamsBaseManager::IsMaster(int teamid, int roleid)
{
	Map::iterator it = teams.find(teamid);
	if(it==teams.end())
		return false;
	return it->second->GetMasterID()==roleid;
}

bool CrssvrTeamsBaseManager::CheckRoleExsit(int teamid, int roleid)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return false;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	return detail->IsMemberExsit(roleid);
}


bool CrssvrTeamsBaseManager::Broadcast(int teamid, Protocol& data, unsigned int &localsid)
{
	if(!teamid)
		return false;
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return false;
	ic->second->Broadcast(data, localsid);
	return true;
}

bool CrssvrTeamsBaseManager::BroadcastGS(int teamid, Protocol& data, int & roleid)
{
	if(!teamid)
		return false;
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return false;
	ic->second->BroadcastGS(data, roleid);
	return true;
}

bool CrssvrTeamsBaseManager::ValidName(const Octets& name)
{
        int maxlen = atoi(Conf::GetInstance()->find(GDeliveryServer::GetInstance()->Identification(), "max_name_len").c_str());
        if (maxlen <= 0) maxlen = 16;
        return (name.size()>0 && name.size()<=(size_t)maxlen &&  Matcher::GetInstance()->Match((char*)name.begin(),name.size())==0);
}

bool CrssvrTeamsBaseManager::CheckJoinPolicy(int roleid, int rolelvl)
{
	int role_reborn;
        GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
        if (prole == NULL)
        {
	        role_reborn = 0;
        }
        else
        {
        	role_reborn = prole->reborn_cnt;
        }
	//LOG_TRACE("CrssvrTeamsBaseManager::CheckJoinPolicy roleid=%d, level=%d, reborn=%d", roleid, rolelvl, role_reborn);
	if (rolelvl < 135 || role_reborn < 1)
		return false;   
	return true;    
}

void CrssvrTeamsBaseManager::OnRolenameChange(int roleid, const Octets& newname)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::OnRolenameChange, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	
	
	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::OnRolenameChange roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return;

	detail->UpdateRoleName(roleid, newname);
}

void CrssvrTeamsBaseManager::Packer::Unpack(std::vector<GCrssvrTeamsRole>& rolelist, int& zoneid, const Octets & src)
{
	if (src.size() <= 13)
		return;
	Marshal::OctetsStream os(src);
	try
	{
		os >> rolelist >> zoneid;
	}
	catch (...)
	{	
		Log::log(LOG_ERR, "CrssvrTeamsManager Unpack data err");
	}
}

void CrssvrTeamsBaseManager::SendSeasonTopSave(int updatetime, const GTopTable & toptable, const CrssvrTeamsTop& pos_map)
{
	DBCrssvrTeamsSeasonTopSaveArg arg;
	arg.seasontop.pos_map = pos_map;
	arg.seasontop.updatetime = updatetime;
	arg.seasontop.table = toptable;
	DBCrssvrTeamsSeasonTopSave* rpc = (DBCrssvrTeamsSeasonTopSave*) Rpc::Call( RPC_DBCRSSVRTEAMSSEASONTOPSAVE,arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void CrssvrTeamsBaseManager::LoadSeasonTop()
{
	DBCrssvrTeamsGetSeasonTopArg arg;	
	DBCrssvrTeamsGetSeasonTop* rpc = (DBCrssvrTeamsGetSeasonTop*) Rpc::Call( RPC_DBCRSSVRTEAMSGETSEASONTOP,arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void CrssvrTeamsBaseManager::ConvertTopTableToMap(const GTopTable & toptable, int toptype)
{
	CrssvrTeamsTop& ct_top = (toptype == CRSSVRTEAMS_WEEKLY_TOP_TYPE) ? lastweektop : lastseasontop;

	if(toptype == CRSSVRTEAMS_WEEKLY_TOP_TYPE)
	{
		ct_act_weektoptime = toptable.time;
		SetWeekReward(true);
	}
	else if(toptype == CRSSVRTEAMS_SEASON_TOP_TYPE)
	{
		ct_act_seasontoptime = toptable.time;	
		SetSeasonReward(true);
	}

	ct_top.clear();
	int pos_index = 0;
	LOG_TRACE("CrssvrTeamsBaseManager : ConvertTopTableToMap toptable.id=%d, toptype=%d, toptable.items.size=%d, toptable.items_detail.size=%d", toptable.id, toptype, toptable.items.size(), toptable.items_detail.size());
	std::vector<GTopDetail>::const_iterator it, ite = toptable.items_detail.end();	
	for (it = toptable.items_detail.begin(); it != ite ; ++it)
	{
		TCtRolesVec rolelist;
		int detail_zoneid = -1;
		int curr_zoneid = GDeliveryServer::GetInstance()->zoneid;
		Packer::Unpack(rolelist, detail_zoneid, (*it).content);
		if(servertype == ECT_SERVER)
			//LOG_TRACE("CrssvrTeamsBaseManager : ConvertTopTableToMap toptable.id=%d, rolelist.size=%d, detail_zoneid=%d, curr_zoneid=%d", 
			//	toptable.id, rolelist.size(), detail_zoneid, curr_zoneid);

		if((servertype == ECT_SERVER) && (detail_zoneid != curr_zoneid)) 
		{
			LOG_TRACE("CrssvrTeamsBaseManager : ConvertTopTableToMap toptable.id=%d, rolelist.size=%d, detail_zoneid=%d, curr_zoneid=%d", 
				toptable.id, rolelist.size(), detail_zoneid, curr_zoneid);

			++pos_index;
			continue;
		}
		TCtRolesVec::iterator it_roles, it_roles_end = rolelist.end();
		for(it_roles = rolelist.begin(); it_roles != it_roles_end; ++it_roles)
		{
			const GTopItem& topitem = toptable.items[pos_index];
			LOG_TRACE("CrssvrTeamsBaseManager : ConvertTopTableToMap insert ct_top roleid=%d, pos=%d, topitem.id=%d, teamscore=%d", (*it_roles).roleid, pos_index, topitem.id, topitem.value);

			ct_top.insert(std::make_pair((*it_roles).roleid, pos_index));
		}

		++pos_index;
		//如果map中已经有了则插入失败
	}
	
	LOG_TRACE("CrssvrTeamsBaseManager : ConvertTopTableToMap table.maxsize=%d, ct_top.size=%d, ct_act_weektoptime=%d, ct_act_seasontoptime=%d",  
			toptable.maxsize, ct_top.size(), ct_act_weektoptime, ct_act_seasontoptime);
	Log::formatlog("CrssvrTeamsBaseManager", "ConvertTopTableToMap ct_top size %d", ct_top.size());

}

void CrssvrTeamsBaseManager::ConvertDailyTableToMap(const GTopTable & toptable)
{
	CrssvrTeamsTop& ct_top = dailytop;
	ct_act_dailytoptime = toptable.time;	
	ct_top.clear();
	int pos_index = 0;
	//LOG_TRACE("CrssvrTeamsBaseManager : ConvertDailyTableToMap table.maxsize=%d", toptable.maxsize);
	std::vector<GTopItem>::const_iterator it, ite = toptable.items.end();	
	for (it = toptable.items.begin(); it != ite ; ++it)
	{
		const GTopItem& topitem = *it;
		LOG_TRACE("CrssvrTeamsBaseManager : ConvertDailyTableToMap insert ct_top topitem.id=%d, pos=%d", 
				topitem.id, pos_index);
		ct_top.insert(std::make_pair(topitem.id, pos_index));
		++pos_index;
	}
	LOG_TRACE("CrssvrTeamsBaseManager : ConvertDailyTableToMap ct_top.size=%d, ct_act_dailytoptime=%d", ct_top.size(), ct_act_dailytoptime);
}

int CrssvrTeamsBaseManager::CalcTeamScore(int src_teamid, int team_b_score, int result)
{
	LOG_TRACE("CrssvrTeamsBaseManager : CalcTeamScore src_teamid=%d, team_b_score=%d, result=%d", src_teamid, team_b_score, result);
	Map::iterator ic = teams.find(src_teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : CalcTeamScore src_teamid=%d team not find", src_teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* src_detail = ic->second;
	if(!src_detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	src_detail->CalcTeamScore(team_b_score, result);

	return ERR_SUCCESS;
}

int CrssvrTeamsBaseManager::CalcRoleScore(int src_teamid, std::vector<int>& team_roles, int team_b_score, int result)
{
	LOG_TRACE("CrssvrTeamsBaseManager : CalcRoleScore src_teamid=%d, team_b_score=%d, result=%d", src_teamid, team_b_score, result);
	Map::iterator ic = teams.find(src_teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : CalcRoleScore src_teamid=%d team not find", src_teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* src_detail = ic->second;
	if(!src_detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	src_detail->CalcRoleScore(team_roles, team_b_score, result);

	return ERR_SUCCESS;
}

void CrssvrTeamsBaseManager::GetPunishScore(int roleid, int week_times, int& score, bool isrole)
{
	if(week_times < 0) return;
	if(week_times--)
	{
		int punish_period = GetPunishPeriod(score);
		score -= punish_period;
		if(score <= 0) score = 0; 
		
		if(isrole) // 是角色惩罚
			PunishRoleScore(roleid, score);
		LOG_TRACE("CrssvrTeamsBaseManager : GetPunishScore roleid=%d, week_times=%d, punish_period=%d, score=%d", roleid, week_times, punish_period, score);

		GetPunishScore(roleid, week_times, score, isrole);
	}

	return;
}

void CrssvrTeamsBaseManager::CalcRolePunish(int roleid)
{
	int now = GetTime();

	SCTDataGroup sdatagroup;
	bool bdata = GetRoleDataGroup(roleid, sdatagroup);
	if(!bdata)
	{
		LOG_TRACE( "CrssvrTeamsBaseManager : CalcRolePunish roleid=%d bdata is false", roleid);
		return;
	}
/*
	int teamid = sdatagroup.ct_teamid;
	Map::iterator it = teams.find(teamid);
	if (it != teams.end())
	{
		it->second->PrintData("Pre CalcRolePunish", roleid);
	}
	*/
	int last_joinbattle_time = (sdatagroup.ct_last_punish_time == 0)?GetTime():sdatagroup.ct_last_punish_time;
	int battle_times = sdatagroup.ct_last_battle_count;
	int role_score = sdatagroup.ct_score;

	int reduce_interval = CalSeasonInterval(last_joinbattle_time, now); // 需要刨除的，有可能跨赛季了
	int interval = now -  last_joinbattle_time; // 与最后一次战斗的时间间隔
	int week_times = interval / ONEWEEK_SECOND - reduce_interval; // 隔了几周

	LOG_TRACE("CrssvrTeamsBaseManager : CalcRolePunish roleid=%d, last_joinbattle_time=%d,battle_times=%d,role_score=%d, interval=%d, week_times=%d, reduce_interval=%d", 
			roleid, last_joinbattle_time, battle_times, role_score, interval, week_times, reduce_interval);
	if(week_times > 0)
	{
		int final_score = role_score;
		if(battle_times < 10)
		{
			// 要惩罚所有周
			GetPunishScore(roleid, week_times, final_score, true);
		}
		else
		{
			// 第一周够场次,还要看week_times是几周，第二周以后一定不够，因为如果够的话，早已结算, 惩罚第二周以后
			GetPunishScore(roleid, week_times-1, final_score, true);
		}
		
		PunishRoleScore(roleid, final_score);
		//UpdateRolePunishScore(roleid, final_score);

		//SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_LAST_PUNISH_TIME, GetTime());
		//SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_LAST_BATTLE_COUNT, 0);
		//SyncRolePunishScore2DB(roleid, final_score, 0);
		//LOG_TRACE("CrssvrTeamsBaseManager : CalcRolePunish roleid=%d, final_score=%d", roleid, final_score);
		Log::formatlog("CrssvrTeamsBaseManager", "roleid=%d, src_score=%d, final_score=%d", roleid, role_score, final_score);
	}
}

void CrssvrTeamsBaseManager::CalcTeamPunish(int roleid)
{
	int now = GetTime();
	// 战队惩罚，也是有玩家上线即计算
	int teamid = GetRoleTeamId(roleid);

	Map::iterator it = teams.find(teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : CalcTeamPunish roleid=%d, teamid=%d team not find", roleid, teamid);
		return;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return;

	detail->CalcTeamPunish(roleid, now);
}

int  CrssvrTeamsBaseManager::UpdateRoleScore(int roleid, int score)
{
	int teamid = GetRoleTeamId(roleid);
 
	Map::iterator it = teams.find(teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : UpdateRoleScore roleid=%d, teamid=%d team not find", roleid, teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->SetRoleScore(roleid, score);

	return ERR_SUCCESS;

}

int  CrssvrTeamsBaseManager::PunishRoleScore(int roleid, int score)
{
	int teamid = GetRoleTeamId(roleid);
 
	Map::iterator it = teams.find(teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager : PunishRoleScore roleid=%d, teamid=%d team not find", roleid, teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->SetPunishRoleScore(roleid, score);

	return ERR_SUCCESS;
}

void CrssvrTeamsBaseManager::DebugClearCD(int roleid)
{
	SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_COOLTIME, 0);

	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugClearCD, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugClearCD roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->ClearRenameCD();
	LOG_TRACE("CrssvrTeamsBaseManager::DebugClearCD ok roleid=%d, teamid=%d", roleid, role_teamid);
}

void CrssvrTeamsBaseManager::DebugRoleScore(int roleid, int score)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleScore, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleScore roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->DebugRoleScore(roleid, score);
	LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleScore ok, roleid=%d, teamid=%d, score=%d", roleid, role_teamid, score);
}

void CrssvrTeamsBaseManager::DebugRoleLastMaxScore(int roleid, int score)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleLastMaxScore, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleLastMaxScore roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->DebugRoleLastMaxScore(roleid, score);
	LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleLastMaxScore ok, roleid=%d, teamid=%d, score=%d", roleid, role_teamid, score);
}

void CrssvrTeamsBaseManager::DebugRoleTeamScore(int roleid, int score)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleTeamScore, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleTeamScore roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->DebugRoleTeamScore(roleid, score);
	LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleTeamScore ok, roleid=%d, teamid=%d, score=%d", roleid, role_teamid, score);
}

void CrssvrTeamsBaseManager::DebugRolesScore(int roleid, int score)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRolesScore, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRolesScore roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->DebugRolesScore(roleid, score);
	LOG_TRACE("CrssvrTeamsBaseManager::DebugRolesScore ok, roleid=%d, teamid=%d, score=%d", roleid, role_teamid, score);
}

void CrssvrTeamsBaseManager::DebugTeamWin(int roleid, int times)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugTeamWin, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugTeamWin roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->DebugTeamWin(roleid, times);
	LOG_TRACE("CrssvrTeamsBaseManager::DebugTeamWin ok, roleid=%d, teamid=%d, times=%d", roleid, role_teamid, times);
}

void CrssvrTeamsBaseManager::DebugTeamPunish(int roleid, int times)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugTeamPunish, roleid=%d, role_teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugTeamPunish roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->DebugTeamPunish(roleid, times);
	LOG_TRACE("CrssvrTeamsBaseManager::DebugTeamPunish ok, roleid=%d, teamid=%d, times=%d", roleid, role_teamid, times);
}

void CrssvrTeamsBaseManager::DebugRolePunish(int roleid, int times)
{
	int role_teamid = GetRoleTeamId(roleid); 
	if( role_teamid <= 0) 
	{ 
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRolePunish, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid); 
		return; 
	}	

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsBaseManager::DebugRolePunish roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return;
	}
	it->second->DebugRolePunish(roleid, times);
	LOG_TRACE("CrssvrTeamsBaseManager::DebugRolePunish ok, roleid=%d, teamid=%d, times=%d", roleid, role_teamid, times);
}

void CrssvrTeamsBaseManager::DebugRoleSeasonPos(int roleid, int pos)
{
	lastseasontop[roleid] = pos;
	LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleSeasonPos ok, roleid=%d, pos=%d", roleid, pos);
}

void CrssvrTeamsBaseManager::DebugRoleWeekPos(int roleid, int pos)
{
	lastweektop[roleid] = pos;
	LOG_TRACE("CrssvrTeamsBaseManager::DebugRoleWeekPos ok, roleid=%d, pos=%d", roleid, pos);
}

/*
void CrssvrTeamsBaseManager::SyncRolePunishScore2DB(int roleid, int role_score, int times)
{
	DBCrssvrTeamsRolePunishScoreArg arg;
	arg.roleid = roleid;
	arg.ct_score = role_score; 
	arg.ct_last_punish_time = GetTime(); 
	arg.ct_last_battle_count = times; 
	DBCrssvrTeamsRolePunishScore* rpc = (DBCrssvrTeamsRolePunishScore*) Rpc::Call( RPC_DBCRSSVRTEAMSROLEPUNISHSCORE,arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);

}*/


void CrssvrTeamsBaseManager::UpdateTeamNewSeason(int teamid, CrssvrTeams_Info* detail)
{
	time_t start_time = 0;
	time_t now = GetTime();
	IsOpenTime(now, start_time);
	//detail->PrintData("Pre UpdateTeamNewSeason");
	if(start_time != 0 && detail->info.seasonlastbattletime < start_time)
	{
		LOG_TRACE("CrssvrTeamsBaseManager: UpdateTeamNewSeason need to reset");
		if(!ClearTeamByTeamId(teamid))
		{
			return;
		}

		Log::formatlog("CrssvrTeamsBaseManager", "ClearTeamByTeamId ok, teamid=%d, start_time=%d", teamid, start_time);
	
		DBCrssvrTeamsGet* rpc = (DBCrssvrTeamsGet*) Rpc::Call( RPC_DBCRSSVRTEAMSGET,TeamId(teamid,start_time));
		rpc->roleid = 0;
		//rpc->send2central = true;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}

	//PrintData("Post CrssvrTeamsBaseManager");
}


};

