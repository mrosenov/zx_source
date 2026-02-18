
#ifndef __GNET_GETUICONFIG_RE_HPP
#define __GNET_GETUICONFIG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class GetUIConfig_Re : public GNET::Protocol
{
	#include "getuiconfig_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("glinkd::receive getuiconfig_re from gdelivery.roleid=%d,retcode=%d\n",roleid,result);
		if (!GLinkServer::ValidLocalsid(localsid,roleid))
			return;
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
