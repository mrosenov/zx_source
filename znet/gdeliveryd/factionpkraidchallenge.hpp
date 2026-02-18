
#ifndef __GNET_FACTIONPKRAIDCHALLENGE_HPP
#define __GNET_FACTIONPKRAIDCHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "factionpkraidchallenge_re.hpp"
namespace GNET
{

class FactionPkRaidChallenge : public GNET::Protocol
{
	#include "factionpkraidchallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
                int ret = RaidManager::GetInstance()->FactionPkChallenge(roleid,dst_rolename,map_id);
                //int ret = RaidManager::GetInstance()->FactionPkChallenge(roleid,dst_roleid,map_id);
		if(ret)
		{
			FactionPkRaidChallenge_Re re(ret, 0, dst_rolename, localsid);
		//	FactionPkRaidChallenge_Re re(ret, 0, dst_roleid, localsid);
			manager->Send(sid, re);
		}
                LOG_TRACE("FactionPkRaidChallenge roleid=%d ret=%d",roleid,ret);
	}
};

};

#endif
