
#ifndef __GNET_FRIENDCALLBACKAWARD_HPP
#define __GNET_FRIENDCALLBACKAWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackAward : public GNET::Protocol
{
	#include "friendcallbackaward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!GLinkServer::ValidRole(sid, roleid))
			return;
		DEBUG_PRINT("GLink::FriendCallbackAward, roleid=%d\n", roleid);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
