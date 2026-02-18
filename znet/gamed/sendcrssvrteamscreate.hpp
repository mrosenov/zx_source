
#ifndef __GNET_SENDCRSSVRTEAMSCREATE_HPP
#define __GNET_SENDCRSSVRTEAMSCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class SendCrssvrTeamsCreate : public GNET::Protocol
{
	#include "sendcrssvrteamscreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
