
#ifndef __GNET_S2CMULTICAST2_HPP
#define __GNET_S2CMULTICAST2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "player"

namespace GNET
{

class S2CMulticast2 : public GNET::Protocol
{
	#include "s2cmulticast2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
