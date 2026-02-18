
#ifndef __GNET_GTSYNCTEAMS_HPP
#define __GNET_GTSYNCTEAMS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtteambean"

namespace GNET
{

class GTSyncTeams : public GNET::Protocol
{
	#include "gtsyncteams"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
