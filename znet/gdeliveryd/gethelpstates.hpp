
#ifndef __GNET_GETHELPSTATES_HPP
#define __GNET_GETHELPSTATES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "gethelpstates_re.hpp"

#include "getrolebase.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"
namespace GNET
{

class GetHelpStates : public GNET::Protocol
{
	#include "gethelpstates"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
		if(info)
		{
			LOG_TRACE( "GetHelpStates: roleid=%d,size=%d",roleid,info->help_states.size());
			GetHelpStates_Re re(ERR_SUCCESS,roleid,localsid,Octets());
			re.help_states = info->help_states;
			manager->Send(sid,re);
		}
		else
			Log::log(LOG_ERR, "GetHelpStates, role cache miss, roleid=%d", roleid);
	}
};

};

#endif
