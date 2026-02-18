
#ifndef __GNET_SYNCTERRITORYLIST_HPP
#define __GNET_SYNCTERRITORYLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "territoryowner"

namespace GNET
{

class SyncTerritoryList : public GNET::Protocol
{
	#include "syncterritorylist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
