
#ifndef __GNET_HIDESEEKRAIDQUIT_HPP
#define __GNET_HIDESEEKRAIDQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HideSeekRaidQuit : public GNET::Protocol
{
	#include "hideseekraidquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		//bool iscentral = dsm->IsCentralDS();
		{
			LOG_TRACE("HideSeekRaidQuit roleid=%d, map_id=%d, zoneid=%d", roleid, map_id, zoneid);
			
			PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if(pinfo==NULL)
			{
				LOG_TRACE("HideSeekRaidQuit roleid=%d is not online", roleid);
				return;
			}

			int retcode = RaidManager::GetInstance()->HideSeekApplyingRoleQuit(roleid);

			RaidQuit_Re quit_re(retcode,0);
			quit_re.roleid = roleid;
			quit_re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, quit_re);
		}
	}
};

};

#endif
