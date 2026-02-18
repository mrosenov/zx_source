
#ifndef __GNET_SYNCFACTIONLEVEL_HPP
#define __GNET_SYNCFACTIONLEVEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SyncFactionLevel : public GNET::Protocol
{
	#include "syncfactionlevel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
