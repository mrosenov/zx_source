
#ifndef __GNET_INSTANCINGLEAVE_HPP
#define __GNET_INSTANCINGLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingLeave : public GNET::Protocol
{
	#include "instancingleave"

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
