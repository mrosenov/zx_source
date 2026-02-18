
#ifndef __GNET_UNIQUEBIDHISTORY_HPP
#define __GNET_UNIQUEBIDHISTORY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class UniqueBidHistory : public GNET::Protocol
{
	#include "uniquebidhistory"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid, roleid))
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
