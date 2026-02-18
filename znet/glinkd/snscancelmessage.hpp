
#ifndef __GNET_SNSCANCELMESSAGE_HPP
#define __GNET_SNSCANCELMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSCancelMessage : public GNET::Protocol
{
	#include "snscancelmessage"

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
