
#ifndef __GNET_TOPFLOWERREQ_HPP
#define __GNET_TOPFLOWERREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TopFlowerReq : public GNET::Protocol
{
	#include "topflowerreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv TopFlowerReq page:%d, send_roleid:%d", page, send_roleid);

		if (!GLinkServer::ValidRole(sid,send_roleid))
			return;
		
		localsid = sid;
		LOG_TRACE("send TopFlowerReq to gdeliveryd page:%d, send_roleid:%d, localsid:%d", page, send_roleid, localsid);

		GDeliveryClient::GetInstance()->SendProtocol(this);

	}
};

};

#endif
