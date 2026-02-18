
#ifndef __GNET_RAIDOPENVOTE_HPP
#define __GNET_RAIDOPENVOTE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"
#include "raidopenvote_re.hpp"

namespace GNET
{

class RaidOpenVote : public GNET::Protocol
{
	#include "raidopenvote"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidOpenVote: roleid=%d,voted_roleid=%d,raidroom_id=%d", roleid,voted_roleid,raidroom_id);
		int retcode = RaidManager::GetInstance()->BeginVote(voted_roleid,roleid,raidroom_id);
		RaidOpenVote_Re re(retcode,voted_roleid,localsid);
		GDeliveryServer::GetInstance()->Send(sid,re);
	}
};

};

#endif
