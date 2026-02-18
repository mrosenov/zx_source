
#ifndef __GNET_RAIDJOINACCEPT_HPP
#define __GNET_RAIDJOINACCEPT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "raidjoinaccept_re.hpp"
namespace GNET
{

class RaidJoinAccept : public GNET::Protocol
{
	#include "raidjoinaccept"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidJoinAccept masterid=%d, raidroom_id=%d accept=%d requester=%d",masterid, raidroom_id,accept,requester);
		int retcode = RaidManager::GetInstance()->OnAccept(masterid, accept, raidroom_id, requester);
		GDeliveryServer::GetInstance()->Send(sid, RaidJoinAccept_Re(retcode, accept, requester, localsid));
	}
};

};

#endif
