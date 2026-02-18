
#ifndef __GNET_AUCTIONCLOSE_HPP
#define __GNET_AUCTIONCLOSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "dbauctionclose.hrp"
#include "mapuser.h"
namespace GNET
{

class AuctionClose : public GNET::Protocol
{
	#include "auctionclose"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
