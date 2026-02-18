
#ifndef __GNET_INSTANCINGAVAILABLELIST_HPP
#define __GNET_INSTANCINGAVAILABLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAvailableList : public GNET::Protocol
{
	#include "instancingavailablelist"

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
