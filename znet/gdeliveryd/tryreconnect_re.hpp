
#ifndef __GNET_TRYRECONNECT_RE_HPP
#define __GNET_TRYRECONNECT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TryReconnect_Re : public GNET::Protocol
{
	#include "tryreconnect_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
