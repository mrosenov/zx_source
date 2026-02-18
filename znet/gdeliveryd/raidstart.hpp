
#ifndef __GNET_RAIDSTART_HPP
#define __GNET_RAIDSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h" 
#include "raidstart_re.hpp"
namespace GNET
{

class RaidStart : public GNET::Protocol
{
	#include "raidstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidStart roleid=%d, raidroom_id=%d",roleid, raidroom_id);
		int retcode = RaidManager::GetInstance()->RaidStart(roleid,raidroom_id,raid_type,raid_data);
		if(retcode != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(sid,RaidStart_Re(retcode,raidroom_id,localsid));
	}
};

};

#endif
