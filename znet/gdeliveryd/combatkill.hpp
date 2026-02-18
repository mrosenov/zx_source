
#ifndef __GNET_COMBATKILL_HPP
#define __GNET_COMBATKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "combatmanager.h"

namespace GNET
{

class CombatKill : public GNET::Protocol
{
	#include "combatkill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		CombatManager::Instance()->UpdateKill(mapid, killer, victim, victim_faction);
	}
};

};

#endif
