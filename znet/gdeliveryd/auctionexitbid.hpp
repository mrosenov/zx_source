
#ifndef __GNET_AUCTIONEXITBID_HPP
#define __GNET_AUCTIONEXITBID_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "auctionmarket.h"
#include "auctionexitbid_re.hpp"
#include "gdeliveryserver.hpp"
#include "mapuser.h"

namespace GNET
{

class AuctionExitBid : public GNET::Protocol
{
	#include "auctionexitbid"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
