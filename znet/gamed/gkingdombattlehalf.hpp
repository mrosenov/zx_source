
#ifndef __GNET_GKINGDOMBATTLEHALF_HPP
#define __GNET_GKINGDOMBATTLEHALF_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleHalf : public GNET::Protocol
{
	#include "gkingdombattlehalf"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
