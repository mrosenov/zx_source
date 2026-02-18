
#ifndef __GNET_SENDINSTANCINGSTART_HPP
#define __GNET_SENDINSTANCINGSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void battleground_start( int battle_world_tag, int battle_id, int end_time, int apply_count, bool is_instance_field, char battle_type);

namespace GNET
{

class SendInstancingStart : public GNET::Protocol
{
	#include "sendinstancingstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		battleground_start( tag, map_id, end_time, 0, true, 2);
	}
};

};

#endif
