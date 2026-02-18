
#ifndef __GNET_TRYRECONNECT_HPP
#define __GNET_TRYRECONNECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TryReconnect : public GNET::Protocol
{
	#include "tryreconnect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
