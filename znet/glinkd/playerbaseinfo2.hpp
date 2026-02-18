
#ifndef __GNET_PLAYERBASEINFO2_HPP
#define __GNET_PLAYERBASEINFO2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerBaseInfo2 : public GNET::Protocol
{
	#include "playerbaseinfo2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
		{
			GLinkServer::GetInstance()->SessionError(sid,ERR_INVALID_ACCOUNT,"Error userid or roleid. PlayerBaseInfo2");
			return;
		}	
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);		
	}
};

};

#endif
