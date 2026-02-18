
#ifndef __GNET_GATEROLECREATE_HPP
#define __GNET_GATEROLECREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggateroleinfo"

namespace GNET
{

class GateRoleCreate : public GNET::Protocol
{
	#include "gaterolecreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateRoleCreate: roleid=%d userid=%d rolenamesize=%d", roleid, userid, roledata.rolename.size());
		GTManager::Instance()->GameRoleCreate(roleid, userid, roledata);
	}
};

};

#endif
