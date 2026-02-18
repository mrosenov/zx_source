
#ifndef __GNET_GTERRITORYLEAVE_HPP
#define __GNET_GTERRITORYLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTerritoryLeave : public GNET::Protocol
{
	#include "gterritoryleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
