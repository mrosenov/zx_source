
#ifndef __GNET_GETTASKDATA_HPP
#define __GNET_GETTASKDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "gettaskdata_re.hpp"
#include "maptaskdata.h"
namespace GNET
{

class GetTaskData : public GNET::Protocol
{
	#include "gettaskdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GTaskData data;
		if( TaskData::GetInstance().GetTaskData(taskid, data) )
			manager->Send(sid,GetTaskData_Re(taskid,playerid,env,0,data.taskdata));
		else
			TaskData::GetInstance().GameGetTaskData(taskid, sid,playerid,env);
	}
};

};

#endif
