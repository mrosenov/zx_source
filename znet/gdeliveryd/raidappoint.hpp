
#ifndef __GNET_RAIDAPPOINT_HPP
#define __GNET_RAIDAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "raidappoint_re.hpp"

namespace GNET
{

class RaidAppoint : public GNET::Protocol
{
	#include "raidappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidAppoint roleid=%d, target=%d raidroom_id=%d",roleid, target, raidroom_id);
		int retcode = RaidManager::GetInstance()->Appoint(roleid,raidroom_id,target,false);
//		if(retcode != ERR_SUCCESS)
		GDeliveryServer::GetInstance()->Send(sid,RaidAppoint_Re(retcode,roleid,raidroom_id,target,localsid));
	}
};

};

#endif
