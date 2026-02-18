
#ifndef __GNET_INSTANCINGAPPOINT_HPP
#define __GNET_INSTANCINGAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAppoint : public GNET::Protocol
{
	#include "instancingappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
		{
			return;
		}
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);         
	}
};

};

#endif
