
#ifndef __GNET_GRAIDOPEN_HPP
#define __GNET_GRAIDOPEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GRaidOpen : public GNET::Protocol
{
	#include "graidopen"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
