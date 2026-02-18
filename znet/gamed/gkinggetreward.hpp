
#ifndef __GNET_GKINGGETREWARD_HPP
#define __GNET_GKINGGETREWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingGetReward : public GNET::Protocol
{
	#include "gkinggetreward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
