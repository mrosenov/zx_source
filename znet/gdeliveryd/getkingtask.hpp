
#ifndef __GNET_GETKINGTASK_HPP
#define __GNET_GETKINGTASK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetKingTask : public GNET::Protocol
{
	#include "getkingtask"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GetKingTask_Re re(-1, roleid, localsid, 0, 0);
		re.retcode = KingdomManager::GetInstance()->GetTask(roleid, re.task_type, re.complete_times, re.task_status, re.task_issue_time);
		manager->Send(sid, re);
		LOG_TRACE("getkingtask roleid %d, ret %d task_type %d complete_times %d task_status %d issue_time %d", roleid, re.retcode, re.task_type, re.complete_times, re.task_status, re.task_issue_time);
	}
};

};

#endif
