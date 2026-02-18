
#ifndef __GNET_AUCTIONGET_HPP
#define __GNET_AUCTIONGET_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbauctionget.hrp"
#include "mapuser.h"
#include "auctionget_re.hpp"
#include "auctionmarket.h"
#include "gdeliveryserver.hpp"
namespace GNET
{

class AuctionGet : public GNET::Protocol
{
	#include "auctionget"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
