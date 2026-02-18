
#ifndef __GNET_RAIDGETROOM_HPP
#define __GNET_RAIDGETROOM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidgetroom_re.hpp"
#include "raidmanager.h"
namespace GNET
{

class RaidGetRoom : public GNET::Protocol
{
	#include "raidgetroom"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidGetRoom roleid=%d, raidroom_id=%d",roleid, raidroom_id);
		RaidGetRoom_Re re;
		re.localsid = localsid;
		re.retcode = RaidManager::GetInstance()->GetRoomInfo(roleid,raidroom_id,re);
		GDeliveryServer::GetInstance()->Send(sid,re);
		LOG_TRACE("RaidGetRoomRe roleid=%d, raidroom_id=%d",roleid, raidroom_id);
	}
};

};

#endif
