
#ifndef __GNET_KINGDOMDISCHARGE_HPP
#define __GNET_KINGDOMDISCHARGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomDischarge : public GNET::Protocol
{
	#include "kingdomdischarge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int ret = KingdomManager::GetInstance()->Discharge(roleid, candidate, title);
		if (ret != ERR_SUCCESS)
			manager->Send(sid, KingdomDischarge_Re(ret, roleid, candidate, title, localsid));
		LOG_TRACE("kingdodischarge ret %d roleid %d", ret, roleid);
	}
};

};

#endif
