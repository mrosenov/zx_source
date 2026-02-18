
#ifndef __GNET_HOSTILEPROTECT_HPP
#define __GNET_HOSTILEPROTECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HostileProtect : public GNET::Protocol
{
	#include "hostileprotect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
