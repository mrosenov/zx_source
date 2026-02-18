
#ifndef __GNET_QUERYBATTLEFLAGBUFF_HPP
#define __GNET_QUERYBATTLEFLAGBUFF_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionidbean"

namespace GNET
{

class QueryBattleFlagBuff : public GNET::Protocol
{
	#include "querybattleflagbuff"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
