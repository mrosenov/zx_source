
#ifndef __GNET_GCHANGEFACTIONNAME_HPP
#define __GNET_GCHANGEFACTIONNAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GChangeFactionName : public GNET::Protocol
{
	#include "gchangefactionname"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
