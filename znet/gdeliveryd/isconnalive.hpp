
#ifndef __GNET_ISCONNALIVE_HPP
#define __GNET_ISCONNALIVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class IsConnAlive : public GNET::Protocol
{
	#include "isconnalive"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		manager->Send(sid, this);
	}
};

};

#endif
