
#ifndef __GNET_STARTFACBASE_HPP
#define __GNET_STARTFACBASE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StartFacBase : public GNET::Protocol
{
	#include "startfacbase"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
