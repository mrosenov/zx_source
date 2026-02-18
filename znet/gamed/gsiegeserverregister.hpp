
#ifndef __GNET_GSIEGESERVERREGISTER_HPP
#define __GNET_GSIEGESERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GSiegeServerRegister : public GNET::Protocol
{
	#include "gsiegeserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
