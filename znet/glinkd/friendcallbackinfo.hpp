
#ifndef __GNET_FRIENDCALLBACKINFO_HPP
#define __GNET_FRIENDCALLBACKINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackInfo : public GNET::Protocol
{
	#include "friendcallbackinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!GLinkServer::ValidRole(sid, roleid))
			return;
		DEBUG_PRINT("GLink::FriendCallbackInfo, roleid=%d\n", roleid);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
