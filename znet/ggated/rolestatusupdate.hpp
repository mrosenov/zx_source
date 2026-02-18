
#ifndef __GNET_ROLESTATUSUPDATE_HPP
#define __GNET_ROLESTATUSUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "rolestatusbean"
#include "factionidbean"
#include "gtmanager.h"
namespace GNET
{

class RoleStatusUpdate : public GNET::Protocol
{
	#include "rolestatusupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleStatusUpdate roleid=%lld friends.size=%d",roleid,friends.size());
		GTManager::Instance()->OnRoleStatusUpdate(*this);
	}
};

};

#endif
