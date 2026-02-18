
#ifndef __GNET_RAIDSTARTNOTIFY_HPP
#define __GNET_RAIDSTARTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidStartNotify : public GNET::Protocol
{
	#include "raidstartnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidStartNotify gs_id=%d, map_id=%d, raidroom_id=%d, localsid=%d, roleid=%d", gs_id, map_id, raidroom_id, localsid, roleid);
		PlayerInfo *info = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(info != NULL && info->ingame)
		{
			localsid = info->localsid;
			GDeliveryServer::GetInstance()->Send(info->linksid, this);
		}
	}
};

};

#endif
