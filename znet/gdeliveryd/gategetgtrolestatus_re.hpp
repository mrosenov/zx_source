
#ifndef __GNET_GATEGETGTROLESTATUS_RE_HPP
#define __GNET_GATEGETGTROLESTATUS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggaterolegtstatus"

namespace GNET
{

class GateGetGTRoleStatus_Re : public GNET::Protocol
{
	#include "gategetgtrolestatus_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateGetGTRoleStatus_Re roleid=%d retcode=%d size=%d",roleid,retcode,statuslist.size());
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("GateGetGTRoleStatus_Re err");
			return;
		}
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole((roleid));
		if( NULL == pinfo )
			return;
		localsid = pinfo->localsid;
		GDeliveryServer::GetInstance()->Send(pinfo->linksid, this);
	}
};

};

#endif
