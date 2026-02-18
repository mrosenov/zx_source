
#ifndef __GNET_AUCTIONLIST_HPP
#define __GNET_AUCTIONLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "auctionlist_re.hpp"
#include "auctionmarket.h"
#include "mapuser.h"
namespace GNET
{

class AuctionList : public GNET::Protocol
{
	#include "auctionlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
