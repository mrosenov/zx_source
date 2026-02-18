
#ifndef __GNET_SETUICONFIG_HPP
#define __GNET_SETUICONFIG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "updaterolebase.hrp"

#include "gdeliveryserver.hpp"
#include "setuiconfig_re.hpp"
#include "gamedbclient.hpp"
#include "mapuser.h"
namespace GNET
{

class SetUIConfig : public GNET::Protocol
{
	#include "setuiconfig"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("gdelivery::receive setuiconfig. roleid=%d,localsid=%d,size=%d\n",roleid,localsid,ui_config.size());
		GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
		if(info)
		{
			info->config_data = ui_config;
			UpdateRoleBaseArg arg;
			arg.roleid = roleid;
			arg.datatype = 1;
			arg.data = ui_config;
			GameDBClient::GetInstance()->SendProtocol(Rpc::Call(RPC_UPDATEROLEBASE,&arg));
			manager->Send(sid,SetUIConfig_Re(ERR_SUCCESS,roleid,localsid));
		}
		else
			Log::log(LOG_ERR, "SetUIConfig, role cache miss, roleid=%d", roleid);
	}
};

};

#endif
