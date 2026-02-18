
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
		LOG_TRACE("RaidKickNotify roleid=%d, raidroom_id=%d, reason=%d, localsid=%d", roleid, raidroom_id, reason, localsid);
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
