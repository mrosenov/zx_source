
#ifndef __GNET_SYNCFACTIONHOSTILES_HPP
#define __GNET_SYNCFACTIONHOSTILES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionhostiles"

void sync_faction_hostiles_info( unsigned int faction_id, std::vector<unsigned int>& hostiles );

namespace GNET
{

class SyncFactionHostiles : public GNET::Protocol
{
	#include "syncfactionhostiles"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		std::vector<FactionHostiles>::iterator it;
		for( it = list.begin(); it != list.end(); ++it )
		{
			sync_faction_hostiles_info( it->fid, it->hostiles );
		}
	}
};

};

#endif
