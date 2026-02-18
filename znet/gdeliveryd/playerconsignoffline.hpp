
#ifndef __GNET_PLAYERCONSIGNOFFLINE_HPP
#define __GNET_PLAYERCONSIGNOFFLINE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerConsignOffline : public GNET::Protocol
{
	#include "playerconsignoffline"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
