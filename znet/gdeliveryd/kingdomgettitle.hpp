
#ifndef __GNET_KINGDOMGETTITLE_HPP
#define __GNET_KINGDOMGETTITLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomGetTitle : public GNET::Protocol
{
	#include "kingdomgettitle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("KingdomGetTitle\n");
		KingdomGetTitle_Re re(ERR_SUCCESS, roleid, 0, localsid, 0, 0);
		re.retcode = KingdomManager::GetInstance()->GetTitle(roleid, re);
		manager->Send(sid, re);
	}
};

};

#endif
