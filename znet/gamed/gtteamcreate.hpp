
#ifndef __GNET_GTTEAMCREATE_HPP
#define __GNET_GTTEAMCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtteambean"

namespace GNET
{

class GTTeamCreate : public GNET::Protocol
{
	#include "gtteamcreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
