
#ifndef __GNET_SENDRAIDENTER_HPP
#define __GNET_SENDRAIDENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

extern void raid_enter(int role_id, int map_id, int raid_template_id, int index, int raid_type, const void* buf, size_t size);

namespace GNET
{

class SendRaidEnter : public GNET::Protocol
{
	#include "sendraidenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		raid_enter(roleid, map_id, raid_template_id, index, raid_type, raid_data.begin(), raid_data.size());
	}
};

};

#endif
