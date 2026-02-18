
#ifndef __GNET_FENGSHENRAIDQUIT_HPP
#define __GNET_FENGSHENRAIDQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FengShenRaidQuit : public GNET::Protocol
{
	#include "fengshenraidquit"

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
