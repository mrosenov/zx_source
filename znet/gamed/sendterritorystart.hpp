
#ifndef __GNET_SENDTERRITORYSTART_HPP
#define __GNET_SENDTERRITORYSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void territory_start(int territory_id, int map_id, unsigned int defender_id, unsigned int attacker_id, int end_timestamp);

namespace GNET
{

class SendTerritoryStart : public GNET::Protocol
{
	#include "sendterritorystart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		territory_start(territoryid, map_tag, defender, attacker, end_time);
	}
};

};

#endif
