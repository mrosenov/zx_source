
#ifndef __GNET_GATEONLINELIST_RE_HPP
#define __GNET_GATEONLINELIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformagent.h"

namespace GNET
{

class GateOnlineList_Re : public GNET::Protocol
{
	#include "gateonlinelist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GTPlatformAgent::SyncTOGate(last_role,false);
	}
};

};

#endif
