
#ifndef __GNET_GINSTANCINGSTATUS_HPP
#define __GNET_GINSTANCINGSTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battleflagmanager.hpp"


namespace GNET
{

class GInstancingStatus : public GNET::Protocol
{
	#include "ginstancingstatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "GInstancingStatus: gs_id=%d,battle_id=%d,world_tag=%d,status=%d.\n",gs_id,battle_id,world_tag,status);
		InstancingManager::GetInstance()->OnChangeStatus(gs_id, battle_id,world_tag,status);
		if(status == BS_CLOSING)
		{
			BattleFlagManager::GetInstance()->BattleFlagEnd(gs_id, world_tag);
		}
	}
};

};

#endif
