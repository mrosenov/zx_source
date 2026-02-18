
#ifndef __GNET_QUERYNETBARREWARD_HPP
#define __GNET_QUERYNETBARREWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueryNetBarReward : public GNET::Protocol
{
	#include "querynetbarreward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
