
#ifndef __GNET_FRIENDCALLBACKMAIL_HPP
#define __GNET_FRIENDCALLBACKMAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackMail : public GNET::Protocol
{
	#include "friendcallbackmail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!GLinkServer::ValidRole(sid, roleid))
			return;
		DEBUG_PRINT("GLink::FriendCallbackMail, sender=%d, receiver=%d, template_id=%d\n", roleid, receiver_roleid, template_id);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
