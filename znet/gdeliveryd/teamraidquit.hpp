
#ifndef __GNET_TEAMRAIDQUIT_HPP
#define __GNET_TEAMRAIDQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidQuit : public GNET::Protocol
{
	#include "teamraidquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		if(!iscentral) // 在原服，需要将消息转发给跨服服务器
		{
			LOG_TRACE("src recv TeamRaidQuit roleid=%d, map_id=%d, teamid=%d", roleid, map_id, teamid);
			int zoneid = GDeliveryServer::GetInstance()->zoneid;
			(*this).zoneid = zoneid;
	                CentralDeliveryClient::GetInstance()->SendProtocol(this);
		}
		else
		{
			LOG_TRACE("central recv TeamRaidQuit roleid=%d, map_id=%d, zoneid=%d, teamid=%d", roleid, map_id, zoneid, teamid);
		
			int retcode = 0;
			if(teamid > 0)
			{
				retcode = RaidManager::GetInstance()->TeamApplyingRoleQuit(roleid, teamid);
			}
			else
			{
				retcode = RaidManager::GetInstance()->TeamApplyingRoleQuit(roleid);
			}

			//if(retcode != ERR_SUCCESS)
			{
				RaidQuit_Re quit_re(retcode,0);
				quit_re.roleid = roleid;
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if(pinfo==NULL)
				{
					//跨服没有，转发到原服
					CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, quit_re);
				}
				else if(pinfo != NULL && pinfo->ingame) // 在跨服
				{
					LOG_TRACE("RaidManager::OnRaidTeamApply send to client, roleid=%d, localsid=%d, retcode=%d", roleid,  pinfo->localsid, retcode);
					quit_re.localsid = pinfo->localsid;
					GDeliveryServer::GetInstance()->Send(pinfo->linksid, quit_re);
				}
			}
			//GDeliveryServer::GetInstance()->Send(sid,RaidQuit_Re(retcode,0,localsid, roleid));
		}
	}
};

};

#endif
