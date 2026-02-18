
#ifndef __GNET_STOPFACBASE_HPP
#define __GNET_STOPFACBASE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StopFacBase : public GNET::Protocol
{
	#include "stopfacbase"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
