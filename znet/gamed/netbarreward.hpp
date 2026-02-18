
#ifndef __GNET_NETBARREWARD_HPP
#define __GNET_NETBARREWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void handle_netbar_reward(int roleid, int netbar_level, int award_type);

namespace GNET
{

class NetBarReward : public GNET::Protocol
{
	#include "netbarreward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(netbar_level > 3 || netbar_level <= 0) return;                                                
		if(award_type > 4 || award_type <= 0) return;    

		handle_netbar_reward(roleid, netbar_level, award_type);
	}
};

};

#endif
