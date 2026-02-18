
#ifndef __GNET_GINSTANCINGEND_HPP
#define __GNET_GINSTANCINGEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GInstancingEnd : public GNET::Protocol
{
	#include "ginstancingend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
