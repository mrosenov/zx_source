
#ifndef __GNET_GSIEGESTATUS_HPP
#define __GNET_GSIEGESTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GSiegeStatus : public GNET::Protocol
{
	#include "gsiegestatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
