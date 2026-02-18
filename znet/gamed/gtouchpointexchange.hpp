
#ifndef __GNET_GTOUCHPOINTEXCHANGE_HPP
#define __GNET_GTOUCHPOINTEXCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTouchPointExchange : public GNET::Protocol
{
	#include "gtouchpointexchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
