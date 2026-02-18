
#ifndef __GNET_SETTASKDATA_HPP
#define __GNET_SETTASKDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
//#include "settaskdata_re.hpp"
#include "maptaskdata.h"
namespace GNET
{

class SetTaskData : public GNET::Protocol
{
	#include "settaskdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		TaskData::GetInstance().GameSetTaskData( taskid, taskdata );
	}
};

};

#endif
