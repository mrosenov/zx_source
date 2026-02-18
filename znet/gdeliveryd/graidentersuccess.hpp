
#ifndef __GNET_GRAIDENTERSUCCESS_HPP
#define __GNET_GRAIDENTERSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"

namespace GNET
{

class GRaidEnterSuccess : public GNET::Protocol
{
	#include "graidentersuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int retcode = RaidManager::GetInstance()->OnGsRaidEnterSuccess(roleid,gs_id,index);
		LOG_TRACE("GRaidEnterSuccess:roleid=%d,retcode=%d",roleid, retcode);
	}
};

};

#endif
