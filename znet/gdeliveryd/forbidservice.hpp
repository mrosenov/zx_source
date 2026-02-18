
#ifndef __GNET_FORBIDSERVICE_HPP
#define __GNET_FORBIDSERVICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "serviceforbidcmd"

namespace GNET
{

class ForbidService : public GNET::Protocol
{
	#include "forbidservice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
