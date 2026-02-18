
#ifndef __GNET_KINGDOMPOINTSYNC_HPP
#define __GNET_KINGDOMPOINTSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void kingdom_point_sync(int point);

namespace GNET
{

class KingdomPointSync : public GNET::Protocol
{
	#include "kingdompointsync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		kingdom_point_sync(value);
	}
};

};

#endif
