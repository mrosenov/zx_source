
#ifndef __GNET_FACBASEDATABROADCAST_HPP
#define __GNET_FACBASEDATABROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBaseDataBroadcast : public GNET::Protocol
{
	#include "facbasedatabroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
