
#ifndef __GNET_QUERYROLENAMEHISTORY_HPP
#define __GNET_QUERYROLENAMEHISTORY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueryRolenameHistory : public GNET::Protocol
{
	#include "queryrolenamehistory"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (name.size() < 2 || name.size() > MAX_NAME_SIZE)
			return;
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(QUERYNAME_POLICY))
			return;
		this->localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);		
	}
};

};

#endif
