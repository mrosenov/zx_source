
#ifndef __GNET_BATTLESTATUS_HPP
#define __GNET_BATTLESTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleStatus : public GNET::Protocol
{
	#include "battlestatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::Role(sid,roleid))
		{
			GLinkServer::GetInstance()->SessionError(sid,ERR_INVALID_ACCOUNT,"Error userid or roleid. BattleStatus");
			return;
		}	
		DEBUG_PRINT("BattleStatus: %d\n", roleid);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
