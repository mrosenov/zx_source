
#ifndef __GNET_SENDRAIDKICKOUT_HPP
#define __GNET_SENDRAIDKICKOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

extern void raid_kickout(int role_id, int index);

namespace GNET
{

class SendRaidKickout : public GNET::Protocol
{
	#include "sendraidkickout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		raid_kickout(roleid, index);
	}
};

};

#endif
