
#ifndef __GNET_GCRSSVRTEAMSGETSCORE_HPP
#define __GNET_GCRSSVRTEAMSGETSCORE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GCrssvrTeamsGetScore : public GNET::Protocol
{
	#include "gcrssvrteamsgetscore"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
