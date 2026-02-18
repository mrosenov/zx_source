
#ifndef __GNET_GKINGDOMATTACKERFAIL_HPP
#define __GNET_GKINGDOMATTACKERFAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomAttackerFail : public GNET::Protocol
{
	#include "gkingdomattackerfail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
