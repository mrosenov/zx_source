
#ifndef __GNET_GFACTIONPKRAIDKILL_HPP
#define __GNET_GFACTIONPKRAIDKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFactionPkRaidKill : public GNET::Protocol
{
	#include "gfactionpkraidkill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
