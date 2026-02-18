
#ifndef __GNET_KINGDOMSETGAMEATTRI_HPP
#define __GNET_KINGDOMSETGAMEATTRI_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "kingdomsetgameattri_re.hpp"

namespace GNET
{

class KingdomSetGameAttri : public GNET::Protocol
{
	#include "kingdomsetgameattri"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int ret = KingdomManager::GetInstance()->SetGameAttr(roleid, flag);
		manager->Send(sid, KingdomSetGameAttri_Re(ret, roleid, flag, localsid));
		LOG_TRACE("kingdomsetgameattri ret %d roleid %d flag %d", ret, roleid, flag);
	}
};

};

#endif
