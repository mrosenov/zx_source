
#ifndef __GNET_GKINGDOMBATTLEENTER_HPP
#define __GNET_GKINGDOMBATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleEnter : public GNET::Protocol
{
	#include "gkingdombattleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
