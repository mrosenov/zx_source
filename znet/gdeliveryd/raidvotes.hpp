
#ifndef __GNET_RAIDVOTES_HPP
#define __GNET_RAIDVOTES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"


namespace GNET
{

class RaidVotes : public GNET::Protocol
{
	#include "raidvotes"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int retcode = RaidManager::GetInstance()->OnVote(voted_roleid,roleid,vote==1);
		LOG_TRACE("RaidVotes: roleid=%d,voted_roleid=%d retcode=%d", roleid,voted_roleid,retcode);
	}
};

};

#endif
