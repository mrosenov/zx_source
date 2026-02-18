#include <list>
#include <string.h>
#include <algorithm>
#include "raidmanager.h"
#include "mapuser.h"
#include "hashstring.h"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "matcher.h"
#include "factionmanager.h"

#include "dbraidopen.hrp"
#include "gmailendsync.hpp"
#include "raidjoinapplylist.hpp"
#include "raidjoinreq.hpp"
#include "raidjoin_re.hpp"
#include "raidkicknotify.hpp"
#include "sendraidstart.hpp"
#include "raidstartnotify.hpp"
#include "raidappointnotify.hpp"
#include "sendraidenter.hpp"
#include "sendraidclose.hpp"
#include "playerchangegs.hpp"
#include "raidstart_re.hpp"
#include "raidvoteresult.hpp"
#include "sendraidkickout.hpp"
#include "raidbeginvotes.hpp"
#include "raidbriefinfo.hpp"
#include "gcrssvrteamsdata"
#include "teamraidmappingsuccess.hpp"
#include "teamraidbeginchoosechar.hpp"
#include "teamraidacktimeout.hpp"
#include "teamraidapply_re.hpp"
#include "teamraidupdatechar.hpp"
#include "factionpkraidinvite.hpp"
#include "factionpkraidchallenge_re.hpp"
#include "factionpkraidbegin.hpp"
#include "factionpkraidgetscore_re.hpp"
#include "factionpkraidstopfight.hpp"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"
#include "teamraidapplynotice.hpp"
#include "crossbattlemanager.hpp"
#include "centraldeliveryserver.hpp"
#include "crosscrssvrteamsmanager.h"
#include "raidenter_re.hpp"
#include "teamraidactiveapplynotice.hpp"
#include "synccrosscrssvrteamsendbrief.hrp"
#include "crossraidenter_re.hpp"
#include "raidquit_re.hpp"
#include "hideseekmappingsuccess.hpp"
#include "hideseekmappingsuccess_re.hpp"
#include "hideseekraidquit.hpp"

namespace GNET
{
static bool raidmanager_debug_mode = false;
static size_t TEAMRAID_MEMBER_LIMIT = 6;//each side
WaitingTeamContainer::TeamPolicy WaitingTeamContainer::teampolicies;
int RaidTeamBase::nextid = 1;
int HideSeekRaidBase::nextid = 1;

static int g_teamfrag_create = 0;
static int g_teamfrag_delete = 0;
static int g_applyrole_create = 0;
static int g_applyrole_delete = 0;

bool RaidMap_Info::AddLine(const RaidFieldInfo& finfo,int gs_id)
{
	if(finfo.raid_max_instance<0)
		return false;
	if(raidnumber.insert(std::make_pair(gs_id,RaidNumber(finfo.raid_max_instance))).second){
		room_limit += finfo.raid_max_instance;
		return true;
	}
	else
		return false;
}

bool RaidMap_Info::DelLine(int gs_id)
{
	RaidNumberMap::iterator it = raidnumber.find(gs_id);
	if(it!=raidnumber.end()){
		room_limit -= it->second.max_instance;
		raidnumber.erase(it);
		return true;
	}
	else 
		return false;
}

bool RaidMap_Info::Apply_Instance_Finish(int gs_id,bool success)
{
	RaidNumberMap::iterator it = raidnumber.find(gs_id);
	if(it == raidnumber.end())
		return false;
	return it->second.Apply_Instance_Finish(success);
}

bool RaidMap_Info::Dec_Instance(int gs_id)
{
	RaidNumberMap::iterator it = raidnumber.find(gs_id);
	if(it == raidnumber.end())
		return false;
	it->second.Dec_Instance();
	return true;
}

bool RaidMap_Info::Try_Get_Instance(int &gs_id)
{
	gs_id=-1;
	int free_num=0;
	RaidNumberMap::iterator it,ite=raidnumber.end();
	for(it=raidnumber.begin();it!=ite;++it)
	{
		int newnum = it->second.GetFreeNum();
		if(newnum>free_num)
		{	gs_id = it->first;
			free_num = newnum;
		}
	}
	if(gs_id!=-1)
	{
		it=raidnumber.find(gs_id);
		return it->second.Add_Applying_Instance();
	}
	else
		return false;
}

bool RaidMap_Info::StartPolicy(int player_num) const 
{
	if (raidmanager_debug_mode)
		return true;
	else
		return (!group1_info.min_start_player_num||player_num >= group1_info.min_start_player_num);
}

////////////////////////////////////////////////////////////////////////////////////////////////
RaidBasic_Info::~RaidBasic_Info()
{
	LOG_TRACE("~RaidBasic_Info()");
	RoomClose();
}
RaidRoom_Info::~RaidRoom_Info()
{
	LOG_TRACE("~RaidRoom_Info()");
	RoomClose();
}

TeamRaid_Info::TeamRaid_Info(int raidroomid,int mapid, RaidMap_Info & mapinfo, const TeamRaidMatchData& team_data, RaidTeamBase * a, RaidTeamBase * b)
: RaidBasic_Info(raidroomid,mapid,mapinfo),teamdata(team_data), teama(a), teamb(b)
{
	SetStatus(RAID_WAITACK);
}

TeamRaid_Info::~TeamRaid_Info()
{
	//LOG_TRACE("~TeamRaid_Info()");
	RoomClose();
}

HideSeekRaid_Info::HideSeekRaid_Info(int raidroomid,int mapid, RaidMap_Info & mapinfo)
	: RaidBasic_Info(raidroomid,mapid,mapinfo)
{
	SetStatus(RAID_WAITACK);
}

HideSeekRaid_Info::~HideSeekRaid_Info()
{
	//LOG_TRACE("~HideSeekRaid_Info()");
	RoomClose();
}

void HideSeekRaid_Info::Init(HideSeekRaidBase * teama)
{
	LOG_TRACE("HideSeekRaid_Info::Init, team1.size=%d, teama.size=%d", team1.size(), teama->GetSize());
	HideSeekMappingSuccess msg(raidroom_id);

	TeamBroadcast(team1,msg,msg.localsid,msg.roleid);

	TeamType::iterator it, ite= team1.end();
	for(it=team1.begin();it!=ite;++it)
		role_need_mapping_re.insert(it->roleid);

}

RaidBasic_Info::TeamType::iterator RaidBasic_Info::FindRole(TeamType &team,int roleid)
{
	TeamType::iterator it,ite=team.end();
	for(it=team.begin();it!=ite;++it)
	{
		if(it->roleid == roleid)
			return it;
	}
	return team.end();
}
int TeamRaid_Info::OnMappingSuccess_Re(int roleid, char agree)
{
	if(status!=RAID_WAITACK)
		return ERR_RAID_STATUS_CHANGE;
	if(role_need_mapping_re.erase(roleid)==0)
	{
		Log::log(LOG_ERR,"TeamRaid_Info::OnMappingSuccess_Re role deleted, roleid=%d",roleid);
	}
	
	if(IsCrssvrTeamsRaid())
	{
		TeamRaidActiveApplyNotice notice(roleid, 1, 0);// 点亮跨服6v6图标
		TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
	}

	if(role_need_mapping_re.size()!=0)
	{
		return ERR_SUCCESS;
	}
	int ret = StartRaid();
	if (ret != ERR_SUCCESS)
	{
		if(IsCrssvrTeamsRaid())
		{
			TeamRaidApplyNotice notice(0, 2, 0);
			TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
			SetStatus(RAID_STARTFAIL);
		}
		else
		{
			TeamRaidApplyNotice notice(0, 1, 0);
			TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
			SetStatus(RAID_STARTFAIL);
		}
	}

	LOG_TRACE("TeamRaid_Info::OnMappingSuccess_Re roleid=%d ret=%d" ,roleid, ret);

	return ret;
}

int HideSeekRaid_Info::OnMappingSuccess_Re(int roleid, char agree)
{
	if(status!=RAID_WAITACK)
		return ERR_RAID_STATUS_CHANGE;
	if(role_need_mapping_re.erase(roleid)==0)
	{
		Log::log(LOG_ERR,"HideSeekRaid_Info::OnMappingSuccess_Re role deleted, roleid=%d",roleid);
	}
	
	if(role_need_mapping_re.size()!=0)
	{
		return ERR_SUCCESS;
	}
	int ret = StartRaid();
	if (ret != ERR_SUCCESS)
	{
		TeamRaidApplyNotice notice(0, 3, 0);
		TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
		SetStatus(RAID_STARTFAIL);
	}

	LOG_TRACE("HideSeekRaid_Info::OnMappingSuccess_Re roleid=%d ret=%d" ,roleid, ret);

	return ret;
}

int TeamRaid_Info::MappingSuccess_Re_Timeout()
{
	LOG_TRACE("TeamRaid_Info::MappingSuccess_Re_Timeout roomid=%d, role_need_mapping_re.size=%d " , raidroom_id, role_need_mapping_re.size());

	if(IsCrssvrTeamsRaid())
	{
		RoleSet::iterator it, ite = role_need_mapping_re.end();
		for(it = role_need_mapping_re.begin(); it != ite; ++it)
		{
			TeamRaidActiveApplyNotice notice(*it, 1, 0);// 点亮跨服6v6图标
			TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
		}
	}

	role_need_mapping_re.clear();
	
	int ret = StartRaid();
	if (ret != ERR_SUCCESS)
	{
		if(IsCrssvrTeamsRaid())
		{
			TeamRaidApplyNotice notice(0, 2, 0);
			TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
		}
		else
		{
			TeamRaidApplyNotice notice(0, 1, 0);
			TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
		}

	}
	return ret;
}

int HideSeekRaid_Info::MappingSuccess_Re_Timeout()
{
	LOG_TRACE("HideSeekRaid_Info::MappingSuccess_Re_Timeout roomid=%d, role_need_mapping_re.size=%d " , raidroom_id, role_need_mapping_re.size());

	role_need_mapping_re.clear();
	
	int ret = StartRaid();
	if (ret != ERR_SUCCESS)
	{
		TeamRaidApplyNotice notice(0, 3, 0);
		TeamBroadcast(team1, notice, notice.localsid, notice.roleid);
	}
	return ret;
}

RaidTeamBase::RaidTeamBase(const TeamRoleList & roles, /*int t,*/WaitingTeamContainer * con) : id(0)/*, type(0)*/, ave_score(0), score_range_delta(TEAMRAID_SCORE_RANGE_BASE), rolelist(roles), container(con)/*, del_from_pool(false)*/
{
	id = nextid++;
	create_time = Timer::GetTime();
	g_teamfrag_create++;
	LOG_TRACE("teamfrag %d is created ave_score %d size %d g_teamfrag_create=%d", id, ave_score, GetSize(), g_teamfrag_create );
}
RaidTeamBase::~RaidTeamBase()
{
	g_teamfrag_delete++;
	LOG_TRACE("teamfrag %d is deleted size %d g_teamfrag_delete=%d", id, GetSize(), g_teamfrag_delete);
}


void RaidTeamBase::BroadRoleList(Protocol & data, unsigned int & localsid, int & t_roleid)
{
	TeamRoleList::iterator it = rolelist.begin(), ite = rolelist.end();
	for (; it != ite; ++it)
	{
		int roleid = (*it)->GetRoleid();
		CrossBattleManager *manager = CrossBattleManager::GetInstance();
		PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(pinfo==NULL)
		{
			//跨服没有，转发到原服
			int src_zoneid = manager->GetSrcZoneID(roleid);
			t_roleid = roleid;
			CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, data);
		}
		else if(pinfo != NULL && pinfo->ingame) // 在跨服
		{
			localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, data);
		}
	}
}

void TeamRaid_Info::RoomClose()
{
	/*
	//6v6 副本 gs 先发 graidend 再发 graidleave 将还留在房间中的角色的冷却清除
	RaidManager* man=RaidManager::GetInstance();
	TeamType::iterator it;
	for(it=team1.begin();it!=team1.end();++it)
		man->ClearTeamApplyCD(it->roleid);
		*/
	LOG_TRACE("TeamRaid_Info::RoomClose");
	if (teama)
	{
		LOG_TRACE("TeamRaid_Info::RoomClose, id = %d", teama->GetId());
		if (teama->GetSize() && (status == RAID_WAITACK || status == RAID_STARTFAIL)
				&& teama->GetContainer()->IsJoinTime())
			teama->GetContainer()->AddTeam(teama, true); //稍后再触发match 否则当前RoomClose()还没执行完毕 又将角色加入到新的room 会失败
		else
		{
			teama->DeleteRoleList();
			delete teama;
		}
		teama = NULL;
	}
	if (teamb)
	{
		if (teamb->GetSize() && (status == RAID_WAITACK || status == RAID_STARTFAIL)
				&& teamb->GetContainer()->IsJoinTime())
			teamb->GetContainer()->AddTeam(teamb, true);
		else
		{
			teamb->DeleteRoleList();
			delete teamb;
		}
		teamb = NULL;
	}
}

void TeamRaid_Info::SendBattleResult(int result)
{
	CrssvrTeamsFrag* pteama = dynamic_cast<CrssvrTeamsFrag*>(teama);
	CrssvrTeamsFrag* pteamb = dynamic_cast<CrssvrTeamsFrag*>(teamb);

	if(!pteama || !pteamb)
	{
		LOG_TRACE("TeamRaid_Info::SendBattleResult err pteaa==NULL or petamb==NULL");
		return;
	}

	//result = gs 中，1表示攻防胜，2表示守方胜，3表示平
	// teama结果
	int resultA = 0;// 平
	int resultB = 0;
	if (result == 1)// teama 胜
	{
		resultA = 1;
		resultB = -1;
	}
	else if(result == 2) // teama 负
	{
		resultA = -1;
		resultB = 1;
	}
	LOG_TRACE("TeamRaid_Info::SendBattleResult teama=%d, teamb=%d, a_score=%d, b_score=%d, resultA=%d, resultB=%d, zoneida=%d, zoneidb=%d", 
			pteama->teamid, pteamb->teamid, pteama->teamscore, pteamb->teamscore, resultA, resultB, pteama->src_zoneid, pteama->src_zoneid);
	
	// 队伍积分结算统一回原服，个人积分统一在跨服计算
	std::vector<int> teama_id_list;
	pteama->GetRoleIdList(teama_id_list);
	SyncCrossCrssvrTeamsEndBrief* rpc_a = (SyncCrossCrssvrTeamsEndBrief*) Rpc::Call(RPC_SYNCCROSSCRSSVRTEAMSENDBRIEF, SyncCrossCrssvrTeamsEndBriefArg(pteama->teamid, teama_id_list, pteamb->teamscore, resultA));
	CentralDeliveryServer::GetInstance()->DispatchProtocol(pteama->src_zoneid, rpc_a);
	//CrossCrssvrTeamsManager::Instance()->CalcRoleScore(pteama->teamid, pteamb->teamscore, resultA);
	// teamb结果
	std::vector<int> teamb_id_list;
	pteamb->GetRoleIdList(teamb_id_list);
	SyncCrossCrssvrTeamsEndBrief* rpc_b = (SyncCrossCrssvrTeamsEndBrief*) Rpc::Call(RPC_SYNCCROSSCRSSVRTEAMSENDBRIEF, SyncCrossCrssvrTeamsEndBriefArg(pteamb->teamid, teamb_id_list, pteama->teamscore, resultB));
	CentralDeliveryServer::GetInstance()->DispatchProtocol(pteamb->src_zoneid, rpc_b);
	//CrossCrssvrTeamsManager::Instance()->CalcRoleScore(pteamb->teamid, pteama->teamscore, resultB);
}

void TeamRaid_Info::DelCrssvrTeamsApplying()
{
	CrssvrTeamsFrag* pteama = dynamic_cast<CrssvrTeamsFrag*>(teama);
	CrssvrTeamsFrag* pteamb = dynamic_cast<CrssvrTeamsFrag*>(teamb);

	if(!pteama || !pteamb)
	{
		LOG_TRACE("TeamRaid_Info::DelCrssvrTeamsApplying err pteaa==NULL or petamb==NULL");
		return;
	}

	LOG_TRACE("TeamRaid_Info::DelCrssvrTeamsApplying teamida=%d, teamidb=%d", pteama->teamid, pteamb->teamid);
	RaidManager::GetInstance()->DelCrssvrTeamsApplying(pteama->teamid);
	RaidManager::GetInstance()->DelCrssvrTeamsApplying(pteamb->teamid);
}
/*
int TeamRaid_Info::AutoChooseChar()
{
	LOG_TRACE("TeamRaid_Info::AutoChooseChar roomid=%d " , raidroom_id);

	RaidManager::GetInstance()->DelRoomEvent(raidroom_id,status_end_time);
	TeamRaidBeginChooseChar msg(raidroom_id,map_id,teamdata);
	TeamBroadcast(team1,msg,msg.localsid);
	SetStatus(RAID_WAITCHOOSECHAR);
	return ERR_SUCCESS;
}
	*/
int FactionPkRaid_Info::FactionPkJoin(const RaidRole& role, int mapid,int groupid)
{
	int roleid = role.roleid;
	LOG_TRACE("FactionPkRaid_Info::JoinRaid roleid=%d" ,roleid);
	if(mapid!=this->map_id)
		return ERR_RAID_ARGUMENT_ERR;

	if(status!=RAID_FIGHTING)
		return ERR_RAID_STATUS;
	
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if(!pinfo)
		return ERR_RAID_ROLE_OFFLINE;
	switch(groupid)
	{
		case RAIDTEAM_ATTACK:
			if(pinfo->factionid != static_cast<unsigned int>(attacker))
				return ERR_RAID_FACTION_JOIN_GROUP;
			break;	
		case RAIDTEAM_DEFEND:
			if(pinfo->factionid != static_cast<unsigned int>(defender))
				return ERR_RAID_FACTION_JOIN_GROUP;
			break;
		case RAIDTEAM_LOOKER:
			break;
		default:
			return ERR_RAID_FACTION_JOIN_GROUP;
			break;
	}
	if(!map_info.JoinPolicy(group_size[groupid]))//need multi-team expansion
		return ERR_RAID_APPLY_TEAM_FULL;
	group_size[groupid]++;	
	if(role_group.find(roleid)!=role_group.end())
		Log::log(LOG_ERR,"FactionPkRaid_Info::JoinRaid role %d already in room",roleid);
	role_group.insert(std::make_pair(roleid,groupid));
	AcceptRole(role);
	RaidEnter(roleid,true);
	return ERR_SUCCESS;
}

int RaidRoom_Info::JoinRaid(const RaidRole& role, int mapid)
{
	LOG_TRACE("RaidRoom_Info::JoinRaid roleid=%d" , role.roleid);
	if(mapid!=this->map_id)
		return ERR_RAID_ARGUMENT_ERR;

	if(status!=RAID_QUEUEING)
		return ERR_RAID_STARTED;
	if(!map_info.JoinPolicy(team1_apply.size()))//need multi-team expansion
	{
		return ERR_RAID_APPLY_TEAM_FULL;
	}	
	if(!AddApplyRole(role))
		return ERR_RAID_ROLE_ALREADY_IN_ROOM;
	else
	{
		SendReq(role.roleid);
		return ERR_SUCCESS;
	}
}
void RaidRoom_Info::SendReq(int roleid)
{
	if(team1.size()>0)
	{
		RaidRole & master = *team1.begin();
		RaidJoinReq req(master.roleid, map_id, raidroom_id, roleid, master.localsid);
		GDeliveryServer::GetInstance()->Send(master.linksid, req);
	}
}
void FactionPkRaid_Info::RoomClose()
{
	RaidManager* man=RaidManager::GetInstance();
	switch (status)
	{
		case RAID_ENDING:
			man->ClearInstance(gs_id,index);
			map_info.Dec_Instance(gs_id);
			break;
		default:
			break;
	}
}
void RaidBasic_Info::RoomClose()
{
	//LOG_TRACE("RaidBasic_Info::RoomClose roomid=%d" , raidroom_id);
	TeamType::iterator it;
	int reason;
	switch (status)
	{
		case RAID_SENDSTART:
			reason = RAID_INSTANCE_START_ERR;
			break;
		case RAID_SENDSTART_NOTIFY:
			reason = RAID_INSTANCE_ENTER_ERR;
			break;
		case RAID_FIGHTING:
			reason = RAID_INSTANCE_CLOSE;
			break;
		default:
			reason = RAID_ROOMCLOSE;
			break;
	}
	if(!gs_has_close_instance)//if gs has notify instance-close, need not to kick client,it is a normal close
		KickNotifyAll(reason);
	RaidManager* man=RaidManager::GetInstance();
	for(it=team1.begin();it!=team1.end();++it)
		man->ClearRole(it->roleid);
	if(ISFighting()&& gs_has_close_instance == false)
	{
		LOG_TRACE("RaidBasic_Info::RoomClose roomid=%d SendRaidClose to gs" , raidroom_id);
		Log::formatlog("raid_stat","RaidClose raidroom_id=%d gs_id=%d index=%d",raidroom_id,gs_id,index);
		GProviderServer::GetInstance()->DispatchProtocol(gs_id,SendRaidClose(raidroom_id,map_id,index,0));
	}
	switch (status)
	{
		case RAID_SENDSTART:
			man->ClearInstance(gs_id,index);
			map_info.Apply_Instance_Finish(gs_id,false);
			break;
		case RAID_SENDSTART_NOTIFY:
		case RAID_FIGHTING:
			man->ClearInstance(gs_id,index);
			map_info.Dec_Instance(gs_id);
			break;
		default:
			break;
	}
//	man->DelRoomEvent(raidroom_id,status_end_time);
	map_info.Dec_Room();
}
void RaidRoom_Info::RoomClose()
{
	LOG_TRACE("RaidRoom_Info::RoomClose roomid=%d" , raidroom_id);
	TeamType::iterator it;
	int reason;
	switch (status)
	{
		case RAID_SENDSTART:
			reason = RAID_INSTANCE_START_ERR;
			break;
		case RAID_QUEUEING:
			reason = RAID_ROOMCLOSE;
			break;
		case RAID_SENDSTART_NOTIFY:
			reason = RAID_INSTANCE_ENTER_ERR;
			break;
		case RAID_FIGHTING:
			reason = RAID_INSTANCE_CLOSE;
			break;
		default:
			reason = RAID_ROOMCLOSE;
			break;
	}
	if(!gs_has_close_instance)//if gs has notify instance-close, need not to kick client,it is a normal close
		KickNotifyAll(reason);
	RaidManager* man=RaidManager::GetInstance();
	for(it=team1.begin();it!=team1.end();++it)
		man->ClearRole(it->roleid);
	for(it=team1_apply.begin();it!=team1_apply.end();++it)
		man->ClearRole(it->roleid);
	//clear here, prevent double release in base class
	team1.clear();
	team1_apply.clear();

	switch (status)
	{
		case RAID_QUEUEING:
			break;
	}
	if(pvote) 
	{
		delete pvote;
		pvote=NULL;
	}
}
int RaidRoom_Info::OnAccept(int roleid, unsigned char accept, int target)
{
	LOG_TRACE("RaidBasic_Info::OnAccept roomid=%d target=%d roleid=%d accept=%d" , raidroom_id,target,roleid,accept);
	if(roleid!=leader)
		return ERR_RAID_OPERATOR_NOT_MASTER;
	if(status!=RAID_QUEUEING)
		return ERR_RAID_STARTED;
	if(accept)
	{
		if(!map_info.JoinPolicy(team1.size()))//need multi-team expansion
		{
			return ERR_RAID_TEAM_FULL;
		}
		TeamType::iterator it=FindRole(team1_apply,target);
		if(it!=team1_apply.end())
		{
			AcceptRole(*it);
			GDeliveryServer::GetInstance()->Send(it->linksid, RaidJoin_Re(ERR_SUCCESS,raidroom_id,map_id,it->localsid));
			team1_apply.erase(it);
		}
		else
			return ERR_RAID_NOT_IN_APPLYLIST;
	}
	else
	{
		TeamType::iterator it=FindRole(team1_apply,target);
		if(it!=team1_apply.end())
		{
			GDeliveryServer::GetInstance()->Send(it->linksid, RaidJoin_Re(ERR_RAID_JOIN_REFUSED,raidroom_id,map_id,it->localsid));
			team1_apply.erase(it);
		}
		else
			return ERR_RAID_NOT_IN_APPLYLIST;
	}	
	return ERR_SUCCESS;
}
int RaidRoom_Info::Start(int roleid)
{
	LOG_TRACE("RaidRoom_Info::Start roomid=%d roleid=%d" , raidroom_id,roleid);
	if(roleid!=leader)
		return ERR_RAID_OPERATOR_NOT_MASTER;
	if(status!=RAID_QUEUEING)
		return ERR_RAID_STARTED;
	if(!map_info.StartPolicy(team1.size()))
		return ERR_RAID_START_TEAMNUMBER;
	int retcode = RaidBasic_Info::Start();
	if(retcode == ERR_SUCCESS)
		ClearApplyList();
	return retcode;
}
int RaidBasic_Info::Start()
{
	LOG_TRACE("RaidBasic_Info::Start roomid=%d " , raidroom_id);
	int gs_id_;
	if(map_info.Try_Get_Instance(gs_id_) == false)
	{
		LOG_TRACE("RaidBasic_Info::Start roomid=%d Try_Get_Instance err" , raidroom_id);
		return ERR_RAID_GET_INSTANCE_FAIL;
	}
	else
		gs_id = gs_id_;

	RaidManager::GetInstance()->DelRoomEvent(raidroom_id,status_end_time);
	SetStatus(RAID_SENDSTART);
	SendRaidStart st(map_id, raidroom_id);
	TeamType::iterator it,ite=team1.end();
	for(it=team1.begin();it!=ite;++it)
	{
		st.group1_list.roleid_list.push_back(it->roleid);
	}
	st.raid_data = raid_data;
	LOG_TRACE("RaidBasic_Info::SendRAIDStart roomid=%d raid_data size=%d, map_id=%d" , raidroom_id,raid_data.size(), map_id);
	st.difficulty = raid_difficulty;
	GProviderServer::GetInstance()->DispatchProtocol(gs_id,st); 

	return ERR_SUCCESS;
}
int TeamRaid_Info::StartRaid()
{
	LOG_TRACE("TeamRaid_Info::StartRaid roomid=%d " , raidroom_id);
	if(status!=RAID_WAITACK)
		return ERR_RAID_STARTED;
	
	Octets tmp;
	int att_num=0;
	int def_num=0;
	for(std::vector<TeamRaidRoleData>::iterator it=teamdata.roles.begin();it!=teamdata.roles.end();++it)
	{
		tmp.insert(tmp.end(),&(it->roleid),sizeof(int));
		tmp.insert(tmp.end(),&(it->team),sizeof(int));
		tmp.insert(tmp.end(),&(it->score),sizeof(int));
		if(it->team==RAIDTEAM_ATTACK)
			att_num++;
		else if(it->team==RAIDTEAM_DEFEND)
			def_num++;
		else
			LOG_TRACE("TeamRaid_Info::StartRaid roleid=%d team=%d err",it->roleid,it->team);
	}
	if(TEAMRAID_MEMBER_LIMIT>(unsigned int)MIN_ENTER_NUM  &&( att_num<MIN_ENTER_NUM ||def_num<MIN_ENTER_NUM))
		return ERR_RAID_START_TEAMNUMBER;
	//LOG_TRACE("TeamRaid_Info::AutoStart TeamRaidRoleData size=%d " , sizeof(TeamRaidRoleData));
	SetRaidData(tmp);
	
	int retcode = Start();
	if(retcode != ERR_SUCCESS )
	{
		RaidStart_Re msg(retcode,raidroom_id);
		TeamBroadcast(team1,msg,msg.localsid, msg.roleid);
	}
	return retcode;
}
int FactionPkRaid_Info::StartRaid()
{
	LOG_TRACE("FactionPkRaid_Info::StartRaid roomid=%d " , raidroom_id);
	if(status!=RAID_WAIT_INVITE_RE)
		return ERR_RAID_STARTED;
	
	//SetRaidData(tmp);
	
	return Start();
}
int HideSeekRaid_Info::StartRaid()
{
	LOG_TRACE("HideSeekRaid_Info::StartRaid roomid=%d, role_size=%d" , raidroom_id, team1.size());
	
	if(status!=RAID_WAITACK)
		return ERR_RAID_STARTED;

	Octets tmp;
	int att_num=0;
	int def_num=0;
	int tmp_faction = RAIDTEAM_ATTACK;

	//打乱顺序防止玩家刷
	std::random_shuffle(team1.begin(), team1.end());

	TeamType::iterator it,ite=team1.end();
	for(it=team1.begin();it!=ite;++it)
	{
		tmp.insert(tmp.end(),&(it->roleid),sizeof(int));
		tmp.insert(tmp.end(),&tmp_faction,sizeof(int));
		if(tmp_faction == RAIDTEAM_ATTACK)
		{
			att_num ++;
			tmp_faction = RAIDTEAM_DEFEND;
		}
		else
		{
			def_num ++;
			tmp_faction = RAIDTEAM_ATTACK;
		}
	}
	
	LOG_TRACE("HideSeekRaid_Info::StartRaid att_num=%d, def_num=%d " , att_num, def_num);
	SetRaidData(tmp);

	int retcode = Start();
	if(retcode != ERR_SUCCESS )
	{
		RaidStart_Re msg(retcode,raidroom_id);
		TeamBroadcast(team1,msg,msg.localsid, msg.roleid);
	}
	return retcode;
}

int RaidRoom_Info::AutoStart()
{
	LOG_TRACE("RaidRoom_Info::AutoStart roomid=%d " , raidroom_id);
	int retcode = Start(leader);
	if(retcode != ERR_SUCCESS && team1.size()>0)
	        GDeliveryServer::GetInstance()->Send(team1[0].linksid,RaidStart_Re(retcode,raidroom_id,team1[0].localsid));
	return retcode;
}
void RaidRoom_Info::ClearApplyList()
{
//	LOG_TRACE("RaidBasic_Info::ClearApplyList roomid=%d " , raidroom_id);
	RaidManager* man=RaidManager::GetInstance();
	for(TeamType::iterator it=team1_apply.begin();it!=team1_apply.end();++it)
		man->ClearRole(it->roleid);
	team1_apply.clear();
}
int RaidBasic_Info::OnGsStart(int gs_id_,int index_,bool success)
{
	LOG_TRACE("RaidBasic_Info::OnGsStart roomid=%d index=%d gsid=%d success=%d mapid=%d" , raidroom_id, index_, gs_id_,success,map_id);
	if(success)//start fail deal in roomclose
		map_info.Apply_Instance_Finish(gs_id_,success);
	RaidManager::GetInstance()->DelRoomEvent(raidroom_id,status_end_time);
	if(success==false)
		return ERR_SUCCESS;
	if(status!=RAID_SENDSTART)
		return ERR_RAID_STATUS_CHANGE;
	gs_id=gs_id_;
	index=index_;
	//skip notify to client
	for(TeamType::iterator it=team1.begin();it!=team1.end();++it)
	{
		role_need_auto_enter.insert(it->roleid);
		role_need_enter_re.insert(it->roleid);
	}
	AutoEnter();
	return ERR_SUCCESS;
}
int FactionPkRaid_Info::OnGsStart(int gs_id_,int index_,bool success)
{
	LOG_TRACE("FactionPkRaid_Info::OnGsStart roomid=%d index=%d gsid=%d success=%d mapid=%d" , raidroom_id, index_, gs_id_,success,map_id);
	if(success)//start fail deal in roomclose
		map_info.Apply_Instance_Finish(gs_id_,success);
	RaidManager::GetInstance()->DelRoomEvent(raidroom_id,status_end_time);
	if(success==false)
		return ERR_SUCCESS;
	if(status!=RAID_SENDSTART)
		return ERR_RAID_STATUS_CHANGE;
	gs_id=gs_id_;
	index=index_;
	//skip notify to client and auto enter
	AutoEnter();//change state
	FactionPkRaidBegin msg(ERR_SUCCESS,map_id,raidroom_id,attacker,defender,attfaction_name,deffaction_name);
//	FactionPkRaidBegin msg(ERR_SUCCESS,map_id,raidroom_id,attacker,defender,Octets(0),Octets(0));
        FactionManager::Instance()->Broadcast(attacker, msg, msg.localsid);
        FactionManager::Instance()->Broadcast(defender, msg, msg.localsid);
	BroadCastMsg(MSG_COMBATSTART);
	return ERR_SUCCESS;
}
int RaidRoom_Info::OnGsStart(int gs_id_,int index_,bool success)
{
	LOG_TRACE("RaidRoom_Info::OnGsStart roomid=%d index=%d gsid=%d success=%d mapid=%d" , raidroom_id, index_, gs_id_,success,map_id);
	if(success)//start fail deal in roomclose
		map_info.Apply_Instance_Finish(gs_id_,success);
	RaidManager::GetInstance()->DelRoomEvent(raidroom_id,status_end_time);
	if(success==false)
		return ERR_SUCCESS;
	if(status!=RAID_SENDSTART)
		return ERR_RAID_STATUS_CHANGE;
	gs_id=gs_id_;
	index=index_;
	RaidStartNotify msg(gs_id,map_id,raidroom_id);
	TeamBroadcast(team1,msg,msg.localsid,msg.roleid);
	SetStatus(RAID_SENDSTART_NOTIFY);
	for(TeamType::iterator it=team1.begin();it!=team1.end();++it)
	{
		role_need_auto_enter.insert(it->roleid);
		role_need_enter_re.insert(it->roleid);
	}
	return ERR_SUCCESS;
}
void RaidBasic_Info::TeamBroadcast(TeamType& team,Protocol & data, unsigned int & localsid, int & t_roleid)
{
	GDeliveryServer * dsm = GDeliveryServer::GetInstance();
	for (TeamType::const_iterator it = team.begin(); it != team.end(); ++it)
	{
		int roleid = it->roleid;
		if(IsTeamRaid() || IsCrssvrTeamsRaid()) // 跨服战队6v6
		{
			CrossBattleManager *manager = CrossBattleManager::GetInstance();
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if ( NULL == pinfo )// 跨服开启但是报名者不在跨服，通知原服
			{
				int src_zoneid = manager->GetSrcZoneID(roleid);
				//LOG_TRACE("RaidBasic_Info::TeamBroadcast IsCrssvrTeams roleid=%d, src_zoneid=%d", roleid, src_zoneid);

				if(src_zoneid != 0)
				{
					localsid = 0;
					t_roleid = roleid;
					CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, data);
				}
				
			}
			else if(pinfo != NULL && pinfo->ingame) // 在跨服
			{
				//LOG_TRACE("RaidBasic_Info::TeamBroadcast IsCrssvrTeams send to client, roleid=%d, localsid=%d", roleid,  pinfo->localsid);
				localsid = pinfo->localsid;
				t_roleid = roleid;
				dsm->Send(pinfo->linksid, data);
			}
			else
			{
				LOG_TRACE("RaidBasic_Info::TeamBroadcast IsCrssvrTeams roleid=%d is not in game", roleid);
			}
		}
		else
		{
			LOG_TRACE("RaidBasic_Info::TeamBroadcast roleid=%d", roleid);
			localsid = it->localsid;
			t_roleid = roleid;
			dsm->Send(it->linksid, data); 
		}
	}
}
int RaidBasic_Info::KickNotifyAll(int reason)
{
	int troleid;
	RaidKickNotify msg(raidroom_id,reason);
	TeamBroadcast(team1,msg,msg.localsid,troleid);
	return 0;
}
int RaidRoom_Info::KickNotifyAll(int reason)
{
	int troleid;
	RaidKickNotify msg(raidroom_id,reason);
	TeamBroadcast(team1,msg,msg.localsid,troleid);
	TeamBroadcast(team1_apply,msg,msg.localsid,troleid);
	return 0;
}
void RaidBasic_Info::AutoEnter()
{
	LOG_TRACE("RaidEnter auto raidroom_id=%d",raidroom_id);
	for(RoleSet::iterator it=role_need_auto_enter.begin();it!=role_need_auto_enter.end();++it)
		RaidEnter(*it,true);
	role_need_auto_enter.clear();
	SetStatus(RAID_FIGHTING);
}
int RaidBasic_Info::RaidEnter(int roleid,bool autoenter)
{
	LOG_TRACE("RaidBasic_Info::RaidEnter raidroom_id=%d roleid=%d",raidroom_id,roleid);
	if(FindRole(team1,roleid)==team1.end())
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	if(autoenter == false)
		role_need_auto_enter.erase(roleid);
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if(pinfo)
	{
		if(!IsRoomRaid())
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,SendRaidEnter(roleid,map_id,map_info.GetTemplateId(),index,map_info.RaidType(),Octets(0)));
		else
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,SendRaidEnter(roleid,map_id,map_info.GetTemplateId(),index,map_info.RaidType(),raid_data));
	}
	else
	{
		return ERR_RAID_ROLE_OFFLINE;
	}
	return ERR_SUCCESS; 
}

int TeamRaid_Info::RaidEnter(int roleid,bool autoenter)
{
	//LOG_TRACE("TeamRaid_Info::RaidEnter raidroom_id=%d roleid=%d",raidroom_id,roleid);
	if(FindRole(team1,roleid)==team1.end())
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	if(autoenter == false)
		role_need_auto_enter.erase(roleid);
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if(pinfo)
	{
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,SendRaidEnter(roleid,map_id,map_info.GetTemplateId(),index,map_info.RaidType(),Octets(0)));
	}
	
	if(!pinfo)
	{
		if(IsCrssvrTeamsRaid() || IsTeamRaid())// 可能是跨服
		{
			CrossBattleManager *manager = CrossBattleManager::GetInstance();
			int src_zoneid = manager->GetSrcZoneID(roleid);
			LOG_TRACE("TeamRaid_Info::RaidEnter roleid=%d, src_zoneid=%d, gs_id=%d", roleid, src_zoneid, gs_id);

			if(src_zoneid != 0)
			{
				manager->InsertFightingTag(gs_id, roleid, 0, 0, 0, CRSSVRTEAMS_BATTLE);
				//manager->EraseCrossBattle(roleid, CRSSVRTEAMS_BATTLE);
				CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, CrossRaidEnter_Re(ERR_SUCCESS, 0, roleid, gs_id, map_id,map_info.GetTemplateId(),index,map_info.RaidType()));
				
				return ERR_SUCCESS;		
			}
			return ERR_RAID_ROLE_OFFLINE;
		}
		else
		{
			return ERR_RAID_ROLE_OFFLINE;
		}
	}
	return ERR_SUCCESS; 
}

int RaidBasic_Info::ReEnter(int roleid,int& gs_id_,int& index_)
{
	LOG_TRACE("RaidBasic_Info::ReEnter raidroom_id=%d roleid=%d index=%d gsid=%d",raidroom_id,roleid,index,gs_id);
	if(!ISFighting())
		return ERR_RAID_STATUS;
        if(FindRole(team1,roleid)==team1.end())
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	gs_id_ = gs_id;
	index_ = index;
	return ERR_SUCCESS; 
}
int RaidBasic_Info::SetReenterRole(int roleid, const PlayerInfo& role)
{
	TeamType::iterator it = FindRole(team1,roleid);
	if(it == team1.end())
	{
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	}
	else
	{
		it->localsid = role.localsid;
		it->linksid = role.linksid;
	}
	return ERR_SUCCESS; 
}
/*int RaidBasic_Info::LeaveRoom(int roleid, unsigned char team)
{
	if(status == RAID_SENDSTART_NOTIFY)
		role_need_auto_enter.erase(roleid);
	return ERR_SUCCESS;
}*/
int RaidBasic_Info::SetStatus(int status_)
{
	if(CheckStateChange(status_)!= CHANGE_PERMIT)
	{
		LOG_TRACE("state change not permit, from %d to %d, room %d map %d",
				status, status_, raidroom_id, map_id);
		return ERR_RAID_STATUS_CHANGE;
	}
	status=status_;
	SetStatusEndTime(status);
	return ERR_SUCCESS;
}
void FactionPkRaid_Info::SetStatusEndTime(int status_)
{
	int now = Timer::GetTime();
	RaidManager* man=RaidManager::GetInstance();
	LOG_TRACE("FactionPkRaid_Info Raid SetStatusEndTime now=%d, status=%d",now, status_);
	switch (status_)
	{
		case RAID_SENDSTART:
			status_end_time = now+SWITCH_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		case RAID_FIGHTING:
			status_end_time	= NO_TIMEOUT;
			break;
		case RAID_ENDING:
			status_end_time	= NO_TIMEOUT;
			break;
		case RAID_WAIT_INVITE_RE:
			status_end_time = now+INVITE_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		default:
			LOG_TRACE("Raid SetStatusEndTime now=%d, status=%d undefined status",now, status_);
			status_end_time = now;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
	}
}
void TeamRaid_Info::SetStatusEndTime(int status_)
{
	int now = Timer::GetTime();
	RaidManager* man=RaidManager::GetInstance();
	LOG_TRACE("TeamRaid_Info Raid SetStatusEndTime now=%d, status=%d",now, status_);
	switch (status_)
	{
		case RAID_SENDSTART:
			status_end_time = now+SWITCH_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		case RAID_FIGHTING:
			status_end_time	= NO_TIMEOUT;
			break;
	/*	case RAID_SENDSTART_NOTIFY:
			status_end_time = now+ENTER_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;*/
		case RAID_WAITACK:
			status_end_time = now+ACK_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		case RAID_STARTFAIL:
			status_end_time = now;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
	/*
		case RAID_WAITCHOOSECHAR:
			status_end_time = now+CHOOSECHAR_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
			*/
		default:
			LOG_TRACE("Raid SetStatusEndTime now=%d, status=%d undefined status",now, status_);
			status_end_time = now;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
	}
}

void HideSeekRaid_Info::SetStatusEndTime(int status_)
{
	int now = Timer::GetTime();
	RaidManager* man=RaidManager::GetInstance();
	//LOG_TRACE("HideSeekRaid_Info Raid SetStatusEndTime now=%d, status=%d",now, status_);
	switch (status_)
	{
		case RAID_SENDSTART:
			status_end_time = now+SWITCH_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		case RAID_FIGHTING:
			status_end_time	= NO_TIMEOUT;
			break;
		case RAID_WAITACK:
			status_end_time = now+ACK_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		case RAID_STARTFAIL:
			status_end_time = now;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		default:
			LOG_TRACE("HideSeekRaid_Info SetStatusEndTime now=%d, status=%d undefined status",now, status_);
			status_end_time = now;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
	}
}

void RaidBasic_Info::SetStatusEndTime(int status_)
{
	int now = Timer::GetTime();
	RaidManager* man=RaidManager::GetInstance();
	//LOG_TRACE("RaidBasic_Info Raid SetStatusEndTime now=%d, status=%d",now, status_);
	switch (status_)
	{
		case RAID_INIT:
			status_end_time	= NO_TIMEOUT;
			break;
		default:
			LOG_TRACE("Raid SetStatusEndTime now=%d, status=%d undefined status",now, status_);
			status_end_time = now;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
	}
}
void RaidRoom_Info::SetStatusEndTime(int status_)
{
	int now = Timer::GetTime();
	RaidManager* man=RaidManager::GetInstance();
	LOG_TRACE("RaidRoom_Info Raid SetStatusEndTime now=%d, status=%d",now, status_);
	switch (status_)
	{
		case RAID_QUEUEING:
			status_end_time = now+map_info.GetQueuingTime();
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		case RAID_SENDSTART:
			status_end_time = now+SWITCH_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		case RAID_FIGHTING:
			status_end_time	= NO_TIMEOUT;
			break;
		case RAID_SENDSTART_NOTIFY:
			status_end_time = now+ENTER_TIMEOUT;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
		default:
			LOG_TRACE("Raid SetStatusEndTime now=%d, status=%d undefined status",now, status_);
			status_end_time = now;
			man->AddRoomEvent(raidroom_id,status_end_time);
			break;
	}
}
bool FactionPkRaid_Info::CheckStateChange(int target_state)
{
	static int init=0;
	static int matrix[RAID_MAX][RAID_MAX];
	
	if(init == 0)
	{
	memset(matrix,CHANGE_FORBID,sizeof(matrix));
	//status change,[from]->[to]
	matrix[RAID_INIT][RAID_WAIT_INVITE_RE]=CHANGE_PERMIT;
	matrix[RAID_WAIT_INVITE_RE][RAID_SENDSTART]=CHANGE_PERMIT;
	matrix[RAID_SENDSTART][RAID_FIGHTING]=CHANGE_PERMIT;
	matrix[RAID_FIGHTING][RAID_ENDING]=CHANGE_PERMIT;
	init=1;
	}
        int ret=matrix[status][target_state];	
	return ret;
}
bool TeamRaid_Info::CheckStateChange(int target_state)
{
	static int init=0;
	static int matrix[RAID_MAX][RAID_MAX];
	
	if(init == 0)
	{
	memset(matrix,CHANGE_FORBID,sizeof(matrix));
	//status change,[from]->[to]
	matrix[RAID_INIT][RAID_WAITACK]=CHANGE_PERMIT;
//	matrix[RAID_WAITACK][RAID_WAITCHOOSECHAR]=CHANGE_PERMIT;
//	matrix[RAID_WAITCHOOSECHAR][RAID_SENDSTART]=CHANGE_PERMIT;
//	matrix[RAID_SENDSTART][RAID_SENDSTART_NOTIFY]=CHANGE_PERMIT;
//	matrix[RAID_SENDSTART_NOTIFY][RAID_FIGHTING]=CHANGE_PERMIT;
	matrix[RAID_WAITACK][RAID_SENDSTART]=CHANGE_PERMIT;
	matrix[RAID_WAITACK][RAID_STARTFAIL]=CHANGE_PERMIT;
	matrix[RAID_SENDSTART][RAID_STARTFAIL]=CHANGE_PERMIT;
	matrix[RAID_SENDSTART][RAID_FIGHTING]=CHANGE_PERMIT;
	init=1;
	}
        int ret=matrix[status][target_state];	
	return ret;
}
bool HideSeekRaid_Info::CheckStateChange(int target_state)
{
	static int init=0;
	static int matrix[RAID_MAX][RAID_MAX];
	
	if(init == 0)
	{
		memset(matrix,CHANGE_FORBID,sizeof(matrix));
		//status change,[from]->[to]
		matrix[RAID_INIT][RAID_WAITACK]=CHANGE_PERMIT;
		matrix[RAID_WAITACK][RAID_SENDSTART]=CHANGE_PERMIT;
		matrix[RAID_WAITACK][RAID_STARTFAIL]=CHANGE_PERMIT;
		matrix[RAID_SENDSTART][RAID_STARTFAIL]=CHANGE_PERMIT;
		matrix[RAID_SENDSTART][RAID_FIGHTING]=CHANGE_PERMIT;
		init=1;
	}
        int ret=matrix[status][target_state];	
	return ret;
}

bool RaidRoom_Info::CheckStateChange(int target_state)
{
	static int init=0;
	static int matrix[RAID_MAX][RAID_MAX];
	
	if(init == 0)
	{
	memset(matrix,CHANGE_FORBID,sizeof(matrix));
	//status change,[from]->[to]
	matrix[RAID_INIT][RAID_QUEUEING]=CHANGE_PERMIT;
	matrix[RAID_QUEUEING][RAID_SENDSTART]=CHANGE_PERMIT;
	matrix[RAID_SENDSTART][RAID_SENDSTART_NOTIFY]=CHANGE_PERMIT;
	matrix[RAID_SENDSTART_NOTIFY][RAID_FIGHTING]=CHANGE_PERMIT;
	init=1;
	}
        int ret=matrix[status][target_state];	
	return ret;
}
int RaidBasic_Info::OnGsRaidEnter(int roleid,int gs_now)
{
	LOG_TRACE("RaidBasic_Info::OnGsRaidEnter raidroom_id=%d roleid=%d gsnow=%d",raidroom_id,roleid,gs_now);
	TeamType::iterator it = FindRole(team1,roleid);
	if(it==team1.end())
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	if(status!=RAID_FIGHTING && status!=RAID_SENDSTART_NOTIFY)
		return ERR_RAID_STATUS;
//	role_need_enter_re.erase(roleid);
	Log::formatlog("raid_stat","OnGsRaidEnter raidroom_id=%d gs_id=%d roleid=%d level=%d",raidroom_id,gs_id,roleid,it->level);
	int reason = 0;
	if(IsTeamRaid() || IsCrssvrTeamsRaid())
	{
                reason = CHG_GS_REASON_COLLISION_RAID;	
	}

	// 跨服-跨服
	GProviderServer::GetInstance()->DispatchProtocol(gs_now,PlayerChangeGS(roleid,it->localsid,gs_id,index,reason,0,0,0));
	return ERR_SUCCESS; 
}
int RaidBasic_Info::OnGsRaidEnterSuccess(int roleid,int gs_now,int index_)
{
	LOG_TRACE("RaidBasic_Info::OnGsRaidEnterSuccess raidroom_id=%d roleid=%d gsnow=%d mapid=%d",raidroom_id,roleid,gs_now,map_id);
	if(index_!=index || gs_now!= gs_id)
	{
		Log::log(LOG_ERR,"RaidBasic_Info::OnGsRaidEnterSuccess index_=%d gs_now=%d index=%d gs_id=%d unequal",index_,gs_now,index,gs_id);
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	}
	TeamType::iterator it = FindRole(team1,roleid);
	if(it==team1.end())
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	if(status!=RAID_FIGHTING && status!=RAID_SENDSTART_NOTIFY)
		return ERR_RAID_STATUS;
	role_need_enter_re.erase(roleid);
	return ERR_SUCCESS; 
}
int RaidRoom_Info::OnGsRaidEnterSuccess(int roleid,int gs_now,int index_)
{
	int retcode = RaidBasic_Info::OnGsRaidEnterSuccess(roleid,gs_now,index_);
	if(retcode == ERR_SUCCESS)
	{
		TeamType::iterator it = FindRole(team1,roleid);
		if(it!=team1.end() && roleid == leader)
			GDeliveryServer::GetInstance()->Send(it->linksid,RaidBriefInfo(leader,raidroom_id,it->localsid));
	}
	return retcode;
}
int TeamRaid_Info::OnGsRaidEnterSuccess(int roleid,int gs_now,int index_)
{
	int retcode = RaidBasic_Info::OnGsRaidEnterSuccess(roleid,gs_now,index_);
	/*
	if(retcode == ERR_SUCCESS)
		RaidManager::GetInstance()->SetTeamApplyCD(roleid);
		*/
	return retcode;
}
bool FactionPkRaid_Info::GetRoomBrief(GFactionPkRoomInfo& info,int mapid)
{
	if(status != RAID_FIGHTING || mapid != map_id)
		return false;
	info.attacker_level = FactionManager::Instance()->GetLevel(attacker);
	info.defender_level = FactionManager::Instance()->GetLevel(defender);
	if(info.defender_level <1 || info.defender_level<1)
	{
		LOG_TRACE("FactionPkRaid_Info::GetRoomBrief att_level=%d def_level=%d",info.attacker_level,info.defender_level);
	}
	info.raidroom_id = raidroom_id;
	info.attacker_num = group_size[RAIDTEAM_ATTACK];
	info.defender_num = group_size[RAIDTEAM_DEFEND];
	info.watcher_num = group_size[RAIDTEAM_LOOKER];
	info.attacker = attacker;
	info.defender = defender;
	return true;
}
bool RaidRoom_Info::GetRoomBrief(GRaidRoomInfo& info)
{
	if(status != RAID_QUEUEING)
		return false;
	info.raidroom_id = raidroom_id;
	info.map_id = map_id;
	info.player_number = team1.size();
	info.apply_endtime = status_end_time;
	info.player_limit = map_info.GetPlayerLimit();
	if(team1.size())info.leadername = team1[0].name;
	info.leaderid = leader;
	info.leader_line = leader_line;
	info.roomname = roomname;
	info.can_vote = permit_vote;
	info.difficulty = raid_difficulty;
	return true;
}
int RaidRoom_Info::GetRoomInfo(int roleid, RaidGetRoom_Re & re)
{
	LOG_TRACE("RaidBasic_Info::GetRoomInfo roleid=%d ",roleid);
	TeamType::iterator it, ite= team1.end();
	for(it=team1.begin();it!=ite;++it)
		re.group1_player.push_back(*it);
	for(it=team1_apply.begin(),ite=team1_apply.end();it!=ite;++it)
		re.group1_applyer.push_back(*it);
	re.raidroom_id = raidroom_id;
	re.map_id = map_id;
	return ERR_SUCCESS; 
}	
void TeamRaid_Info::Init(RaidTeamBase * teama, RaidTeamBase * teamb)
{
	//LOG_TRACE("TeamRaid_Info::Init");
	//LOG_TRACE("TeamRaid_Info::Init, teama id=%d score=%d, teamb id=%d score=%d", teama->GetId(), teama->GetScore(), teamb->GetId(), teamb->GetScore());

	TeamRaidMappingSuccess msg(raidroom_id);

	if(IsCrssvrTeamsRaid())
	{
		msg.iscross = true;
		LOG_TRACE("TeamRaid_Info::Init IsCrssvrTeamsRaid, teamid_a=%d,size_a=%d,score_a=%d,zoneid_a=%d, teamid_b=%d,size_b=%d,score_b=%d,zoneid_b=%d", 
				teama->GetTeamId(),teama->GetSize(),teama->GetTeamScore(),teama->GetSrcZoneId(), teamb->GetTeamId(),teamb->GetSize(),teamb->GetTeamScore(),teamb->GetSrcZoneId());
		CrossCrssvrTeamsManager::Instance()->GetCrssvrTeamsData(teama->GetTeamId(), teama->GetRoleList(), msg.teamdata_a, msg.teamdata_a_detail);
		CrossCrssvrTeamsManager::Instance()->GetCrssvrTeamsData(teamb->GetTeamId(), teamb->GetRoleList(), msg.teamdata_b, msg.teamdata_b_detail);
		//LOG_TRACE("TeamRaid_Info::Init, IsCrssvrTeamsRaid, a_role.size=%d, msg.teamdata_a_detail=%d, b_role.size=%d, msg.teamdata_b_detail=%d,teamaid=%d,teambid=%d", 
		//		msg.teamdata_a.roles.size(), msg.teamdata_a_detail.size(), msg.teamdata_b.roles.size(), msg.teamdata_b_detail.size(), msg.teamdata_a.info.teamid, msg.teamdata_b.info.teamid);
	}

	TeamBroadcast(team1,msg,msg.localsid,msg.roleid);

	TeamType::iterator it, ite= team1.end();
	for(it=team1.begin();it!=ite;++it)
		role_need_mapping_re.insert(it->roleid);
	//SetStatus(RAID_WAITACK);
}	
int FactionPkRaid_Info::PkInvite_Re_Timeout()
{
	PlayerInfo * pinviter = UserContainer::GetInstance().FindRoleOnline(inviter_roleid);
	if(NULL != pinviter)
	{
		FactionPkRaidChallenge_Re re(ERR_RAID_FACTION_CHALLENGE_TIMEOUT, defender, defrole_name, pinviter->localsid);
	//	FactionPkRaidChallenge_Re re(ERR_RAID_FACTION_CHALLENGE_TIMEOUT, defender, Octets(0), pinviter->localsid);
		GDeliveryServer::GetInstance()->Send(pinviter->linksid, re);
	}
	BroadCastMsg(MSG_COMBATTIMEOUT);
	return 0;
}
int FactionPkRaid_Info::Update(int now)
{
	LOG_TRACE("FactionPkRaid_Info::Update roomid=%d now=%d status=%d status_end_time=%d",raidroom_id,now,status,status_end_time);
	if(status == RAID_FIGHTING || status == RAID_ENDING)
		return 0;
	if(team1.size()==0)
	{
		;//team1 size can be 0
	}
	if(now < status_end_time)
		return 0;
	else
	{
		switch (status)
		{
			case RAID_SENDSTART:
				//map_info.Apply_Instance_Finish(gs_id,false); //deal in RoomClose
				return -1;//gs start timeout
				break;
			case RAID_WAIT_INVITE_RE:
				PkInvite_Re_Timeout();
				return -1;//defender resp		break;
			default:
				return -1;
		}
	}
	return -1;
}
int TeamRaid_Info::Update(int now)
{
	LOG_TRACE("TeamRaid_Info::Update roomid=%d now=%d status=%d status_end_time=%d",raidroom_id,now,status,status_end_time);
	if(status == RAID_FIGHTING)
		return 0;
	if(team1.size()==0)
	{
		Log::log(LOG_ERR,"TeamRaid_Info::Update no role in room,close. roomid=%d now=%d status=%d status_end_time=%d",raidroom_id,now,status,status_end_time);
		return -1;
	}
	if(now < status_end_time)
		return 0;
	else
	{
		switch (status)
		{
			case RAID_SENDSTART:
				//map_info.Apply_Instance_Finish(gs_id,false); //deal in RoomClose
				return -1;//gs start timeout
				break;
				/*
			case RAID_WAITACK:
				MappingSuccess_Re_Timeout();
				return -1;//client ack timeout
				break;
			case RAID_WAITCHOOSECHAR:
				return StartRaid();//waitchoosechar timeout
				break;
				*/
	/*		case RAID_SENDSTART_NOTIFY:
				AutoEnter();	//enter notify timeout
				return 0;
				break;*/
			case RAID_WAITACK:
				//return StartRaid();
				return MappingSuccess_Re_Timeout();
				break;
			case RAID_STARTFAIL:
				Log::log(LOG_ERR, "TeamRaid_info error status %d timeout, roomid=%d", status, raidroom_id);
				return -1;
				break;
			default:
				Log::log(LOG_ERR, "TeamRaid_info error status %d timeout, roomid=%d", status, raidroom_id);
				return -1;
		}
	}
	return -1;
}
int RaidRoom_Info::Update(int now)
{
	LOG_TRACE("RaidRoom_Info::Update roomid=%d now=%d status=%d status_end_time=%d",raidroom_id,now,status,status_end_time);
	if(status == RAID_FIGHTING)
		return 0;
	if(team1.size()==0)
	{
		Log::log(LOG_ERR,"RaidBasic_Info::Update no role in room,close. roomid=%d now=%d status=%d status_end_time=%d",raidroom_id,now,status,status_end_time);
		return -1;
	}
	if(now < status_end_time)
		return 0;
	else
	{
		switch (status)
		{
			case RAID_SENDSTART:
				//map_info.Apply_Instance_Finish(gs_id,false); //deal in RoomClose
				return -1;//gs start timeout
				break;
			case RAID_QUEUEING:
				return AutoStart();//applying timeout
				break;
			case RAID_SENDSTART_NOTIFY:
				AutoEnter();	//enter notify timeout
				return 0;
				break;
			default:
				return -1;
		}
	}
	return -1;
}

int HideSeekRaid_Info::Update(int now)
{
	LOG_TRACE("HideSeekRaid_Info::Update roomid=%d now=%d status=%d status_end_time=%d",raidroom_id,now,status,status_end_time);
	if(status == RAID_FIGHTING)
		return 0;
	if(team1.size()==0)
	{
		Log::log(LOG_ERR,"HideSeekRaid_Info::Update no role in room,close. roomid=%d now=%d status=%d status_end_time=%d",raidroom_id,now,status,status_end_time);
		return -1;
	}
	if(now < status_end_time)
		return 0;
	else
	{
		switch (status)
		{
			case RAID_SENDSTART:
				//map_info.Apply_Instance_Finish(gs_id,false); //deal in RoomClose
				return -1;//gs start timeout
				break;
			case RAID_WAITACK:
				return MappingSuccess_Re_Timeout();
				break;
			case RAID_STARTFAIL:
				Log::log(LOG_ERR, "HideSeekRaid_info error status %d timeout, roomid=%d", status, raidroom_id);
				return -1;
				break;
			default:
				Log::log(LOG_ERR, "HideSeekRaid_info error status %d timeout, roomid=%d", status, raidroom_id);
				return -1;
		}
	}
	return -1;
}

int RaidRoom_Info::Kick(int roleid, int target, int reason)
{
	LOG_TRACE("RaidBasic_Info::Kick roleid=%d target=%d",roleid,target);
	if(roleid != leader)
		return ERR_RAID_OPERATOR_NOT_MASTER;
	if(roleid == target)
		return ERR_RAID_ARGUMENT_ERR;
	if(status != RAID_QUEUEING)
		return ERR_RAID_STATUS;
	TeamType::iterator it = FindRole(team1,target);
	if (it!=team1.end())
	{	
		GDeliveryServer::GetInstance()->Send(it->linksid,RaidKickNotify(target, raidroom_id,reason,it->localsid));
		team1.erase(it);
		return ERR_SUCCESS; 
	}
	else 
		return ERR_RAID_ROLE_NOT_IN_ROOM;
}
int RaidBasic_Info::DelRole(int roleid,unsigned char teamid)
{
	LOG_TRACE("RaidBasic_Info::DelRole roleid=%d ",roleid);
	//dont delete raidroleevent in this function
//	bool exist=false;
	TeamType::iterator it = FindRole(team1,roleid);
	if (it!=team1.end())
	{	team1.erase(it);
		role_need_enter_re.erase(roleid);
		role_need_auto_enter.erase(roleid);
//		exist=true;
	}
//	if(exist==false)
//		return ERR_RAID_ROLE_NOT_IN_ROOM;
	return ERR_SUCCESS;
}
int FactionPkRaid_Info::DelRole(int roleid,unsigned char teamid)
{
	LOG_TRACE("FactionPkRaid_Info::DelRole roleid=%d ",roleid);
	RaidBasic_Info::DelRole(roleid,teamid);
	//dont delete raidroleevent in this function
	RoleGroup::iterator it = role_group.find(roleid);
	if(it!=role_group.end())
	{
		group_size[it->second]--;
		role_group.erase(it);
		return ERR_SUCCESS;
	}
	else
		return ERR_RAID_ROLE_NOT_IN_ROOM;
}
int TeamRaid_Info::DelRole(int roleid,unsigned char teamid)
{
	LOG_TRACE("TeamRaid_Info::DelRole roleid=%d ",roleid);
	RaidBasic_Info::DelRole(roleid,teamid);
	//dont delete raidroleevent in this function
	RoleVec& tdata = teamdata.roles;	
	RoleVec::iterator it,ite=tdata.end();
	for(it=tdata.begin();it!=ite;++it)
	{
		if(it->roleid == roleid)
		{
			tdata.erase(it);
			break;
		}
	}
	if ((teama) && teama->DelRole(roleid))
		;
	else if (teamb)
		teamb->DelRole(roleid);
//	role_need_mapping_re.erase(roleid); liuyue 20121124
	RaidManager::GetInstance()->SetTeamApplyCD(roleid);
	if(IsCrssvrTeamsRaid())
	{
		DelCrssvrTeamsApplying();
	}

	return ERR_SUCCESS;
}
/*
int TeamRaid_Info::TryLeave(int roleid, int reason)
{
	if (reason == 0)
		RaidManager::GetInstance()->ClearTeamApplyCD(roleid);
	return ERR_SUCCESS;
}
*/
int RaidRoom_Info::DelRole(int roleid,unsigned char teamid)
{
	LOG_TRACE("RaidRoom_Info::DelRole roleid=%d ",roleid);
	RaidBasic_Info::DelRole(roleid,teamid);
	TeamType::iterator it = FindRole(team1_apply,roleid);
	if (it!=team1_apply.end())
	{
		team1_apply.erase(it);
//		exist=true;
	}
	if(pvote)
	{
		pvote->ClearRole(roleid);
		GetVoteResult();
	}
	return ERR_SUCCESS;
}

int HideSeekRaid_Info::DelRole(int roleid, unsigned char teamid)
{
	//LOG_TRACE("HideSeekRaid_Info::DelRole roleid=%d ",roleid);
	RaidBasic_Info::DelRole(roleid,teamid);
	//dont delete raidroleevent in this function
	RaidManager::GetInstance()->SetTeamApplyCD(roleid);

	return ERR_SUCCESS;
}

int RaidRoom_Info::Appoint(int roleid, int target,bool auto_appoint)
{
	LOG_TRACE("RaidBasic_Info::Appoint roleid=%d target=%d auto=%d",roleid,target,auto_appoint);
	if(roleid != leader)
		return ERR_RAID_OPERATOR_NOT_MASTER;
	if(roleid == target)
		return ERR_RAID_ARGUMENT_ERR;
//	if(status != RAID_QUEUEING)
//		return ERR_RAID_STATUS;
	if(auto_appoint)
	{
		if(team1.size()>1)
		{
			RaidRole & r =team1[1];	
			if(!ISFighting())
			{
				int troleid;
				RaidAppointNotify msg(r.roleid,raidroom_id);
			        TeamBroadcast(team1,msg,msg.localsid,troleid);
			        TeamBroadcast(team1_apply,msg,msg.localsid,troleid);
			}
		/*	else
			{
				RaidBriefInfo msg(r.roleid,raidroom_id);
			        TeamBroadcast(team1,msg,msg.localsid);
			}*/
			//GDeliveryServer::GetInstance()->Send(r.linksid,RaidAppointNotify(r.roleid,raidroom_id,r.localsid));
			leader = r.roleid;
			team1.push_back(team1[0]);
			team1.erase(team1.begin());//move old leader node to team end
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(leader);
			if(pinfo)
				leader_line = pinfo->gameid;
			else
				leader_line = -1;
			return ERR_SUCCESS; 
		}
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	}
	else
	{
		TeamType::iterator it = FindRole(team1,target);
		if (it!=team1.end())
		{	
			int troleid;
			RaidAppointNotify msg(target,raidroom_id);
		        TeamBroadcast(team1,msg,msg.localsid,troleid);
		        TeamBroadcast(team1_apply,msg,msg.localsid,troleid);
//			GDeliveryServer::GetInstance()->Send(it->linksid,RaidAppointNotify(target,raidroom_id,it->localsid));
			leader = target;
			RaidRole r = *it;
			*it = team1[0];
			team1[0] =r;
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(leader);
			if(pinfo)
				leader_line = pinfo->gameid;
			else
				leader_line = -1;
			return ERR_SUCCESS; 
		}
		else 
			return ERR_RAID_ROLE_NOT_IN_ROOM;
	}
}
int RaidRoom_Info::UpdateRoomToMaster()
{
	if(status!=RAID_QUEUEING)
		return -1;
	if(team1.size()>0)
	{
		RaidGetRoom_Re re;
		GetRoomInfo(leader,re);
		RaidRole & masterrole = *team1.begin();
		re.retcode = ERR_SUCCESS;
		re.localsid = masterrole.localsid;
		re.map_id = map_id;
		GDeliveryServer::GetInstance()->Send(masterrole.linksid, re);
	}
	return 0;
}
int RaidRoom_Info::OnVote(int voted_roleid,int voter,bool agree)
{
	if(status!=RAID_FIGHTING)
	{
		return ERR_RAID_STATUS;
	}
	if(EnterNotPermit(voter))
	{
		return ERR_RAID_VOTER_NOT_IN_RAID;
	}
	if(pvote==NULL)
	{
		return ERR_RAID_NOT_VOTEING;
	}
	else
	{
		if(pvote->voted_roleid!=voted_roleid)
		{
			return ERR_RAID_VOTEDROLE_MISMATCH;
		}
		else
		{
			pvote->room_vote_map[voter]=agree;
			GetVoteResult();
			return ERR_SUCCESS;
		}
	}
}
int RaidRoom_Info::BeginVote(int voted_roleid,int begin_roleid)
{
	if(!PermitVote())
	{
		return ERR_RAID_ROOM_FORBID_VOTE;	
	}	
	if(begin_roleid != leader)
	{
		return ERR_RAID_OPERATOR_NOT_MASTER;
	}
	if(pvote)
	{
		return ERR_RAID_VOTEING; 
	}	
	int now=Timer::GetTime();
	if(now-lastvote_begin_time < VOTE_INTERVAL)
	{
		return ERR_RAID_VOTECOOLDOWN; 
	}
	if(!ISFighting())
	{
		return ERR_RAID_STATUS;
	}
	if(EnterNotPermit(voted_roleid))
	{
		return ERR_RAID_VOTEDROLE_NOT_IN_RAID;
	}

	pvote = new RaidVote(voted_roleid);
	lastvote_begin_time = now;
	RaidManager::GetInstance()->AddVoteEvent(raidroom_id,now+VOTE_TIMEOUT);
	RaidBeginVotes msg(voted_roleid);
	GDeliveryServer * dsm = GDeliveryServer::GetInstance();
	for (TeamType::const_iterator it = team1.begin(); it != team1.end(); ++it)
	{
		if(EnterNotPermit(it->roleid))
			continue;
		msg.localsid = it->localsid;
		dsm->Send(it->linksid, msg); 
	}

	return ERR_SUCCESS;
}
void RaidRoom_Info::VoteTimeoutResult()
{
	if(pvote==NULL)
		return;
	int totalvote=team1.size()-role_need_enter_re.size();
	if(totalvote==0)
	{
		delete pvote;
		pvote=NULL;
		return;
	}
	int approve,reject;
	RaidVote::RoomVoteMap::iterator it = pvote->room_vote_map.begin();
	RaidVote::RoomVoteMap::iterator ite = pvote->room_vote_map.end();
	for(approve=0,reject=0;it!=ite;++it)
	{
		if(it->second)
		{
			approve++;
		}
		else
		{
			reject++;
		}
	}
	RaidVoteResult msg(VOTE_FAIL,pvote->voted_roleid,approve,totalvote);
	if(approve>totalvote/2)
	{
		msg.result = VOTE_PASS;
		RaidManager::GetInstance()->AsyncLeaveRoom(pvote->voted_roleid,raidroom_id);
		GProviderServer::GetInstance()->DispatchProtocol(gs_id,SendRaidKickout(pvote->GetVotedRoleid(),index));
	}
	else  //send fail msg, even if no enough votes
	{
		msg.result = VOTE_FAIL;
	}
	GDeliveryServer * dsm = GDeliveryServer::GetInstance();
	for (TeamType::const_iterator it = team1.begin(); it != team1.end(); ++it)
	{
		if(EnterNotPermit(it->roleid))
			continue;
		msg.localsid = it->localsid;
		dsm->Send(it->linksid, msg); 
	}
	delete pvote;
	pvote=NULL;
}
void RaidRoom_Info::GetVoteResult()
{
	if(pvote==NULL)
		return;
	int totalvote=team1.size()-role_need_enter_re.size();
	if(totalvote==0)
	{
		delete pvote;
		pvote=NULL;
		RaidManager::GetInstance()->DelVoteEvent(raidroom_id,lastvote_begin_time+VOTE_TIMEOUT);
		return;
	}
	int approve,reject;
	RaidVote::RoomVoteMap::iterator it = pvote->room_vote_map.begin();
	RaidVote::RoomVoteMap::iterator ite = pvote->room_vote_map.end();
	for(approve=0,reject=0;it!=ite;++it)
	{
		if(it->second)
		{
			approve++;
		}
		else
		{
			reject++;
		}
	}
	if(approve>totalvote/2 || reject>totalvote/2)
	{
		RaidVoteResult msg(VOTE_FAIL,pvote->voted_roleid,approve,totalvote);
		if(approve>totalvote/2)
		{
			msg.result = VOTE_PASS;
			RaidManager::GetInstance()->AsyncLeaveRoom(pvote->voted_roleid,raidroom_id);
			GProviderServer::GetInstance()->DispatchProtocol(gs_id,SendRaidKickout(pvote->GetVotedRoleid(),index));
		}
		else
		{
			msg.result = VOTE_FAIL;
		}
		GDeliveryServer * dsm = GDeliveryServer::GetInstance();
		for (TeamType::const_iterator it = team1.begin(); it != team1.end(); ++it)
		{
			if(EnterNotPermit(it->roleid))
				continue;
			msg.localsid = it->localsid;
			dsm->Send(it->linksid, msg); 
		}
		delete pvote;
		pvote=NULL;
		RaidManager::GetInstance()->DelVoteEvent(raidroom_id,lastvote_begin_time+VOTE_TIMEOUT);
	}
}
/////////////////////////////////////////////////////////////////////////
int RaidManager::Appoint(int roleid, int raidroom_id, int target, bool auto_appoint)
{
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::Appoint getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
	if(!proom || !pbasic->IsRoomRaid())
	{
		LOG_TRACE("RaidManager::Appoint getroom type err");
		return ERR_RAID_TYPE;
	}
	int retcode =  proom->Appoint(roleid,target,auto_appoint);
	LOG_TRACE("RaidManager::Appoint roleid=%d target=%d auto=%d raidroom_id=%d retcode=%d",roleid,target,auto_appoint,raidroom_id,retcode);
	return retcode;
}
int RaidManager::Kick(int roleid, int raidroom_id, int target, int reason)
{
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::Kick getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
	if(!proom || !pbasic->IsRoomRaid())
	{
		LOG_TRACE("RaidManager::Kick getroom type err");
		return ERR_RAID_TYPE;
	}
	int retcode =  proom->Kick(roleid,target,reason);
	if(retcode == ERR_SUCCESS)
		ClearRole(target);
	LOG_TRACE("RaidManager::Kick roleid=%d target=%d retcode=%d",roleid,target,retcode);
	return retcode;
}

int RaidManager::DebugRaidEnd(int roleid, int result)
{
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
	{
		LOG_TRACE("RaidManager::DebugRaidEnd getroomid err");
		return ERR_RAID_INSTANCE_INEXIST;
	}

	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		LOG_TRACE("RaidManager::DebugRaidEnd getroom err roomid=%d",raidroom_id);
		return ERR_RAID_ROOM_INEXIST;
	}
	else
	{
		TeamRaid_Info* pteamroom = dynamic_cast<TeamRaid_Info*>(proom);
		if(pteamroom && proom->IsCrssvrTeamsRaid())
		{
			pteamroom->SendBattleResult(result);	
		}
	}
	return ERR_SUCCESS;
}
int RaidManager::OnRaidEnd(int gs_id,int index,int mapid, int result)
{
	int raidroom_id = GetIndexRoomId(gs_id,index);
	if (raidroom_id == -1)
	{
		LOG_TRACE("RaidManager::OnRaidEnd getroomid err");
		return ERR_RAID_INSTANCE_INEXIST;
	}

	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		LOG_TRACE("RaidManager::OnRaidEnd getroom err roomid=%d",raidroom_id);
		return ERR_RAID_ROOM_INEXIST;
	}
	else
	{
		if(proom->GetMapid()!=mapid)
			LOG_TRACE("RaidManager::OnRaidEnd mapid err mapid=%d roomid=%d",mapid,raidroom_id);

		TeamRaid_Info* pteamroom = dynamic_cast<TeamRaid_Info*>(proom);
		if(pteamroom && proom->IsCrssvrTeamsRaid())
		{
			pteamroom->SendBattleResult(result);	
			pteamroom->DelCrssvrTeamsApplying();
		}

		proom->GS_Close_Instance();
		delete proom;
		proom = NULL;
		raidroom_map.erase(raidroom_id);
	}
	return ERR_SUCCESS; 
}
int RaidManager::GSRoleLeave(int gs_id, int roleid)
{
	LOG_TRACE("RaidManager::GSRoleLeave gs_id=%d roleid=%d",gs_id,roleid);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return ERR_RAID_ROLE_NOT_IN_RAID;
	return LeaveRoom(roleid,raidroom_id,1);//use default team
}
/*
int RaidManager::TryLeaveRaid(int gs_id, int roleid, int reason)
{
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return ERR_RAID_ROLE_NOT_IN_RAID;
	return TryLeaveRoom(roleid,raidroom_id,reason);
}
*/
int RaidManager::OnGSEnterErr(int gs_id, int index, int roleid)
{
	LOG_TRACE("RaidManager::OnGSEnterErr gs_id=%d index=%d roleid=%d",gs_id,index,roleid);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return ERR_RAID_ROLE_NOT_IN_RAID;
	return LeaveRoom(roleid,raidroom_id,1);//use default team
}
bool RaidManager::GetMapid(int roleid,int & raid_mapid)
{
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return false;
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		LOG_TRACE("RaidManager::GetMapid getroom err roomid=%d roleid=%d",raidroom_id,roleid);
		return false;
	}
	else 
	{
		raid_mapid = proom->GetMapid();
		return true;
	}
}
int RaidManager::AsyncLeaveRoom(int roleid,int raidroom_id)
{
	async_leaving_role_list.push_back(AsyncRoleLeave(roleid,raidroom_id));
	return 0;
}
int RaidManager::LeaveRoom(int roleid, int raidroom_id, unsigned char team)
{
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	int retcode = ERR_SUCCESS;
	if(!proom)
	{
		LOG_TRACE("RaidManager::LeaveRoom getroom err");
		retcode = ERR_RAID_ROOM_INEXIST;
	}
	else
	{
		proom->Appoint(roleid,-1/*target roleid no use*/,true);
		retcode = proom->DelRole(roleid,team);
	}


	ClearRole(roleid);
	if(proom && proom->NeedClose())
	{
		delete proom;
		proom = NULL;
		raidroom_map.erase(raidroom_id);
	}
	else if(proom)
	{
		proom->UpdateRoomToMaster();
	}

	LOG_TRACE("RaidManager::LeaveRoom roleid=%d, raidroom_id=%d retcode=%d",roleid, raidroom_id,retcode);
	return retcode;
}
/*
int RaidManager::TryLeaveRoom(int roleid, int raidroom_id, int reason)
{
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		Log::log(LOG_ERR, "RaidManager::TryLeaveRoom getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	return proom->TryLeave(roleid,reason);
}
*/
int RaidManager::GetRoomInfo(int roleid, int raidroom_id, RaidGetRoom_Re & re)
{
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::GetRoomInfo getroom err raidroom_id=%d",raidroom_id);
		return ERR_RAID_ROOM_INEXIST;
	}
	RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
	if(!proom || !pbasic->IsRoomRaid())
	{
		LOG_TRACE("RaidManager::GetRoomInfo getroom type err");
		return ERR_RAID_TYPE;
	}
	return proom->GetRoomInfo(roleid,re);
}
int RaidManager::GetRoomList(int roleid, int map_id, std::vector<GRaidRoomInfo> & fields)
{
	//need expansion, if raid not just one map  
	RaidRoomMap::iterator it,ite=raidroom_map.end();
	GRaidRoomInfo info;
	for(it=raidroom_map.begin();it!=ite;++it)
	{
		RaidBasic_Info* pbasic=it->second;
		RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
		if(!proom || !pbasic->IsRoomRaid())//only for RoomRaid
		{
		//	LOG_TRACE("RaidManager::GetRoomList getroom type err");
		//	return ERR_RAID_TYPE;
			continue;
		}

		if(proom)
		{
			if(proom->GetRoomBrief(info) && info.map_id == map_id)
				fields.push_back(info);
		}
	}
	return 0;
}
void RaidManager::OnLogout(int roleid, int gs_id)
{
//	LOG_TRACE("RaidManager::OnLogout roleid=%d, gs_id=%d",roleid, gs_id);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return ;
	LeaveRoom(roleid,raidroom_id,1);//use default team
	DelOpeningRole(roleid);
}

ApplyingRole::ApplyingRole(const TeamRaidApplyingRole & role_, int map_id):role(role_)
{
	RaidManager::GetInstance()->AddTeamApplyingRole(role.roleid,map_id);
	g_applyrole_create++;
	//LOG_TRACE("teamraid applying role %d is created g_applyrole_create=%d", role.roleid, g_applyrole_create);
}

ApplyingRole::~ApplyingRole()
{
	RaidManager::GetInstance()->DelTeamApplyingRole(GetRoleid());
	g_applyrole_delete++;
	//LOG_TRACE("teamraid applying role %d is deleted g_applyrole_delete=%d", GetRoleid(), g_applyrole_delete);
}

CrssvrTeamsApplyingRole::CrssvrTeamsApplyingRole(const TeamRaidApplyingRole & role_, int map_id, int teamid):ApplyingRole(role_,map_id), teamid(teamid)
{
	RaidManager::GetInstance()->AddCrssvrTeamsApplying(teamid, role.roleid);
	//LOG_TRACE("CrssvrTeamsApplyingRole applying role=%d, teamid=%d", role.roleid, GetTeamId());
}

CrssvrTeamsApplyingRole::~CrssvrTeamsApplyingRole()
{
	RaidManager::GetInstance()->DelCrssvrTeamsApplying(GetTeamId(), GetRoleid());
	//LOG_TRACE("~CrssvrTeamsApplyingRole applying role=%d, teamid=%d", GetRoleid(), GetTeamId());
}

/*
RaidFullTeam::~RaidFullTeam()
{
	_HalfTeam::const_iterator it,ite;
	for(it=roleids.begin(),ite=roleids.end();it!=ite;it++)
		delete *it;
}
*/
WaitingTeamContainer::~WaitingTeamContainer()
{
	ClearAll();
}
void WaitingTeamContainer::ClearAll()
{
	for(SubTeamPool::iterator pit=subteampool.begin(),pite=subteampool.end();pit!=pite;++pit)
	{
		TeamList & tlist = pit->second;
		TeamList::iterator it1=tlist.begin(),ite1=tlist.end();
		for(;it1!=ite1;++it1)
		{
			TeamRoleList::const_iterator tit, tite = (*it1)->GetRoleList().end();
			for (tit = (*it1)->GetRoleList().begin(); tit!=tite; tit++)
			{
				PlayerInfo * pmember = UserContainer::GetInstance().FindRoleOnline((*tit)->GetRoleid());
				if (pmember)
					GDeliveryServer::GetInstance()->Send(pmember->linksid, TeamRaidApplyNotice((*tit)->GetRoleid(), 0, pmember->localsid));
			}
			(*it1)->DeleteRoleList();
			delete *it1;
		}
	}
	subteampool.clear();

	for(FullTeamList::iterator it=fullteamlist.begin();it!=fullteamlist.end();++it)
	{
		TeamRoleList::const_iterator tit, tite = (*it)->GetRoleList().end();
		for (tit = (*it)->GetRoleList().begin(); tit!=tite; tit++)
		{
			PlayerInfo * pmember = UserContainer::GetInstance().FindRoleOnline((*tit)->GetRoleid());
			if (pmember)
				GDeliveryServer::GetInstance()->Send(pmember->linksid, TeamRaidApplyNotice((*tit)->GetRoleid(), 0, pmember->localsid));
		}
		(*it)->DeleteRoleList();
		delete *it;
	}
	fullteamlist.clear();
	subteam_reverse_map.clear();
	fullteam_reverse_map.clear();
}

bool WaitingTeamContainer::IsJoinTime()
{
	if (raidmanager_debug_mode)
		return true;
	else
		return status == STATUS_OPEN; 
}

int RaidManager::TeamApplyingRoleQuit(int roleid)
{
	TeamApplyingRoleSet::iterator it = team_applying_role_set.find(roleid);
	if(it!=team_applying_role_set.end())
	{
		int map_id = it->second;

		LOG_TRACE("RaidManager::TeamApplyingRoleQuit roleid=%d, map_id=%d",roleid, map_id);
		RaidWaitingMap::iterator itm = raid_waiting_map.find(map_id);
		if(itm != raid_waiting_map.end())
		{
			std::map<int, WaitingTeamContainer*>::iterator wit, wite = itm->second.end();
			for (wit = itm->second.begin(); wit != wite; ++wit)
			{
				if(wit->second->RoleQuit(roleid)==0)
				{
					LOG_TRACE("RaidManager::TeamApplyingRoleQuit raid_waiting_map roleid=%d from type %d",
							roleid, wit->first);

					return ERR_SUCCESS;
				}
			}
		}
		else
		{
			//Log::log(LOG_ERR,"RaidManager::TeamApplyingRoleQuit map_id=%d map not found",map_id);
			//匹配成功之后 副本结束之前 角色不会出现在 WaitingTeamContainer 中 但有可能重新加到其中 所以不能直接 DelTeamApplyingRole
			//DelTeamApplyingRole(roleid);
			return ERR_SUCCESS;
		}
	}
	return ERR_RAID_NOT_APPLY_TEAM;
}

int RaidManager::TeamApplyingRoleQuit(int roleid, int teamid)
{
	TeamApplyingRoleSet::iterator it = team_applying_role_set.find(roleid);
	if(it!=team_applying_role_set.end())
	{
		int map_id = it->second;

		RaidWaitingMap::iterator itm = crssvrteams_waiting_map.find(map_id);
		if(itm != crssvrteams_waiting_map.end())
		{
			std::map<int, WaitingTeamContainer*>::iterator wit, wite = itm->second.end();
			for (wit = itm->second.begin(); wit != wite; ++wit)
			{
				if(wit->second->CrssvrTeamsQuit(teamid)==0)
				{
					LOG_TRACE("RaidManager::TeamApplyingRoleQuit crssvrteams_waiting_map roleid=%d, teamid=%d from type %d",
							roleid, teamid, wit->first);
					return ERR_SUCCESS;
				}
			}
		}
		else
		{
			//Log::log(LOG_ERR,"RaidManager::TeamApplyingRoleQuit map_id=%d map not found",map_id);
			//匹配成功之后 副本结束之前 角色不会出现在 WaitingTeamContainer 中 但有可能重新加到其中 所以不能直接 DelTeamApplyingRole
			//DelTeamApplyingRole(roleid);
			return ERR_SUCCESS;
		}
	}
	return ERR_RAID_NOT_APPLY_TEAM;

}

int RaidManager::HideSeekApplyingRoleQuit(int roleid)
{
	TeamApplyingRoleSet::iterator it = team_applying_role_set.find(roleid);
	if(it!=team_applying_role_set.end())
	{
		int map_id = it->second;

		LOG_TRACE("RaidManager::HideSeekApplyingRoleQuit roleid=%d, map_id=%d",roleid, map_id);
		RaidHideSeekWaitingMap::iterator itm = raid_hideseek_waiting_map.find(map_id);
		if(itm != raid_hideseek_waiting_map.end())
		{
			std::map<int, WaitingHideSeekContainer*>::iterator wit, wite = itm->second.end();
			for (wit = itm->second.begin(); wit != wite; ++wit)
			{
				if(wit->second->RoleQuit(roleid)==0)
				{
					LOG_TRACE("RaidManager::HideSeekApplyingRoleQuit raid_hideseek_waiting_map roleid=%d from type %d",
							roleid, wit->first);

					return ERR_SUCCESS;
				}
			}
		}
		else
		{
			//Log::log(LOG_ERR,"RaidManager::TeamApplyingRoleQuit map_id=%d map not found",map_id);
			return ERR_SUCCESS;
		}
	}
	else
	{
		LOG_TRACE("RaidManager::HideSeekApplyingRoleQuit not find roleid=%d in team_applying_role_set", roleid);
	}
	return ERR_RAID_NOT_APPLY_TEAM;
}

void RaidManager::TryAbnormalOffline(int roleid, int gs_id)
{
	TeamApplyingRoleQuit(roleid);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return; //如果玩家正常退出，将在此返回，因为已经从管理器中删除了
	LOG_TRACE("RaidManager::TryAbnormalOffline roleid=%d, gsid=%d",roleid, gs_id);
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(proom)
	{
		if( (!proom->ISFighting()) || proom->EnterNotPermit(roleid) || proom->IsFactionPkRaid())
		{
			LeaveRoom(roleid,raidroom_id,1);//use default team
		}
		else
		{
			int now = Timer::GetTime();
		//	AddRoleEvent(roleid,now+RAID_REENTER_LIMIT);
		//	RaidRoleMap::iterator it = raidrole_map.find(roleid);
		//	if(it!=raidrole_map.end())
		//		it->second.SetTimeout(now+RAID_REENTER_LIMIT);
			SetTimeout(roleid,now+proom->GetReenterLimit());
		}
	}
}
bool RaidManager::SetTimeout(int roleid,int t)
{
	RaidRoleMap::iterator it = raidrole_map.find(roleid);
	if(it==raidrole_map.end())
		return false;
	DelRoleEvent(roleid,it->second.GetTimeout());
	it->second.SetTimeout(t);
	AddRoleEvent(roleid,t);
	return true;
}
void RaidManager::SetReenterRole(int roleid, const PlayerInfo& role)
{
	LOG_TRACE("Raid SetReenterRole");
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return; 
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(proom)
		proom->SetReenterRole(roleid,role);
}
bool RaidManager::TryReenterRaid(int roleid, int& gs_id, int& world_tag)
{
	LOG_TRACE("RaidManager::TryReenterRaid roleid=%d",roleid);
	RaidRoleMap::iterator it = raidrole_map.find(roleid);
	if(it==raidrole_map.end())
		return false;
	if(it->second.CheckCountLimit()==false)
	{
		LOG_TRACE("Raid TryReenterRaid CheckCountLimit fail");
		DelRoleEvent(roleid,it->second.GetTimeout());
		LeaveRoom(roleid,it->second.GetRoomId(),1);//use default team
		return false;
	}
//	it->second.SetTimeout(Timer::GetTime()+RAID_REENTER_LIMIT);
//	SetTimeout(roleid,Timer::GetTime()+RAID_REENTER_LIMIT);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return false; 
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(proom)
	{
		SetTimeout(roleid,Timer::GetTime()+proom->GetReenterLimit());
		if(proom->ReEnter(roleid,gs_id,world_tag)==ERR_SUCCESS)
			return true;
	}
	return false;
}
int RaidManager::OnGsRaidEnter(int roleid, int gs_now)
{
	LOG_TRACE("RaidManager::OnGsRaidEnter roleid=%d, gs_now=%d",roleid, gs_now);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		LOG_TRACE("RaidManager::OnGsRaidEnter getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	else
	{
		return proom->OnGsRaidEnter(roleid,gs_now);
	}
}
int RaidManager::OnGsRaidEnterSuccess(int roleid, int gs_now, int index)
{
	LOG_TRACE("RaidManager::OnGsRaidEnterSuccess roleid=%d, gs_now=%d index=%d",roleid, gs_now, index);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		LOG_TRACE("RaidManager::OnGsRaidEnterSuccess getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	else
	{
		return proom->OnGsRaidEnterSuccess(roleid,gs_now,index);
	}
}
int RaidManager::RaidEnter(int roleid, int raidroom_id)
{
	LOG_TRACE(" RaidManager::RaidEnter roleid=%d raidroom_id=%d",roleid,raidroom_id);
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		LOG_TRACE("RaidManager::RaidEnter getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	else
	{
		return proom->RaidEnter(roleid,false);
	}
}
int RaidManager::OnGSRaidStart(int gs_id, int raidroom_id, int index, int retcode)
{
	LOG_TRACE(" RaidManager::OnGSRaidStart raidroom_id=%d gs_id=%d index=%d retcode=%d",raidroom_id,gs_id,index,retcode);
	RaidBasic_Info* proom=GetRoom(raidroom_id);
	if(!proom)
	{
		LOG_TRACE("RaidManager::OnGSRaidStart room has close roomid=%d" , raidroom_id);
	#ifndef DEBUG_RAID
		GProviderServer::GetInstance()->DispatchProtocol(gs_id,SendRaidClose(raidroom_id,0,index,0));
	#endif
		return ERR_RAID_ROOMCLOSE;
	}

	if(retcode != ERR_SUCCESS)
	{
		proom->OnGsStart(gs_id,index,false);
		delete proom;
		proom = NULL;
		raidroom_map.erase(raidroom_id);
		return ERR_RAID_GS_GET_INSTANCE_FAIL; 
	}
	else
	{
		if(AddInstance(gs_id,index,raidroom_id) == false)
		{
			Log::log(LOG_ERR,"AddInstance err. Dup raid index gsid=%d index=%d\n",gs_id,index);
			delete proom;
			proom = NULL;
			raidroom_map.erase(raidroom_id);
			return ERR_RAID_GS_GET_INSTANCE_FAIL;
		}

		int ret = proom->OnGsStart(gs_id,index,true);
		if(ret!= ERR_SUCCESS)
		{
			LOG_TRACE("RaidManager::OnGSRaidStart start fail ret=%d" , ret);
			delete proom;
			proom = NULL;
			raidroom_map.erase(raidroom_id);
			return ret;	
		}
		Log::formatlog("raid_stat","OnGSRaidStart raidroom_id=%d gs_id=%d index=%d",raidroom_id,gs_id,index);
	}
	return ERR_SUCCESS;
}

int RaidManager::RaidStart(int roleid, int raidroom_id, int raid_type, const Octets & raid_data)
{
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	int retcode;
	if(pbasic)
	{
		RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
		if(!proom || !pbasic->IsRoomRaid())
		{
			LOG_TRACE("RaidManager::RaidStart getroom type err");
			return ERR_RAID_TYPE;
		}
		proom->SetRaidData(raid_data);
		retcode = proom->Start(roleid);
		if(retcode == ERR_RAID_GET_INSTANCE_FAIL)
		{
			delete proom;
			proom = NULL;
			raidroom_map.erase(raidroom_id);
		}
	}
	else
	{
		retcode = ERR_RAID_ROOM_INEXIST;
		LOG_TRACE("RaidManager::RaidStart getroom err");
	}
	LOG_TRACE("RaidManager::RaidStart roleid=%d raidroom_id=%d retcode=%d raid_data size=%d",roleid,raidroom_id,retcode,raid_data.size());
	return retcode;
}
int RaidManager::OnAccept(int masterid, unsigned char accept, int raidroom_id, int target)
{
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	int retcode;
	if(pbasic)
	{
		RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
		if(!proom || !pbasic->IsRoomRaid())
		{
			LOG_TRACE("RaidManager::OnAccept getroom type err");
			return ERR_RAID_TYPE;
		}
		
		retcode = proom->OnAccept(masterid,accept,target);
		if(accept==0 && retcode == ERR_SUCCESS)//target refused
			ClearRole(target);
	}
	else
	{
		retcode = ERR_RAID_ROOM_INEXIST;
		LOG_TRACE("RaidManager::OnAccept getroom err");
	}
	LOG_TRACE("RaidManager::OnAccept masterid=%d raidroom_id=%d accept=%d target=%d retcode=%d",masterid,raidroom_id,accept,target,retcode);
	return retcode;
}
int RaidManager::OnTeamRaidMappingSuccess_Re(int roleid, int raidroom_id, char agree)
{
	//LOG_TRACE("RaidManager::OnTeamRaidMappingSuccess_Re roleid=%d raidroom_id=%d agree=%d",roleid,raidroom_id,agree);
	int retcode = ERR_SUCCESS;
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		retcode = ERR_RAID_ROOM_INEXIST;
		return retcode;
	}
	TeamRaid_Info* proom = dynamic_cast<TeamRaid_Info*>(pbasic);
	if(!proom || (!pbasic->IsTeamRaid() && !pbasic->IsCrssvrTeamsRaid()))
	{
		LOG_TRACE("RaidManager::OnTeamRaidMappingSuccess_Re getroom type err");
		retcode = ERR_RAID_TYPE;
	}
	else
	{
		if (agree == 0 && !pbasic->IsCrssvrTeamsRaid())
		{
			//同LeaveRoom逻辑
			proom->DelRole(roleid, 1);
			ClearRole(roleid);
		}
		retcode = proom->OnMappingSuccess_Re(roleid, agree);
	}
	return retcode;
}
int RaidManager::OnHideSeekRaidMappingSuccess_Re(int roleid, int raidroom_id, char agree)
{
	//LOG_TRACE("RaidManager::OnHideSeekRaidMappingSuccess_Re roleid=%d raidroom_id=%d agree=%d",roleid,raidroom_id,agree);
	int retcode = ERR_SUCCESS;
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		retcode = ERR_RAID_ROOM_INEXIST;
		return retcode;
	}
	HideSeekRaid_Info* proom = dynamic_cast<HideSeekRaid_Info*>(pbasic);
	if(!proom)
	{
		LOG_TRACE("RaidManager::OnHideSeekRaidMappingSuccess_Re getroom type err");
		retcode = ERR_RAID_TYPE;
	}
	else
	{
		retcode = proom->OnMappingSuccess_Re(roleid, agree);
	}
	return retcode;
}

int FactionPkRaid_Info::SetRoleGroup(int roleid,char& groupid)
{
	RoleGroup::iterator it = role_group.find(roleid);
	if(it!=role_group.end())
	{
		groupid = static_cast<char>(it->second);
		return ERR_SUCCESS;
	}
	else
		return ERR_RAID_ROLE_NOT_IN_ROOM;
}

int RaidManager::SetRoleGroup(int roleid,char& groupid)
{
	LOG_TRACE("RaidManager::SetRoleGroup roleid=%d",roleid);
	int raidroom_id = GetRoleRoomId(roleid);
	if (raidroom_id == -1)
		return ERR_RAID_ROLE_NOT_IN_RAID;
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::SetRoleGroup getroom err raidroom_id=%d",raidroom_id);
		return ERR_RAID_ROOM_INEXIST;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::SetRoleGroup getroom type err");
		return ERR_RAID_TYPE;
	}
	return proom->SetRoleGroup(roleid,groupid);
}
int RaidManager::OnGRaidJoin(int roleid, int map_id, int raidroom_id, int groupid)
{
	LOG_TRACE("RaidManager::OnGRaidJoin roleid=%d raidroom_id=%d",roleid,raidroom_id);
	RaidRole role;
	if(GetRoleInfo(roleid,role))
	{
		//role offline
		return ERR_RAID_ROLE_OFFLINE;
	}
	int retcode = ERR_SUCCESS;
/*	RaidRoleMap::iterator roleit = raidrole_map.find(roleid);
	if(roleit != raidrole_map.end())
	{
		LOG_TRACE("JoinRaid err, already in raid roleid=%d roomid=%d",roleit->first,roleit->second.GetRoomId());
		retcode = ERR_RAID_JOIN_ALREADY;
	}
	OpeningRoomRoleSet::iterator openit = opening_room_role_set.find(roleid);
	if(openit != opening_room_role_set.end())
		retcode = ERR_RAID_IS_OPENING;*/
	retcode=CheckExistRole(roleid);
	if(retcode!=ERR_SUCCESS)
		return retcode;

	RaidMap::iterator it = raidmap.find(map_id);
	if (it == raidmap.end())
		retcode = ERR_RAID_MAP_NOTEXIST;
	
	if(retcode == ERR_SUCCESS)
	{
		RaidBasic_Info* pbasic=GetRoom(raidroom_id);
		if(!pbasic)
		{
			retcode = ERR_RAID_ROOM_INEXIST;
			GDeliveryServer::GetInstance()->Send(role.linksid,RaidJoinApplyList(retcode,raidroom_id,role.localsid));
			return retcode;
		}
		switch(it->second->RaidType())
		{
		case ROOM_RAID:
		case LEVEL_RAID:
		case STEP_RAID:
		case TRANSFORM_RAID:
		{	
			RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
			if(!proom || !pbasic->IsRoomRaid())
			{
				LOG_TRACE("RaidManager::OnGRaidJoin getroomraid type err");
				retcode = ERR_RAID_TYPE;
			}
			else
			{	
				retcode = proom->JoinRaid(role,map_id);
			}

			if(retcode == ERR_SUCCESS)
			{
				if(AddRoleInfo(roleid,raidroom_id,ROLE_NOTIMEOUT)==false)
				{
					Log::log( LOG_ERR, "JoinRaid AddRoleInfo err. roleid=%d,roomid=%d\n", roleid,raidroom_id);
					proom->DelRole(roleid,0);
					retcode = ERR_RAID_JOIN_ALREADY;
				}
			}
			GDeliveryServer::GetInstance()->Send(role.linksid,RaidJoinApplyList(retcode,raidroom_id,role.localsid));
			break;
		}
		case FACTION_PK_RAID:
		{
			FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
			if(!proom || !pbasic->IsFactionPkRaid())
			{
				LOG_TRACE("RaidManager::OnGRaidJoin getfactionraid type err");
				retcode = ERR_RAID_TYPE;
			}
			else
			{	
				retcode = proom->FactionPkJoin(role,map_id,groupid);
			}

			if(retcode == ERR_SUCCESS)
			{
				if(AddRoleInfo(roleid,raidroom_id,ROLE_NOTIMEOUT)==false)
				{
					Log::log( LOG_ERR, "JoinRaid AddRoleInfo err. roleid=%d,roomid=%d\n", roleid,raidroom_id);
					proom->DelRole(roleid,0);
					retcode = ERR_RAID_JOIN_ALREADY;
				}
			}
			break;
		}
		default:
			return ERR_RAID_TYPE; 
			break;
		}
	}

	return retcode;
}

int RaidManager::RegisterServer(int sid, int gs_id, const std::vector<RaidFieldInfo> & info)
{
	LOG_TRACE("Raid RegisterServer : gs_id=%d, sid=%d", gs_id, sid);

	for (std::vector<RaidFieldInfo>::const_iterator it = info.begin(); it != info.end(); ++it)
	{
		LOG_TRACE("RaidFieldInfo Init: gs_id=%d, map_id=%d raid_type=%d queuing_time=%d raid_max_instance=%d max_player_limit=%d min_start_player_num=%d raid_template_id=%d", gs_id, it->map_id, it->raid_type, it->queuing_time,it->raid_max_instance,it->group1_limit.max_player_limit,it->group1_limit.min_start_player_num,it->raid_template_id);
		AddRaidMap(gs_id,*it);
	}
	AddRaidLine(gs_id);
	return 0;
}

bool RaidManager::AddRaidMap(int gs_id,const RaidFieldInfo& finfo)
{
	LOG_TRACE("Raid AddRaidMap : gs_id=%d, type=%d, map_id=%d", gs_id, finfo.raid_type, finfo.map_id);
	if(finfo.raid_type == TEAM_RAID)
	{
		RaidWaitingMap::iterator it = raid_waiting_map.find(finfo.map_id);
		if(it == raid_waiting_map.end())
		{
			std::map<int, WaitingTeamContainer*> containers;
			containers[TEAM_RAID_NORMAL] = new WaitingTeamContainer(finfo.map_id);
			containers[TEAM_RAID_REBORN] = new WaitingTeamContainer(finfo.map_id);
			raid_waiting_map.insert(std::make_pair(finfo.map_id, containers));
		}
	}
	if(finfo.raid_type == CRSSVRTEAMS_RAID)
	{
		LOG_TRACE("Raid AddRaidMap : gs_id=%d, type=%d, map_id=%d", gs_id, finfo.raid_type, finfo.map_id);
		RaidWaitingMap::iterator it = crssvrteams_waiting_map.find(finfo.map_id);
		if(it == crssvrteams_waiting_map.end())
		{
			std::map<int, WaitingTeamContainer*> containers;
			//containers[TEAM_RAID_NORMAL] = new WaitingTeamContainer(finfo.map_id);
			containers[TEAM_RAID_REBORN] = new WaitingTeamContainer(finfo.map_id);
			crssvrteams_waiting_map.insert(std::make_pair(finfo.map_id, containers));
		}
	}
	if(finfo.raid_type == HIDEANDSEEK_RAID)
	{
		RaidHideSeekWaitingMap::iterator it = raid_hideseek_waiting_map.find(finfo.map_id);
		if(it == raid_hideseek_waiting_map.end())
		{
			std::map<int, WaitingHideSeekContainer*> containers;
			containers[TEAM_RAID_NORMAL] = new WaitingHideSeekContainer(finfo.map_id, finfo.raid_type, HIDESEEK_MEMBER_LIMIT);
			//containers[TEAM_RAID_REBORN] = new WaitingTeamContainer(finfo.map_id);
			raid_hideseek_waiting_map.insert(std::make_pair(finfo.map_id, containers));
		}
	}

	if(finfo.raid_type == FENGSHEN_RAID)
	{
		RaidHideSeekWaitingMap::iterator it = raid_hideseek_waiting_map.find(finfo.map_id);
		if(it == raid_hideseek_waiting_map.end())
		{
			std::map<int, WaitingHideSeekContainer*> containers;
			//containers[TEAM_RAID_NORMAL] = new WaitingHideSeekContainer(finfo.map_id);
			containers[TEAM_RAID_REBORN] = new WaitingHideSeekContainer(finfo.map_id, finfo.raid_type, FENGSHEN_MEMBER_LIMIT);
			raid_hideseek_waiting_map.insert(std::make_pair(finfo.map_id, containers));
		}
	}

	RaidMap::iterator it = raidmap.find(finfo.map_id);
	if(it == raidmap.end())
		return raidmap.insert(std::make_pair(finfo.map_id,new RaidMap_Info(finfo,gs_id))).second;
	else
		return it->second->AddLine(finfo,gs_id);
}

void RaidManager::OnDisconnect(int gs_id)
{
	LOG_TRACE("Raid OnDisconnect : gs_id=%d, raidroom_map=%d raidmap=%d raidinstance_map=%d", gs_id, raidroom_map.size(),raidmap.size(),raidinstance_map.size());
	if(!IsRaidLine(gs_id))
		return;
	DelRaidLine(gs_id);
	
	RaidInstanceMap::iterator iit = GetGsInstanceBegin(gs_id);
	RaidInstanceMap::iterator iite = GetGsInstanceEnd(gs_id);
	std::vector<int> roomids;
	if(GsInstanceRangeValid(iit,iite))
	{
		for(;iit!=iite;++iit)
		{
			roomids.push_back(iit->second);
			//dont delete raidroom_map nodes here, because RaidBasic_Info destruction function will erase raidinstance_map node and make iterator iit invalid!!!
		}
	}
	for(std::vector<int>::iterator vit=roomids.begin();vit!=roomids.end();++vit)
	{
		RaidBasic_Info *proom = GetRoom(*vit);
		if(proom)
			delete(proom);
		raidroom_map.erase(*vit);
	}
	LOG_TRACE("Raid OnDisconnect over try: gs_id=%d, raidroom_map=%d raidmap=%d raidinstance_map=%d", gs_id, raidroom_map.size(),raidmap.size(),raidinstance_map.size());

	
	RaidMap::iterator ite,it = raidmap.begin();
	ite = raidmap.end();
	for(;it!=ite;)
	{
		it->second->DelLine(gs_id);
		if( it->second->GetLineNum() > 0)
			++it;
		else
		{
			//remove all rooms, if raidmap erased
			RaidRoomMap::iterator mit,mite=raidroom_map.end();
			for(mit=raidroom_map.begin();mit!=mite;)
			{
				if(mit->second->GetMapid() == it->first)
				{
					delete mit->second;
					raidroom_map.erase(mit++);
				}
				else
					++mit;
			}
			if(it->second->RaidType()==TEAM_RAID)
			{
				RaidWaitingMap::iterator wit = raid_waiting_map.find(it->first);
				if(wit != raid_waiting_map.end())
				{
					std::map<int, WaitingTeamContainer*>::iterator cit, cite = wit->second.end();
					for (cit = wit->second.begin(); cit != cite; ++cit)
						delete cit->second;
					raid_waiting_map.erase(wit);
				}
			}	
			if(it->second->RaidType()==CRSSVRTEAMS_RAID)
			{
				RaidWaitingMap::iterator wit = crssvrteams_waiting_map.find(it->first);
				if(wit != crssvrteams_waiting_map.end())
				{
					std::map<int, WaitingTeamContainer*>::iterator cit, cite = wit->second.end();
					for (cit = wit->second.begin(); cit != cite; ++cit)
						delete cit->second;
					crssvrteams_waiting_map.erase(wit);
				}
			}	
			if(it->second->RaidType()==HIDEANDSEEK_RAID)
			{
				RaidHideSeekWaitingMap::iterator wit = raid_hideseek_waiting_map.find(it->first);
				if(wit != raid_hideseek_waiting_map.end())
				{
					LOG_TRACE("Raid OnDisconnect raid_hideseek_waiting_map size=%d", raid_hideseek_waiting_map.size());
					std::map<int, WaitingHideSeekContainer*>::iterator cit, cite = wit->second.end();
					for (cit = wit->second.begin(); cit != cite; ++cit)
						delete cit->second;
					raid_hideseek_waiting_map.erase(wit);
				}
			}	

			delete it->second;
			raidmap.erase(it++);
		}
	}
	LOG_TRACE("Raid OnDisconnect over: gs_id=%d, raidroom_map=%d raidmap=%d raidinstance_map=%d", gs_id, raidroom_map.size(),raidmap.size(),raidinstance_map.size());
}

RaidManager::~RaidManager(){}
int RaidManager::CheckExistRole(int roleid)
{
	//LOG_TRACE("RaidManager::CheckExistRole find roleid=%d",roleid);
	RaidRoleMap::iterator roleit = raidrole_map.find(roleid);
	if(roleit != raidrole_map.end())
	{
		LOG_TRACE("CheckExistRole err, already in raid roleid=%d roomid=%d",roleit->first,roleit->second.GetRoomId());
		return ERR_RAID_JOIN_ALREADY;
	}
	OpeningRoomRoleSet::iterator openit = opening_room_role_set.find(roleid);
	if(openit != opening_room_role_set.end())
		return ERR_RAID_IS_OPENING;
	TeamApplyingRoleSet::iterator appit = team_applying_role_set.find(roleid);
	if(appit != team_applying_role_set.end())
	{
		LOG_TRACE("RaidManager::CheckExistRole find roleid=%d in team_apply_set",roleid);
		return ERR_RAID_IS_APPLYING;
	}
	return ERR_SUCCESS;
}
int RaidManager::CheckTeamApplyCD(int roleid)
{
	TeamApplyCoolingMap::iterator it = team_coolings.find(roleid);
	if (it == team_coolings.end())
		return ERR_SUCCESS;
	int cd = 0;
	if (raidmanager_debug_mode)
		cd = TEAM_APPLY_COOLDOWN_LIMIT_DEBUG;
	else
		cd = TEAM_APPLY_COOLDOWN_LIMIT;
	if(Timer::GetTime()-it->second<cd)
		return ERR_RAID_TEAMAPPLY_COOLING;
	team_coolings.erase(it);
	return ERR_SUCCESS;
}
void RaidManager::SetTeamApplyCD(int roleid)
{
	team_coolings[roleid] = Timer::GetTime();
}
void RaidManager::ClearTeamApplyCD(int roleid)
{
	team_coolings.erase(roleid);
}
int RaidManager::OpenRaid(int roleid, int gs_id, int map_id, int item_id, int item_count,GMailSyncData & syncdata, Octets& roomname,char can_vote, char difficulty)
{
	LOG_TRACE("RaidManager::OpenRaid roleid=%d map_id=%d",roleid,map_id);
//	RaidRoleMap::iterator roleit = raidrole_map.find(roleid);
//	if(roleit != raidrole_map.end())
//	{
//		LOG_TRACE("OpenRaid err, already in raid roleid=%d roomid=%d",roleit->first,roleit->second.GetRoomId());
//		return ERR_RAID_JOIN_ALREADY;
//	}
	RaidMap::iterator it = raidmap.find(map_id);
	if (it == raidmap.end())
		return ERR_RAID_MAP_NOTEXIST;
	if(it->second->RaidType()!=ROOM_RAID && it->second->RaidType()!= TOWER_RAID && it->second->RaidType()!=LEVEL_RAID && it->second->RaidType()!=TRANSFORM_RAID &&
			it->second->RaidType() != STEP_RAID)
		return ERR_RAID_TYPE;
	if( (roomname.size()>(size_t)RAID_ROOMNAME_LIMIT) || (Matcher::GetInstance()->Match((char*)roomname.begin(),roomname.size())!=0))
		return ERR_RAID_ROOMNAME;
	int ret=CheckExistRole(roleid);
	if(ret)
		return ret;
//	OpeningRoomRoleSet::iterator openit = opening_room_role_set.find(roleid);
//	if(openit != opening_room_role_set.end())
//		return ERR_RAID_IS_OPENING;
	if(it->second->Add_Room())//add room number first, forbid room applyer when exist rooms+ applying rooms reach room number limit
	{
		AddOpeningRole(roleid);
#ifdef DEBUG_RAID_ROBOT
		if(item_count==0)//bypass db
		{
			syncdata.inventory.items.clear();
			OnDBRaidOpen(ERR_SUCCESS,roleid,gs_id,map_id,syncdata,roomname,can_vote, difficulty);
			return ERR_SUCCESS;
		}
#endif		
		DBRaidOpenArg arg(roleid, map_id,item_id, item_count, syncdata);
		DBRaidOpen *rpc = (DBRaidOpen *)Rpc::Call(RPC_DBRAIDOPEN, arg);
		rpc->gs_id = gs_id;
		rpc->map_id = map_id;
		rpc->roomname = roomname;
		rpc->can_vote = can_vote;
		rpc->difficulty = difficulty;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;
	}
	else
	{
		DelOpeningRole(roleid);
		return ERR_RAID_ROOM_NUMBER_FULL;
	}
}
int RaidManager::OnTeamRaidMappingSuccess(RaidTeamBase * teama, RaidTeamBase * teamb, int map_id)
{
	LOG_TRACE("RaidManager::OnTeamRaidMappingSuccess, teama id=%d score=%d, teamb id=%d score=%d", teama->GetId(), teama->GetScore(), teamb->GetId(), teamb->GetScore());
	RaidMap::iterator mapit = raidmap.find(map_id);
        if (mapit == raidmap.end())
	{
		//impossible
		Log::log(LOG_ERR,"RaidManager::OnTeamRaidMappingSuccess map_id=%d map not found",map_id);
		return ERR_RAID_MAP_NOTEXIST;
	}
	if(teama->GetSize()!=TEAMRAID_MEMBER_LIMIT || teamb->GetSize()!=TEAMRAID_MEMBER_LIMIT)
	{
		Log::log(LOG_ERR,"OnTeamRaidMappingSuccess err teama size=%d, teamb size=%d",teama->GetSize(), teamb->GetSize());
	}
	TeamRaidMatchData tdata;
	tdata.roles.reserve(TEAMRAID_MEMBER_LIMIT*2);
	TeamRoleList::const_iterator it,ite;
	std::vector<RaidRole> rolevec;
	rolevec.reserve(TEAMRAID_MEMBER_LIMIT*2);
	RaidRole role;

	for(it=teama->GetRoleList().begin(),ite=teama->GetRoleList().end();it!=ite;++it)
	{
		tdata.roles.push_back(TeamRaidRoleData((*it)->GetRoleid(),RAIDTEAM_ATTACK,(*it)->role.score));
		if(GetRoleInfo((*it)->GetRoleid(),role))
		{
			// 如果是跨服
			if(teama->GetType() == CRSSVRTEAMS_RAID || teama->GetType() == TEAM_RAID)
			{
				role.roleid = (*it)->GetRoleid();
				rolevec.push_back(role);
			}
			else
				Log::log(LOG_ERR,"OnTeamRaidMappingSuccess err role=%d offline",(*it)->GetRoleid());
		}
		else
			rolevec.push_back(role);
	}
	for(it=teamb->GetRoleList().begin(),ite=teamb->GetRoleList().end();it!=ite;++it)
	{
		tdata.roles.push_back(TeamRaidRoleData((*it)->GetRoleid(),RAIDTEAM_DEFEND,(*it)->role.score));
		if(GetRoleInfo((*it)->GetRoleid(),role))
		{
			// 如果是跨服
			if(teama->GetType() == CRSSVRTEAMS_RAID || teama->GetType() == TEAM_RAID)
			{
				role.roleid = (*it)->GetRoleid();
				rolevec.push_back(role);
			}
			else
				Log::log(LOG_ERR,"OnTeamRaidMappingSuccess err role=%d offline",(*it)->GetRoleid());
		}
		else
			rolevec.push_back(role);
	}
	int rid = GetNextRoomId();
	TeamRaid_Info* room= new TeamRaid_Info(rid,map_id,*(mapit->second),tdata, teama, teamb);
	if(room==NULL || raidroom_map.insert(std::make_pair(rid,room)).second == false)
	{
		//impossible
		Log::log(LOG_ERR,"OnTeamRaidMappingSuccess err room=%x roomid=%d roomid exist",room,rid);
		return ERR_RAID_ROOMID_EXIST;
	}
	std::vector<RaidRole>::iterator rit,rite=rolevec.end();
	for(rit=rolevec.begin();rit!=rite;++rit)
	{
		if(AddRoleInfo(rit->roleid,rid,ROLE_NOTIMEOUT)==false)
			Log::log(LOG_ERR,"AddRoleInfo err. roleid=%d,roomid=%d\n", rit->roleid,rid);
		else
		{
			room->AcceptRole(*rit);
	//		DelTeamApplyingRole(rit->roleid);
		}
	}
	room->Init(teama, teamb);
	return ERR_SUCCESS;
}
int RaidManager::OnHideSeekRaidMappingSuccess(HideSeekRaidBase* teama, int map_id)
{
	//LOG_TRACE("RaidManager::OnHideSeekRaidMappingSuccess");
	RaidMap::iterator mapit = raidmap.find(map_id);
        if (mapit == raidmap.end())
	{
		//impossible
		Log::log(LOG_ERR,"RaidManager::OnHideSeekRaidMappingSuccess map_id=%d map not found",map_id);
		return ERR_RAID_MAP_NOTEXIST;
	}
	/*if((int)teama->GetSize()!=limit_member)
	{
		Log::log(LOG_ERR,"OnHideSeekRaidMappingSuccess err teama size=%d",teama->GetSize());
	}*/
	TeamRoleList::const_iterator it,ite;
	std::vector<RaidRole> rolevec;
	//rolevec.reserve(limit_member);
	RaidRole role;

	for(it=teama->GetRoleList().begin(),ite=teama->GetRoleList().end();it!=ite;++it)
	{
		LOG_TRACE("OnHideSeekRaidMappingSuccess teama roleid = %d", (*it)->GetRoleid());
		if(GetRoleInfo((*it)->GetRoleid(),role))
		{
			if(teama->GetType() == HIDEANDSEEK_RAID)
			{
				role.roleid = (*it)->GetRoleid();
				rolevec.push_back(role);
			}
			else
				Log::log(LOG_ERR,"OnHideSeekRaidMappingSuccess err role=%d offline",(*it)->GetRoleid());
		}
		else
			rolevec.push_back(role);
	}
	int rid = GetNextRoomId();
	HideSeekRaid_Info* room= new HideSeekRaid_Info(rid,map_id,*(mapit->second));
	if(room==NULL || raidroom_map.insert(std::make_pair(rid,room)).second == false)
	{
		//impossible
		Log::log(LOG_ERR,"OnHideSeekRaidMappingSuccess err room=%x roomid=%d roomid exist",room,rid);
		return ERR_RAID_ROOMID_EXIST;
	}
	std::vector<RaidRole>::iterator rit,rite=rolevec.end();
	for(rit=rolevec.begin();rit!=rite;++rit)
	{
		if(AddRoleInfo(rit->roleid,rid,ROLE_NOTIMEOUT)==false)
			Log::log(LOG_ERR,"AddRoleInfo err. roleid=%d,roomid=%d\n", rit->roleid,rid);
		else
		{
			LOG_TRACE("OnHideSeekRaidMappingSuccess AccepRole roleid = %d", rit->roleid);
			room->AcceptRole(*rit);
		}
	}
	room->Init(teama);
	return ERR_SUCCESS;
}

int RaidManager::OnDBRaidOpen(int retcode,int roleid, int gs_id, int map_id, GMailSyncData & syncdata, Octets& roomname,char can_vote, char difficulty)
{
	LOG_TRACE("RaidManager::OnDBRaidOpen roleid=%d map_id=%d",roleid,map_id);
	RaidOpen_Re re;
	re.retcode = retcode;
	re.difficulty = difficulty;
	if(DelOpeningRole(roleid)!=1)
		Log::log(LOG_ERR,"OnDBRaidOpen err. no match opening roleid =%d\n",roleid);

	RaidRole role;
	if(GetRoleInfo(roleid,role))
	{
		GProviderServer::GetInstance()->DispatchProtocol(gs_id, GMailEndSync(0, retcode, roleid, syncdata));
		//role offline
		return -1;
	}
	re.localsid = role.localsid;

	RaidRoleMap::iterator roleit = raidrole_map.find(roleid);
	if(roleit != raidrole_map.end())
	{
		Log::log(LOG_ERR,"OnDBRaidOpen err, already in raid roleid=%d",roleid);
		GProviderServer::GetInstance()->DispatchProtocol(gs_id, GMailEndSync(0,ERR_RAID_JOIN_ALREADY, roleid, syncdata));
		return -1;
	}

	RaidMap::iterator it = raidmap.find(map_id);
        if (it == raidmap.end())
	{
		LOG_TRACE("RaidManager::OnDBRaidOpen roleid=%d map_id=%d map not found",roleid,map_id);
		re.retcode = ERR_RAID_MAP_NOTEXIST;
	}
	else
	{
		if(retcode != ERR_SUCCESS)
		{
			it->second->Dec_Room();
		}
		else
		{
			int rid = GetNextRoomId();
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			int leaderline=-1;
			if(pinfo)leaderline = pinfo->gameid;
			RaidBasic_Info* room= new RaidRoom_Info(rid,map_id,roleid,leaderline,roomname,*(it->second),can_vote, difficulty);
			if(room==NULL || raidroom_map.insert(std::make_pair(rid,room)).second == false)
			{
				re.retcode = ERR_RAID_ROOMID_EXIST;
				LOG_TRACE("RaidManager::OnDBRaidOpen roleid=%d roomid=%d roomid exist",roleid,rid);
			}
			else
			{
				room->AcceptRole(role);
				re.map_id = map_id;
				re.raidroom_id = rid;
				re.roomname = roomname;
				re.apply_endtime = room->GetStatusEndTime();
				if(AddRoleInfo(roleid,rid,ROLE_NOTIMEOUT)==false)
				{
					Log::log( LOG_ERR, "OnDBRaidOpen AddRoleInfo err. roleid=%d,roomid=%d\n", roleid,rid);
					room->DelRole(roleid,0);
					re.retcode = ERR_RAID_JOIN_ALREADY;
				}
			}
		}
	}
	GProviderServer::GetInstance()->DispatchProtocol(gs_id, GMailEndSync(0, retcode, roleid, syncdata));
	GDeliveryServer::GetInstance()->Send(role.linksid,re);
	return 0;
}
int RaidManager::GetRoleInfo(int roleid,RaidRole& role)
{
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
	if (NULL==pinfo || prole==NULL)
	{
		return -1;
	}
	role.roleid = pinfo->roleid;
	role.level = pinfo->level;
	role.name = pinfo->name;
	role.occupation = pinfo->occupation;
	role.score = 0;//no use
	role.reborn_cnt = prole->reborn_cnt;
	role.localsid = pinfo->localsid;
	role.linksid = pinfo->linksid;
	role.cultivation = prole->cultivation;
	return 0;	
}
bool RaidManager::Initialize()
{
	//IntervalTimer::Attach(this,60000000/IntervalTimer::Resolution());
	IntervalTimer::Attach(this,1000000/IntervalTimer::Resolution());
	WaitingTeamContainer::BuildTeamMappingPolicy();
        return true;
}
void RaidManager::AddRoomEvent(int raidroom_id,int eventtime)
{
	LOG_TRACE("Raid AddRoomEvent eventtime=%d, raidroom_id=%d",eventtime, raidroom_id);
	raidroom_reverse_map.insert(std::make_pair(eventtime,raidroom_id));
}
void RaidManager::AddRoleEvent(int roleid,int eventtime)
{
	LOG_TRACE("Raid AddRoleEvent eventtime=%d, roleid=%d",eventtime, roleid);
	raidrole_reverse_map.insert(std::make_pair(eventtime,roleid));
}
void RaidManager::AddVoteEvent(int raidroom_id,int eventtime)
{
	LOG_TRACE("Raid AddVoteEvent eventtime=%d, raidroom_id=%d",eventtime, raidroom_id);
	raidvote_reverse_map.insert(std::make_pair(eventtime,raidroom_id));
}
void RaidManager::DelRoomEvent(int raidroom_id,int eventtime)
{
	LOG_TRACE("Raid DelRoomEvent eventtime=%d, raidroom_id=%d",eventtime, raidroom_id);
	RaidRoomReverseMap::iterator it = raidroom_reverse_map.find(eventtime);
	for(;it!=raidroom_reverse_map.end();)
	{
		if(it->first != eventtime)
			break;
		if(it->second == raidroom_id)
			raidroom_reverse_map.erase(it++);
		else
			++it;
	}
}
void RaidManager::DelRoleEvent(int roleid,int eventtime)
{
	LOG_TRACE("Raid DelRoleEvent eventtime=%d, roleid=%d",eventtime, roleid);
	RaidRoleReverseMap::iterator it = raidrole_reverse_map.find(eventtime);
	for(;it!=raidrole_reverse_map.end();)
	{
		if(it->first != eventtime)
			break;
		if(it->second == roleid)
			raidrole_reverse_map.erase(it++);
		else
			++it;
	}
}
void RaidManager::DelVoteEvent(int raidroom_id,int eventtime)
{
	LOG_TRACE("Raid DelVoteEvent eventtime=%d, raidroom_id=%d",eventtime, raidroom_id);
	RaidVoteReverseMap::iterator it = raidvote_reverse_map.find(eventtime);
	for(;it!=raidvote_reverse_map.end();)
	{
		if(it->first != eventtime)
			break;
		if(it->second == raidroom_id)
			raidvote_reverse_map.erase(it++);
		else
			++it;
	}
}
int RaidManager::BeginVote(int voted_roleid,int begin_roleid, int raidroom_id)
{
	int retcode = ERR_SUCCESS;
	RaidRoleMap::iterator roleit = raidrole_map.find(voted_roleid);
	if(roleit == raidrole_map.end() || roleit->second.GetRoomId()!=raidroom_id)
	{
		return ERR_RAID_VOTEDROLE_NOT_IN_ROOM;
	}
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(pbasic)
	{
		RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
		if(!proom || !pbasic->IsRoomRaid())
		{
			LOG_TRACE("RaidManager::BeginVote getroom type err");
			retcode = ERR_RAID_TYPE;
		}
		else
			retcode = proom->BeginVote(voted_roleid,begin_roleid);
	}
	else
		retcode = ERR_RAID_ROOM_INEXIST;
	LOG_TRACE("RaidManager::BeginVote voted_roleid=%d roleid=%d raidroom_id=%d retcode=%d",voted_roleid,begin_roleid,raidroom_id,retcode);
	return retcode;
}
int RaidManager::OnVote(int voted_roleid,int voter,bool agree)
{
	LOG_TRACE("RaidManager::OnVote voted_roleid=%d voter=%d result=%d",voted_roleid,voter,agree);
	int retcode = ERR_SUCCESS;
	RaidRoleMap::iterator roleit = raidrole_map.find(voter);
	if(roleit == raidrole_map.end())
	{
	//	LOG_TRACE("OnVote err,  voter=%d not in raid",voter);
		retcode = ERR_RAID_VOTER_NOT_IN_ROOM;
		return retcode;
	}

	RaidBasic_Info* pbasic=GetRoom(roleit->second.GetRoomId());
	if(pbasic)
	{
		RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
		if(!proom || !pbasic->IsRoomRaid())
		{
			LOG_TRACE("RaidManager::OnVote getroom type err");
			retcode = ERR_RAID_TYPE;
		}
		else
			retcode = proom->OnVote(voted_roleid,voter,agree);
	}
	else
		retcode = ERR_RAID_ROOM_INEXIST;
	return retcode;
}

/*void RaidBasic_Info::Check()
{
	LOG_TRACE("roomid=%d status=%d endtime=%d",raidroom_id,status,status_end_time);
}
void RaidBasic_Info::CheckAll()
{
	RaidManager * rm = RaidManager::GetInstance();
	if(gs_id!=-1 && status!=RAID_SENDSTART)
	{	
		RaidManager::RaidInstanceMap::iterator iti= rm->raidinstance_map.find(rm->GetInstanceMapKey(gs_id,index));
		if(iti==rm->raidinstance_map.end() )
			Log::log( LOG_ERR, "RaidRoom CheckAll instance not found. gs_id=%d,index=%d\n", gs_id,index);
		else if(iti->second != raidroom_id)
			Log::log( LOG_ERR, "RaidRoom CheckAll instance roomid not equal. RaidRoomid=%d, gs_id=%d,index=%d,instance'roomid=%d\n", raidroom_id,gs_id,index,iti->second);
		if(map_info.raidnumber.find(gs_id)==map_info.raidnumber.end())
			Log::log( LOG_ERR, "RaidRoom CheckAll mapinfo gs not found. gs_id=%d,index=%d\n", gs_id,index);
		if(rm->line_set.find(gs_id) == rm->line_set.end())
			Log::log( LOG_ERR, "RaidRoom CheckAll line_set gs not found. gs_id=%d,index=%d\n", gs_id,index);
	}
	for(TeamType::iterator it = team1.begin();it!=team1.end();++it)
	{
		RaidManager::RaidRoleMap::iterator itr = rm->raidrole_map.find(it->roleid);
		if(itr==rm->raidrole_map.end() )
			Log::log( LOG_ERR, "RaidRoom CheckAll role not found. roleid=%d,roomid=%d\n", it->roleid,raidroom_id);
		else if(itr->second.raidroom_id != raidroom_id)
			Log::log( LOG_ERR, "RaidRoom CheckAll role roomid not equal. RaidRoomid=%d, roleid=%d,role'roomid=%d\n", raidroom_id,it->roleid,itr->second.raidroom_id);
	}
	for(TeamType::iterator it = team1_apply.begin();it!=team1_apply.end();++it)
	{
		RaidManager::RaidRoleMap::iterator itr = rm->raidrole_map.find(it->roleid);
		if(itr==rm->raidrole_map.end() )
			Log::log( LOG_ERR, "RaidRoom CheckAll role not found. roleid=%d,roomid=%d\n", it->roleid,raidroom_id);
		else if(itr->second.raidroom_id != raidroom_id)
			Log::log( LOG_ERR, "RaidRoom CheckAll role roomid not equal. RaidRoomid=%d, roleid=%d,role'roomid=%d\n", raidroom_id,it->roleid,itr->second.raidroom_id);
	}
}
bool RaidManager::CheckAll(int now)
{
	if(now%60!=0)
		return true;
	//check role
	{
		unsigned int total_role=0;
		for (RaidRoomMap::iterator it = raidroom_map.begin(); it != raidroom_map.end();++it)
		{
			it->second->CheckAll();
			total_role += it->second->GetRoleNum();
		}
		if(total_role != raidrole_map.size())
			Log::log( LOG_ERR, "Raid CheckAll role not equal. RaidRoleMap num=%d,raidroom_map num=%d\n", raidrole_map.size(),total_role);

	}
	//check instance and room ,each map
	for(RaidMap::iterator itm = raidmap.begin();itm!=raidmap.end();++itm)
	{
		std::map<int,int> gs_instance1;
		for(RaidMap_Info::RaidNumberMap::iterator itrn = itm->second->raidnumber.begin();itrn!= itm->second->raidnumber.end();++itrn)
		{	
			if((itrn->second.instance_number + itrn->second.applying_number) > 0)	
				gs_instance1[itrn->first]=itrn->second.instance_number + itrn->second.applying_number;
		}
		int roomnum1 = 0;
		std::map<int,int> gs_instance2;
		for (RaidRoomMap::iterator it = raidroom_map.begin(); it != raidroom_map.end();++it)
		{
			if(it->second->map_id != itm->first)
				continue;
			if(it->second->gs_id != -1)
			{
				int instance_num = gs_instance2[it->second->gs_id];
				gs_instance2[it->second->gs_id] = instance_num +1;
			}
			roomnum1++;	
		}
		if(gs_instance1.size()!= gs_instance2.size())
			Log::log( LOG_ERR, "Raid CheckAll instance gs not equal. RaidNumberMap gsnum=%d,raidroom_map gsnum=%d,mapid=%d\n", gs_instance1.size(),gs_instance2.size(),itm->first);
		for(std::map<int,int>::iterator itii = gs_instance1.begin();itii!=gs_instance1.end();++itii)
			if(itii->second!= gs_instance2[itii->first])
				Log::log( LOG_ERR, "Raid CheckAll instance not equal. gs=%d RaidNumberMap num=%d,raidroom_map num=%d,mapid=%d\n", itii->first,itii->second,gs_instance2[itii->first],itm->first);
		for(std::map<int,int>::iterator itii = gs_instance2.begin();itii!=gs_instance2.end();++itii)
			if(itii->second!= gs_instance1[itii->first])
				Log::log( LOG_ERR, "Raid CheckAll instance not equal. gs=%d RaidNumberMap num=%d,raidroom_map num=%d,mapid=%d\n", itii->first,gs_instance1[itii->first],itii->second,itm->first);
		if(itm->second->room_number != roomnum1)
			Log::log( LOG_ERR, "Raid CheckAll room not equal. RaidMap num=%d,raidroom_map num=%d,mapid=%d\n", itm->second->room_number,roomnum1,itm->first);

	}
	//check instance and room ,all maps
	{
		std::map<int,int> gs_instance1;
		unsigned int roomnum1 = 0;
		std::map<int,int> gs_instance2;
		for(RaidMap::iterator itm = raidmap.begin();itm!=raidmap.end();++itm)
		{
			for(RaidMap_Info::RaidNumberMap::iterator itrn = itm->second->raidnumber.begin();itrn!= itm->second->raidnumber.end();++itrn)
			{
				if((itrn->second.instance_number + itrn->second.applying_number) > 0)	
					gs_instance1[itrn->first] +=itrn->second.instance_number + itrn->second.applying_number;
			}
			roomnum1+=itm->second->room_number;
		}
		for (RaidRoomMap::iterator it = raidroom_map.begin(); it != raidroom_map.end();++it)
		{
			if(it->second->gs_id != -1)
			{
				gs_instance2[it->second->gs_id] +=1;
			}
		}
		if(gs_instance1.size()!= gs_instance2.size())
			Log::log( LOG_ERR, "Raid CheckAll all map,instance gs not equal. RaidNumberMap gsnum=%d,raidroom_map gsnum=%d\n", gs_instance1.size(),gs_instance2.size());
		for(std::map<int,int>::iterator itii = gs_instance1.begin();itii!=gs_instance1.end();++itii)
			if(itii->second!= gs_instance2[itii->first])
				Log::log( LOG_ERR, "Raid CheckAll all map,instance not equal. gs=%d RaidNumberMap num=%d,raidroom_map num=%d\n", itii->first,itii->second,gs_instance2[itii->first]);
		for(std::map<int,int>::iterator itii = gs_instance2.begin();itii!=gs_instance2.end();++itii)
			if(itii->second!= gs_instance1[itii->first])
				Log::log( LOG_ERR, "Raid CheckAll all map,instance not equal. gs=%d RaidNumberMap num=%d,raidroom_map num=%d\n", itii->first,gs_instance1[itii->first],itii->second);
		if(raidroom_map.size()!= roomnum1)
			Log::log( LOG_ERR, "Raid CheckAll all map, room not equal. RaidMap num=%d,raidroom_map num=%d\n", roomnum1,raidroom_map.size());
	}
	//check opening
	for(OpeningRoomRoleSet::iterator ito=opening_room_role_set.begin();ito!=opening_room_role_set.end();++ito)
	{
		Log::log( LOG_ERR, "Raid CheckAll opening room role=%d",*ito);
	}	
	return true;
}*/
bool RaidManager::Update()
{
	//test code
	//FactionPkRaid_Info test;
	time_t now = Timer::GetTime();
//	CheckAll(now);
	if(line_set.size()==0 && raidroom_map.size()>0)
	{
		LOG_TRACE("Raid Update no line, check all room");
/*		for(RaidRoomMap::iterator roomit=raidroom_map.begin();roomit!=raidroom_map.end();++roomit)
		{
			roomit->second->Check();
		}*/
	
		for(RaidRoomReverseMap::iterator it=raidroom_reverse_map.begin();it!=raidroom_reverse_map.end();++it)
		{
			LOG_TRACE("Raid room event time=%d roomid=%d",it->first,it->second);
		}
			
	}
	for(RaidWaitingMap::iterator it=raid_waiting_map.begin(), ite=raid_waiting_map.end();it!=ite;++it)
	{
		//LOG_TRACE("Raid raid_waiting_map update, size=%d", raid_waiting_map.size());
		std::map<int, WaitingTeamContainer*>::iterator cit, cite = it->second.end();
		for (cit = it->second.begin(); cit != cite; ++cit)
			cit->second->Update(now);
	}
	for(RaidWaitingMap::iterator it=crssvrteams_waiting_map.begin(), ite=crssvrteams_waiting_map.end();it!=ite;++it)
	{
		//LOG_TRACE("Raid crssvrteams_waiting_map update, size=%d", crssvrteams_waiting_map.size());
		std::map<int, WaitingTeamContainer*>::iterator cit, cite = it->second.end();
		for (cit = it->second.begin(); cit != cite; ++cit)
			cit->second->Update(now);
	}
	for(RaidHideSeekWaitingMap::iterator it=raid_hideseek_waiting_map.begin(), ite=raid_hideseek_waiting_map.end();it!=ite;++it)
	{
		//LOG_TRACE("Raid raid_hideseek_waiting_map update, size=%d", raid_hideseek_waiting_map.size());
		std::map<int, WaitingHideSeekContainer*>::iterator cit, cite = it->second.end();
		for (cit = it->second.begin(); cit != cite; ++cit)
			cit->second->Update(now);
	}

	for(AsyncLeavingRoleList::iterator it=async_leaving_role_list.begin();it!=async_leaving_role_list.end();++it)
	{
		LeaveRoom(it->roleid,it->raidroom_id,1);
	}
	async_leaving_role_list.clear();
	for(RaidVoteReverseMap::iterator it=raidvote_reverse_map.begin();it!=raidvote_reverse_map.end();)
	{
		if(it->first>=now)
			break;
		//LOG_TRACE("Raid update now=%d, eventtime=%d raidvote_reverse_map size=%d",now, it->first,raidvote_reverse_map.size());
		
		RaidRoomMap::iterator roomit = raidroom_map.find(it->second);
		if(roomit != raidroom_map.end())
		{
			RaidBasic_Info* pbasic = roomit->second;
			if(pbasic)
			{
				RaidRoom_Info* proom = dynamic_cast<RaidRoom_Info*>(pbasic);
				if(!proom || !pbasic->IsRoomRaid())
				{
					LOG_TRACE("RaidManager::OnVote getroom type err");
				}
				else
					proom->VoteTimeoutResult();
			}
		}
		raidvote_reverse_map.erase(it++);
	}

	RaidRoomTimeoutSet raidroom_timeout_map;
	raidroom_timeout_map.clear();
	for(RaidRoomReverseMap::iterator it=raidroom_reverse_map.begin();it!=raidroom_reverse_map.end();)
	{
		if(it->first>=now)
			break;

		raidroom_timeout_map.insert(it->second);
		LOG_TRACE("Raid update RaidRoomReverseMap now=%d, eventtime=%d room_id=%d raidroom_reverse_map size=%d",now, it->first,it->second,raidroom_reverse_map.size());
		
		raidroom_reverse_map.erase(it++);
	}

	for(RaidRoomTimeoutSet::iterator it=raidroom_timeout_map.begin();it!=raidroom_timeout_map.end();++it)
	{
		int raidroom_id = *it;

		LOG_TRACE("Raid update RaidRoomTimeoutSet room_id=%d raidroom_timeout_map size=%d",raidroom_id,raidroom_timeout_map.size());
		
		RaidRoomMap::iterator roomit = raidroom_map.find(raidroom_id);
		if(roomit != raidroom_map.end())
		{
			if(roomit->second)
			{
				if(roomit->second->Update(now))
				{
					delete roomit->second;
					raidroom_map.erase(roomit);
				}
			}
			else
				raidroom_map.erase(roomit);
		}
	}	

	for(RaidRoleReverseMap::iterator it=raidrole_reverse_map.begin();it!=raidrole_reverse_map.end();)
	{
		if(it->first>=now)
			break;
		int roleid = it->second;
		RaidRoleMap::iterator roleit = raidrole_map.find(roleid);
		if(roleit!=raidrole_map.end() && roleit->second.IsTimeout(now))
		{	
			PlayerInfo* role = UserContainer::GetInstance().FindRole(roleid);
			int cur_gs = -1;
			if(role)
				cur_gs = role->gameid;
			int raidroom_id = roleit->second.GetRoomId();
			RaidRoomMap::iterator roomit = raidroom_map.find(raidroom_id);
			if(roomit != raidroom_map.end() && roomit->second)
			{
				if(roomit->second->GetGs()!=cur_gs)
				{
					LOG_TRACE("RaidManager::Update clear timeout offlinerole roleid=%d, raidroom_id=%d",roleid, raidroom_id);
					roomit->second->DelRole(roleid,0);
					raidrole_map.erase(roleit);
					if(roomit->second->NeedClose())
					{
						LOG_TRACE("RaidManager::Update clear timeout offlinerole close raidroom_id=%d",raidroom_id);
						delete roomit->second;
						raidroom_map.erase(roomit);
					}	
				}
				else
					roleit->second.ClearTimeout();
			}
			else
				raidrole_map.erase(roleit);
		}
		raidrole_reverse_map.erase(it++);
	}
	//LOG_TRACE("g_teamfrag_create %d g_teamfrag_delete %d g_applyrole_create %d g_applyrole_delete %d", g_teamfrag_create, g_teamfrag_delete, g_applyrole_create, g_applyrole_delete);
	return true;
}
/*
int RaidManager::OnTeamRaidUpdateChar(const TeamRaidUpdateChar& msg)
{
	int retcode = ERR_SUCCESS;
	RaidRoleMap::iterator roleit = raidrole_map.find(msg.roledata.roleid);
	if(roleit == raidrole_map.end() || roleit->second.GetRoomId()!=msg.raidroom_id)
	{
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	}
	RaidBasic_Info* pbasic=GetRoom(msg.raidroom_id);
	if(pbasic)
	{
		TeamRaid_Info* proom = dynamic_cast<TeamRaid_Info*>(pbasic);
		if(!proom || !pbasic->IsTeamRaid())
		{
			LOG_TRACE("RaidManager::OnTeamRaidUpdateChar getroom type err");
			retcode = ERR_RAID_TYPE;
		}
		else
			retcode = proom->OnTeamRaidUpdateChar(msg);
	}
	else
		retcode = ERR_RAID_ROOM_INEXIST;
	return retcode;
}
*/
void RaidManager::OnRaidTeamApply(int map_id,const std::vector<TeamRaidApplyingRole>& rawroles, bool reborn, int sid, int src_zoneid, int src_gsid)
{
	if(rawroles.size()==0)
	{
		//to do
		return;
	}
	/*
	PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(rawroles[0].roleid);
	if(pinfo==NULL)
	{
		//to do
		return;
	}*/
	int retcode=ERR_SUCCESS;
	int err_roleid=0;
	if(rawroles.size()>TEAMRAID_MEMBER_LIMIT)
	{
		retcode = ERR_RAID_TEAM_SIZE;
		//GDeliveryServer::GetInstance()->Send(pinfo->linksid,TeamRaidApply_Re(retcode,err_roleid,pinfo->localsid));
		return;
	}
	std::map<unsigned char/*occup*/, int/*count*/> occup_count;
	std::vector<TeamRaidApplyingRole>::const_iterator rit,rite=rawroles.end();
	for(rit=rawroles.begin();rit!=rite;++rit)
	{
		retcode=CheckExistRole(rit->roleid);
		if(retcode)
		{
			err_roleid=rit->roleid;
			break;
		}
		retcode=CheckTeamApplyCD(rit->roleid);
		if(retcode)
		{
			err_roleid=rit->roleid;
			break;
		}
		occup_count[rit->occupation]++;
		if (occup_count[rit->occupation] > RaidTeamFrag::OCCUPATION_PLAYER_LIMIT)
		{
			retcode = ERR_RAID_TEAM_APPLY_COND;
			break;
		}
	}
 	//check mapid, map type
	std::vector<TeamRaidApplyingRole>::const_iterator tit,tite=rawroles.end();
	if(retcode==ERR_SUCCESS)
	{
		RaidWaitingMap::iterator it = raid_waiting_map.find(map_id);
		if(it==raid_waiting_map.end())
			retcode = ERR_RAID_MAP_NOTEXIST;
		else
		{
			int type = reborn ? TEAM_RAID_REBORN : TEAM_RAID_NORMAL;
			WaitingTeamContainer * container = NULL;
			std::map<int, WaitingTeamContainer*>::iterator cit, cite = it->second.end();
			for (cit = it->second.begin(); cit != cite; ++cit)
			{
				if (cit->first == type)
				{
					container = cit->second;
					break;
				}
			}
			if (container)
			{
				if (!container->IsJoinTime())
					retcode = ERR_RAID_NOT_TEAMAPPLY_TIME;
				else
				{
					TeamRoleList roles;
					for (tit=rawroles.begin();tit!=tite;tit++)
					{
						roles.push_back(new ApplyingRole(*tit, map_id));
						PlayerInfo* pmember = UserContainer::GetInstance().FindRole(tit->roleid);
						if (pmember)
							GDeliveryServer::GetInstance()->Send(pmember->linksid, TeamRaidApplyNotice(tit->roleid, 1, pmember->localsid));

						CrossBattleManager::GetInstance()->JoinTeam(CRSSVRTEAMS_BATTLE, CRSSVRTEAMS_BATTLE, src_gsid/*gs_id*/, map_id, tit->roleid, 0/*end_time*/, 0/*team*/, src_zoneid);
					}

					RaidTeamFrag * team = new RaidTeamFrag(roles/*, type*/, container);
					retcode = container->AddTeam(team, true);
				}
			}
			else
				retcode = ERR_RAID_MAP_NOTEXIST;
		}
	}

	if(retcode == ERR_SUCCESS)
	{
		for(tit=rawroles.begin();tit!=tite;tit++)	
		{
			PlayerInfo* pmember = UserContainer::GetInstance().FindRoleOnline(tit->roleid);
			if (pmember)
			{
				TeamRaidApplyNotice apply_notice(tit->roleid, 1, pmember->localsid);
				GDeliveryServer::GetInstance()->Send(pmember->linksid, apply_notice);
			}
			else
			{
				TeamRaidApplyNotice apply_notice(tit->roleid, 1, 0);
				//跨服没有，转发到原服
				CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, apply_notice);
			}
		}
	}

	if (retcode != ERR_SUCCESS)
	{
		TeamRaidApply_Re applay_re(retcode,err_roleid,0,rawroles[0].roleid);
		PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(rawroles[0].roleid);
		if(pinfo==NULL)
		{
			LOG_TRACE("RaidManager::OnRaidTeamApply send to src_server, roleid=%d, src_zoneid=%d, retcode=%d, err_roleid=%d", 
					rawroles[0].roleid, src_zoneid, retcode, err_roleid);

			//跨服没有，转发到原服
			CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, applay_re);
		}
		else if(pinfo != NULL && pinfo->ingame) // 在跨服
		{
			LOG_TRACE("RaidManager::OnRaidTeamApply send to client, roleid=%d, retcode=%d, err_roleid=%d", 
					rawroles[0].roleid, retcode, err_roleid);
			applay_re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,applay_re);
		}
	}

	return;
}

void RaidManager::OnRaidTeamApply(int map_id,const std::vector<TeamRaidApplyingRole>& rawroles, bool reborn, bool iscrssvrteams, int sid, int src_zoneid, int src_gsid, int teamid, int issrc)
{
	if(rawroles.size()==0)
	{
		return;
	}
	if(src_zoneid == -1)
	{
		return;
	}
	int retcode=ERR_SUCCESS;
	int err_roleid=0;
	if(rawroles.size()>TEAMRAID_MEMBER_LIMIT)
	{
		retcode = ERR_RAID_TEAM_SIZE;
		err_roleid = 0;
	}

	if(!CrossCrssvrTeamsManager::Instance()->IsSeasonOpen())
	{
		LOG_TRACE("RaidManager::OnRaidTeamApply, IsSeasonOpen fail");
		retcode = ERR_RAID_NOT_TEAMAPPLY_TIME;
	}
	else
	{
		std::map<unsigned char/*occup*/, int/*count*/> occup_count;
		std::vector<TeamRaidApplyingRole>::const_iterator rit,rite=rawroles.end();
		for(rit=rawroles.begin();rit!=rite;++rit)
		{
			retcode=CheckExistRole(rit->roleid);
			if(retcode)
			{
				err_roleid=rit->roleid;
				break;
			}
			retcode=CheckTeamApplyCD(rit->roleid);
			if(retcode)
			{
				err_roleid=rit->roleid;
				break;
			}
			occup_count[rit->occupation]++;
			if (occup_count[rit->occupation] > RaidTeamBase::OCCUPATION_PLAYER_LIMIT)
			{
				retcode = ERR_RAID_TEAM_APPLY_COND;
				break;
			}
		}
	}
 	//check mapid, map type
	
	RaidWaitingMap& waiting_map = iscrssvrteams ? crssvrteams_waiting_map : raid_waiting_map;
	RaidWaitingMap::iterator it = waiting_map.find(map_id);
	std::vector<TeamRaidApplyingRole>::const_iterator tit,tite=rawroles.end();

	if(retcode==ERR_SUCCESS)
	{
		if(it==waiting_map.end())
			retcode = ERR_RAID_MAP_NOTEXIST;
		else
		{
			int type = reborn ? TEAM_RAID_REBORN : TEAM_RAID_NORMAL;
			WaitingTeamContainer * container = NULL;
			std::map<int, WaitingTeamContainer*>::iterator cit, cite = it->second.end();
			for (cit = it->second.begin(); cit != cite; ++cit)
			{
				if (cit->first == type)
				{
					container = cit->second;
					break;
				}
			}
			if (container)
			{
				if (!container->IsJoinTime())
					retcode = ERR_RAID_NOT_TEAMAPPLY_TIME;
				else
				{
					/*TeamRoleList roles;
					TTeamRaidRoleBrief rolebriefs;
					for (tit=rawroles.begin();tit!=tite;tit++)
					{
						LOG_TRACE("RaidManager::OnRaidTeamApply, for rawroles=%d", tit->roleid);
						roles.push_back(new ApplyingRole(*tit, map_id));
						rolebriefs.push_back((*tit).rolebrief);
					}*/
					
					RaidTeamBase * team = NULL;
					if(iscrssvrteams)
					{
						bool check_flag = true;
						if(check_flag && teamid <= 0)
						{
							retcode = ERR_RAID_TEAM_APPLY_CRSSVRTEAM_NOT_IN;
							check_flag = false;
						}

						if(check_flag && FindCrssvrTeamsApplying(teamid) != crssvrteams_applying_set.end())
						{
							retcode = ERR_RAID_TEAM_APPLY_CRSSVRTEAM_MUST_ONTIME;
							check_flag = false;
						}
					
						if(check_flag && rawroles.size() != TEAMRAID_MEMBER_LIMIT)
						{
							retcode = ERR_RAID_TEAM_APPLY_CRSSVRTEAM_ROLESIZE;
							check_flag = false;
						}

						if(check_flag && !issrc)
						{
							for (tit=rawroles.begin();tit!=tite;tit++)
							{
								bool isExsit = CrossCrssvrTeamsManager::Instance()->CheckRoleExsit(teamid, (*tit).roleid);
								if(!isExsit)
								{
									retcode = ERR_RAID_TEAM_APPLY_CRSSVRTEAM_HAVE_OTHER;
									err_roleid=(*tit).roleid;
									check_flag = false;
									break;
								}
							}
						}
						
						if(check_flag)
						{
							TeamRoleList roles;
							TTeamRaidRoleBrief rolebriefs;
							for (tit=rawroles.begin();tit!=tite;tit++)
							{
								//LOG_TRACE("RaidManager::OnRaidTeamApply, for rawroles=%d", tit->roleid);
								roles.push_back(new CrssvrTeamsApplyingRole(*tit, map_id, teamid));
								rolebriefs.push_back((*tit).rolebrief);
							}
		
							TeamRoleList::iterator it_roles_begin = roles.begin();
							TeamRoleList::iterator it_roles_end = roles.end();

							for(; it_roles_begin != it_roles_end ; ++ it_roles_begin)
							{
								//LOG_TRACE("RaidManager::OnRaidTeamApply JoinTeam roleid=%d, map_id=%d, src_zoneid=%d", (*it_roles_begin)->GetRoleid(), map_id, src_zoneid);
								CrossBattleManager::GetInstance()->JoinTeam(CRSSVRTEAMS_BATTLE, CRSSVRTEAMS_BATTLE, src_gsid/*gs_id*/, map_id, (*it_roles_begin)->GetRoleid(), 0/*end_time*/, 0/*team*/, src_zoneid);
							}
							CrossCrssvrTeamsManager::Instance()->SetRoleBriefs(teamid, rolebriefs);
							int team_score = CrossCrssvrTeamsManager::Instance()->GetTeamScore(teamid);
							team = new CrssvrTeamsFrag(roles/*, type*/, container, teamid, src_zoneid, team_score);
							retcode = container->AddTeam(team, true);

							LOG_TRACE("RaidManager::OnRaidTeamApply, retcode=%d, waiting_map.size=%d, src_zoneid=%d, team_score=%d", 
									retcode, waiting_map.size(), src_zoneid, team_score);
						}
					}
				}
			}
			else
				retcode = ERR_RAID_MAP_NOTEXIST;
		}
	}
	
	if(retcode == ERR_SUCCESS)
	{
		for(tit=rawroles.begin();tit!=tite;tit++)	
		{
			PlayerInfo* pmember = UserContainer::GetInstance().FindRoleOnline(tit->roleid);
			if (pmember)
			{
				TeamRaidApplyNotice apply_notice(tit->roleid, 2, pmember->localsid);
				GDeliveryServer::GetInstance()->Send(pmember->linksid, apply_notice);
			}
			else
			{
				TeamRaidApplyNotice apply_notice(tit->roleid, 2, 0);
				//跨服没有，转发到原服
				CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, apply_notice);
			}
		}
	}

	if (retcode != ERR_SUCCESS)
	{
		TeamRaidApply_Re applay_re(retcode,err_roleid,0,rawroles[0].roleid);
		PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(rawroles[0].roleid);
		if(pinfo==NULL)
		{
			LOG_TRACE("RaidManager::OnRaidCrssvrTeamApply send to src_server, roleid=%d, src_zoneid=%d, retcode=%d, err_roleid=%d", 
					rawroles[0].roleid, src_zoneid, retcode, err_roleid);

			//跨服没有，转发到原服
			CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, applay_re);
		}
		else if(pinfo != NULL && pinfo->ingame) // 在跨服
		{
			LOG_TRACE("RaidManager::OnRaidCrssvrTeamApply send to client, roleid=%d, retcode=%d, err_roleid=%d", 
					rawroles[0].roleid, retcode, err_roleid);
			applay_re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,applay_re);
		}
	}
	return;
}

void RaidManager::OnHideSeekRaidApply(int map_id,const std::vector<TeamRaidApplyingRole>& rawroles, HideSeekType raidtype, bool reborn, int sid, int src_zoneid, int src_gsid)
{
	if(rawroles.size()==0)
	{
		return;
	}
	int retcode=ERR_SUCCESS;
	int err_roleid=0;
	if(rawroles.size()>TEAMRAID_MEMBER_LIMIT)
	{
		retcode = ERR_RAID_TEAM_SIZE;
		return;
	}
	std::vector<TeamRaidApplyingRole>::const_iterator rit,rite=rawroles.end();
	for(rit=rawroles.begin();rit!=rite;++rit)
	{
		retcode=CheckExistRole(rit->roleid);
		if(retcode)
		{
			err_roleid=rit->roleid;
			break;
		}
		retcode=CheckTeamApplyCD(rit->roleid);
		if(retcode)
		{
			err_roleid=rit->roleid;
			break;
		}
	}
 	//check mapid, map type
	std::vector<TeamRaidApplyingRole>::const_iterator tit,tite=rawroles.end();
	if(retcode==ERR_SUCCESS)
	{
		RaidHideSeekWaitingMap::iterator it = raid_hideseek_waiting_map.find(map_id);
		if(it==raid_hideseek_waiting_map.end())
			retcode = ERR_RAID_MAP_NOTEXIST;
		else
		{
			int type = reborn ? TEAM_RAID_REBORN : TEAM_RAID_NORMAL;
			WaitingHideSeekContainer * container = NULL;
			std::map<int, WaitingHideSeekContainer*>::iterator cit, cite = it->second.end();
			for (cit = it->second.begin(); cit != cite; ++cit)
			{
				if (cit->first == type)
				{
					container = cit->second;
					break;
				}
			}
			if (container)
			{
				{
					TeamRoleList roles;
					for (tit=rawroles.begin();tit!=tite;tit++)
					{
						roles.push_back(new ApplyingRole(*tit, map_id));
						/*PlayerInfo* pmember = UserContainer::GetInstance().FindRole(tit->roleid);
						if (pmember)
							GDeliveryServer::GetInstance()->Send(pmember->linksid, TeamRaidApplyNotice(tit->roleid, 1, pmember->localsid));
							*/
					}

					HideSeekRaidBase * team = new HideSeekRaidBase(roles, raidtype, container);
					retcode = container->AddTeam(team, true);
				}
			}
			else
				retcode = ERR_RAID_MAP_NOTEXIST;
		}
	}

	if(retcode == ERR_SUCCESS)
	{
		for(tit=rawroles.begin();tit!=tite;tit++)	
		{
			PlayerInfo* pmember = UserContainer::GetInstance().FindRoleOnline(tit->roleid);
			if (pmember)
			{
				if(raidtype == HST_HIDESEEK)
				{
					TeamRaidApplyNotice apply_notice(tit->roleid, 3, pmember->localsid);
					GDeliveryServer::GetInstance()->Send(pmember->linksid, apply_notice);
				}
				else if(raidtype == HST_FENGSHEN)
				{
					TeamRaidApplyNotice apply_notice(tit->roleid, 4, pmember->localsid);
					GDeliveryServer::GetInstance()->Send(pmember->linksid, apply_notice);
				}
				
			}
		}
	}

	if (retcode != ERR_SUCCESS)
	{
		TeamRaidApply_Re applay_re(retcode,err_roleid,0,rawroles[0].roleid);
		PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(rawroles[0].roleid);
		if(pinfo != NULL && pinfo->ingame) // 在跨服
		{
			LOG_TRACE("RaidManager::OnHideSeekRaidApply send to client, roleid=%d, retcode=%d, err_roleid=%d", 
					rawroles[0].roleid, retcode, err_roleid);
			applay_re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,applay_re);
		}
	}

	return;
}

int RaidManager::CanFactionPkChallenge(int attacker_fac, int defender_fac)
{
	if(Timer::GetTime()-coolings[attacker_fac]<FACTION_CHALLENGE_COOLDOWN_LIMIT)
		return ERR_RAID_CHALLENGE_COOLING;
	FactionRoomMap::iterator it = faction_room_map.find(attacker_fac);
	if(it!=faction_room_map.end())
		return ERR_RAID_FACTION_COMBAT_BUSY;
	it = faction_room_map.find(defender_fac);
	if(it!=faction_room_map.end())
		return ERR_RAID_FACTION_COMBAT_BUSY;
	return 0;
}
int FactionPkRaid_Info::OnInviteRe(int roleid, int attacker, int defender, int map_id, char result)
{
	LOG_TRACE("FactionPkRaid_Info::OnInviteRe roleid=%d attacker=%d defender=%d result=%d",roleid,attacker,defender,result);
	if(status != RAID_WAIT_INVITE_RE)
		return ERR_SUCCESS;
	std::vector<int>::iterator it,ite=invites.end();
	for(it=invites.begin();it!=ite;++it)
	{
		if(*it==roleid)
			break;
	}
	if(it==ite)
		return -1;

	if(result==RAID_REFUSE_INVITE)
	{
		invites.erase(it);
		if(invites.empty())
		{
			PlayerInfo * pinviter = UserContainer::GetInstance().FindRoleOnline(inviter_roleid);
			if(NULL != pinviter)
			{
				FactionPkRaidChallenge_Re re(ERR_RAID_FACTION_CHALLENGE_REJECTED, defender, defrole_name, pinviter->localsid);
			//	FactionPkRaidChallenge_Re re(ERR_RAID_FACTION_CHALLENGE_REJECTED, defender, Octets(0), pinviter->localsid);
				GDeliveryServer::GetInstance()->Send(pinviter->linksid, re);
			}
			BroadCastMsg(MSG_COMBATREFUSE);
			return ERR_RAID_FACTION_CHALLENGE_REJECTED;
		}
	}
	else
	{
		bool ret1, ret2;
		int masterid1,masterid2;
		ret1 = FactionManager::Instance()->FindMaster(defender, masterid1);
		ret2 = FactionManager::Instance()->FindVice(defender, masterid2);
		if((ret1 && (*it)==masterid1) || (ret2 && (*it)==masterid2))
		{
			int ret=StartRaid();
			if(ret)
			{
				PlayerInfo * pinviter = UserContainer::GetInstance().FindRoleOnline(inviter_roleid);
				if(NULL != pinviter)
				{
				//	FactionPkRaidChallenge_Re re(ERR_RAID_FACTION_CHALLENGE_REJECTED, defender, Octets(0), pinviter->localsid);
					FactionPkRaidChallenge_Re re(ERR_RAID_FACTION_CHALLENGE_REJECTED, defender, defrole_name, pinviter->localsid);
					GDeliveryServer::GetInstance()->Send(pinviter->linksid, re);
				}
				//BroadCastMsg(MSG_COMBATTIMEOUT);// for some reason start raid error
				return ret;
			}
			time_t now = Timer::GetTime();
			FAC_DYNAMIC::fac_pk log = {now, 1, 20, {}};
			Octets att_name;
			FactionManager::Instance()->GetName(attacker, att_name);
			FactionDynamic::GetName(att_name, log.enemyname, log.enemynamesize);
			FactionManager::Instance()->RecordDynamic(defender, FAC_DYNAMIC::FAC_PK, log);

			FAC_DYNAMIC::fac_pk log2 = {now, 0, 20, {}};
			Octets def_name;
			FactionManager::Instance()->GetName(defender, def_name);
			FactionDynamic::GetName(def_name, log2.enemyname, log2.enemynamesize);
			FactionManager::Instance()->RecordDynamic(attacker, FAC_DYNAMIC::FAC_PK, log2);
		}
	}
	return ERR_SUCCESS;
}
int RaidManager::OnFactionPkInviteRe(int roleid, int attacker, int defender, int map_id, char result)
{
	FactionRoomMap::iterator it = faction_room_map.find(attacker);
	if(it==faction_room_map.end())
	{
		LOG_TRACE("RaidManager::OnFactionPkInviteRe getroomid err");
		return -1;
	}
	int raidroom_id = it->second;
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::OnFactionPkInviteRe getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::OnFactionPkInviteRe getroom type err");
		return ERR_RAID_TYPE;
	}
	int retcode =  proom->OnInviteRe(roleid,attacker,defender,map_id,result);
	LOG_TRACE("RaidManager::OnFactionPkInviteRe roleid=%d attacker=%d defender=%d map_id=%d result=%d",roleid,attacker,defender,map_id,result);
	if(retcode)
	{
		delete proom;
		proom = NULL;
		raidroom_map.erase(raidroom_id);
	}
	return retcode;
}
int RaidManager::GetFacPkList(int roleid, int map_id, std::vector<GFactionPkRoomInfo> & fields)
{
	RaidRoomMap::iterator it,ite=raidroom_map.end();
	GFactionPkRoomInfo info;
	for(it=raidroom_map.begin();it!=ite;++it)
	{
		RaidBasic_Info* pbasic=it->second;
		if(!pbasic)
		{
			LOG_TRACE("RaidManager::GetFacPkList getroom err");
			continue;
			//return ERR_RAID_ROOM_INEXIST;
		}
		FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
		if(!proom || !pbasic->IsFactionPkRaid())//only for FactionPkRaid
		{
			//LOG_TRACE("RaidManager::GetFacPkList getroom type err");
			//return ERR_RAID_TYPE;
			continue;
		}

		if(proom)
		{
			if(proom->GetRoomBrief(info, map_id))
				fields.push_back(info);
		}
	}
	return 0;
}
int FactionPkRaid_Info::UpdateScore(int roleid, int factionid, bool iskiller)
{
	if(factionid == attacker)
	{
		GFactionPkRaidRecord& data = attacker_score[roleid];
		data.roleid = roleid;
		if(iskiller)
		{
			if(data.kills!=32167)
				data.kills++;
		}
		else
		{
			if(data.deaths!=32167)
				data.deaths++;
		}
	}
	else if(factionid == defender)
	{
		GFactionPkRaidRecord& data = defender_score[roleid];
		data.roleid = roleid;
		if(iskiller)
		{
			if(data.kills!=32167)
				data.kills++;
		}
		else
		{
			if(data.deaths!=32167)
				data.deaths++;
		}
	}
	else
	{
		LOG_TRACE("FactionPkRaid_Info::UpdateScore roleid=%d factionid=%d not in attacker=%d defender=%d",roleid,factionid,attacker,defender);
	}
	return ERR_SUCCESS;	
}
int FactionPkRaid_Info::OnFactionPkKill(int killer, int victim, int victim_fid, int map_id)
{
	PlayerInfo * pkiller = UserContainer::GetInstance().FindRoleOnline(killer);
	if(NULL == pkiller)
		return ERR_RAID_ROLE_OFFLINE;
	PlayerInfo * pvictim = UserContainer::GetInstance().FindRoleOnline(victim);
	if(NULL == pvictim)
		return ERR_RAID_ROLE_OFFLINE;
	if(!IsValidKill(pkiller->factionid,pvictim->factionid))
		return ERR_RAID_FACTION_KILL_INVALID;
	UpdateScore(killer,pkiller->factionid,true);
	UpdateScore(victim,pvictim->factionid,false);

	return ERR_SUCCESS;
}
bool FactionPkRaid_Info::IsValidKill(int killer_fid,int victim_fid)
{
	return ((killer_fid == attacker || killer_fid == defender) && (victim_fid == defender || victim_fid == attacker));
}
int FactionPkRaid_Info::FactionPkRaidGetScore(int mapid,int roleid,FactionPkRaidGetScore_Re& msg)
{
	LOG_TRACE("FactionPkRaid_Info::FactionPkRaidGetScore roleid=%d" ,roleid);
	if(mapid!=this->map_id)
		return ERR_RAID_ARGUMENT_ERR;
       	PlayerInfo * player = UserContainer::GetInstance().FindRoleOnline(roleid);
	if(player == NULL)
		return ERR_RAID_ROLE_OFFLINE;
	int factionid = player->factionid;
	if(factionid!= attacker && factionid!=defender && FindRole(team1,roleid)==team1.end())
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	msg.mapid = mapid;
	msg.raidroom_id = raidroom_id;
	msg.attacker = attacker;
	msg.defender = defender;
	for(Scores::iterator it=attacker_score.begin(),ite=attacker_score.end();it!=ite;++it)
	{
		msg.attack_score.push_back(it->second);
	}
	for(Scores::iterator it=defender_score.begin(),ite=defender_score.end();it!=ite;++it)
	{
		msg.defender_score.push_back(it->second);
	}
	msg.attacker_num = group_size[RAIDTEAM_ATTACK];
	msg.defender_num = group_size[RAIDTEAM_DEFEND];
	msg.watcher_num = group_size[RAIDTEAM_LOOKER];
	msg.remain_time = GetRemainTime();
	msg.bonus = bonus; 
	return ERR_SUCCESS;
}
int FactionPkRaid_Info::GetRemainTime()
{
	if(end_time == 0)
		return INIT_PK_TIME;
	else
	{
		int now = Timer::GetTime();
		int remain_time = 0;
		if(now<end_time)
			remain_time = end_time - now;
		return remain_time; 
	}
}
int RaidManager::FactionPkRaidGetScore(int raidroom_id,int map_id,int roleid,FactionPkRaidGetScore_Re& msg)
{
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::FactionPkRaidGetScore getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::FactionPkRaidGetScore getroom type err");
		return ERR_RAID_TYPE;
	}
	else
	{
		return proom->FactionPkRaidGetScore(map_id,roleid,msg);
	}

}
int RaidManager::OnFactionPkKill(int killer, int victim, int victim_fid, int index, int map_id)
{
//	LOG_TRACE("RaidManager::OnGsRaidEnter roleid=%d, gs_now=%d",roleid, gs_now);
	int raidroom_id = GetRoleRoomId(killer);
	if (raidroom_id == -1)
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::OnFactionPkKill getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::OnFactionPkKill getroom type err");
		return ERR_RAID_TYPE;
	}
	else
	{
		return proom->OnFactionPkKill(killer,victim,victim_fid,map_id);
	}
}
int FactionPkRaid_Info::OnFactionPkNotify(int endtime, bool is_start)
{
	int now = Timer::GetTime();
	if(is_start)
	{
		if(now<endtime)
			this->end_time = endtime;
		else
		{
			this->end_time = 0;
			LOG_TRACE("FactionPkRaid_Info::OnFactionPkNotify now=%d endtime=%d err",now,endtime);
		}
	}
	else
	{
		LOG_TRACE("FactionPkRaid_Info::OnFactionPkNotify end, now=%d endtime=%d",now,endtime);
		FactionPkRaidStopFight msg(GetMapid(),raidroom_id);
	        FactionManager::Instance()->Broadcast(attacker, msg, msg.localsid);
	        FactionManager::Instance()->Broadcast(defender, msg, msg.localsid);
		if(status==RAID_FIGHTING)
		{
			BroadcastResult();
			HandleBonus();
 			SetStatus(RAID_ENDING);
		}
	}
	return ERR_SUCCESS;
}
int RaidManager::SetTeamRaidLimit(int number)
{
	LOG_TRACE("RaidManager::SetTeamRaidLimit number=%d",number);
	if(number!=1 && number!=2 && number!=3 && number!=4 && number!=6)
	{
		LOG_TRACE("RaidManager::SetTeamRaidLimit number=%d err",number);
		return -1;
	}
	TEAMRAID_MEMBER_LIMIT = number;
	//initial TEAMRAID_MEMBER_LIMIT is 6, no need to BuildTeamPool
	/*
	RaidWaitingMap::iterator ite,it=raid_waiting_map.begin();
	for(ite=raid_waiting_map.end();it!=ite;++it)
	{
		it->second->BuildTeamMappingPolicy();
	}
	*/
	WaitingTeamContainer::BuildTeamMappingPolicy();
	return 0;
}
int RaidManager::SetHideSeekRaidLimit(int type,int number)
{
	LOG_TRACE("RaidManager::SetHideSeekRaidLimit number=%d",number);
	for(RaidHideSeekWaitingMap::iterator it=raid_hideseek_waiting_map.begin(), ite=raid_hideseek_waiting_map.end();it!=ite;++it)
	{
		std::map<int, WaitingHideSeekContainer*>::iterator cit, cite = it->second.end();
		for (cit = it->second.begin(); cit != cite; ++cit)
		{
			LOG_TRACE("RaidManager::SetHideSeekRaidLimit type=%d, it->first=%d", type, cit->first);
			if(cit->first == type)
			{
				LOG_TRACE("Raid raid_hideseek_waiting_map update, size=%d", raid_hideseek_waiting_map.size());
				cit->second->SetMemberLimit(number);
			}
		}
	}

	return 0;
}

int RaidManager::OnFactionPkNotify(int end_time, bool is_start, int raidroom_id, int map_id)
{
	//int raidroom_id = GetIndexRoomId(gsid,index);
	//if(raidroom_id == -1)
	if(raidroom_id == 0)
	{
		LOG_TRACE("RaidManager::OnFactionPkNotify getroomid err");
		return ERR_RAID_INSTANCE_INEXIST;
	}

	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::OnFactionPkNotify getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::OnFactionPkNotify getroom type err");
		return ERR_RAID_TYPE;
	}
	else
	{
	//	if(proom->GetMapid()!=mapid)
	//		LOG_TRACE("RaidManager::OnFactionPkNotify mapid err mapid=%d roomid=%d",mapid,raidroom_id);
		return proom->OnFactionPkNotify( end_time, is_start);
	}
	return ERR_SUCCESS; 
}
void FactionPkRaid_Info::BroadCastMsg(int messageid)
{
	ChatBroadCast cbc;
	cbc.channel = GP_CHAT_SYSTEM;
	cbc.srcroleid = messageid; 
	Marshal::OctetsStream data;
	data<<attacker<<defender<<attfaction_name <<deffaction_name;
	cbc.msg = data;
	LinkServer::GetInstance().BroadcastProtocol(cbc);
}
void FactionPkRaid_Info::BroadcastResult()
{
	ChatBroadCast cbc;
	cbc.channel = GP_CHAT_SYSTEM;
	cbc.srcroleid = MSG_COMBATEND; 
	Marshal::OctetsStream data;
	int attacker_total_score=0;
	int defender_total_score=0;
	for(Scores::iterator it=attacker_score.begin(),ite=attacker_score.end();it!=ite;++it)
	{
		attacker_total_score+= it->second.kills;
	}
	for(Scores::iterator it=defender_score.begin(),ite=defender_score.end();it!=ite;++it)
	{
		defender_total_score+= it->second.kills;
	}
	data<<attacker<<defender<<attfaction_name<<deffaction_name<<attacker_total_score<<defender_total_score;
	cbc.msg = data;
	LinkServer::GetInstance().BroadcastProtocol(cbc);
	LOG_TRACE("FactionPkRaid_Info:: BroadcastResult attacker=%d:defender=%d:attacker_kills=%d:defender_kills=%d",\
		attacker, defender, attacker_total_score, defender_total_score);
}
int FactionPkRaid_Info::NotifyRolePkFaction(int factionid,int roleid)
{
	if(factionid!=attacker && factionid!=defender)
		LOG_TRACE("FactionPkRaid_Info::NotifyRolePkFaction roomid=%d factionid=%d attacker=%d defender=%d err",raidroom_id,factionid,attacker,defender);
	FactionPkRaidBegin msg(ERR_SUCCESS,map_id,raidroom_id,attacker,defender,attfaction_name,deffaction_name);
       	PlayerInfo * player = UserContainer::GetInstance().FindRole(roleid);
	if(player == NULL)
		return -1;
	if(status!=RAID_FIGHTING)
		return -1;
	msg.localsid = player->localsid;
	GDeliveryServer::GetInstance()->Send(player->linksid, msg);
	LOG_TRACE("FactionPkRaid_Info::NotifyRolePkFaction roomid=%d factionid=%d attacker=%d defender=%d",raidroom_id,factionid,attacker,defender);
	return 0;
}
int RaidManager::NotifyRolePkFaction(int factionid,int roleid)
{
//	LOG_TRACE("RaidManager::NotifyRolePkFaction fid=%d roleid=%d",factionid,roleid);
	FactionRoomMap::iterator it = faction_room_map.find(factionid);
	if(it!=faction_room_map.end())
	{
		int raidroom_id = it->second;
		RaidBasic_Info* pbasic=GetRoom(raidroom_id);
		if(!pbasic)
		{
			LOG_TRACE("RaidManager::NotifyRolePkFaction getroom err");
			return ERR_RAID_ROOM_INEXIST;
		}
		FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
		if(!proom || !pbasic->IsFactionPkRaid())
		{
			LOG_TRACE("RaidManager::NotifyRolePkFaction getroom type err");
			return ERR_RAID_TYPE;
		}
		else
		{
			return proom->NotifyRolePkFaction(factionid,roleid);
		}
	}
	return ERR_SUCCESS;
}
int RaidManager::FactionPkRaidDeposit(int gsid,int index, int map_id, unsigned int delta_money)
{
	int raidroom_id = GetIndexRoomId(gsid,index);
	if (raidroom_id == -1)
	{
		LOG_TRACE("RaidManager::FactionPkRaidDeposit getroomid err");
		return ERR_RAID_INSTANCE_INEXIST;
	}

	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::FactionPkRaidDeposit getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::FactionPkRaidDeposit getroom type err");
		return ERR_RAID_TYPE;
	}
	else
	{
		proom->AddBonus(delta_money);
		return ERR_SUCCESS;
	}
}
int FactionPkRaid_Info::OnCancelChallenge(int cancel_fid, int dst_factionid, int map_id)
{
	if(cancel_fid != attacker || dst_factionid != defender)
		return ERR_RAID_FACTION_CANCEL_ARU_ERR;
	else 
		return ERR_SUCCESS;
}
int RaidManager::OnFactionPkCancelChallenge(int roleid, unsigned int dst_factionid, int map_id)
{
	PlayerInfo * pinviter = UserContainer::GetInstance().FindRoleOnline(roleid);
	if(NULL == pinviter)
		return ERR_RAID_ROLE_OFFLINE;
	if(pinviter->factionid==0  || (!ValidTitle(pinviter->factiontitle)))
	{
		return ERR_RAID_NOT_VALID_FACTITLE;
	}
	RaidRoleMap::iterator roleit = raidrole_map.find(roleid);
	if(roleit == raidrole_map.end())
	{
		return ERR_RAID_ROLE_NOT_IN_ROOM;
	}
	int raidroom_id = roleit->second.GetRoomId();
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::OnFactionPkCancelChallenge getroom err");
		return ERR_RAID_ROOM_INEXIST;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::OnFactionPkCancelChallenge getroom type err");
		return ERR_RAID_TYPE;
	}
	else
	{
		int ret= proom->OnCancelChallenge(pinviter->factionid,dst_factionid,map_id);
		if(ret==ERR_SUCCESS)
		{
			if(proom->GetMapid()!=map_id)
				LOG_TRACE("RaidManager::OnFactionPkCancelChallenge mapid err mapid=%d roomid=%d",map_id,raidroom_id);
		//	proom->GS_Close_Instance();//not inform client
		//	proom->RoomClose();
			delete proom;
			proom = NULL;
			raidroom_map.erase(raidroom_id);
		}
		return ret;
	}
}
void RaidManager::OnFactionNameChange(int fid, const Octets & newname)
{
	LOG_TRACE("RaidManager::OnFactionNameChange fid=%d",fid);
	FactionRoomMap::iterator it = faction_room_map.find(fid);
	if(it==faction_room_map.end())
		return;
		
	int raidroom_id = it->second;
	RaidBasic_Info* pbasic=GetRoom(raidroom_id);
	if(!pbasic)
	{
		LOG_TRACE("RaidManager::OnFactionNameChange getroom err");
		return;
	}
	FactionPkRaid_Info* proom = dynamic_cast<FactionPkRaid_Info*>(pbasic);
	if(!proom || !pbasic->IsFactionPkRaid())
	{
		LOG_TRACE("RaidManager::OnFactionNameChange getroom type err");
		return;
	}
	proom->SetFactionName(fid,newname);
}

/*
void RaidManager::AddWaitingTeam(RaidTeamFrag * team)
{
	LOG_TRACE("add waiting team %d again size=%d", team->GetId(), team->GetSize());
	WaitingTeamContainer * container = NULL;
	std::map<int, WaitingTeamContainer*>::iterator cit, cite = it->second.end();
	for (cit = it->second.begin(); cit != cite; ++cit)
	{
		if (cit->first == team->GetType())
		{
			container = cit->second;
			break;
		}
	}
	if (container)
		container->AddTeam(team);
	else
	{
		team->DeleteRoleList();
		delete team;
	}
}
*/
int RaidManager::FactionPkChallenge(int roleid, Octets defender_name, int map_id)
//int RaidManager::FactionPkChallenge(int roleid, int dst_roleid, int map_id)
{
	PlayerInfo * pinviter = UserContainer::GetInstance().FindRoleOnline(roleid);
	if(NULL == pinviter)
		return ERR_RAID_ROLE_OFFLINE;
	if(pinviter->factionid==0  || (!ValidTitle(pinviter->factiontitle)))
	{
		return ERR_RAID_NOT_VALID_FACTITLE;
	}

	int dst_roleid=0;
	if(!UserContainer::GetInstance().FindRoleId(defender_name,dst_roleid))
	{
		return ERR_RAID_TARGET_NOT_ONLINE;
	}
	PlayerInfo* pinvited = UserContainer::GetInstance().FindRoleOnline(dst_roleid);
	if(pinvited==NULL)
	{
		return ERR_RAID_TARGET_NOT_ONLINE;
	}
	if(pinvited->factionid == 0)
	{
		return ERR_RAID_NOT_IN_FACTION;
	}
	int attacker = pinviter->factionid;
	int defender = pinvited->factionid;
	Octets att_name,def_name;	
//	Octets att_name("xxx",3);
//	Octets def_name("yyy",3);
	int att_level,def_level;
	if(attacker==defender)
		return ERR_RAID_IN_SAME_FACTION;
	if(!FactionManager::Instance()->GetBrief(attacker, att_name, att_level))
		return ERR_RAID_ROLE_OFFLINE;
	if(!FactionManager::Instance()->GetBrief(defender, def_name, def_level))
		return ERR_RAID_TARGET_NOT_ONLINE;
	if(att_level<1 || def_level<1)
		return ERR_RAID_FAC_LOWLEVEL;

	int ret = CanFactionPkChallenge(attacker, defender);
	if(ret)
		return ret;
	if(!FactionManager::Instance()->HasProsperity(attacker, FACTION_CHALLENGE_PROSPERITY))
	{
		return ERR_RAID_FACTION_NOPROSPERITY;
	}

	bool ret1, ret2;
	unsigned int linksid1, linksid2, localsid1, localsid2;
	int masterid1,masterid2;	
	ret1 = FactionManager::Instance()->FindMaster(defender, linksid1, localsid1);
	ret2 = FactionManager::Instance()->FindVice(defender, linksid2, localsid2);
	ret1 = ret1 && FactionManager::Instance()->FindMaster(defender, masterid1);
	ret2 = ret2 && FactionManager::Instance()->FindVice(defender, masterid2);

	if(!ret1&&!ret2)
	{
	//	DEBUG_PRINT("CombatChallenge, master is not online, factionid=%d", factionid);
		return ERR_RAID_TARGET_MASTER_NOT_ONLINE;
	}
	int ret3=AddRaidRoom(map_id,FACTION_PK_RAID);
	if(ret3!=ERR_SUCCESS)
	{
		return ret3;
	}
	RaidMap::iterator mapit = raidmap.find(map_id);
        if (mapit == raidmap.end())
	{
		LOG_TRACE("RaidManager::FactionPkChallenge map_id=%d map not found",map_id);
		return ERR_RAID_MAP_NOTEXIST;
	}
	//then need to dec room when err occur
	int rid = GetNextRoomId();
/*	Octets att_name_dup=att_name;
	att_name_dup.reserve(att_name.size()*2);
	Octets def_name_dup=def_name;
	def_name_dup.reserve(def_name.size()*2);
	Octets defender_name_dup=defender_name;
	defender_name_dup.reserve(defender_name.size()*2);
	FactionPkRaid_Info* room= new FactionPkRaid_Info(rid,map_id,*(mapit->second),attacker,defender,roleid,defender_name_dup,att_name_dup,def_name_dup);*/
	FactionPkRaid_Info* room= new FactionPkRaid_Info(rid,map_id,*(mapit->second),attacker,defender,roleid,defender_name,att_name,def_name);
//	FactionPkRaid_Info* room= new FactionPkRaid_Info(rid,map_id,*(mapit->second),attacker,defender,roleid);
	LOG_TRACE("RaidManager::FactionPkChallenge roleid=%d factionid=%d def_factionid roomid=%d mapid=%d",roleid,attacker,defender,rid,map_id);
	if(room==NULL || raidroom_map.insert(std::make_pair(rid,room)).second == false)
	{
		//impossible
		LOG_TRACE("RaidManager::FactionPkChallenge err room=%x roomid=%d roomid exist",room,rid);
		Log::log(LOG_ERR,"FactionPkChallenge err room=%x roomid=%d roomid exist",room,rid);
		DecRaidRoom(map_id,FACTION_PK_RAID);
		return ERR_RAID_ROOMID_EXIST;
	}
//	room->SetNames(defender_name_dup,att_name,def_name);


	FactionManager::Instance()->DecProsperity(attacker, 50);
	FactionPkRaidInvite msg1(map_id,attacker,att_name,defender,def_name);
//	FactionPkRaidInvite msg2(map_id,attacker,att_name,defender,def_name);
	if(ret1)
	{
		msg1.localsid = localsid1;
		room->AddInvited(masterid1);
		GDeliveryServer::GetInstance()->Send(linksid1, msg1);
	}
	if(ret2)
	{
		msg1.localsid = localsid2;
		room->AddInvited(masterid2);
		GDeliveryServer::GetInstance()->Send(linksid2, msg1);
	}
	return ERR_SUCCESS;	
}
FactionPkRaid_Info::FactionPkRaid_Info(int raidroomid,int mapid, RaidMap_Info & mapinfo, int attacker_fid, int defender_fid,int inviter, \
		const Octets& def_role_name, const Octets& att_fac_name,  const Octets& def_fac_name)
	: RaidBasic_Info(raidroomid,mapid,mapinfo),attacker(attacker_fid),defender(defender_fid),inviter_roleid(inviter),\
		  defrole_name(def_role_name),attfaction_name(att_fac_name),deffaction_name(def_fac_name),end_time(0),bonus(0)
/*FactionPkRaid_Info::FactionPkRaid_Info(int raidroomid,int mapid, RaidMap_Info & mapinfo, int attacker_fid, int defender_fid,int inviter)
	: RaidBasic_Info(raidroomid,mapid,mapinfo),attacker(attacker_fid),defender(defender_fid),inviter_roleid(inviter)*/
{
	RaidManager::GetInstance()->AddPkFaction(attacker,raidroom_id);
	RaidManager::GetInstance()->AddPkFaction(defender,raidroom_id);
	for(unsigned int i=0;i<(RAIDTEAM_END+1);i++)
		group_size[i]=0;
 	SetStatus(RAID_WAIT_INVITE_RE);
	BroadCastMsg(MSG_COMBATCHALLENGE);
}

FactionPkRaid_Info::~FactionPkRaid_Info()
{
//	if(status==RAID_FIGHTING)
//		BroadcastResult();
	LOG_TRACE("~FactionPkRaid_Info()");
	RaidManager::GetInstance()->DelPkFaction(attacker);
	RaidManager::GetInstance()->DelPkFaction(defender);
	RoomClose();
}
void FactionPkRaid_Info::SetFactionName(int fid, const Octets & newname)
{
	if(fid==attacker)
		attfaction_name = newname;
	else if(fid== defender)
		deffaction_name = newname;
}
void FactionPkRaid_Info::HandleBonus()
{
	LOG_TRACE("FactionPkRaid_Info::HandleBonus bonus=%d",bonus);
	if(bonus==0)
		return;
	int attacker_total_score=0;
	int defender_total_score=0;
	for(Scores::iterator it=attacker_score.begin(),ite=attacker_score.end();it!=ite;++it)
	{
		attacker_total_score+= it->second.kills;
	}
	for(Scores::iterator it=defender_score.begin(),ite=defender_score.end();it!=ite;++it)
	{
		defender_total_score+= it->second.kills;
	}
	if(attacker_total_score >defender_total_score)
		FactionManager::Instance()->AddFactionMoney(attacker,bonus);
	else if(defender_total_score >attacker_total_score)
		FactionManager::Instance()->AddFactionMoney(defender,bonus);
	//equal score, no one get money
}

int WaitingHideSeekContainer::AddTeam(HideSeekRaidBase * team, bool match_later)
{
	int size = team->GetSize();
	if(size + subteamcount <= limit_member)
	{
		// 人数未满或已满
		return AddSubMap(team);
	}
	else
	{
		return AddPrepareMap(team);
	}

}

int WaitingHideSeekContainer::AddFullTeam(HideSeekRaidBase * fullteam, bool match_later)
{
	LOG_TRACE("WaitingHideSeekContainer::AddFullTeam match_later=%d",match_later);
	//time_t now = Timer::GetTime();
	
	// add room or return
	int ret=RaidManager::GetInstance()->AddRaidRoom(map_id, fullteam->GetType());
	if(ret!=ERR_SUCCESS)
	{
		LOG_TRACE("WaitingHideSeekContainer::AddFullTeam AddRaidRoom, map_id=%d, ret=%d, type=%d",map_id, ret, fullteam->GetType());
		return ERR_SUCCESS;
	}	

	//LOG_TRACE("fullteam %d score %d match success,use time %d", matchteam->GetId(), matchteam->GetScore(), now - matchteam->GetCreateTime());
	//DelAllFromSubList(fullteam->GetId()); // 删除
	RaidManager::GetInstance()->OnHideSeekRaidMappingSuccess(fullteam, map_id);

	return ERR_SUCCESS;
}

void 	WaitingHideSeekContainer::DelFromSubList(int id, int roleid)
{
	SubTeamReverseMultMap::iterator it = subteam_reverse_map.find(id);
	if(it  != subteam_reverse_map.end())
	{
		subteamcount -= it->second->GetSize();
		EraseRoleRaidBaseMap(it->second, roleid);
		if(it->second->GetSize() == 0)
		{
			delete it->second;
			it->second = NULL;
			subteam_reverse_map.erase(it);
		}
		LOG_TRACE("DelFromSubList roleid=%d, subteamcount=%d, role_raidbase_map size=%d", 
				roleid, subteamcount, role_raidbase_map.size());
	}
}

void 	WaitingHideSeekContainer::DelFromPrepareList(int id, int roleid)
{
	SubTeamReverseMultMap::iterator it = prepare_reverse_map.find(id);
	if(it  != prepare_reverse_map.end())
	{
		EraseRoleRaidBaseMap(it->second, roleid);
		if(it->second->GetSize() == 0)
		{
			delete it->second;
			it->second = NULL;
			prepare_reverse_map.erase(it);
		}

		LOG_TRACE("DelFromPrepareList roleid=%d, subteamcount=%d, role_raidbase_map size=%d", 
				roleid, subteamcount, role_raidbase_map.size());
	}
}

void 	WaitingHideSeekContainer::DelAllFromSubList(int id)
{
	SubTeamReverseMultMap::iterator it = subteam_reverse_map.find(id);
	if(it  != subteam_reverse_map.end())
	{
		subteamcount -= it->second->GetSize();
		EraseRoleRaidBaseMap(it->second);
		delete it->second;
		subteam_reverse_map.erase(it);
		LOG_TRACE("DelAllFromSubList pbase id %d, subteamcount=%d, role_raidbase_map size=%d", 
				it->second->GetId(), subteamcount, role_raidbase_map.size());
	}
}

void 	WaitingHideSeekContainer::DelAllFromPrepareList(int id)
{
	SubTeamReverseMultMap::iterator it = prepare_reverse_map.find(id);
	if(it  != prepare_reverse_map.end())
	{
		EraseRoleRaidBaseMap(it->second);
		delete it->second;
		prepare_reverse_map.erase(it);
		LOG_TRACE("DelAllFromPrepareList pbase id %d, subteamcount=%d, role_raidbase_map size=%d", 
				it->second->GetId(), subteamcount, role_raidbase_map.size());
	}
}

int WaitingHideSeekContainer::Update(int now)
{
	if(Timer::GetTime() > next_print_time)
	{
		LOG_TRACE("WaitingHideSeekContainer::Update type=%d, sub size=%d, subteamcount=%d, prepare size=%d, GetPrepareMapSize=%d, LIMIT=%d", 
				type, subteam_reverse_map.size(), subteamcount, prepare_reverse_map.size(), GetPrepareMapSize(), limit_member);

		next_print_time = Timer::GetTime() + PRINT_INFO_INTERVAL;
	}

	if((GetPrepareMapSize() + subteamcount) >= limit_member)
	{
		// 已经人数，可以组建一个房间 
		//LOG_TRACE("WaitingHideSeekContainer::Update prepare size=%d", prepare_reverse_map.size());
		SubTeamReverseMultMap::iterator it, ite = prepare_reverse_map.end();
		for(it=prepare_reverse_map.begin(); it!=ite;)
		{
			HideSeekRaidBase* pbase = it->second;
			if(!pbase)
			{
				Log::log(LOG_ERR, "WaitingHideSeekContainer pbase is NULL");
				continue;
			}

			unsigned int needcount = limit_member - subteamcount;

			LOG_TRACE("WaitingHideSeekContainer::Update, type=%d,  pbase size=%d, needcount=%d, subteamcount=%d", type, pbase->GetSize(), needcount, subteamcount);
			if(pbase->GetSize() <= needcount)
			{
				// 将prepare的转移到sub队列中
				AddSubMap(pbase);
				prepare_reverse_map.erase(it++);					
			}
			else
			{
				++it;
			}

		}

		if(subteamcount >= limit_member)
		{
			HideSeekRaidBase * fullteam = CreateFullTeam();
			if (fullteam != NULL)
			{
				AddFullTeam(fullteam);
				delete fullteam;
			}
		}
	}
	
	return ERR_SUCCESS;
}

int WaitingHideSeekContainer::RoleQuit(int roleid)
{
	int hs_raid_id = -1;
	if((hs_raid_id = GetRoleHideSeekRaidId(roleid)) == -1)
	{
		Log::log(LOG_ERR, "WaitingHideSeekContainer::RoleQuit not find RaidId roleid=%d", roleid);
		return -1;
	}

	HideSeekRaidBase* pbase =  GetHideSeekRaid(hs_raid_id);
	if(!pbase)
	{
		Log::log(LOG_ERR, "WaitingHideSeekContainer::RoleQuit not find pbase roleid = %d, id=%d", roleid, hs_raid_id);
		return -1;
	}

	DelFromSubList(pbase->GetId(), roleid);
	DelFromPrepareList(pbase->GetId(), roleid);

	return ERR_SUCCESS;
}

void WaitingHideSeekContainer::ClearAll()
{
	SubTeamReverseMultMap::const_iterator it, ite = subteam_reverse_map.end();
	for (it = subteam_reverse_map.begin(); it != ite; ++it)
	{
		LOG_TRACE("WaitingHideSeekContainer::ClearAll subteam_reverse_map id %d size %d", 
				it->second->GetId(), (it->second)->GetSize());
		DelAllFromSubList(it->second->GetId()); // 删除subteam_reverse_map中所有对象	
	}
	subteam_reverse_map.clear();

	SubTeamReverseMultMap::const_iterator it_pre, ite_pre = prepare_reverse_map.end();
	for (it_pre = prepare_reverse_map.begin(); it_pre != ite_pre; ++it_pre)
	{
		LOG_TRACE("WaitingHideSeekContainer::ClearAll prepare_reverse_map id %d size %d", 
				it_pre->second->GetId(), (it_pre->second)->GetSize());
		DelAllFromPrepareList(it_pre->second->GetId()); // 删除prepare_reverse_map中所有对象	
	}
	prepare_reverse_map.clear();
}

HideSeekRaidBase* WaitingHideSeekContainer::CreateFullTeam()
{
	if (subteamcount != limit_member)
	{
		Log::log(LOG_ERR, "WaitingHideSeekContainer::CreateFullTeam type=%d createfullteam member size %d invalid", type, subteamcount);
	}

	LOG_TRACE("WaitingHideSeekContainer::CreateFullTeam type=%d size=%d, team_count=%d", type, subteamcount, subteam_reverse_map.size());
	
	time_t now = Timer::GetTime();
	SubTeamReverseMultMap::const_iterator it,it2, ite = subteam_reverse_map.end();
	TeamRoleList rolelist;
	for (it = subteam_reverse_map.begin(); it != ite;)
	{
		TeamRoleList::const_iterator tit, tite = (it->second)->GetRoleList().end();
		for (tit = (it->second)->GetRoleList().begin(); tit!=tite; tit++)
		{
			LOG_TRACE("WaitingHideSeekContainer::CreateFullTeam insert rolelist roleid=%d", (*tit)->GetRoleid());
			ApplyingRole * prole = new ApplyingRole();
			prole->role = (*tit)->role;
			rolelist.push_back(prole);
		}

		//rolelist.insert(rolelist.end(), (it->second)->GetRoleList().begin(), (it->second)->GetRoleList().end());
		LOG_TRACE("WaitingHideSeekContainer::CreateFullTeam type=%d, id %d size %d match success,use time %d", 
				type, it->second->GetId(), (it->second)->GetSize(), now - (it->second)->GetCreateTime());

		it2 = it;
		++it;
		DelAllFromSubList(it2->second->GetId()); // 删除subteam_reverse_map中所有对象	
	}

	HideSeekRaidBase* fullteam = new HideSeekRaidBase(rolelist, type, this);

	return fullteam;
}

int 	WaitingHideSeekContainer::GetPrepareMapSize()// 等待队伍列表中玩家个数
{
	int count = 0;
	SubTeamReverseMultMap::iterator it = prepare_reverse_map.begin(), ite = prepare_reverse_map.end();
	for (; it != ite; ++it)
	{
		count += it->second->GetSize();
	}
	return count;
}

int WaitingHideSeekContainer::AddSubMap(HideSeekRaidBase* subteam)
{
	//time_t now = Timer::GetTime();
	subteam_reverse_map.insert(std::make_pair(subteam->GetId(), subteam));
	subteamcount += subteam->GetSize();
	AddRoleRaidBaseMap(subteam);

	LOG_TRACE("AddSubMap id %d, subteamcount=%d, GetSize=%d, role_raidbase_map size=%d", 
			subteam->GetId(), subteamcount, subteam->GetSize(), role_raidbase_map.size());
	return 0;

}

int WaitingHideSeekContainer::AddPrepareMap(HideSeekRaidBase* subteam)
{
	//time_t now = Timer::GetTime();
	prepare_reverse_map.insert(std::make_pair(subteam->GetId(), subteam));
	AddRoleRaidBaseMap(subteam);
	LOG_TRACE("AddPrepareMap id %d, subteamcount=%d, GetSize=%d, role_raidbase_map size=%d", 
			subteam->GetId(), subteamcount, subteam->GetSize(), role_raidbase_map.size());

	return 0;
}

void WaitingHideSeekContainer::AddRoleRaidBaseMap(HideSeekRaidBase* subteam)
{
	TeamRoleList::const_iterator it = subteam->GetRoleList().begin(), ite = subteam->GetRoleList().end();
	for (; it != ite; ++it)
	{
		role_raidbase_map[(*it)->GetRoleid()] = subteam->GetId();
	}
}

void WaitingHideSeekContainer::EraseRoleRaidBaseMap(HideSeekRaidBase* subteam)
{
	TeamRoleList::const_iterator it = subteam->GetRoleList().begin(), ite = subteam->GetRoleList().end();
	for (; it != ite; ++it)
	{
		role_raidbase_map.erase((*it)->GetRoleid());
	}
	subteam->DeleteRoleList();
}

void WaitingHideSeekContainer::EraseRoleRaidBaseMap(HideSeekRaidBase* subteam, int roleid)
{
	TeamRoleList::const_iterator it = subteam->GetRoleList().begin(), ite = subteam->GetRoleList().end();
	for (; it != ite; ++it)
	{
		if((*it)->GetRoleid() == roleid)
		{
			role_raidbase_map.erase((*it)->GetRoleid());
			break;
		}
	}
	subteam->DelRole(roleid);
}


int WaitingHideSeekContainer::GetRoleHideSeekRaidId(int roleid)
{
	RoleRaidBaseMap::iterator it = role_raidbase_map.find(roleid);
	if(it==role_raidbase_map.end()){
		return -1;
	}
	return it->second;
}

HideSeekRaidBase* WaitingHideSeekContainer::GetHideSeekRaid(int id)
{
	SubTeamReverseMultMap::iterator it = prepare_reverse_map.find(id);
	if(it != prepare_reverse_map.end())
	{
		return it->second;
	}

	it = subteam_reverse_map.find(id);
	if(it != subteam_reverse_map.end())
	{
		return it->second;
	}
	
	return NULL;
}

void WaitingTeamContainer::OnClosing(time_t now)
{
	LOG_TRACE("WaitingTeamContainer OnClosing");
	status = STATUS_CLOSING;
	status_end_time = now + CLOSING_TIME;
}
void WaitingTeamContainer::OnClosed(time_t now)
{
	LOG_TRACE("WaitingTeamContainer OnClosed");
	status = STATUS_CLOSED;
	status_end_time = FindNextOpenTime(now);
	ClearAll();
}
void WaitingTeamContainer::OnOpen(time_t now)
{
	LOG_TRACE("WaitingTeamContainer OnOpen");
	time_t endtime = 0;
	if (IsOpenTime(now, endtime))
	{
		status = STATUS_OPEN;
		status_end_time = endtime;
	}
	else
	{
		Log::log(LOG_ERR, "now %d is not opentime", now);
		status = STATUS_CLOSED;
		status_end_time = FindNextOpenTime(now);
	}
}
void WaitingTeamContainer::UpdateStatus(time_t now)
{
	/*LOG_TRACE("WatingTeamContainer status %d status_end_time %d",
			status, status_end_time);*/
	if (now < status_end_time)
		return;
	switch(status)
	{
		case STATUS_OPEN:
			OnClosing(now);
			break;
		case STATUS_CLOSING:
			OnClosed(now);
			break;
		case STATUS_CLOSED:
			OnOpen(now);
			break;
		default:
			Log::log(LOG_ERR, "WaitingTeamContainer unknow status %d", status);
			break;
	}
}
int WaitingTeamContainer::Update(int now)
{
	UpdateStatus(now);
	std::vector<std::pair<int, int> >checksublist;
	for(SubTeamReverseMap::iterator it=subteam_reverse_map.begin();it!=subteam_reverse_map.end();)
	{
		if(it->first>=now)
			break;
		//LOG_TRACE("Raid update now=%d, eventtime=%d checkid %d subteam_reverse_map size=%d",now, it->first, it->second.second, subteam_reverse_map.size());
		checksublist.push_back(it->second);
		subteam_reverse_map.erase(it++);
	}
	std::vector<std::pair<int, int> >::iterator cit, cite = checksublist.end();
	for (cit = checksublist.begin(); cit != cite; ++cit)
	{
		int size = cit->first;
		int id = cit->second;
		TeamList & list = subteampool[size];
		TeamList::iterator tit, tite = list.end();
		for (tit = list.begin(); tit != tite; ++tit)
		{
			if ((*tit)->GetId() == id)
			{
				//LOG_TRACE("real check subteam %d", id);
				RaidTeamBase * subteam = *tit;
				list.erase(tit);//先从pool 删除 再插入
				subteam->ExtendRange();
				AddSubTeam(subteam);
				break;
			}
		}
	}

	std::vector<int> checkfulllist;
	for(FullTeamReverseMap::iterator it2=fullteam_reverse_map.begin();it2!=fullteam_reverse_map.end();)
	{
		if(it2->first>=now)
			break;
		LOG_TRACE("Raid update now=%d, eventtime=%d checkid %d fullteam_reverse_map size=%d",now, it2->first, it2->second, fullteam_reverse_map.size());
		checkfulllist.push_back(it2->second);
		fullteam_reverse_map.erase(it2++);
	}
	//LOG_TRACE("fullteam size %d", fullteamlist.size());
	std::vector<int>::iterator cit2, cite2 = checkfulllist.end();
	for (cit2 = checkfulllist.begin(); cit2 != cite2; ++cit2)
	{
		int id = *cit2;
		FullTeamList::iterator tit2, tite2 = fullteamlist.end();
		for (tit2 = fullteamlist.begin(); tit2 != tite2; ++tit2)
		{
			if ((*tit2)->GetId() == id)
			{
				//LOG_TRACE("real check fullteam %d", id);
				RaidTeamBase * fullteam = *tit2;
				fullteamlist.erase(tit2);
				LOG_TRACE("fullteam size %d after erase %d", fullteamlist.size(), id);
				fullteam->ExtendRange();
				AddFullTeam(fullteam);
				break;
			}
		}
	}
/*
	for(NewFullTeamPool::iterator it=newfullteams.begin();it!=newfullteams.end();)
	{
//		if(now - it->second->GetCreateTime() > CHOOSECHAR_TIMEOUT)
		if(now - it->second->GetCreateTime() > SCORE_MATCH_TIME)
		{
			fullpool.push_back(it->second);
			newfullteams.erase(it++);
		}
		else
			++it;
	}
	CheckFullPool();
	*/
	return ERR_SUCCESS;
}
/*
int WaitingTeamContainer::CheckFullPool()
{
	while(fullpool.size()>1)
	{
		LOG_TRACE("Make raid team mapping in FullPool");
		int ret=RaidManager::GetInstance()->AddTeamRaidRoom(map_id);
		if(ret!=ERR_SUCCESS)
			break;

		if(RaidManager::GetInstance()->OnTeamRaidMappingSuccess(fullpool[0]->GetRoleList(),fullpool[1]->GetRoleList(),map_id) != ERR_SUCCESS)
		{
			LOG_TRACE("CheckFullPool Add raid err");
		}
		delete fullpool[0];
		delete fullpool[1];
		fullpool.pop_front();
		fullpool.pop_front();
	}
	return ERR_SUCCESS;
}
*/
/*
void WaitingTeamContainer::BuildTeamPool()
{
	teampool[1];
	for(size_t i=2;i<TEAMRAID_MEMBER_LIMIT;i++)
		teampool[i];
}
*/
int WaitingTeamContainer::CrssvrTeamsQuit(int id)
{
	{
		FullTeamList::iterator it,ite=fullteamlist.end();
		for(it=fullteamlist.begin();it!=ite;++it)
		{
			int teamid = (*it)->GetTeamId();
			LOG_TRACE("WaitingTeamContainer::CrssvrTeamsQuit fullteamlist teamid=%d, id=%d", teamid, id);
			if( teamid == id )
			{
				DelFromFullTimeList(teamid);
				RaidTeamBase * newteam = *it;
				if(newteam)
				{
					RaidQuit_Re quit_re(0);
					newteam->BroadRoleList(quit_re, quit_re.localsid, quit_re.roleid);
					newteam->DeleteRoleList();
					delete newteam;
					newteam = NULL;
				}
				fullteamlist.erase(it);
				return 0;
			}
		}
	}
	return -1;
}

int WaitingTeamContainer::RoleQuit(int roleid)
{
	for(size_t i=1;i<TEAMRAID_MEMBER_LIMIT;i++)
	{
		TeamList & tlist = subteampool[i];
		TeamList::iterator it1=tlist.begin(),ite1=tlist.end();
		for(;it1!=ite1;++it1)
		{
			LOG_TRACE("WaitingTeamContainer::RoleQuit tlist roleid=%d", roleid);
			if ((*it1)->DelRole(roleid))
			{
				std::set<int> ids;
				ids.insert((*it1)->GetId());
				DelFromSubTimeList(ids);
				RaidTeamBase * newteam = *it1;
				tlist.erase(it1); //先从pool删除 再插入
				if (newteam->GetSize())
					AddSubTeam(newteam, true);
				else
					delete newteam;
				return 0;
			}
		}
	}
	{
		FullTeamList::iterator it,ite=fullteamlist.end();
		for(it=fullteamlist.begin();it!=ite;++it)
		{
			LOG_TRACE("WaitingTeamContainer::RoleQuit fullteamlist roleid=%d", roleid);
			if((*it)->DelRole(roleid))
			{
				DelFromFullTimeList((*it)->GetId());
				RaidTeamBase * newteam = *it;
				fullteamlist.erase(it);
				if (newteam->GetSize())
					AddSubTeam(newteam, true);
				else
					delete newteam;
				return 0;
			}
		}
	}
	return -1;
}

int WaitingTeamContainer::AddTeam(RaidTeamBase * team, bool match_later)
{
	size_t size = team->GetSize();
	if(size == TEAMRAID_MEMBER_LIMIT)
	{
		return AddFullTeam(team, match_later);
	}
	else
		return AddSubTeam(team, match_later);
}

int WaitingTeamContainer::AddSubTeam(RaidTeamBase * subteam, bool match_later)
{
	size_t size = subteam->GetSize();
	time_t now = Timer::GetTime();
	if(subteam->GetType() == CRSSVRTEAMS_RAID)
	{
		LOG_TRACE("WaitingTeamContainer::AddSubTeam err id %d", subteam->GetId());

	}
	if (match_later)
	{
		subteampool[size].push_back(subteam);
		subteam_reverse_map.insert(std::make_pair(now+CHECK_INTERVAL, std::make_pair(size, subteam->GetId())));
		return ERR_SUCCESS;
	}
	if(size > TEAMRAID_MEMBER_LIMIT/2 && !subteampool[size].empty())
	{
		subteampool[size].push_back(subteam);
		subteam_reverse_map.insert(std::make_pair(now+CHECK_INTERVAL, std::make_pair(size, subteam->GetId())));
		return ERR_SUCCESS;
	}
	int extra_delta = subteam->GetExtraDelta();
	const PolicyList & policy = teampolicies[size]; //假设 count = 3
	PolicyList::const_iterator it, ite = policy.end();
	for (it = policy.begin(); it != ite; ++it) //{3}, {2,1}, {1,1,1}
	{
		bool match = true;
		PolicySet::const_iterator sit = it->begin(), site = it->end();
		std::map<int/*entry_team_count*/, std::vector<TeamListIT> > matchindex;
		std::map<unsigned char/*occup*/, int/*count*/> occup_count(subteam->GetOccupCount());
		while( sit != site)
		{
			int team_shard_size = *sit;
			int shard_count = 1;
			++sit;
			while( sit != site && *sit == team_shard_size)
			{
				shard_count++;
				++sit;
			}//计算需要 size为 team_shared_size 的subteam几个
			TeamList & entry = subteampool[team_shard_size];
			if ((int)entry.size() < shard_count)
			{
				match = false;
				break;
			}
			TeamListIT it, ite = entry.end();
//			int index = 0;
			for (it = entry.begin(); it != ite && shard_count>0; ++it/*, ++index*/)
			{
				if (subteam->ScoreMatch((*it)->GetScore())
					&& (*it)->ScoreMatch(subteam->GetScore(), extra_delta)
					&& (*it)->OccupMatch(occup_count))
				{
					shard_count--;
//					matchindex[team_shard_size].push_back(index);
					matchindex[team_shard_size].push_back(it);
					//使用贪婪算法 只要有 teamfrag 符合职业条件 就把它加进来 不是最优的
					std::map<unsigned char, int> it_occup = (*it)->GetOccupCount();
					std::map<unsigned char, int>::const_iterator oit, oite = it_occup.end();
					for (oit = it_occup.begin(); oit != oite; ++oit)
						occup_count[oit->first] += oit->second;
				}
			}
			if (shard_count > 0)
			{
				match = false;
				break;
			}
		}

		if (match)//build fullteam
		{
			if (extra_delta > 0)
				LOG_TRACE("ScoreMatch teamfrag %d selfscore %d need extra_delta %d",
						subteam->GetId(), subteam->GetScore(), extra_delta);
			TeamList subteams;
			subteams.push_back(subteam);
			std::map<int, std::vector<TeamListIT> >::const_iterator iit, iite = matchindex.end();
			LOG_TRACE("coming teamsize %d", size);
			for (iit = matchindex.begin(); iit != iite; ++iit)
			{
				TeamList & entry = subteampool[iit->first];
				std::vector<TeamListIT>::const_iterator sit, site = iit->second.end();
				for (sit = iit->second.begin(); sit != site; ++sit)
				{
					LOG_TRACE("\tmatch a %d size teamfrag", iit->first);
					subteams.push_back(*(*sit));
					entry.erase(*sit);
				}
			}
			//success = true;
			RaidTeamBase * fullteam = CreateFullTeam(subteams);
			if (fullteam == NULL)
				return -2;
			return AddFullTeam(fullteam);
		}
	}
	subteampool[size].push_back(subteam);
	subteam_reverse_map.insert(std::make_pair(now+CHECK_INTERVAL, std::make_pair(size, subteam->GetId())));
	return ERR_SUCCESS;
}

RaidTeamBase * WaitingTeamContainer::CreateFullTeam(const TeamList& subteamlist)
{
	//LOG_TRACE("CreateFullTeam");
	TeamList::const_iterator it, ite = subteamlist.end();
	TeamRoleList rolelist;
	std::set<int> del_idlist;
	time_t now = Timer::GetTime();
	for (it = subteamlist.begin(); it != ite; ++it)
	{
		rolelist.insert(rolelist.end(), (*it)->GetRoleList().begin(), (*it)->GetRoleList().end());
		del_idlist.insert((*it)->GetId());
		LOG_TRACE("teamfrag %d size %d score %d match success,use time %d", (*it)->GetId(), (*it)->GetSize(), (*it)->GetScore(), now - (*it)->GetCreateTime());
		delete (*it);
	}
	DelFromSubTimeList(del_idlist);	
	if (rolelist.size() != TEAMRAID_MEMBER_LIMIT)
	{
		Log::log(LOG_ERR, "createfullteam member size %d invalid", rolelist.size());
		TeamRoleList::iterator rit, rite = rolelist.end();
		for (rit = rolelist.begin(); rit != rite; ++rit)
			delete (*rit);
		return NULL;
	}
	RaidTeamBase * fullteam = new RaidTeamFrag(rolelist/*, type*/, this);
	std::map<unsigned char, int> occup_count = fullteam->GetOccupCount();
	std::map<unsigned char, int>::const_iterator oit, oite = occup_count.end();
	for (oit = occup_count.begin(); oit != oite; ++oit)
	{
		if (oit->second > RaidTeamBase::OCCUPATION_PLAYER_LIMIT)
		{
			Log::log(LOG_ERR, "createfullteam occup %d count %d invalid", oit->first, oit->second);
			assert(false);
		}
	}
	
	return fullteam;
}

int WaitingTeamContainer::AddFullTeam(RaidTeamBase * fullteam, bool match_later)
{
	//LOG_TRACE("WaitingTeamContainer::AddFullTeam match_later=%d",match_later);
	time_t now = Timer::GetTime();
	if (match_later)
	{
		fullteamlist.push_back(fullteam);
		fullteam_reverse_map.insert(std::make_pair(now+CHECK_INTERVAL, fullteam->GetId()));
		return ERR_SUCCESS;
	}	
	// add room or return
	int ret=RaidManager::GetInstance()->AddTeamRaidRoom(map_id, fullteam->GetType());
	if(ret!=ERR_SUCCESS)
	{
		LOG_TRACE("WaitingTeamContainer::AddFullTeam AddTeamRaidRoom, map_id=%d, fullteam type=%d, ret=%d",map_id, fullteam->GetType(), ret);
		fullteamlist.push_back(fullteam);
		fullteam_reverse_map.insert(std::make_pair(now+CHECK_INTERVAL, fullteam->GetId()));
		return ERR_SUCCESS;
	}	

	LOG_TRACE("fullteam size %d before match", fullteamlist.size());
	int extra_delta = fullteam->GetExtraDelta();
	FullTeamList::iterator it, ite = fullteamlist.end();
	for (it = fullteamlist.begin(); it != ite; ++it)
	{
		if (fullteam->ScoreMatch((*it)->GetScore()) &&
					(*it)->ScoreMatch(fullteam->GetScore(), extra_delta))
			break;
	}
	if(it!=ite)
	{
		RaidTeamBase * matchteam = *it;
		fullteamlist.erase(it);
		if (extra_delta > 0)
			LOG_TRACE("ScoreMatch fullteam %d selfscore %d need extra_delta %d",
					fullteam->GetId(), fullteam->GetScore(), extra_delta);
		LOG_TRACE("fullteam %d score %d type %d match success,use time %d", fullteam->GetId(), fullteam->GetScore(), fullteam->GetType(), now - fullteam->GetCreateTime());
		LOG_TRACE("fullteam %d score %d type %d match success,use time %d", matchteam->GetId(), matchteam->GetScore(), matchteam->GetType(), now - matchteam->GetCreateTime());
		DelFromFullTimeList(matchteam->GetId());
		RaidManager::GetInstance()->OnTeamRaidMappingSuccess(fullteam, matchteam, map_id);
		return ERR_SUCCESS;
	}
	
	//del room
	RaidManager::GetInstance()->DecTeamRaidRoom(map_id, fullteam->GetType());
	fullteamlist.push_back(fullteam);
	fullteam_reverse_map.insert(std::make_pair(now+CHECK_INTERVAL, fullteam->GetId()));
	return ERR_SUCCESS;
}

void 	WaitingTeamContainer::DelFromSubTimeList(const std::set<int> & ids)
{
	SubTeamReverseMap::iterator it, ite = subteam_reverse_map.end();
	for (it = subteam_reverse_map.begin(); it != ite;)
	{
		if (std::find(ids.begin(), ids.end(), it->second.second) != ids.end())
		{
			LOG_TRACE("real delete id %d from subtimelist", it->second.second);
			subteam_reverse_map.erase(it++);
		}
		else
			++it;
	}
}

void 	WaitingTeamContainer::DelFromFullTimeList(int id)
{
	FullTeamReverseMap::iterator it, ite = fullteam_reverse_map.end();
	for (it = fullteam_reverse_map.begin(); it != ite;)
	{
		if (id == it->second)
		{
			LOG_TRACE("real delete id %d from fulltimelist", it->second);
			fullteam_reverse_map.erase(it++);
		}
		else
			++it;
	}
}

int RaidManager::AddRaidRoom(int map_id,int type)
{
	RaidMap::iterator it = raidmap.find(map_id);
	if (it == raidmap.end())
		return ERR_RAID_MAP_NOTEXIST;
	if(it->second->RaidType() != type)
		return ERR_RAID_TYPE;
	if(it->second->Add_Room() == false)
		return ERR_RAID_ROOM_NUMBER_FULL; 
	return ERR_SUCCESS;
}
int RaidManager::DecRaidRoom(int map_id,int type)
{
	RaidMap::iterator it = raidmap.find(map_id);
	if (it == raidmap.end())
		return ERR_RAID_MAP_NOTEXIST;
	if(it->second->RaidType() != type)
		return ERR_RAID_TYPE;
	it->second->Dec_Room();
	return ERR_SUCCESS;
}

void RaidManager::SetDebugMode(bool debug)
{
	LOG_TRACE("RaidManager::SetDebugMode debug=%d", debug);
	raidmanager_debug_mode = debug;
}
/*
WaitingTeamContainer::NewFullTeamPool::iterator WaitingTeamContainer::MatchFullTeam(int score)
{
	NewFullTeamPool::iterator it, ite = newfullteams.end();
	for (it = newfullteams.begin(); it != ite; ++it)
	{
		if (
	return tmpit;
}
*/
void WaitingTeamContainer::BuildTeamMappingPolicy(/*TeamPolicy & policies*/)
{
	/*  <halfteamsize, need_count>
	5: {{<1,1>}} # 1*1 = 1
	4: {{<2,1>},{<1,2>} # 2*1 = 2; 1*2 = 2
	3: {{<3,1>},{<2,1>,<1,1>},{<1,3>}} # 3*1 = 3; 2*1+1*1 = 3; 1*3 = 3
	2: {{<4,1>},{<3,1>,<1,1>},{<2,2>},{<2,1>,<1,2>},{<1,4>}} # 4*1 = 4; 3*1+1*1 = 4; 2*2 = 4; 2*1+1*2 = 4; 1*4 = 4
	1: {{<5,1>},{<4,1>,<1,1>},{<3,1>,<2,1>},{<3,1>,<1,2>},{<2,2>,<1,1>},{<2,1>,<1,3>},{<1,5>}} # 5*1 = 5; 4*1+1*1=5 ...
	*/
	/* 
	5: {{1}}
	4: {{<2,1>},{<1,2>} # 2*1 = 2; 1*2 = 2
	3: {{<3,1>},{<2,1>,<1,1>},{<1,3>}} # 3*1 = 3; 2*1+1*1 = 3; 1*3 = 3
	2: {{<4,1>},{<3,1>,<1,1>},{<2,2>},{<2,1>,<1,2>},{<1,4>}} # 4*1 = 4; 3*1+1*1 = 4; 2*2 = 4; 2*1+1*2 = 4; 1*4 = 4
	1: {{<5,1>},{<4,1>,<1,1>},{<3,1>,<2,1>},{<3,1>,<1,2>},{<2,2>,<1,1>},{<2,1>,<1,3>},{<1,5>}} # 5*1 = 5; 4*1+1*1=5 ...
	*/

//	assert(6 == TEAMRAID_MEMBER_LIMIT);
	//test,to do
	teampolicies.clear();
	switch(TEAMRAID_MEMBER_LIMIT)
	{
	case 1:	
	case 2:	
	{
		PolicyList * ref1 = NULL;
		ref1 = &(teampolicies[1]);
		PolicySet tmp_set1;
		tmp_set1.insert(1);
		ref1->push_back(tmp_set1); tmp_set1.clear();
		break;
	}	
	case 3:
	{
		PolicyList * ref = NULL;
		ref = &(teampolicies[2]);
		PolicySet tmp_set;
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();

		ref = &(teampolicies[1]);
		tmp_set.insert(2);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
	}
	break;
	case 4:
	{
		PolicyList * ref = NULL;
		ref = &(teampolicies[3]);
		PolicySet tmp_set;
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();

		ref = &(teampolicies[2]);
		tmp_set.insert(2);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();

		ref = &(teampolicies[1]);
		tmp_set.insert(3);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(2);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
	}
	break;
	case 6:
	{	
		PolicyList * ref = NULL;
		ref = &(teampolicies[5]);
		PolicySet tmp_set;
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		
		ref = &(teampolicies[4]);
		tmp_set.insert(2);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
	
		ref = &(teampolicies[3]);
		tmp_set.insert(3);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(2);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
	
		ref = &(teampolicies[2]);
		tmp_set.insert(4);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(3);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(2);
		tmp_set.insert(2);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(2);
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
	
		ref = &(teampolicies[1]);
		tmp_set.insert(5);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(4);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(3);
		tmp_set.insert(2);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(3);
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(2);
		tmp_set.insert(2);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(2);
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		tmp_set.insert(1);
		ref->push_back(tmp_set); tmp_set.clear();
		break;
	}
	default:
		LOG_TRACE("TEAMRAID_MEMBER_LIMIT size=%d err",TEAMRAID_MEMBER_LIMIT);
		exit(-1);
		break;
	}
}

bool WaitingTeamContainer::IsOpenTime(time_t now, time_t & end_time)
{
	struct tm dt;
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 0;
	time_t midnight = mktime(&dt);
	int time_diff = now - midnight;
	OpenTimePeriods::const_iterator it, ite = open_times.end();
	for (it = open_times.begin(); it != ite; ++it)
	{
		if (time_diff >= it->first && time_diff < it->second)
		{
			end_time = midnight + it->second;
			return true;
		}
	}
	return false;
}

time_t WaitingTeamContainer::FindNextOpenTime(time_t now)
{
	if (open_times.empty())
		return 0;
	struct tm dt;
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 0;
	time_t midnight = mktime(&dt);
	int time_diff = now - midnight;
	OpenTimePeriods::const_iterator it, ite = open_times.end();
	for (it = open_times.begin(); it != ite; ++it)
	{
		if (time_diff < it->first)
			return midnight + it->first;
	}
	return midnight + 86400 + open_times.front().first;
}

};

