
#ifndef __GNET_ROLEINFOREQ_HPP
#define __GNET_ROLEINFOREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtmanager.h"

namespace GNET
{

class RoleInfoReq : public GNET::Protocol
{
	#include "roleinforeq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleInfoReq: localuid=%lld roleid=%lld", localuid,roleid);
		GTManager::Instance()->OnRoleInfoReq(localuid,roleid);
	}
};

};

#endif
