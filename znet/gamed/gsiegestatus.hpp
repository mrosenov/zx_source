
#ifndef __GNET_GSIEGESTATUS_HPP
#define __GNET_GSIEGESTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gsp_if.h"
namespace GNET
{

class GSiegeStatus : public GNET::Protocol
{
	#include "gsiegestatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GMSV::SetCityOwner(battle_id, 1, owner);
		GMSV::RefreshCityBonus();
	}
};

};

#endif
