
#ifndef __GNET_GTRECONNECT_HPP
#define __GNET_GTRECONNECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTReconnect : public GNET::Protocol
{
	#include "gtreconnect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
