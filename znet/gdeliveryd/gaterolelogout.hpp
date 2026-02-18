
#ifndef __GNET_GATEROLELOGOUT_HPP
#define __GNET_GATEROLELOGOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggateroleinfo"

namespace GNET
{

class GateRoleLogout : public GNET::Protocol
{
	#include "gaterolelogout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
