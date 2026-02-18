
#ifndef __GNET_SIEGEBROADCAT_HPP
#define __GNET_SIEGEBROADCAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeBroadcat : public GNET::Protocol
{
	#include "siegebroadcat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
