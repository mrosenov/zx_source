
#ifndef __GNET_KINGDOMANNOUNCE_HPP
#define __GNET_KINGDOMANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomAnnounce : public GNET::Protocol
{
	#include "kingdomannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int ret = KingdomManager::GetInstance()->Announce(roleid, announce);
		if (ret != ERR_SUCCESS)
			manager->Send(sid, KingdomAnnounce_Re(ret, roleid, announce, localsid));
		LOG_TRACE("kingdomannounce ret %d roleid %d", ret, roleid);
	}
};

};

#endif
