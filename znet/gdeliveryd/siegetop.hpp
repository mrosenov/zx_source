
#ifndef __GNET_SIEGETOP_HPP
#define __GNET_SIEGETOP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "siegetop_re.hpp"


namespace GNET
{

class SiegeTop : public GNET::Protocol
{
	#include "siegetop"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("SiegeTop: roleid=%d, battle_id=%d", roleid, battle_id);
		SiegeTop_Re re;
		re.battle_id = battle_id;
		re.localsid = localsid;
		SiegeManager::GetInstance()->GetTop(battle_id, re);
		manager->Send(sid, re);
	}
};

};

#endif
