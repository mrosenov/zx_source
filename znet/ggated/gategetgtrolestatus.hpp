
#ifndef __GNET_GATEGETGTROLESTATUS_HPP
#define __GNET_GATEGETGTROLESTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtmanager.h"
#include "gategetgtrolestatus_re.hpp"
#include "gamegateclient.hpp"
namespace GNET
{

class GateGetGTRoleStatus : public GNET::Protocol
{
	#include "gategetgtrolestatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateGetGTRoleStatus: rolelist size=%d roleid=%d",rolelist.size(),roleid);
		GateGetGTRoleStatus_Re re;
		re.roleid = roleid;
		re.retcode=GTManager::Instance()->OnGetGTRoleStatus(rolelist,re.statuslist);
		if(re.retcode!=ERR_SUCCESS)
		{
			DEBUG_PRINT("GateGetGTRoleStatus:OnGateGetGTRoleStatus error");
		}
		GameGateClient::GetInstance()->SendProtocol(re);
	}
};

};

#endif
