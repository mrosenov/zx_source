
#ifndef __GNET_KINGDOMINFOSYNC_HPP
#define __GNET_KINGDOMINFOSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "kingdominformation"

namespace GNET
{

class KingdomInfoSync : public GNET::Protocol
{
	#include "kingdominfosync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
