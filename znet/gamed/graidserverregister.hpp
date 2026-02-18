
#ifndef __GNET_GRAIDSERVERREGISTER_HPP
#define __GNET_GRAIDSERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidfieldinfo"

namespace GNET
{

class GRaidServerRegister : public GNET::Protocol
{
	#include "graidserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
