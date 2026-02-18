
#ifndef __GNET_NETMARBLELOGOUT_HPP
#define __GNET_NETMARBLELOGOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class NetMarbleLogout : public GNET::Protocol
{
	#include "netmarblelogout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
