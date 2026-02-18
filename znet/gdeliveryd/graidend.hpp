
#ifndef __GNET_GRAIDEND_HPP
#define __GNET_GRAIDEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"
#include "battleflagmanager.hpp"


namespace GNET
{

class GRaidEnd : public GNET::Protocol
{
	#include "graidend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("graidend gsid=%d index=%d result=%d",gs_id,index,result);
		RaidManager::GetInstance()->OnRaidEnd(gs_id,index,map_id,result);
		BattleFlagManager::GetInstance()->BattleFlagEnd(gs_id, map_id);
	}
};

};

#endif
