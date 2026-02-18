
#ifndef __GNET_GATEROLELOGOUT_HPP
#define __GNET_GATEROLELOGOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtmanager.h"

#include "ggateroleinfo"

namespace GNET
{

class GateRoleLogout : public GNET::Protocol
{
	#include "gaterolelogout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateRoleLogout: roleid=%d rolenamesize=%d level=%d", roledata.roleid,roledata.rolename.size(), roledata.level);
		GTManager::Instance()->GameLogout(roledata.roleid,roledata);
	}
};

};

#endif
