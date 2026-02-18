
#ifndef __GNET_TOPFLOWERGETGIFT_HPP
#define __GNET_TOPFLOWERGETGIFT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TopFlowerGetGift : public GNET::Protocol
{
	#include "topflowergetgift"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv TopFlowerGetGift isRecv%d, roleid:%d", isRecv, roleid);

		if (!GLinkServer::ValidRole(sid,roleid))
			return;
		
		localsid = sid;
		LOG_TRACE("send TopFlowerReq to gdeliveryd roleid:%d, localsid:%d", roleid, localsid);

		GDeliveryClient::GetInstance()->SendProtocol(this);

	}
};

};

#endif
