
#ifndef __GNET_GSIEGESERVERREGISTER_HPP
#define __GNET_GSIEGESERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "siegemanager.h"


namespace GNET
{

class GSiegeServerRegister : public GNET::Protocol
{
	#include "gsiegeserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int gs_id = GProviderServer::GetInstance()->FindGameServer(sid);
		LOG_TRACE("GSiegeServerRegister: battle_id=%d,world_tag=%d,use_for_battle=%d,gs_id=%d",
				battle_id, world_tag, use_for_battle, gs_id);
		SiegeManager::GetInstance()->Register(gs_id, *this);
	}
};

};

#endif
