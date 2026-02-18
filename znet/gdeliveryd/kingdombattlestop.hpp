
#ifndef __GNET_KINGDOMBATTLESTOP_HPP
#define __GNET_KINGDOMBATTLESTOP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomBattleStop : public GNET::Protocol
{
	#include "kingdombattlestop"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
