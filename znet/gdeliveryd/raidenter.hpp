
#ifndef __GNET_RAIDENTER_HPP
#define __GNET_RAIDENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "raidenter_re.hpp"
namespace GNET
{

class RaidEnter : public GNET::Protocol
{
	#include "raidenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidEnter roleid=%d, raidroom_id=%d",roleid, raidroom_id);
		int retcode = RaidManager::GetInstance()->RaidEnter(roleid, raidroom_id);
		if(retcode != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(sid, RaidEnter_Re(retcode, localsid));
	}
};

};

#endif
