
#ifndef __GNET_HOSTILEADD_HPP
#define __GNET_HOSTILEADD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class HostileAdd : public GNET::Protocol
{
#include "hostileadd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
