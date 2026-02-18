
#ifndef __GNET_ROLEACTIVATION_HPP
#define __GNET_ROLEACTIVATION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtmanager.h"
namespace GNET
{

class RoleActivation : public GNET::Protocol
{
	#include "roleactivation"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleActivation: roleid=%lld,operation=%d",roleid,operation);
		if(GTManager::Instance()->OnRoleActivation(roleid,operation))
		{
			DEBUG_PRINT("RoleActivation:OnRoleActivation error");
		}
	}
};

};

#endif
