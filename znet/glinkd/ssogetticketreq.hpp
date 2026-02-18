
#ifndef __GNET_SSOGETTICKETREQ_HPP
#define __GNET_SSOGETTICKETREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "userssoinfo"

namespace GNET
{

class SSOGetTicketReq : public GNET::Protocol
{
	#include "ssogetticketreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(GET_SSO_TICKET))
			return;
		if (!GLinkServer::GetInstance()->ValidUser(sid, static_cast<int>(user.userid))) 
		{
			Log::log(LOG_ERR, "sid %d try to get another user(%lld)'s SSO ticket, disconnect it!", sid, user.userid);
			GLinkServer::GetInstance()->Close(sid);
			return;
		}
/*
		OctetsStream os;
		os << sid;
		this->reserved = os;
*/
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
