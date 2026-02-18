
#ifndef __GNET_CONSIGNCANCELPOST_HPP
#define __GNET_CONSIGNCANCELPOST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignCancelPost : public GNET::Protocol
{
	#include "consigncancelpost"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
                if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
