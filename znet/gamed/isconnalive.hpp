
#ifndef __GNET_ISCONNALIVE_HPP
#define __GNET_ISCONNALIVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include <glog.h>
#include "gproviderclient.hpp"


namespace GNET
{

class IsConnAlive : public GNET::Protocol
{
	#include "isconnalive"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		((GProviderClient*)manager)->SetReceiveCounter(counter);
		GLog::log(GLOG_INFO, "receive connect counter=%d, sid=%d", counter, sid);
	}
};

};

#endif
