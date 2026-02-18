
#ifndef __GNET_GFACBASESERVERREGISTER_HPP
#define __GNET_GFACBASESERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFacBaseServerRegister : public GNET::Protocol
{
	#include "gfacbaseserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
