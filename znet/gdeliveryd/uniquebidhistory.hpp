
#ifndef __GNET_UNIQUEBIDHISTORY_HPP
#define __GNET_UNIQUEBIDHISTORY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "uniqueauctionmanager.h"

namespace GNET
{

class UniqueBidHistory : public GNET::Protocol
{
	#include "uniquebidhistory"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		UniqueAuctionManager::GetInstance()->SendBidHistory(roleid, sid, localsid);
	}
};

};

#endif
