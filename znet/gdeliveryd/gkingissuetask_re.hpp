
#ifndef __GNET_GKINGISSUETASK_RE_HPP
#define __GNET_GKINGISSUETASK_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingIssueTask_Re : public GNET::Protocol
{
	#include "gkingissuetask_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
