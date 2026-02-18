
#ifndef __GNET_PLAYERRECONNECT_HPP
#define __GNET_PLAYERRECONNECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerReconnect : public GNET::Protocol
{
	#include "playerreconnect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
