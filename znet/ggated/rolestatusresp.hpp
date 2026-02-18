
#ifndef __GNET_ROLESTATUSRESP_HPP
#define __GNET_ROLESTATUSRESP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtmanager.h"

namespace GNET
{

class RoleStatusResp : public GNET::Protocol
{
	#include "rolestatusresp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleStatusResp: localrid=%lld ,mapsize=%d", localrid,rolestatus.size());
		GTManager::Instance()->OnRoleStatusResp(rolestatus);
	}
};

};

#endif
