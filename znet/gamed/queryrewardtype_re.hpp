
#ifndef __GNET_QUERYREWARDTYPE_RE_HPP
#define __GNET_QUERYREWARDTYPE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void notify_player_reward(int roleid, int reward, int param, int fill_info_mask);

namespace GNET
{

class QueryRewardType_Re : public GNET::Protocol
{
	#include "queryrewardtype_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		notify_player_reward(roleid,reward,param, fill_info_mask);
	}
};

};

#endif
