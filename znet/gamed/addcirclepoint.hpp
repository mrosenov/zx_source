
#ifndef __GNET_ADDCIRCLEPOINT_HPP
#define __GNET_ADDCIRCLEPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class AddCirclePoint : public GNET::Protocol
{
	#include "addcirclepoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
