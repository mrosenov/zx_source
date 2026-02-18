
#ifndef __GNET_ROLEFRIENDUPDATE_HPP
#define __GNET_ROLEFRIENDUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformclient.hpp"
#include "gtmanager.h"

#include "rolebean"

namespace GNET
{

class RoleFriendUpdate : public GNET::Protocol
{
	#include "rolefriendupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(operation == FRIEND_ADD)
		{
			rolefriend.status = GTManager::Instance()->RoleStatusQuery(rolefriend.info.roleid);
		}
		DEBUG_PRINT("RoleFriendUpdate roleid=%lld operation=%d status=%d",roleid,operation,rolefriend.status.status);
		GTPlatformClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
