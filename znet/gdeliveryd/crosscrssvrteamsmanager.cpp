#include "crosscrssvrteamsmanager.h"
#include "maplinkserver.h"
#include "gproviderserver.hpp"
#include "centraldeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "matcher.h"
#include "crosscrssvrteamsgetreq.hpp"
#include "synccrosscrssvrteamstop.hrp"
#include "topmanager.h"
#include "crosscrssvrteamsonlinestatus.hpp"

namespace GNET
{

//====================================================================================
void CrossCrssvrTeamsSaver::DoSave()
{
	if (!isWork)
		return;
	CrossCrssvrTeamsManager::Map& teamsmap = CrossCrssvrTeamsManager::Instance()->teams;
	CrossCrssvrTeamsManager::Map::iterator it = teamsmap.upper_bound(curcor);
	if(it != teamsmap.end())
	{
		curcor = it->first;
		it->second->Save2DB(true);
		return;
	}
	isWork = false;
}

/////////////////////////////////////////////////////////////////////////////////
bool CrossCrssvrTeamsManager::Initialize()
{
	CrssvrTeamsBaseManager::Initialize();
	IntervalTimer::Attach(this,500000/IntervalTimer::Resolution());
	SetServerType(ECT_CROSS_SERVER);
	return true;
}

bool CrossCrssvrTeamsManager::Update()
{
	time_t now = GetTime();
	// UpdateStatus更新频率要低
	if(t_status_update < now)
	{
		UpdateStatus(now);
		t_status_update = now + STATUS_UPDATE_INTERVAL;
	}

	Map::iterator it = teams.upper_bound(cursor);
	if(it==teams.end())
	{
		//LOG_TRACE("CrossCrssvrTeamsManager : update, teamid=%d, mapsize=%d, upper_bound", cursor, teams.size());
		cursor = 0;
		return true;
	}
	
	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return true;

	cursor = detail->info.teamid;
	if(now - detail->updatetime > UPDATE_INTERVAL)
	{
		//LOG_TRACE("CrossCrssvrTeamsManager : update, teamid=%d, mapsize=%d, update", cursor, teams.size());
		detail->Update();
		detail->updatetime = now;
		if(!detail->IsRoleCrossOnline() && !detail->IsRoleSrcOnline())
		{
			LOG_TRACE("CrossCrssvrTeamsManager : update IsRoleCrossOnline");
			ClearTeamByTeamId(cursor);
		}
	}

	return true;
}

int CrossCrssvrTeamsManager::OnLoad(GCrssvrTeamsData& info)
{
	Map::iterator it = teams.find(info.info.teamid);
	if (it != teams.end())
	{
		LOG_TRACE("CrossCrssvrTeamsManager : OnLoad team had load, teamid=%d", info.info.teamid);
		return ERR_SUCCESS;
	}
	
	return OnCreate(info, 0);
}

int CrossCrssvrTeamsManager::OnCreate(GCrssvrTeamsData& info, int zoneid)
{
	LOG_TRACE("CrossCrssvrTeamsManager : OnCreate teamid=%d, zoneid=%d", info.info.teamid, zoneid);

	Map::iterator ic = teams.find(info.info.teamid);
	if(ic != teams.end())
	{
		LOG_TRACE("CrossCrssvrTeamsManager : OnCreate teamid=%d, zoneid=%d has create", info.info.teamid, zoneid);
		if(!ClearTeamByTeamId(info.info.teamid))
		{
			return ERR_CRSSVRTEAMS_CROSS_CREATE_FAIL;
		}
	}

	CrssvrTeams_Info*  detail = new CrssvrTeams_Info(ECT_CROSS_SERVER, info, this);
	teams[info.info.teamid] = detail;

	detail->SetLoadFinish(true);
	detail->SetDirty(CT_DIRTY_TEAM_DB);
	return ERR_SUCCESS;
}

int CrossCrssvrTeamsManager::OnDelRole(int teamid, int roleid, int zoneid)
{
	LOG_TRACE("CrossCrssvrTeamsManager : OnDelRole teamid=%d, roleid=%d, zoneid=%d", teamid, roleid, zoneid);

	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrossCrssvrTeamsManager : OnDelRole teamid=%d, roleid=%d, zoneid=%d team not find", teamid, roleid, zoneid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}	
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->DeleteRole(roleid);
	detail->SetDirty(CT_DIRTY_TEAM_DB);

	return ERR_SUCCESS;
}

int CrossCrssvrTeamsManager::OnAddRole(int teamid, int zoneid, GCrssvrTeamsRole& roleinfo, GCrssvrTeamsScore& data_score)
{
	LOG_TRACE("CrossCrssvrTeamsManager : OnAddRole teamid=%d, roleid=%d, zoneid=%d", teamid, roleinfo.roleid, zoneid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrossCrssvrTeamsManager : OnAddRole teamid=%d, roleid=%d, zoneid=%d team not find", teamid, roleinfo.roleid, zoneid);
		//SendTeamGetReq(roleinfo.roleid, teamid, zoneid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	if(detail->IsMemberExsit(roleinfo.roleid))
	{
		detail->UpdateRole(roleinfo, data_score);
	}
	else
	{
		detail->Join(roleinfo);
	}
	detail->SetDirty(CT_DIRTY_TEAM_DB);// 跨服要在外部dirty

	return ERR_SUCCESS;
}

int CrossCrssvrTeamsManager::OnDismiss(int teamid, int zoneid)
{
	LOG_TRACE("CrossCrssvrTeamsManager : OnDelete teamid=%d, zoneid=%d", teamid, zoneid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrossCrssvrTeamsManager : OnDelete teamid=%d, zoneid=%d team not find", teamid, zoneid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->Dismiss();
	delete detail;
	
	teams.erase(ic);

	return ERR_SUCCESS;
}

int CrossCrssvrTeamsManager::OnNameChange(int teamid, const Octets & newname, int zoneid)
{
	LOG_TRACE("CrossCrssvrTeamsManager : OnNameChange teamid=%d, zoneid=%d", teamid, zoneid);

	Map::iterator it = teams.find(teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrossCrssvrTeamsManager : OnNameChange teamid=%d team not find", teamid);
		//SendTeamGetReq(0, teamid, zoneid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}
	
	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->info.name = newname;
	detail->SetDirty(CT_DIRTY_TEAM_DB);

	return ERR_SUCCESS;
}

int CrossCrssvrTeamsManager::OnChgCaptain(int teamid, int master_new, int zoneid)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		//SendTeamGetReq(0, teamid, zoneid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	if(!detail->IsMemberExsit(master_new))
	{
		return ERR_CRSSVRTEAMS_ROLE_NOT_IN;
	}

	detail->ChgCaptain(master_new);
	detail->SetDirty(CT_DIRTY_TEAM_DB);

	return ERR_SUCCESS;
}


int CrossCrssvrTeamsManager::OnLogin(int roleid,int zoneid,int linksid,int localsid)
{
	int role_teamid = GetRoleTeamId(roleid);
	if( role_teamid <= 0)
	{
		return ERR_CRSSVRTEAMS_ROLE_NOT_IN;
	}

	// 更新自己为跨服在线
	SyncRoleOnlineStatus(role_teamid, roleid, CRSSVRTEAMS_CROSS_ONLINE_STATUS, zoneid);
	LOG_TRACE("CrossCrssvrTeamsManager : OnLogin, roleid=%d, zoneid=%d, linksid=%d, localsid=%d", roleid, zoneid, linksid, localsid); 

	Map::iterator ic = teams.find(role_teamid);
	if(ic==teams.end())  // not found
	{
		// 请求整个战队
		SendTeamGetReq(roleid, role_teamid, zoneid);
		return ERR_SUCCESS;
	}
	else // found and load finish
	{
		// 更新角色信息，因为跨服中自己的数据有可能已经不是最新的了
		SendRoleGetReq(roleid, role_teamid, zoneid);
		return ERR_SUCCESS;
	}
	return ERR_SUCCESS;
}

int CrossCrssvrTeamsManager::OnLogout(int roleid, int zoneid)
{
	int role_teamid = GetRoleTeamId(roleid);
	if( role_teamid <= 0)
	{
		//LOG_TRACE("CrossCrssvrTeamsManager : OnLogout, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_ROLE_NOT_IN;
	}

	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrossCrssvrTeamsManager : OnLogout, roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	SyncRoleOnlineStatus(role_teamid, roleid, CRSSVRTEAMS_OFFLINE_STATUS, zoneid);
	detail->UpdateRoleOnline(roleid, CRSSVRTEAMS_OFFLINE_STATUS);

	LOG_TRACE("CrossCrssvrTeamsManager : OnLogout roleid=%d, zoneid=%d", roleid, zoneid);
	return ERR_SUCCESS;
}

void CrossCrssvrTeamsManager::SetRoleBriefs(int teamid, TTeamRaidRoleBrief& rolebrief)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		Log::log( LOG_ERR, "CrossCrssvrTeamsManager::SetRoleBriefs, teamid=%d is not find", teamid);
		return;
	}

	TTeamRaidRoleBrief::iterator it, ite = rolebrief.end();
	for(it = rolebrief.begin(); it != ite; ++it)
	{
		//LOG_TRACE("CrossCrssvrTeamsManager : SetRoleBriefs, teamid=%d, roleid=%d, ", teamid, (*it).roleid);
		ic->second->SetRoleBrief(*it);
	}

}

void CrossCrssvrTeamsManager::Save2DB()
{
	saver.Reset();
	saver.Begin();
}

void CrossCrssvrTeamsManager::OnSync(int teamid) { saver.OnSync(teamid);}

void CrossCrssvrTeamsManager::OnCrossTopUpdate(int table_type, int zoneid)
{
	switch(table_type)
	{
		case CRSSVRTEAMS_DAILY_TOP_TYPE:
		{
			GTopTable daily;

			TopManager::Instance()->GetDailyTable(TOP_CRSSVRTEAMS_SCORE, daily);
			bool isready = TopManager::Instance()->Ready();

			LOG_TRACE("CrossCrssvrTeamsManager::OnCrossTopUpdate daily.id=%d, isready=%d, table_type=%d", daily.id, isready, table_type);
			if (isready && daily.id == TOP_CRSSVRTEAMS_SCORE)// 榜完全同步过来之后，再同步
			{
				SendSyncTopUpdate(daily, table_type, zoneid);
				SendSyncTopUpdate(daily, CRSSVRTEAMS_SEASON_TOP_TYPE, zoneid);
			}
		}
		break;
		case CRSSVRTEAMS_WEEKLY_TOP_TYPE:
		{
			GTopTable weekly;

			int ret = TopManager::Instance()->GetWeeklyTableByTableID(TOP_CRSSVRTEAMS_SCORE, weekly);
			bool isready = TopManager::Instance()->Ready();

			LOG_TRACE("CrossCrssvrTeamsManager::OnCrossTopUpdate weekly.id=%d, isready=%d, table_type=%d, ret=%d", weekly.id, isready, table_type, ret);
			if (isready && weekly.id == (TOP_CRSSVRTEAMS_SCORE))// 榜完全同步过来之后，再同步
			{
				SendSyncTopUpdate(weekly, table_type, zoneid);
			}
		}
		break;
	}
}

void CrossCrssvrTeamsManager::SendSyncTopUpdate(const GTopTable& toptable, int table_type, int zoneid)
{
	LOG_TRACE("CrossCrssvrTeamsManager::SendSyncTopUpdate table.id=%d, table_type=%d, zoneid=%d", toptable.id, table_type, zoneid);
	SyncCrossCrssvrTeamsTop* rpc = (SyncCrossCrssvrTeamsTop*) Rpc::Call( RPC_SYNCCROSSCRSSVRTEAMSTOP,SyncCrossCrssvrTeamsTopArg(table_type, toptable));
	if(zoneid == 0)
		CentralDeliveryServer::GetInstance()->BroadcastProtocol(rpc);
	else
	{
		// 一定是某个原服请求的排榜
		CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, rpc);
		return;
	}

	if(table_type == CRSSVRTEAMS_WEEKLY_TOP_TYPE)
	{
		OnTopUpdate(toptable, table_type);
	}
	else if(table_type == CRSSVRTEAMS_DAILY_TOP_TYPE)
	{
		OnTopUpdate(toptable, table_type);
	}
	else if(table_type == CRSSVRTEAMS_SEASON_TOP_TYPE)
	{
		OnTopUpdate(toptable, table_type);
	}

}

void CrossCrssvrTeamsManager::UpdateTeamInfo(int teamid, GCrssvrTeamsInfo& teaminfo, int zoneid)
{
	LOG_TRACE("CrossCrssvrTeamsManager::UpdateTeamInfo teamid=%d, zoneid=%d", teamid, zoneid);

	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return;

	if(detail->info.teamid != teamid)
	{
		LOG_TRACE("CrossCrssvrTeamsManager::UpdateTeamInfo src_teamid=%d not equal teamid=%d", detail->info.teamid, teamid);
	}
	detail->info = teaminfo;
}

// 跨服使用
void CrossCrssvrTeamsManager::UpdateSeasonTop(int updatetime, const GTopTable & toptable)
{
	if(updatetime == 0)
	{
		// 从数据库加载时，如果是零，说明榜是空的
		return;
	}
	ConvertTopTableToMap(toptable, CRSSVRTEAMS_SEASON_TOP_TYPE);
}

void CrossCrssvrTeamsManager::OnTopUpdate(const GTopTable & toptable, int toptype)
{
	//可能是服务器启动时 Load 排行榜数据触发
	//也可能是周二周排行榜更新时触发
	if(toptype == CRSSVRTEAMS_WEEKLY_TOP_TYPE)
	{
		ConvertTopTableToMap(toptable, toptype);
	}
	else if(toptype == CRSSVRTEAMS_DAILY_TOP_TYPE)
	{
		// 日榜
		ConvertDailyTableToMap(toptable);
	}
	else if(toptype == CRSSVRTEAMS_SEASON_TOP_TYPE)
	{
		time_t season_close_time = 0;
		IsCloseTime(GetTime(), season_close_time);

		LOG_TRACE("CrossCrssvrTeamsManager : OnTopUpdate IsSeasonClose=%d, ct_act_seasontoptime=%d, toptable.time=%d, SeasonCloseTime=%d", 
				IsSeasonClose(), ct_act_seasontoptime, toptable.time, season_close_time);

		if(IsSeasonClose() && /*ct_act_seasontoptime == 0 &&*/ toptable.time >= season_close_time)
		{
			// 如果赛季结束 && ct_act_seasontoptime == 0，表示赛季排行榜还未赋值,
			LOG_TRACE("CrossCrssvrTeamsManager : OnTopUpdate update the season top, id=%d, size=%d", toptable.id, toptable.maxsize);
			CrssvrTeamsTop pos_null;
			SendSeasonTopSave(toptable.time, toptable, pos_null);
		}
		else
		{
			// 赛季进行中或是已经加载了本赛季榜
			LOG_TRACE("CrossCrssvrTeamsManager : OnTopUpdate neet't update the season top");
			return;
		}

	}
}

void CrossCrssvrTeamsManager::SyncRoleOnlineStatus(int teamid, int roleid, char status, int zoneid)
{
	CrossCrssvrTeamsOnlineStatus re(roleid, teamid, status);
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, re);
}

void CrossCrssvrTeamsManager::SendTeamGetReq(int roleid, int teamid, int zoneid)
{
	CrossCrssvrTeamsGetReq re(roleid, teamid, 0);
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, re);
}

void CrossCrssvrTeamsManager::SendRoleGetReq(int roleid, int teamid, int zoneid)
{
	CrossCrssvrTeamsGetReq re(roleid, teamid, 1);
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, re);
}

};

