
#ifndef __GNET_GFACTIONPKRAIDDEPOSIT_HPP
#define __GNET_GFACTIONPKRAIDDEPOSIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFactionPkRaidDeposit : public GNET::Protocol
{
	#include "gfactionpkraiddeposit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
