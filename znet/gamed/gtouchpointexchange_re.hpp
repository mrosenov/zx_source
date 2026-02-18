
#ifndef __GNET_GTOUCHPOINTEXCHANGE_RE_HPP
#define __GNET_GTOUCHPOINTEXCHANGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

bool touch_point_exchange(int retcode, int roleid, unsigned int flag, unsigned int count);

namespace GNET
{

class GTouchPointExchange_Re : public GNET::Protocol
{
	#include "gtouchpointexchange_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		touch_point_exchange(retcode, roleid, flag, count);
	}
};

};

#endif
