
#ifndef __GNET_GTERRITORYCHALLENGE_HPP
#define __GNET_GTERRITORYCHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GTerritoryChallenge : public GNET::Protocol
{
	#include "gterritorychallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
