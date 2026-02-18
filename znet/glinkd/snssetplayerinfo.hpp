
#ifndef __GNET_SNSSETPLAYERINFO_HPP
#define __GNET_SNSSETPLAYERINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryclient.hpp"
#include "glinkserver.hpp"

namespace GNET
{

class SNSSetPlayerInfo : public GNET::Protocol
{
	#include "snssetplayerinfo"

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
