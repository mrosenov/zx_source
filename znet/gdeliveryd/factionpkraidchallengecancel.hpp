
#ifndef __GNET_FACTIONPKRAIDCHALLENGECANCEL_HPP
#define __GNET_FACTIONPKRAIDCHALLENGECANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "factionpkraidchallengecancel_re.hpp"
namespace GNET
{

class FactionPkRaidChallengeCancel : public GNET::Protocol
{
	#include "factionpkraidchallengecancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	/*	int ret = RaidManager::GetInstance()->OnFactionPkCancelChallenge( roleid, dst_factionid, map_id);
		if(ret)
		{
			FactionPkRaidChallengeCancel_Re re(ret, dst_factionid, localsid);
			manager->Send(sid, re);
		}
                LOG_TRACE("FactionPkRaidChallengeCancel roleid=%d ret=%d",roleid,ret);
		*/
	}
};

};

#endif
