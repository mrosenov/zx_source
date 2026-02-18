
#ifndef __GNET_GKINGDOMBATTLEEND_HPP
#define __GNET_GKINGDOMBATTLEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleEnd : public GNET::Protocol
{
	#include "gkingdombattleend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gkingdombattleend type %d tag %d winner %d", fieldtype, tagid, winnerid);
		KingdomManager::GetInstance()->OnBattleEnd(fieldtype, tagid, winnerid);
	}
};

};

#endif
