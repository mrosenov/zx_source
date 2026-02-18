
#ifndef __GNET_RAIDKICKNOTIFY_HPP
#define __GNET_RAIDKICKNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidKickNotify : public GNET::Protocol
{
	#include "raidkicknotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidKickNotify roleid=%d, raidroom_id=%d, reason=%d", roleid, raidroom_id, reason);
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
