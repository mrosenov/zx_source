
#ifndef __GNET_REMOVEFACTION_HPP
#define __GNET_REMOVEFACTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionidbean"
#include "gtplatformclient.hpp"
namespace GNET
{

class RemoveFaction : public GNET::Protocol
{
	#include "removefaction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RemoveFaction fid=%lld",factionid.factionid);
		GTPlatformClient::GetInstance()->SendProtocol(this);
	}

};

};

#endif
