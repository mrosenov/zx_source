
#ifndef __GNET_AUCTIONATTENDLIST_HPP
#define __GNET_AUCTIONATTENDLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "auctionmarket.h"
#include "auctionattendlist_re.hpp"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include <algorithm>
namespace GNET
{

class AuctionAttendList : public GNET::Protocol
{
	#include "auctionattendlist"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
