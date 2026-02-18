
#ifndef __GNET_KINGDOMSETGAMEATTRI_HPP
#define __GNET_KINGDOMSETGAMEATTRI_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomSetGameAttri : public GNET::Protocol
{
	#include "kingdomsetgameattri"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
			return;
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(KINGDOM_PRIVILEGE_POLICY))
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
