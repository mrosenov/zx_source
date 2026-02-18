
#ifndef __GNET_RAIDENTER_HPP
#define __GNET_RAIDENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidEnter : public GNET::Protocol
{
	#include "raidenter"

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
