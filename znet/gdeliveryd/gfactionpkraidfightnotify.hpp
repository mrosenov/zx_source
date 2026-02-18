
#ifndef __GNET_GFACTIONPKRAIDFIGHTNOTIFY_HPP
#define __GNET_GFACTIONPKRAIDFIGHTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"
//#include "gproviderserver.hpp"


namespace GNET
{

class GFactionPkRaidFightNotify : public GNET::Protocol
{
	#include "gfactionpkraidfightnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	//	int gsid=GProviderServer::GetInstance()->GetGSID(sid);
	//	int ret=RaidManager::GetInstance()->OnFactionPkNotify(gsid, end_time, is_start, raidroom_id, mapid);
		int ret=RaidManager::GetInstance()->OnFactionPkNotify(end_time, is_start, raidroom_id, mapid);
		LOG_TRACE("GFactionPkRaidFightNotify raidroom_id=%d end_time=%d is_start=%d ret=%d",raidroom_id,end_time,is_start,ret);
	}
};

};

#endif
