
#ifndef __GNET_ROLESTATUSREQ_HPP
#define __GNET_ROLESTATUSREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtmanager.h"
namespace GNET
{

class RoleStatusReq : public GNET::Protocol
{
	#include "rolestatusreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleStatusReq: rolelist size=%d localrid=%lld ",rolelist.size(),localrid);
		if(GTManager::Instance()->OnRoleStatusReq(rolelist,localrid))
			DEBUG_PRINT("RoleStatusReq:OnRoleStatusReq error");
	}
};

};

#endif
