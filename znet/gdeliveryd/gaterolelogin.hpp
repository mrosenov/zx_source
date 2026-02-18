
#ifndef __GNET_GATEROLELOGIN_HPP
#define __GNET_GATEROLELOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggateroleinfo"

namespace GNET
{

class GateRoleLogin : public GNET::Protocol
{
	#include "gaterolelogin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
