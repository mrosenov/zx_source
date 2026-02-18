
#ifndef __GNET_GATEGETGTROLESTATUS_HPP
#define __GNET_GATEGETGTROLESTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GateGetGTRoleStatus : public GNET::Protocol
{
	#include "gategetgtrolestatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
