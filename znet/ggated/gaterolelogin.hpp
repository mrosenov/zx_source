
#ifndef __GNET_GATEROLELOGIN_HPP
#define __GNET_GATEROLELOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtmanager.h"

#include "ggateroleinfo"

namespace GNET
{

class GateRoleLogin : public GNET::Protocol
{
	#include "gaterolelogin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateRoleLogin: roleid=%d rolenamesize=%d", roledata.roleid,roledata.rolename.size());
		GTManager::Instance()->GameLogin(roledata.roleid, localsid, linksid,roledata);
	}
};

};

#endif
