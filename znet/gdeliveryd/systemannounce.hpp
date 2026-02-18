
#ifndef __GNET_SYSTEMANNOUNCE_HPP
#define __GNET_SYSTEMANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SystemAnnounce : public GNET::Protocol
{
	#include "systemannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
