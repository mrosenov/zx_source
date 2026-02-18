
#ifndef __GNET_S2CLINEBROADCAST_HPP
#define __GNET_S2CLINEBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class S2CLineBroadcast : public GNET::Protocol
{
	#include "s2clinebroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
