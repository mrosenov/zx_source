
#ifndef __GNET_SYNCFACTIONHOSTILES_HPP
#define __GNET_SYNCFACTIONHOSTILES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SyncFactionHostiles : public GNET::Protocol
{
	#include "syncfactionhostiles"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
