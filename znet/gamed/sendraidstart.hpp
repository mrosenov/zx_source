
#ifndef __GNET_SENDRAIDSTART_HPP
#define __GNET_SENDRAIDSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidgroupinfo"

extern void raid_start(int map_id, int raidroom_id, const std::vector<int>& roleid_list, int raid_type, const void* buf, size_t size, char can_vote, char difficulty);

namespace GNET
{

class SendRaidStart : public GNET::Protocol
{
	#include "sendraidstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		raid_start(map_id, raidroom_id, group1_list.roleid_list, raid_type, raid_data.begin(), raid_data.size(), can_vote, difficulty);
	}
};

};

#endif
