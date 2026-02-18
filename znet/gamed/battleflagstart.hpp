
#ifndef __GNET_BATTLEFLAGSTART_HPP
#define __GNET_BATTLEFLAGSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionidbean"

namespace GNET
{

class BattleFlagStart : public GNET::Protocol
{
	#include "battleflagstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
