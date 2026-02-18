
#ifndef __GNET_SNSGETPLAYERINFO_HPP
#define __GNET_SNSGETPLAYERINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


#include "gdeliveryclient.hpp"
#include "glinkserver.hpp"

namespace GNET
{

class SNSGetPlayerInfo : public GNET::Protocol
{
	#include "snsgetplayerinfo"

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
