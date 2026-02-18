
#ifndef __GNET_SYNCFACTIONLEVEL_HPP
#define __GNET_SYNCFACTIONLEVEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gsp_if.h"
#include "factionlevel"

namespace GNET
{

class SyncFactionLevel : public GNET::Protocol
{
	#include "syncfactionlevel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GMSV::OnUpdateFactionLevel(list);
	}
};

};

#endif
