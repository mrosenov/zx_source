
#ifndef __GNET_GFACBASELEAVE_HPP
#define __GNET_GFACBASELEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFacBaseLeave : public GNET::Protocol
{
	#include "gfacbaseleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
