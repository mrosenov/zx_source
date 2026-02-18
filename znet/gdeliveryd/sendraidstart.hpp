
#ifndef __GNET_SENDRAIDSTART_HPP
#define __GNET_SENDRAIDSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidgroupinfo"

namespace GNET
{

class SendRaidStart : public GNET::Protocol
{
	#include "sendraidstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
