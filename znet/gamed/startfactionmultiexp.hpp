
#ifndef __GNET_STARTFACTIONMULTIEXP_HPP
#define __GNET_STARTFACTIONMULTIEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StartFactionMultiExp : public GNET::Protocol
{
	#include "startfactionmultiexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
