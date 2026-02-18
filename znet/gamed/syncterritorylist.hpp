
#ifndef __GNET_SYNCTERRITORYLIST_HPP
#define __GNET_SYNCTERRITORYLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "territoryowner"

void sync_territory_owner(int territoryid, int territorytype, unsigned int owner);

namespace GNET
{

class SyncTerritoryList : public GNET::Protocol
{
	#include "syncterritorylist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		TerritoryOwnerVector::iterator iter;
		for(iter = tlist.begin(); iter != tlist.end(); ++iter)
		{
			sync_territory_owner(iter->territoryid, iter->territorytype, iter->owner);
		}
	}
};

};

#endif
