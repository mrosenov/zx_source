
#ifndef __GNET_PLAYERRECONNECT_HPP
#define __GNET_PLAYERRECONNECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void user_reconnect(int cs_index,int sid,int uid);

namespace GNET
{

class PlayerReconnect : public GNET::Protocol
{
	#include "playerreconnect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		user_reconnect(provider_link_id,localsid,roleid);
	}
};

};

#endif
