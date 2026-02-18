
#ifndef __GNET_TERRITORYSERVERREGISTER_HPP
#define __GNET_TERRITORYSERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "territoryserver"

namespace GNET
{

class TerritoryServerRegister : public GNET::Protocol
{
	#include "territoryserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
