
#ifndef __GNET_SETHELPSTATES_HPP
#define __GNET_SETHELPSTATES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "putrolebase.hrp"

#include "gdeliveryserver.hpp"
#include "sethelpstates_re.hpp"

#include "getrolebase.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"
namespace GNET
{

class SetHelpStates : public GNET::Protocol
{
	#include "sethelpstates"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("SetHelpStates. roleid=%d,localsid=%d,size=%d\n",roleid,localsid,help_states.size());
		GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
		if(info)
		{
			info->help_states = help_states;
			UpdateRoleBaseArg arg;
			arg.roleid = roleid;
			arg.datatype = 0;
			arg.data = help_states;
			GameDBClient::GetInstance()->SendProtocol(Rpc::Call(RPC_UPDATEROLEBASE,&arg));
			manager->Send(sid,SetHelpStates_Re(ERR_SUCCESS,roleid,localsid));
		}
		else
			Log::log(LOG_ERR, "SetHelpStates, role cache miss, roleid=%d", roleid);
	}
};

};

#endif
