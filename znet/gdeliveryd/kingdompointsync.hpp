
#ifndef __GNET_KINGDOMPOINTSYNC_HPP
#define __GNET_KINGDOMPOINTSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomPointSync : public GNET::Protocol
{
	#include "kingdompointsync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
