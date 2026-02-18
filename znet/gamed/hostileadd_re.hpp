
#ifndef __GNET_HOSTILEADD_RE_HPP
#define __GNET_HOSTILEADD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void add_faction_hostile( unsigned int faction_id, unsigned int hostile );

namespace GNET
{

class HostileAdd_Re : public GNET::Protocol
{
	#include "hostileadd_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		add_faction_hostile( factionid, hostile );
	}
};

};

#endif
