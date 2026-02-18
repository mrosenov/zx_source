
#ifndef __GNET_SNSCANCELLEAVEMESSAGE_HPP
#define __GNET_SNSCANCELLEAVEMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSCancelLeaveMessage : public GNET::Protocol
{
	#include "snscancelleavemessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid, roleid))
			return; 

		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
