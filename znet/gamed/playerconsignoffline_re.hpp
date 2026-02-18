
#ifndef __GNET_PLAYERCONSIGNOFFLINE_RE_HPP
#define __GNET_PLAYERCONSIGNOFFLINE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gconsigngsroleinfo"

namespace GNET
{

class PlayerConsignOffline_Re : public GNET::Protocol
{
	#include "playerconsignoffline_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
