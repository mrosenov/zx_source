
#ifndef __GNET_GKINGISSUETASK_RE_HPP
#define __GNET_GKINGISSUETASK_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void king_deliver_task_response(int retcode, int roleid, int task_type);

namespace GNET
{

class GKingIssueTask_Re : public GNET::Protocol
{
	#include "gkingissuetask_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		king_deliver_task_response(retcode, roleid, task_type);
	}
};

};

#endif
