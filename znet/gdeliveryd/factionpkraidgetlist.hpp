
#ifndef __GNET_FACTIONPKRAIDGETLIST_HPP
#define __GNET_FACTIONPKRAIDGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "factionpkraidgetlist_re.hpp"

namespace GNET
{

class FactionPkRaidGetList : public GNET::Protocol
{
	#include "factionpkraidgetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("FactionPkRaidGetList roleid=%d, map_id=%d",roleid, map_id);
		FactionPkRaidGetList_Re re;
		re.map_id = map_id;
		re.localsid = localsid;
		re.retcode = RaidManager::GetInstance()->GetFacPkList(roleid,map_id,re.fields);
                GDeliveryServer::GetInstance()->Send(sid, re);
	}
};

};

#endif
