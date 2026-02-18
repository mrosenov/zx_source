
#ifndef __GNET_FACTIONPKRAIDGETSCORE_HPP
#define __GNET_FACTIONPKRAIDGETSCORE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidGetScore : public GNET::Protocol
{
	#include "factionpkraidgetscore"

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
