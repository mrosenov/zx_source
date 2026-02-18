
#ifndef __GNET_GETFACTIONHOSTILE_HPP
#define __GNET_GETFACTIONHOSTILE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class GetFactionHostile : public GNET::Protocol
{
	#include "getfactionhostile"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
			return;
		this->localsid=sid;
		//GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
