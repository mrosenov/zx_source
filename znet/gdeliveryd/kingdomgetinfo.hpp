
#ifndef __GNET_KINGDOMGETINFO_HPP
#define __GNET_KINGDOMGETINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomGetInfo : public GNET::Protocol
{
	#include "kingdomgetinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		KingdomGetInfo_Re re(ERR_SUCCESS, roleid, localsid);
		re.retcode = KingdomManager::GetInstance()->GetClientInfo(roleid, re.info);
		manager->Send(sid, re);
		LOG_TRACE("kingdomgetinfo roleid %d win_times %d reward_mask %d", roleid, re.info.win_times, re.info.reward_mask);
	}
};

};

#endif
