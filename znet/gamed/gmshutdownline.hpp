
#ifndef __GNET_GMSHUTDOWNLINE_HPP
#define __GNET_GMSHUTDOWNLINE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GMShutdownLine : public GNET::Protocol
{
	#include "gmshutdownline"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		gm_shutdown_server();
	}
};

};

#endif
