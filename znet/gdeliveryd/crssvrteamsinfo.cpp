#include "crssvrteamsinfo.h"
#include "maplinkserver.h"
#include "gproviderserver.hpp"
#include "centraldeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "matcher.h"

#include "centraldeliveryclient.hpp"
#include "synccrosscrssvrteamsupdate.hrp"
#include "synccrosscrssvrteamsadd.hrp"
#include "synccrssvrteamsrole.hrp"
#include "synccrssvrteams.hrp"
#include "dbcrosscrssvrteams.hrp"
#include "synccrssvrteamsscore.hrp"
#include "crssvrteamsnotify_re.hpp"
#include "crssvrteamsmanager.h"
#include "dbcrosscrssvrteamsdismiss.hrp"
#include "synccrosscrssvrteamsscore.hrp"
#include "dbcrssvrteamsrolepunishscore.hrp"

namespace GNET
{

bool SerialCrssvrTeamsRole(int roleid, GCrssvrTeamsRole& roleinfo, int & gameid, unsigned int & linksid, unsigned int & localsid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* playerinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	GRoleInfo* proleinfo = RoleInfoCache::Instance().Get(roleid);
	if(!playerinfo || !proleinfo)
		return false;
	
	roleinfo.roleid = roleid;
	roleinfo.name = playerinfo->name;
	roleinfo.level = playerinfo->level;
	roleinfo.occupation = playerinfo->occupation;
	roleinfo.score = 0;
	roleinfo.camp = proleinfo->cultivation;
	roleinfo.isonline = true;

	gameid = playerinfo->gameid;
	linksid = playerinfo->linksid;
	localsid = playerinfo->localsid;

	return true;
}

CrssvrTeamsRole::CrssvrTeamsRole()
	: dirty_mask(0)
{

}

void CrssvrTeamsRole::Initialize(GCrssvrTeamsRole& roleinfo)
{
	roleinfo = roleinfo;
}

void CrssvrTeamsRole::PrintRoles()
{
	LOG_TRACE("roleid=%d,level=%d,occupation=%d,camp=%d,isonline=%d,ct_last_battle_count=%d,score=%d,ct_max_score=%d,ct_last_max_score=%d,ct_score_updatetime=%d\n"
			,roleinfo.roleid,roleinfo.level,roleinfo.occupation,roleinfo.camp,roleinfo.isonline,roleinfo.ct_last_battle_count,roleinfo.score
			,data_score.ct_max_score,data_score.ct_last_max_score,data_score.ct_score_updatetime);

	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
        GRoleInfo* pinfo = RoleInfoCache::Instance().Get(roleinfo.roleid);
        if(!pinfo)
        	return;
	LOG_TRACE("roleid=%d,ct_leave_cool=%d,ct_teamid=%d,ct_score=%d,ct_last_max_score=%d,ct_last_punish_time=%d,ct_last_battle_count=%d\n"
			,roleinfo.roleid,pinfo->ct_leave_cool,pinfo->ct_teamid,pinfo->ct_score,pinfo->ct_last_max_score,pinfo->ct_last_punish_time,pinfo->ct_last_battle_count);
}

CrssvrTeams_Info::CrssvrTeams_Info(int servertype, GCrssvrTeamsData& data, CrssvrTeamsBaseManager* pmanager) 
		:  dirty_mask(0), servertype(servertype), m_pmanager(pmanager)
{
	scoreperiod[std::make_pair(0      , 1500)] = 35;
	scoreperiod[std::make_pair(1501   , 1900)] = 20;
	scoreperiod[std::make_pair(1901   , 2200)] = 16;
	scoreperiod[std::make_pair(2201   , 9900)] = 5;

	updatetime = /*loadtime =*/ m_pmanager->GetTime();
	info = data.info;

	std::vector<GCrssvrTeamsRole>::iterator it = data.roles.begin();
	for(; it != data.roles.end() ; ++it)
	{
		Join(*it);
	}

	PrintData("CrssvrTeams_Info::CrssvrTeams_Info");
}

CrssvrTeams_Info::~CrssvrTeams_Info()
{
}

bool CrssvrTeams_Info::IsLoadFinish()
{
	return loaded;
}

void CrssvrTeams_Info::SetLoadFinish(bool isload)
{
	loaded = isload;
}

void CrssvrTeams_Info::SendMsg(int roleid, Protocol& data, unsigned int &localsid)
{
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if(!pinfo)
	{
		return;
	}

	localsid = pinfo->localsid;
	dsm->Send(pinfo->linksid, data);
}

void CrssvrTeams_Info::Broadcast(Protocol& data, unsigned int &localsid)
{
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
                PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(role.roleinfo.roleid);
		if(!pinfo)
		{
			continue;
		}

		localsid = pinfo->localsid;
		dsm->Send(pinfo->linksid, data);
	}
}

void CrssvrTeams_Info::BroadcastGS(Protocol& data, int & roleid)
{
	GProviderServer* psm = GProviderServer::GetInstance();
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
	
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
                PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(role.roleinfo.roleid);
		if(!pinfo)
		{
			continue;
		}

		psm->DispatchProtocol(pinfo->gameid, data);
	}
}

int CrssvrTeams_Info::GetMasterID()
{
	return info.master;
}

void CrssvrTeams_Info::GetMembers(std::vector<GCrssvrTeamsRole>& roles)
{
	roles.clear();
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		roles.push_back(role.roleinfo);
	}
}

void CrssvrTeams_Info::GetMemberBriefs(TTeamRaidRoleBrief& rolebrief)
{
	rolebrief.clear();
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		rolebrief.push_back(role.rolebrief);
	}
}


void CrssvrTeams_Info::GetRoleList(CrssvrTeamsRoleList_Re& ret)
{
	ret.teamid = info.teamid;
	GetMembers(ret.members);
	//PrintRolesData("CrssvrTeams_Info::GetRoleList");
}

void CrssvrTeams_Info::GetTeamBase(CrssvrTeamsTeamBase_Re& ret)
{
	ret.teamid = info.teamid;
	ret.master = info.master;
	ret.name = info.name;
	ret.score = info.score;
	ret.battle_losses = info.battle_losses;
	ret.battle_draws = info.battle_draws;
	ret.battle_wins = info.battle_wins;
	//PrintTeamData("CrssvrTeams_Info::GetTeamBase");
}

void CrssvrTeams_Info::GetCrssvrTeamsInfo(GCrssvrTeamsInfo& tinfo)
{
	tinfo = info;
}

void CrssvrTeams_Info::GetCrssvrTeamsData(GCrssvrTeamsData& tinfo)
{
	tinfo.info = info;
	GetMembers(tinfo.roles);
}

void CrssvrTeams_Info::GetCrssvrTeamsData(const TeamRoleList& rolelist, GCrssvrTeamsData& tinfo, TTeamRaidRoleBrief& rolebrief)
{
	//LOG_TRACE("CrssvrTeams_Info : GetCrssvrTeamsData rolelist=%d", rolelist.size());
	tinfo.info = info;
	TeamRoleList::const_iterator rit, rite = rolelist.end();
	for (rit = rolelist.begin(); rit != rite; ++rit)
	{
		int roleid = (*rit)->GetRoleid();
		for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
		{
			CrssvrTeamsRole& role = *it;
			if(role.roleinfo.roleid == roleid)
			{
				tinfo.roles.push_back(role.roleinfo);
				rolebrief.push_back(role.rolebrief);
			}
		}

	}

}

void CrssvrTeams_Info::GetCrssvrTeamsData(GCrssvrTeamsData& tinfo, TTeamRaidRoleBrief& rolebrief)
{
	tinfo.info = info;
	GetMembers(tinfo.roles);
	GetMemberBriefs(rolebrief);
}

int CrssvrTeams_Info::GetMemberCount()
{
	return rolecache.size();
}

int CrssvrTeams_Info::GetMemberCapacity()
{
	return CRSSVRTEAMS_MAX_ROLES - GetMemberCount(); 
}

/*
int CrssvrTeams_Info::GetPrepCaptain(int roleid)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;

		if(info.master != role.roleinfo.roleid && roleid != role.roleinfo.roleid)
		{
			return role.roleinfo.roleid;
		}		
	}
	
	return -1;
}*/

/*
bool CrssvrTeams_Info::GetScore(int roleid, GCrssvrTeamsPostScore& re)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;

		if(roleid != role.roleinfo.roleid)
		{
			re.teamscore = info.score;
			return true; 
		}		
	}
	
	return false;
}*/

bool CrssvrTeams_Info::IsRoleCrossOnline()
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;

		//LOG_TRACE("CrssvrTeams_Info : IsRoleCrossOnline, teamid=%d, roleid=%d, online=%d", info.teamid, role.roleinfo.roleid, role.roleinfo.isonline);
		if(role.roleinfo.isonline == CRSSVRTEAMS_CROSS_ONLINE_STATUS)
			return true;
	}

	//LOG_TRACE("CrssvrTeams_Info : IsRoleCrossOnline return false");
	return false;
}

bool CrssvrTeams_Info::IsRoleSrcOnline()
{
	//LOG_TRACE("CrssvrTeams_Info : IsRoleSrcOnline rolesize=%d", rolecache.size());
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;

		//LOG_TRACE("CrssvrTeams_Info : IsRoleSrcOnline, teamid=%d, roleid=%d, online=%d", info.teamid, role.roleinfo.roleid, role.roleinfo.isonline);
		if(role.roleinfo.isonline == CRSSVRTEAMS_SRC_ONLINE_STATUS)
			return true;
	}

	return false;
}


bool CrssvrTeams_Info::IsMemberExsit(int roleid)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
			return true;
	}

	return false;
}
int CrssvrTeams_Info::Join(GCrssvrTeamsRole& groleinfo)
{
	int roleid = groleinfo.roleid;

	if(GetMemberCapacity() <= 0)
	{
		LOG_TRACE("CrssvrTeams_Info : Join is Full, teamid=%d, roleid=%d", info.teamid, roleid);
		return ERR_CRSSVRTEAMS_FULL;
	}

	unsigned char occup = groleinfo.occupation;
	//LOG_TRACE("CrssvrTeams_Info : Join occup=%d, occup_count[occup]=%d, GetMemberCapacity=%d", occup, occup_count[occup] + 1, GetMemberCapacity());
	if(occup_count[occup] + 1 > OCCUPATION_PLAYER_LIMIT)
	{
		LOG_TRACE("CrssvrTeams_Info : Join occup is limit, teamid=%d, roleid=%d", info.teamid, roleid);
		return ERR_CRSSVRTEAMS_JOIN_OCCUP_CONT;
	}

	CrssvrTeamsRole roleinfo;
	roleinfo.roleinfo = groleinfo;
	roleinfo.data_score = groleinfo.data_score;
	rolecache.push_back(roleinfo);
	occup_count[occup]++;

	// 创建和加入战队数据库修改的字段，需要缓存的，都在此加入
	m_pmanager->SetRoleDataGroup(roleinfo.roleinfo.roleid, KEY_DATAGOURP_ROLE_CT_TEAMID, info.teamid);
        m_pmanager->SetRoleDataGroup(roleinfo.roleinfo.roleid, KEY_DATAGOURP_ROLE_CT_SCORE, roleinfo.roleinfo.score);
        m_pmanager->SetRoleDataGroup(roleinfo.roleinfo.roleid, KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE, roleinfo.data_score.ct_last_max_score);
        m_pmanager->SetRoleDataGroup(roleinfo.roleinfo.roleid, KEY_DATAGOURP_ROLE_CT_LAST_PUNISH_TIME, roleinfo.roleinfo.ct_last_punish_time);


	LOG_TRACE("CrssvrTeams_Info : Join , teamid=%d, roleid=%d, rolesize=%d, roleinfo.score=%d, ct_max_score=%d, ct_last_max_score=%d,occup=%d, occup_count[occup]=%d", 
			info.teamid, roleid, rolecache.size(), roleinfo.roleinfo.score, roleinfo.data_score.ct_max_score, roleinfo.data_score.ct_last_max_score, occup, occup_count[occup]);

	return ERR_SUCCESS;
}

// 跨服更新玩家数据时调用
int CrssvrTeams_Info::UpdateRole(const GCrssvrTeamsRole& roleinfo, const GCrssvrTeamsScore& data_score)
{
	int roleid = roleinfo.roleid;
	
	//LOG_TRACE("CrssvrTeams_Info : UpdateRole roleid=%d, member=%d", roleid, rolecache.size());
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			role.roleinfo = roleinfo;
			role.data_score = data_score;
			m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE, data_score.ct_last_max_score);
			LOG_TRACE("CrssvrTeams_Info : real UpdateRole, roleid=%d, teamid=%d", roleid, info.teamid);
			return ERR_SUCCESS;
		}
	}
	return ERR_CRSSVRTEAMS_CROSS_UPDATE_ROLE_FAIL;	
}

void CrssvrTeams_Info::ChgCaptain(int master_new)
{
	info.master = master_new;	
}

void CrssvrTeams_Info::DeleteRole(int roleid)
{
	LOG_TRACE("CrssvrTeams_Info : DeleteRole Start roleid=%d, member=%d", roleid, rolecache.size());
	/* 队长没有退出战队操作
	if(GetMasterID()==roleid)
	{
		// 队长退出,先转换队长
		int master_new = GetPrepCaptain(roleid);
		LOG_TRACE("CrssvrTeams_Info : DeleteRole old_master=%d, new_master=%d", roleid, master_new);

		if(master_new != -1)
		{
			ChgCaptain(master_new);
		}
	}*/
	
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			LOG_TRACE("CrssvrTeams_Info : DeleteRole roleid=%d, teamid=%d, occup=%d, occup_count=%d", roleid, info.teamid, role.roleinfo.occupation, occup_count[role.roleinfo.occupation]);
			rolecache.erase(it);
			m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_COOLTIME, m_pmanager->GetTime());
			m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_TEAMID, -1);
			occup_count[role.roleinfo.occupation]--;

			CrssvrTeamsNotify_Re re(ERR_CRSSVRTEAMS_DISMISS_OK, roleid, info.teamid, 0);
			SendMsg(roleid, re, re.localsid);
			break;
		}
	}
	
	LOG_TRACE("CrssvrTeams_Info : DeleteRole End roleid=%d, member=%d", roleid, rolecache.size());
}

void CrssvrTeams_Info::Dismiss()
{
	LOG_TRACE("CrssvrTeams_Info : Dismiss teamid=%d", info.teamid);

	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		int roleid = role.roleinfo.roleid;
		m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_COOLTIME, m_pmanager->GetTime());
		m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_TEAMID, -1);
	}
	
	CrssvrTeamsNotify_Re re(ERR_CRSSVRTEAMS_DISMISS_OK, 0, info.teamid, 0);
	Broadcast(re, re.localsid);
}

// 修改积分的统一接口
void CrssvrTeams_Info::SetRoleScore(int roleid, int final_score)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(roleid == role.roleinfo.roleid)
		{
			SetRoleScore(roleid, final_score, role);
		}
	}
	//PrintData("Post SetRoleScore", roleid);
}

// 标记玩家score
void CrssvrTeams_Info::SetRoleScore(int roleid, int final_score, CrssvrTeamsRole& role, bool isFormula)
{
	// 如果当前积分比当前最高积分大，设置当前最高积分
	int max_score = role.data_score.ct_max_score;
	int curr_score = final_score < 0 ? 0 : final_score;

	if(curr_score > max_score)
	{
		role.data_score.ct_max_score = curr_score;
	}

	role.roleinfo.score = curr_score;

	LOG_TRACE("CrssvrTeams_Info : Post SetRoleScore roleid=%d, max_score=%d, ct_score_updatetime=%d,ct_score=%d", roleid, role.data_score.ct_max_score, role.data_score.ct_score_updatetime, curr_score);
	UpdateRoleLastMaxScore(roleid);

	if(isFormula) // 只有玩家战斗结算才更新当前积分修改时间
		role.data_score.ct_score_updatetime = m_pmanager->GetTime();

	role.SetDirty(CT_DIRTY_SCORE_DB);
	role.SetDirty(CT_DIRTY_SCORE_CENTRAL);
}

void CrssvrTeams_Info::SetPunishRoleScore(int roleid, int final_score)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(roleid == role.roleinfo.roleid)
		{
			SetRoleScore(roleid, final_score, role);
			role.roleinfo.ct_last_battle_count = 0;
			role.roleinfo.ct_last_punish_time = m_pmanager->GetTime();
			m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_LAST_PUNISH_TIME, role.roleinfo.ct_last_punish_time);
			m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_LAST_BATTLE_COUNT, role.roleinfo.ct_last_battle_count);
			SyncRolePunishScore2DB(roleid, final_score, role.roleinfo.ct_last_battle_count, role.roleinfo.ct_last_punish_time);
			LOG_TRACE("CrssvrTeams_Info : SetPunishRoleScore roleid=%d, ct_max_score=%d, ct_score=%d, ct_last_battle_count=%d, ct_last_punish_time=%d", 
					roleid, role.data_score.ct_max_score, final_score, role.roleinfo.ct_last_battle_count, role.roleinfo.ct_last_punish_time);

			role.SetDirty(CT_DIRTY_SCORE_DB);
			role.SetDirty(CT_DIRTY_SCORE_CENTRAL);
		}
	}

}

void CrssvrTeams_Info::SetTeamScore(int final_score)
{
	//LOG_TRACE("CrssvrTeams_Info : SetTeamScore, score=%d", final_score);
	info.score = final_score < 0 ? 0 : final_score;
}

// 更新玩家上周最高积分,战队中所有玩家
void CrssvrTeams_Info::UpdateRoleLastMaxScore()
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		UpdateLastMaxScore(role);
	}
}

// 更新玩家上周最高积分,指定战队玩家
void CrssvrTeams_Info::UpdateRoleLastMaxScore(int roleid)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(roleid == role.roleinfo.roleid)
		{
			UpdateLastMaxScore(role);
			return;
		}
	}
}

void CrssvrTeams_Info::SetRoleBrief(const TeamRaidRoleBrief& rolebrief)
{
	//LOG_TRACE("CrssvrTeams_Info : SetRoleBrief roleid=%d", rolebrief.roleid);
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(rolebrief.roleid == role.roleinfo.roleid)
		{
			role.rolebrief = rolebrief;
			return;
		}
	}

}

void CrssvrTeams_Info::SyncRoleInfo2Central(int roleid)
{
	LOG_TRACE("CrssvrTeams_Info : SyncRoleInfo2Central roleid=%d", roleid);
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			SyncRoleScoreAndInfo2Central(role);
			return;
		}
	}

}

void CrssvrTeams_Info::UpdateRoleLvl(int roleid, int new_lvl)
{
	LOG_TRACE("CrssvrTeams_Info : UpdateRoleLvl roleid=%d, new_lvl=%d", roleid, new_lvl);

	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid && role.roleinfo.level != new_lvl)
		{
			role.roleinfo.level = new_lvl;
			role.SetDirty(CT_DIRTY_ROLE_DB);
			role.SetDirty(CT_DIRTY_ROLE_CENTRAL);
			return;
		}
	}

}

void CrssvrTeams_Info::UpdateRoleName(int roleid, const Octets& new_name)
{
	LOG_TRACE("CrssvrTeams_Info : UpdateRoleName roleid=%d, new_name.size=%d", roleid, new_name.size());

	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid && role.roleinfo.name != new_name)
		{
			role.roleinfo.name = new_name;
			role.SetDirty(CT_DIRTY_ROLE_DB);
			role.SetDirty(CT_DIRTY_ROLE_CENTRAL);
			return;
		}
	}

}

void CrssvrTeams_Info::UpdateRoleOnline(int roleid, char online)
{
	//LOG_TRACE("CrssvrTeams_Info : UpdateRoleOnline roleid=%d, online=%d", roleid, online);

	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			role.roleinfo.isonline = online;
			role.SetDirty(CT_DIRTY_ROLE_DB);
			role.SetDirty(CT_DIRTY_ROLE_ONLINE_CENTRAL);
			return;
		}
	}
}

void CrssvrTeams_Info::UpdateRolesOnline(char online)
{
	// 只是上线加载时调用，所以不用同步和存数据库
	//LOG_TRACE("CrssvrTeams_Info : UpdateRolesOnline online=%d", online);

	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		{
			role.roleinfo.isonline = online;
			role.SetDirty(CT_DIRTY_ROLE_ONLINE_CENTRAL);
		}
	}
}

void CrssvrTeams_Info::UpdateRoleOccup(int roleid, unsigned char occup)
{
	LOG_TRACE("CrssvrTeams_Info : UpdateRoleOccup roleid=%d, occup=%d", roleid, occup);

	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid && role.roleinfo.occupation != occup)
		{
			role.roleinfo.occupation = occup;
			role.SetDirty(CT_DIRTY_ROLE_DB);
			role.SetDirty(CT_DIRTY_ROLE_CENTRAL);
			return;
		}
	}

}

void CrssvrTeams_Info::SendRoleListRe(int retcode)
{
	CrssvrTeamsRoleList_Re re;
	re.retcode = retcode;
	GetRoleList(re);

	Broadcast(re, re.localsid);
}

void CrssvrTeams_Info::SendTeamBaseRe(int retcode)
{
	CrssvrTeamsTeamBase_Re re;
	re.retcode = retcode;
	GetTeamBase(re);

	Broadcast(re, re.localsid);
}

void CrssvrTeams_Info::Update()
{
	UpdateRoleLastMaxScore();

	if(servertype == ECT_SERVER) // 原服
	{
		UserContainer& container = UserContainer::GetInstance();
		Thread::RWLock::RDScoped l(container.GetLocker());
		PlayerInfo * pinfo;
		GRoleInfo* proleinfo;

		for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
		{
			CrssvrTeamsRole& role = *it;
			proleinfo = RoleInfoCache::Instance().Get(role.roleinfo.roleid);
			pinfo = container.FindRoleOnline(role.roleinfo.roleid);
			if (pinfo && proleinfo)
			{
				if(role.roleinfo.occupation != pinfo->occupation)
				{
					role.roleinfo.occupation = pinfo->occupation;
					role.SetDirty(CT_DIRTY_ROLE_DB);
					role.SetDirty(CT_DIRTY_ROLE_CENTRAL);
				}

				if(role.roleinfo.camp != proleinfo->cultivation)
				{
					role.roleinfo.camp = proleinfo->cultivation;
					role.SetDirty(CT_DIRTY_ROLE_DB);
					role.SetDirty(CT_DIRTY_ROLE_CENTRAL);
				}

				//LOG_TRACE("CrssvrTeams_Info::Update, roleid=%d, old_occupation=%d, new_occupation=%d,camp=%d", role.roleinfo.roleid, role.roleinfo.occupation, pinfo->occupation,proleinfo->cultivation);
			}
		}
	}

	Save2DB();
}

void CrssvrTeams_Info::Save2DB(bool isbatch)
{
	//LOG_TRACE("CrssvrTeams_Info : Save2DB servertype=%d, dirty_mask=%x", servertype, dirty_mask);
	if ((dirty_mask & CT_DIRTY_TEAM_DB) && (servertype == ECT_CROSS_SERVER))
	{
		DBCrossCrssvrTeamsArg arg;
		arg.teamid = info.teamid;
		GetCrssvrTeamsData(arg.info);
		DBCrossCrssvrTeams* rpc = (DBCrossCrssvrTeams*) Rpc::Call( RPC_DBCROSSCRSSVRTEAMS,arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		//LOG_TRACE("CrssvrTeams_Info : Save2DB save DBCrossCrssvrTeams");

	}
	if ((dirty_mask & CT_DIRTY_TEAM_DB) && (servertype == ECT_SERVER))
	{
		SyncCrssvrTeamsArg arg;
		arg.teamid = info.teamid;
		GetCrssvrTeamsInfo(arg.info);
		SyncCrssvrTeams* rpc = (SyncCrssvrTeams*) Rpc::Call( RPC_SYNCCRSSVRTEAMS,arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		//LOG_TRACE("CrssvrTeams_Info : Save2DB save SyncCrssvrTeams");
	}
	if ((dirty_mask & CT_DIRTY_TEAM_CENTRAL) && (servertype == ECT_SERVER))
	{
		SyncCrossCrssvrTeamsUpdateArg cross_arg;
		cross_arg.zoneid = GDeliveryServer::GetInstance()->zoneid;
		cross_arg.teamid = info.teamid;
		GetCrssvrTeamsInfo(cross_arg.teaminfo);
		SyncCrossCrssvrTeamsUpdate* cross_rpc = (SyncCrossCrssvrTeamsUpdate*) Rpc::Call( RPC_SYNCCROSSCRSSVRTEAMSUPDATE,cross_arg);
		CentralDeliveryClient::GetInstance()->SendProtocol(cross_rpc);
		//LOG_TRACE("CrssvrTeams_Info : Save2DB send rpc SyncCrossCrssvrTeamsUpdate to central");

	}

	ClearDirty();
	SaveRoles();
}

void CrssvrTeams_Info::SaveRoles()
{
	//LOG_TRACE("CrssvrTeams_Info : SaveRoles rolecache.size=%d", rolecache.size());

	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.dirty_mask & CT_DIRTY_ROLE_DB)
		{
			SyncCrssvrTeamsRoleArg arg;
			arg.teamid = info.teamid;
			arg.role = role.roleinfo;
			SyncCrssvrTeamsRole* rpc = (SyncCrssvrTeamsRole*) Rpc::Call( RPC_SYNCCRSSVRTEAMSROLE,arg);
			GameDBClient::GetInstance()->SendProtocol(rpc);

			//LOG_TRACE("CrssvrTeams_Info : SaveRoles save to db roleid=%d, teamid=%d", role.roleinfo.roleid, info.teamid);		
		}
		if((role.dirty_mask & CT_DIRTY_SCORE_DB) && (servertype == ECT_SERVER))
		{
			SyncRoleScore2DB(role);
		}
		if((role.dirty_mask & CT_DIRTY_ROLE_CENTRAL) && (servertype == ECT_SERVER))
		{
			SyncRoleScoreAndInfo2Central(role);
		}
		if((role.dirty_mask & CT_DIRTY_ROLE_ONLINE_CENTRAL) && (servertype == ECT_SERVER))
		{
			SyncRoleScoreAndInfo2Central(role);
		}

		if((role.dirty_mask & CT_DIRTY_SCORE_CENTRAL) && (servertype == ECT_SERVER))
		{
			SyncRoleScoreAndInfo2Central(role);
		}
		role.ClearDirty();
	}
}

int CrssvrTeams_Info::BattleFormula(int a_score, int b_score, int result)
{
	// 算本队员积分
	// 算队伍积分，跟队员算法一样
	// 玩家最终分数=当前分数+进攻方的积分对应不同区间的基础分*（根据胜负取1或0-陈差调用的修正系数）
	// 通过公示计算+0分的队伍或个人，强制统一+1分

	if(result == 0) return a_score + 1;// 平不得分
	if(result == -1 && a_score <= CRSSVRTEAMS_CALC_SCORE_LOW) return a_score + 1;// 1000-1500分段玩家竞技不扣除积分

	int diff_value = a_score - b_score; // 队伍积分差
	int modulus = CalcScoreModules(diff_value); //修正 系数
	int final_score = (a_score + GetScorePeriod(a_score) * (result-modulus));

	if(a_score > CRSSVRTEAMS_CALC_SCORE_LOW && final_score < CRSSVRTEAMS_CALC_SCORE_LOW)
		return CRSSVRTEAMS_CALC_SCORE_LOW;
	return final_score;
	//return final_score;
}

int CrssvrTeams_Info::CalcTeamScore(int b_score, int result)
{
	//更新战队积分
	int team_score = BattleFormula(info.score, b_score, result);
	SetTeamScore(team_score);
	if(result == 1)
		++info.battle_wins;
	else if(result == -1)
	{
		++info.battle_losses;
	}
	else if(result == 0)
	{
		++info.battle_draws;
	}
	++info.weekbattletimes;
	info.lastbattletime = m_pmanager->GetTime();
	info.seasonlastbattletime = m_pmanager->GetTime();

	SetDirty(CT_DIRTY_TEAM_DB);
	SetDirty(CT_DIRTY_TEAM_CENTRAL);
	LOG_TRACE("CrssvrTeams_Info : CalcTeamScore cal team score, teamid=%d, src_score=%d, final_score=%d, info.battle_wins=%d, info.battle_losses=%d", info.teamid, info.score, team_score, info.battle_wins, info.battle_losses);

	return 0;
}

int CrssvrTeams_Info::CalcRoleScore(std::vector<int>& team_roles, int b_score, int result)
{
	// 更新个人积分
	//LOG_TRACE("CrssvrTeams_Info : CalcRoleScore cal role score, team_roles.size=%d", team_roles.size());
	std::vector<int>::iterator roles_it, roles_ie = team_roles.end();
	for(roles_it = team_roles.begin(); roles_it != roles_ie; ++roles_it)
	{
		for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
		{
			CrssvrTeamsRole& role = *it;
			//LOG_TRACE("CrssvrTeams_Info : CalcRoleScore cal role score, rolecache=%d, roles_it=%d", role.roleinfo.roleid, *roles_it);
			if(role.roleinfo.roleid == *roles_it)
			{
				// 个人分差是个人积分和对方队伍比
				int role_score = role.roleinfo.score;
				int final_rolescore = BattleFormula(role_score, b_score, result);
				Log::formatlog("CrssvrTeams_Info", "CalcRoleScore cal role score,  roleid=%d, src_score=%d, final_score=%d", role.roleinfo.roleid, role_score, final_rolescore);
				SetRoleScore(role.roleinfo.roleid, final_rolescore, role, true);

				++role.roleinfo.ct_last_battle_count;
				m_pmanager->SetRoleDataGroup(role.roleinfo.roleid, KEY_DATAGOURP_ROLE_CT_LAST_BATTLE_COUNT, role.roleinfo.ct_last_battle_count);

				SyncRoleScore2Central(role);
				SyncRoleScore2DB(role);
				role.SetDirty(CT_DIRTY_ROLE_CENTRAL);
			}
		}
	}
	//PrintData("CalcRoleScore");
	return 0;
	
}

void CrssvrTeams_Info::CalcTeamPunish(int roleid, int now)
{
	// 战队惩罚基本思想
	// 玩家结算时间 B，打得场次C，上线时间A
	//PrintTeamData("CrssvrTeams_Info::Pre CalcTeamPunish");
	int src_score = info.score;
	int last_battle_time = (info.lastbattletime == 0)?m_pmanager->GetTime():info.lastbattletime;
	int battle_times = info.weekbattletimes;
	int reduce_interval = m_pmanager->CalSeasonInterval(last_battle_time, now); 
	int interval = now -  last_battle_time; // 与前一个结算时间差
	int week_times = interval / ONEWEEK_SECOND - reduce_interval; // 隔了几周

	LOG_TRACE("CrssvrTeams_Info::CalcTeamPunish roleid=%d, last_battle_time=%d, battle_times=%d, team_score=%d, interval=%d, week_times=%d,reduce_interval=%d", 
			roleid, last_battle_time, battle_times, src_score, interval, week_times,reduce_interval);
	if(week_times > 0)
	{
		int final_score = src_score;
		if(battle_times < 10)
		{
			// 所有的周都没有打够10场， 要惩罚所有周
			m_pmanager->GetPunishScore(roleid, week_times, final_score);
		}
		else
		{
			// 第一周够场次,还要看week_times是几周，第二周以后一定不够，因为如果够的话，早已结算
			// 惩罚第二周以后
			m_pmanager->GetPunishScore(roleid, week_times-1, final_score);
		}

		SetTeamScore(final_score);
		info.weekbattletimes = 0;
		info.lastbattletime = m_pmanager->GetTime();//m_pmanager->ct_act_weektoptime; // 当前周的起始时间
		SetDirty(CT_DIRTY_TEAM_DB);
		SetDirty(CT_DIRTY_TEAM_CENTRAL);
		Log::formatlog("CrssvrTeams_Info", "CalcTeamPunish, teamid=%d, src_score=%d, final_score=%d", info.teamid, src_score, final_score);
	}

	//PrintTeamData("CrssvrTeams_Info::Post CalcTeamPunish");
}

void CrssvrTeams_Info::ClearRenameCD()
{
	info.changenametime = 0;
	SetDirty(CT_DIRTY_TEAM_DB);
	SetDirty(CT_DIRTY_TEAM_CENTRAL);
}

void CrssvrTeams_Info::DebugRoleScore(int roleid, int score)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			SetRoleScore(roleid, score, role, true);
		}
	}
}

void CrssvrTeams_Info::DebugRoleLastMaxScore(int roleid, int score)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			role.data_score.ct_last_max_score = score;
			m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE, role.data_score.ct_last_max_score);

			SyncRoleScore2Central(role);
			role.SetDirty(CT_DIRTY_ROLE_CENTRAL);
		}
	}
}


void CrssvrTeams_Info::DebugRoleTeamScore(int roleid, int score)
{
	info.score = score;
	SetDirty(CT_DIRTY_TEAM_DB);
	SetDirty(CT_DIRTY_TEAM_CENTRAL);
}

void CrssvrTeams_Info::DebugTeamWin(int roleid, int times)
{
	info.battle_wins = times;
	info.seasonlastbattletime = m_pmanager->GetTime();
	SetDirty(CT_DIRTY_TEAM_DB);
	SetDirty(CT_DIRTY_TEAM_CENTRAL);
}

void CrssvrTeams_Info::DebugTeamPunish(int roleid, int times)
{
	info.weekbattletimes = times;
	info.lastbattletime = m_pmanager->GetTime();
	SetDirty(CT_DIRTY_TEAM_DB);
	SetDirty(CT_DIRTY_TEAM_CENTRAL);
}

void CrssvrTeams_Info::DebugRolePunish(int roleid, int times)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			SyncRolePunishScore2DB(roleid, 1000, times, m_pmanager->GetTime());
			return;
		}
	}
}


void CrssvrTeams_Info::DebugRolesScore(int roleid, int score)
{
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			SetRoleScore(roleid, score, role, true);
			return;
		}
	}
}

void CrssvrTeams_Info::PrintData(const char* s)
{
	PrintTeamData(s);
	PrintRolesData(s);
}

void CrssvrTeams_Info::PrintData(const char* s, int roleid)
{
	PrintTeamData(s);
	PrintRolesData(s, roleid);
}

void CrssvrTeams_Info::PrintRolesData(const char* s, int roleid)
{
	LOG_TRACE("%s\n", s);
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		if(role.roleinfo.roleid == roleid)
		{
			role.PrintRoles();
		}
	}
}

void CrssvrTeams_Info::PrintRolesData(const char* s)
{
	LOG_TRACE("%s\n", s);
	for (RoleList::iterator it = rolecache.begin(); it != rolecache.end(); ++it)
	{
		CrssvrTeamsRole& role = *it;
		role.PrintRoles();
	}
}

void CrssvrTeams_Info::PrintTeamData(const char* s)
{
	LOG_TRACE("%s\n", s);
	LOG_TRACE("teamid=%d,teamscore=%d,rolesize=%d, createtime=%d,master=%d,battle_losses=%d,battle_draws=%d,battle_wins=%d,weekbattletimes=%d,lastbattletime=%d,seasonlastbattletime=%d\n"
			, info.teamid, info.score, rolecache.size(), info.createtime, info.master, info.battle_losses, info.battle_draws, info.battle_wins, info.weekbattletimes, info.lastbattletime, info.seasonlastbattletime);
}

void CrssvrTeams_Info::SyncRolePunishScore2DB(int roleid, int role_score, int battle_count, int last_battle_time)
{
	DBCrssvrTeamsRolePunishScoreArg arg;
	arg.roleid = roleid;
	arg.ct_score = role_score; 
	arg.ct_last_punish_time = last_battle_time; 
	arg.ct_last_battle_count = battle_count; 
	DBCrssvrTeamsRolePunishScore* rpc = (DBCrssvrTeamsRolePunishScore*) Rpc::Call( RPC_DBCRSSVRTEAMSROLEPUNISHSCORE,arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void CrssvrTeams_Info::SyncRoleScore2DB(const CrssvrTeamsRole& role)
{
	SyncCrssvrTeamsScoreArg arg;
	arg.roleid = role.roleinfo.roleid;
	arg.data_score = role.data_score;
	arg.ct_score = role.roleinfo.score;
	arg.ct_last_battle_count = role.roleinfo.ct_last_battle_count;

	SyncCrssvrTeamsScore* rpc = (SyncCrssvrTeamsScore*) Rpc::Call( RPC_SYNCCRSSVRTEAMSSCORE,arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void CrssvrTeams_Info::SyncRoleScore2Central(const CrssvrTeamsRole& role)
{
	if((servertype == ECT_SERVER) && IsRoleCrossOnline())
	{
		SyncCrssvrTeamsScoreArg arg;
		arg.roleid = role.roleinfo.roleid;
		arg.data_score = role.data_score;
		arg.ct_score = role.roleinfo.score;
		arg.ct_last_battle_count = role.roleinfo.ct_last_battle_count;

		SyncCrossCrssvrTeamsScore* rpc_cross = (SyncCrossCrssvrTeamsScore*) Rpc::Call( RPC_SYNCCROSSCRSSVRTEAMSSCORE,arg);
		CentralDeliveryClient::GetInstance()->SendProtocol(rpc_cross);
	}
}

void CrssvrTeams_Info::SyncRoleScoreAndInfo2Central(const CrssvrTeamsRole& role)
{
	SyncCrossCrssvrTeamsAddArg cross_arg;
	cross_arg.zoneid = GDeliveryServer::GetInstance()->zoneid;
	cross_arg.teamid = info.teamid;
	cross_arg.info = role.roleinfo;
	cross_arg.data_score = role.data_score;
	SyncCrossCrssvrTeamsAdd* cross_rpc = (SyncCrossCrssvrTeamsAdd*) Rpc::Call( RPC_SYNCCROSSCRSSVRTEAMSADD,cross_arg);
	CentralDeliveryClient::GetInstance()->SendProtocol(cross_rpc);
}

void CrssvrTeams_Info::UpdateLastMaxScore(CrssvrTeamsRole& role)
{
	// 首先，玩家重新上线，要重排上周最高积分D
	// 如果D<周榜时间，说明还未重新排过，上周最高积分=当前最高积分即可,并设置当前最高积分重置
	// 如果D>周榜时间，说明本周已经排过，因为玩家新一周上线，总有D>周榜时间的时候，所以不做处理
	int roleid = role.roleinfo.roleid;
	int role_score = role.roleinfo.score;
	int max_score = role.data_score.ct_max_score;
	int curr_score_updatetime = role.data_score.ct_score_updatetime;
	int top_time = m_pmanager->GetLastWeekTopTime();

	if(curr_score_updatetime < top_time)
	{
		LOG_TRACE("CrssvrTeams_Info : Pre UpdateLastMaxScore roleid=%d, role_score=%d, max_score=%d, curr_score_updatetime=%d, top_time=%d", 
				roleid, role_score, max_score, curr_score_updatetime, top_time);

		if(m_pmanager->SetRoleDataGroup(roleid, KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE, max_score))
		{
			role.data_score.ct_last_max_score = max_score;
			// 此最大积分为本周之前的，更新之
			role.data_score.ct_max_score = role_score;
			role.data_score.ct_score_updatetime = m_pmanager->GetTime();
			role.SetDirty(CT_DIRTY_SCORE_DB);
			role.SetDirty(CT_DIRTY_SCORE_CENTRAL);

			LOG_TRACE("CrssvrTeams_Info : Real UpdateLastMaxScore roleid=%d, role.ct_last_max_score=%d, role.data_score.ct_max_score=%d", 
					roleid, role.data_score.ct_last_max_score, role.data_score.ct_max_score);

		}
	}

	return;

}

};

