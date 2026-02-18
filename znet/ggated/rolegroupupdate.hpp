
#ifndef __GNET_ROLEGROUPUPDATE_HPP
#define __GNET_ROLEGROUPUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformclient.hpp"

namespace GNET
{

class RoleGroupUpdate : public GNET::Protocol
{
	#include "rolegroupupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleGroupUpdate roleid=%lld operation=%d",roleid,operation);
		GTPlatformClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
