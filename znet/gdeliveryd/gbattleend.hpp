
#ifndef __GNET_GBATTLEEND_HPP
#define __GNET_GBATTLEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battlemanager.h"
#include "battleflagmanager.hpp"

namespace GNET
{

class GBattleEnd : public GNET::Protocol
{
	#include "gbattleend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT( "battle end, gs_id=%d, tag=%d, map_id=%d", gs_id, tag, map_id);
		BattleManager::GetInstance()->OnBattleEnd(gs_id, tag, map_id);
		BattleFlagManager::GetInstance()->BattleFlagEnd(gs_id, tag);
	}
};

};

#endif
