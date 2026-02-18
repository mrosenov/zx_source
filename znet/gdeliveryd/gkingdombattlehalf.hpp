
#ifndef __GNET_GKINGDOMBATTLEHALF_HPP
#define __GNET_GKINGDOMBATTLEHALF_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleHalf : public GNET::Protocol
{
	#include "gkingdombattlehalf"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gkingdombattlehalf type %d tag %d res %d failattackers size %d",
				fieldtype, tagid, result, failattackers.size());
		KingdomManager::GetInstance()->OnBattleHalf(fieldtype, tagid, result, failattackers);
	}
};

};

#endif
