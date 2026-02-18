
#ifndef __GNET_NETBARREWARD_HPP
#define __GNET_NETBARREWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class NetBarReward : public GNET::Protocol
{
	#include "netbarreward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
