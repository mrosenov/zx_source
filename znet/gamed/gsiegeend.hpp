
#ifndef __GNET_GSIEGEEND_HPP
#define __GNET_GSIEGEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GSiegeEnd : public GNET::Protocol
{
	#include "gsiegeend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
