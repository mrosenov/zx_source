
#ifndef __GNET_GRAIDLEAVE_HPP
#define __GNET_GRAIDLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GRaidLeave : public GNET::Protocol
{
	#include "graidleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
