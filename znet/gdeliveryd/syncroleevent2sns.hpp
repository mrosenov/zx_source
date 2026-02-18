
#ifndef __GNET_SYNCROLEEVENT2SNS_HPP
#define __GNET_SYNCROLEEVENT2SNS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SyncRoleEvent2SNS : public GNET::Protocol
{
	#include "syncroleevent2sns"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
