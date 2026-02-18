
#ifndef __GNET_COMBATCONTROL_HPP
#define __GNET_COMBATCONTROL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void faction_battle_start(int attacker, int defender, int end_time);
namespace GNET
{

class CombatControl : public GNET::Protocol
{
	#include "combatcontrol"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		if(cmd == 1)
		{
			faction_battle_start(attacker, defender, endtime);
		}
	}
};

};

#endif
