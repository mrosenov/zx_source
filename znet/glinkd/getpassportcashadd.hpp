
#ifndef __GNET_GETPASSPORTCASHADD_HPP
#define __GNET_GETPASSPORTCASHADD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetPassportCashAdd : public GNET::Protocol
{
	#include "getpassportcashadd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
			return;
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(GET_PASSPORT_CASHADD))
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
