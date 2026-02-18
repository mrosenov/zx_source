
#ifndef __GNET_PLAYERRECONNECT_RE_HPP
#define __GNET_PLAYERRECONNECT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerReconnect_Re : public GNET::Protocol
{
	#include "playerreconnect_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("[reconnect]: receive the reconnect response from gs: result=%d, roleid=%d", result, roleid);

		int uid = UserContainer::Roleid2Userid(roleid);
		if(!uid)
			return;

		UserContainer::GetInstance().ReconnectLogin(uid, result==ERR_SUCCESS);
	}
};

};

#endif
