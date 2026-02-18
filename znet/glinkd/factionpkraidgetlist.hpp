
#ifndef __GNET_FACTIONPKRAIDGETLIST_HPP
#define __GNET_FACTIONPKRAIDGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidGetList : public GNET::Protocol
{
	#include "factionpkraidgetlist"

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
