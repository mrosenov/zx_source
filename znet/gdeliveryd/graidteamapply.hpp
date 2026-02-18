
#ifndef __GNET_GRAIDTEAMAPPLY_HPP
#define __GNET_GRAIDTEAMAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"
#include "crossbattlemanager.hpp"
#include "centraldeliveryserver.hpp"
#include "teamraidrolebrief"

namespace GNET
{

class GRaidTeamApply : public GNET::Protocol
{
	#include "graidteamapply"

	void FillRaidTeamRoleBrief(std::vector<TeamRaidApplyingRole>& role_vec)
	{
		LOG_TRACE("FillRaidTeamRoleBrief role_vec.size=%d", role_vec.size());

		std::vector<TeamRaidApplyingRole>::iterator it, ite = role_vec.end();
		for(it = role_vec.begin(); it != ite; ++it)
		{
			int roleid = (*it).roleid;
			TeamRaidRoleBrief& brief = (*it).rolebrief;
			LOG_TRACE("FillRaidTeamRoleBrief roleid=%d", roleid);
			GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
			if (prole != NULL)
			{
				LOG_TRACE("FillRaidTeamRoleBrief into roleid=%d", roleid);

				brief.roleid = roleid;
				brief.occup = prole->occupation;
				brief.gender = prole->gender;
				brief.faceid = prole->faceid;
				brief.hairid = prole->hairid;
			}

		}
	}

	void FillGsidFromCaptain(int master, int& gsid)
	{	
		PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(master);
		if(!pinfo)
			return;
		gsid = pinfo->gameid;
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int roleid=0;
		unsigned char occup=0;
		int score = 0;
		if(roles.size()>0)
		{
			roleid = roles[0].roleid;
			occup = roles[0].occupation;
			score = roles[0].score;
		}
		LOG_TRACE("GRaidTeamApply mapid=%d leader=%d leader_occup=%d reborn %d teamsize=%d, teamid=%d, iscrssvrteams=%d, issrc=%d, role.size=%d, score=%d",
				map_id,roleid,occup,reborn_cnt,roles.size(), crssvrteamid, iscross, issrc, roles.size(), score);
	
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		if(!iscentral) // 在原服，需要将消息转发给跨服服务器
		{
			int zoneid = GDeliveryServer::GetInstance()->zoneid;
			(*this).issrc = true;
			(*this).src_zoneid = zoneid;
			FillGsidFromCaptain(roleid, (*this).src_gsid);
			// 原服的角色验证
			if(iscross)// 跨服6v6
			{
				int role_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
				std::vector<TeamRaidApplyingRole>::const_iterator tit,tite=roles.end();
				for (tit=roles.begin();tit!=tite;tit++)
				{
					PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((*tit).roleid);
					if(!pinfo)
						return;

					bool isExsit = CrssvrTeamsManager::Instance()->CheckRoleExsit(role_teamid, (*tit).roleid);
					//LOG_TRACE("GRaidTeamApply roleid=%d CheckRoleExsit isExsit=%d", (*tit).roleid, isExsit);
					if(!isExsit)
					{
						TeamRaidApply_Re applay_re(ERR_RAID_TEAM_APPLY_CRSSVRTEAM_HAVE_OTHER,0,pinfo->localsid,(*tit).roleid);
						dsm->Send(pinfo->linksid, applay_re);
						return;
					}
				}

				GCrssvrTeamsData teaminfo;
				TTeamRaidRoleBrief rolebrief;
				bool isget = CrssvrTeamsManager::Instance()->GetCrssvrTeamsData(role_teamid, teaminfo, rolebrief);
				if(isget)
				{
					SyncCrossCrssvrTeams* rpc = (SyncCrossCrssvrTeams*) Rpc::Call(RPC_SYNCCROSSCRSSVRTEAMS,	SyncCrossCrssvrTeamsArg(zoneid, role_teamid, teaminfo));
					CentralDeliveryClient::GetInstance()->SendProtocol(rpc);
				}
			
				LOG_TRACE("GRaidTeamApply send to Central leader=%d, role_teamid=%d, src_zonid=%d, src_gsid=%d", roleid, role_teamid, zoneid, (*this).src_gsid);
				(*this).crssvrteamid = role_teamid;
				FillRaidTeamRoleBrief((*this).roles);
			}

			CentralDeliveryClient::GetInstance()->SendProtocol(this);
			return;
		}

		if(iscentral) // 在跨服
		{
			int teamid = -1;
			if(!issrc) //不是 来自原服的消息
			{
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if(!pinfo)
					return;

				src_zoneid = pinfo->user->src_zoneid;
				src_gsid = pinfo->gameid;
				FillRaidTeamRoleBrief(roles);
				teamid = CrossCrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
			}
			else
			{
				teamid = crssvrteamid;
			}

			LOG_TRACE("GRaidTeamApply Recv On Central leader=%d, src_zoneid=%d, src_gsid=%d, teamid=%d, issrc=%d, iscross=%d, roles.size=%d", 
					roleid, src_zoneid, src_gsid, teamid, issrc, iscross, roles.size());
			if(!iscross)
			{
				RaidManager::GetInstance()->OnRaidTeamApply(map_id,roles,reborn_cnt, sid, src_zoneid, src_gsid);
			}
			else
			{
				RaidManager::GetInstance()->OnRaidTeamApply(map_id,roles,reborn_cnt, iscross, sid, src_zoneid, src_gsid, teamid, issrc);
			}
			return;
		}

	}
};

};

#endif
