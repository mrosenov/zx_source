
#ifndef __GNET_FACMALLSYNC_HPP
#define __GNET_FACMALLSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacMallSync : public GNET::Protocol
{
	#include "facmallsync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
