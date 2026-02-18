
#ifndef __GNET_SSOGETTICKETREP_HPP
#define __GNET_SSOGETTICKETREP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "userssoinfo"

namespace GNET
{

class SSOGetTicketRep : public GNET::Protocol
{
	#include "ssogetticketrep"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv ssogetticketrep ret %d userid %lld ticket.size %d local_context.size %d reserved.size %d",
			retcode, user.userid, ticket.size(), local_context.size(), reserved.size());
		UserInfo * userinfo = UserContainer::GetInstance().FindUser(static_cast<int>(user.userid));
		if (NULL == userinfo || userinfo->status != _STATUS_ONGAME) 
			return;
		OctetsStream os;
		os << userinfo->localsid;
		this->reserved = os;
		GDeliveryServer::GetInstance()->Send(userinfo->linksid, this); 
	}
};

};

#endif
