
#ifndef __GNET_GKINGDOMBATTLEREGISTER_HPP
#define __GNET_GKINGDOMBATTLEREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "kingdombattleinfo"

namespace GNET
{

class GKingdomBattleRegister : public GNET::Protocol
{
	#include "gkingdombattleregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
