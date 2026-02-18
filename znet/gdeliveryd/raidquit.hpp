
#ifndef __GNET_RAIDQUIT_HPP
#define __GNET_RAIDQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "raidquit_re.hpp"
namespace GNET
{

class RaidQuit : public GNET::Protocol
{
	#include "raidquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidQuit roleid=%d, raidroom_id=%d",roleid, raidroom_id);
		int retcode = RaidManager::GetInstance()->LeaveRoom(roleid,raidroom_id,team);
		GDeliveryServer::GetInstance()->Send(sid,RaidQuit_Re(retcode,raidroom_id,localsid));
	}
};

};

#endif
