
#ifndef __GNET_AUCTIONGETITEM_HPP
#define __GNET_AUCTIONGETITEM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "auctionmarket.h"
#include "auctiongetitem_re.hpp"
#include "mapuser.h"
#define MAX_IDSET_SIZE 8
namespace GNET
{

class AuctionGetItem : public GNET::Protocol
{
	#include "auctiongetitem"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
