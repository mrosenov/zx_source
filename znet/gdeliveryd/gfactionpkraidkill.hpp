
#ifndef __GNET_GFACTIONPKRAIDKILL_HPP
#define __GNET_GFACTIONPKRAIDKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"


namespace GNET
{

class GFactionPkRaidKill : public GNET::Protocol
{
	#include "gfactionpkraidkill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int ret=RaidManager::GetInstance()->OnFactionPkKill(killer, victim, victim_faction, index, map_id);
		LOG_TRACE("GFactionPkRaidKill killer=%d index=%d ret=%d",killer,index,ret);
	}
};

};

#endif
