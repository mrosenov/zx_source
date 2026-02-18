
#ifndef __GNET_GKINGGETREWARD_HPP
#define __GNET_GKINGGETREWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingGetReward : public GNET::Protocol
{
	#include "gkinggetreward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo)
			return;
		int ret = KingdomManager::GetInstance()->GetReward(roleid);
		if (ret != ERR_SUCCESS)
		{
			manager->Send(sid, GKingGetReward_Re(ret, roleid));
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, KingGetReward_Re(ret, roleid, pinfo->localsid));
		}
		LOG_TRACE("gkinggetreward roleid %d ret %d", roleid, ret);
	}
};

};

#endif
