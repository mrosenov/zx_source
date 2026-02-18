
#ifndef __GNET_SIEGEKILL_HPP
#define __GNET_SIEGEKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeKill : public GNET::Protocol
{
	#include "siegekill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
