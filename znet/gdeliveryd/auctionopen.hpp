
#ifndef __GNET_AUCTIONOPEN_HPP
#define __GNET_AUCTIONOPEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "groleinventory"
#include "gmailsyncdata"
#include "gdeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "gproviderserver.hpp"
#include "auctionopen_re.hpp"
#include "gmailendsync.hpp"
#include "auctionmarket.h"
#include "mapuser.h"

#define EIGHT_HOUR      28800
#define SIXTEEN_HOUR    57600
#define TWENTYFOUR_HOUR 86400
namespace GNET
{
class AuctionOpen : public GNET::Protocol
{
	#include "auctionopen"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
