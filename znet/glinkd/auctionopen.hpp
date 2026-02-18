
#ifndef __GNET_AUCTIONOPEN_HPP
#define __GNET_AUCTIONOPEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gmailsyncdata"
#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class AuctionOpen : public GNET::Protocol
{
	#include "auctionopen"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		if ( !GLinkServer::ValidRole(sid,roleid) ) return;
		localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
