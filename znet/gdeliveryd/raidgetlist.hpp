
#ifndef __GNET_RAIDGETLIST_HPP
#define __GNET_RAIDGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
#include "raidgetlist_re.hpp"
namespace GNET
{

class RaidGetList : public GNET::Protocol
{
	#include "raidgetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidGetList roleid=%d, map_id=%d",roleid, map_id);
		RaidGetList_Re re;
		re.map_id = map_id;
		re.localsid = localsid;
		re.retcode = RaidManager::GetInstance()->GetRoomList(roleid,map_id,re.fields);
                GDeliveryServer::GetInstance()->Send(sid, re);
	}
};

};

#endif
