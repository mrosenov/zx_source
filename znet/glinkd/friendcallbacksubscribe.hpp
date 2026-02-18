
#ifndef __GNET_FRIENDCALLBACKSUBSCRIBE_HPP
#define __GNET_FRIENDCALLBACKSUBSCRIBE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackSubscribe : public GNET::Protocol
{
	#include "friendcallbacksubscribe"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!GLinkServer::ValidRole(sid, roleid))
		      	return;
		DEBUG_PRINT("GLink::FriendCallbackSubscribe, roleid=%d, is_subscribe=%d\n", roleid, is_subscribe);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
