
#ifndef __GNET_GINSTANCINGLEAVE_HPP
#define __GNET_GINSTANCINGLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GInstancingLeave : public GNET::Protocol
{
	#include "ginstancingleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
