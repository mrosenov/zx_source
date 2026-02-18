
#ifndef __GNET_INSTANCINGKICK_HPP
#define __GNET_INSTANCINGKICK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingKick : public GNET::Protocol
{
	#include "instancingkick"

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
