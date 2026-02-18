
#ifndef __GNET_SENDRAIDCLOSE_HPP
#define __GNET_SENDRAIDCLOSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

extern void raid_close(int room_id, int map_id, int index, int reason);

namespace GNET
{
class SendRaidClose : public GNET::Protocol
{
	#include "sendraidclose"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		raid_close(raidroom_id, map_id, index, reason);
	}
};

};

#endif
