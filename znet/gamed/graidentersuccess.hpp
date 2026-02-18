
#ifndef __GNET_GRAIDENTERSUCCESS_HPP
#define __GNET_GRAIDENTERSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GRaidEnterSuccess : public GNET::Protocol
{
	#include "graidentersuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
