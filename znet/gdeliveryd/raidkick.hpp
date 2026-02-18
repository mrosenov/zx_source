
#ifndef __GNET_RAIDKICK_HPP
#define __GNET_RAIDKICK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "raidkick_re.hpp"

namespace GNET
{

class RaidKick : public GNET::Protocol
{
	#include "raidkick"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidKick roleid=%d, target=%d raidroom_id=%d",roleid, target, raidroom_id);
		int retcode = RaidManager::GetInstance()->Kick(roleid,raidroom_id,target,reason);
//		if(retcode != ERR_SUCCESS)
		GDeliveryServer::GetInstance()->Send(sid,RaidKick_Re(retcode,target,raidroom_id,localsid));
	}
};

};

#endif
