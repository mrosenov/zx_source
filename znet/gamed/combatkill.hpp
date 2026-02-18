
#ifndef __GNET_COMBATKILL_HPP
#define __GNET_COMBATKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class CombatKill : public GNET::Protocol
{
	#include "combatkill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
