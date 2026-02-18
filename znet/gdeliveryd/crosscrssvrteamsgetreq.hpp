
#ifndef __GNET_CROSSCRSSVRTEAMSGETREQ_HPP
#define __GNET_CROSSCRSSVRTEAMSGETREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "crssvrteamsmanager.h"
#include "synccrosscrssvrteams.hrp"
#include "centraldeliveryclient.hpp"

namespace GNET
{

class CrossCrssvrTeamsGetReq : public GNET::Protocol
{
	#include "crosscrssvrteamsgetreq"

	void TeamNotOnline(int roleid, int teamid)
	{
		PlayerInfo* pinfo = UserContainer::GetInstance().FindRole(roleid);
		if(!pinfo)
			return;

		CrssvrTeamsManager::Instance()->OnLogin(roleid, teamid, true);
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrossCrssvrTeamsGetReq, roleid=%d,req_type=%d", roleid, req_type);

		if(req_type == 0)// 请求战队完整信息
		{
			GCrssvrTeamsData info;
			TTeamRaidRoleBrief rolebrief;
			bool isget = CrssvrTeamsManager::Instance()->GetCrssvrTeamsData(teamid, info, rolebrief);
			if(isget)
			{
				LOG_TRACE("recv CrossCrssvrTeamsGetReq, roleid=%d, teamid=%d, info.teamid=%d, zoneid=%d, master=%d, score=%d, battle_losses=%d, battle_draws=%d, battle_wins=%d, changenametime=%d,isget=%d",
					roleid, info.info.teamid, info.info.teamid, info.info.zoneid, info.info.master, info.info.score, info.info.battle_losses, info.info.battle_draws, info.info.battle_wins, info.info.changenametime, isget);

				CrssvrTeamsManager::Instance()->UpdateRoleOnline(info.info.teamid, roleid, CRSSVRTEAMS_CROSS_ONLINE_STATUS);
				int zoneid = GDeliveryServer::GetInstance()->zoneid;
				SyncCrossCrssvrTeams* rpc = (SyncCrossCrssvrTeams*) Rpc::Call(RPC_SYNCCROSSCRSSVRTEAMS,	SyncCrossCrssvrTeamsArg(zoneid, teamid, info));
				CentralDeliveryClient::GetInstance()->SendProtocol(rpc);
			}
			else // 原服玩家数据还未加载
			{
				TeamNotOnline(roleid, teamid);
			}

		}
		else if(req_type == 1)
		{
			int err_retcode = CrssvrTeamsManager::Instance()->SyncRoleInfo2Central(roleid, teamid);
			if(err_retcode == ERR_CRSSVRTEAMS_NOTEXIST)
			{
				TeamNotOnline(roleid, teamid);
			}
		}
		else
		{
			Log::log( LOG_ERR, "CrossCrssvrTeamsGetReq, roleid=%d,req_type=%d is false", roleid, req_type);
		}
	
	}
};

};

#endif
