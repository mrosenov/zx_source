
#ifndef __GNET_GETFACTIONDYNAMIC_HPP
#define __GNET_GETFACTIONDYNAMIC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetFactionDynamic : public GNET::Protocol
{
	#include "getfactiondynamic"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(GET_FAC_DYNAMIC) || sinfo->roleid != roleid/*等同于 ValidRole 验证*/)
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
