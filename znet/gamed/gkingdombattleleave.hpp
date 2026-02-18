
#ifndef __GNET_GKINGDOMBATTLELEAVE_HPP
#define __GNET_GKINGDOMBATTLELEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleLeave : public GNET::Protocol
{
	#include "gkingdombattleleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
