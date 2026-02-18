
#ifndef __GNET_FACTIONPKRAIDGETSCORE_HPP
#define __GNET_FACTIONPKRAIDGETSCORE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"
#include "factionpkraidgetscore_re.hpp"


namespace GNET
{

class FactionPkRaidGetScore : public GNET::Protocol
{
	#include "factionpkraidgetscore"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("FactionPkRaidGetScore roleid=%d, map_id=%d",roleid, mapid);
		FactionPkRaidGetScore_Re re;
		re.localsid = localsid;
		re.retcode = RaidManager::GetInstance()->FactionPkRaidGetScore(raidroom_id,mapid,roleid,re);
                GDeliveryServer::GetInstance()->Send(sid, re);
	}
};

};

#endif
