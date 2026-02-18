
#ifndef __GNET_SENDFLOWERREQ_HPP
#define __GNET_SENDFLOWERREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendFlowerReq : public GNET::Protocol
{
	#include "sendflowerreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv SendFlowerReq send_roleid:%d", send_roleid);

		if (!GLinkServer::ValidRole(sid,send_roleid))
			return;

		localsid = sid;		
		LOG_TRACE("send SendFlowerReq to gdeliveryd send_roleid:%d", send_roleid);

		GDeliveryClient::GetInstance()->SendProtocol(this);

	}
};

};

#endif
