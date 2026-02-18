
#ifndef __GNET_SIEGEINFOGET_HPP
#define __GNET_SIEGEINFOGET_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeInfoGet : public GNET::Protocol
{
	#include "siegeinfoget"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
