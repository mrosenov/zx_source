
#ifndef __GNET_GFACTIONPKRAIDFIGHTNOTIFY_HPP
#define __GNET_GFACTIONPKRAIDFIGHTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFactionPkRaidFightNotify : public GNET::Protocol
{
	#include "gfactionpkraidfightnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
