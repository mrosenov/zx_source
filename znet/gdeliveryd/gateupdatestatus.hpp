
#ifndef __GNET_GATEUPDATESTATUS_HPP
#define __GNET_GATEUPDATESTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformagent.h"
#include "ggaterolegtstatus"

namespace GNET
{

class GateUpdateStatus : public GNET::Protocol
{
	#include "gateupdatestatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GTPlatformAgent::UpdateClientGTStatus(rolelist,friendstatus);
	}
};

};

#endif
