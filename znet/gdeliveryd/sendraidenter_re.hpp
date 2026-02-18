
#ifndef __GNET_SENDRAIDENTER_RE_HPP
#define __GNET_SENDRAIDENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "raidenter_re.hpp"
//#include "playerchangegs.hpp"
namespace GNET
{

class SendRaidEnter_Re : public GNET::Protocol
{
	#include "sendraidenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SendRaidEnter_Re:roleid=%d,index=%d,retcode=%d,raid_type=%d",roleid, index, retcode, raid_type);
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL)
			return;
		if (retcode != ERR_SUCCESS)
		{
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, RaidEnter_Re(ERR_RAID_GS_ENTER_CHECK_FAIL, pinfo->localsid));
			RaidManager::GetInstance()->OnGSEnterErr(gs_id,index,roleid);
		}
		if (retcode == ERR_SUCCESS)
//			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, dst_gs, index));
		{
			if (!dsm->IsCentralDS() && (raid_type == CRSSVRTEAMS_RAID || raid_type == TEAM_RAID))// 如果是原服收到
			{
				ProtocolExecutor *task = new ProtocolExecutor(GProviderServer::GetInstance(), 0, new TryChangeDS(roleid, DS_TO_BATTLEMAP));
				Thread::Pool::AddTask(task);
				return;
			}

			int retcode1 = RaidManager::GetInstance()->OnGsRaidEnter(roleid,gs_id);
			if(retcode1 != ERR_SUCCESS)
				GDeliveryServer::GetInstance()->Send(pinfo->linksid, RaidEnter_Re(retcode1, pinfo->localsid));
		}
	
	
	}
};

};

#endif
