
#ifndef __GNET_GKINGDOMBATTLEEND_HPP
#define __GNET_GKINGDOMBATTLEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleEnd : public GNET::Protocol
{
	#include "gkingdombattleend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
