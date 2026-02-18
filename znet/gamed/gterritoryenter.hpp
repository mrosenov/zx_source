
#ifndef __GNET_GTERRITORYENTER_HPP
#define __GNET_GTERRITORYENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTerritoryEnter : public GNET::Protocol
{
	#include "gterritoryenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
