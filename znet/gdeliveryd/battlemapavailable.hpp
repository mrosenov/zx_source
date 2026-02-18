
#ifndef __GNET_BATTLEMAPAVAILABLE_HPP
#define __GNET_BATTLEMAPAVAILABLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleMapAvailable : public GNET::Protocol
{
	#include "battlemapavailable"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("battlemapavailable", "worldtag=%d:battleid=%d",world_tag,battle_id);
                BattleManager::GetInstance()->BattleMapAvailable(world_tag, battle_id);
	}
};

};

#endif
