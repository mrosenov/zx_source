
#ifndef __GNET_GKINGISSUETASK_HPP
#define __GNET_GKINGISSUETASK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingIssueTask : public GNET::Protocol
{
	#include "gkingissuetask"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
