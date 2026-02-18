
#ifndef __GNET_GKINGDOMATTACKERFAIL_HPP
#define __GNET_GKINGDOMATTACKERFAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomAttackerFail : public GNET::Protocol
{
	#include "gkingdomattackerfail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gkingdomattackerfail type %d tag %d failer %d",
				fieldtype, tagid, failer);
		KingdomManager::GetInstance()->OnBattleAttackerFail(fieldtype, tagid, failer);
	}
};

};

#endif
