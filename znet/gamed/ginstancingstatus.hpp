
#ifndef __GNET_GINSTANCINGSTATUS_HPP
#define __GNET_GINSTANCINGSTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GInstancingStatus : public GNET::Protocol
{
	#include "ginstancingstatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
