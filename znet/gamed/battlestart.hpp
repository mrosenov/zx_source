
#ifndef __GNET_BATTLESTART_HPP
#define __GNET_BATTLESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

// 收到delivery发给gs的城站开始的协议
void battleground_start( int battle_world_tag, int battle_id, int end_time, int apply_count, bool is_instance_field , char battle_type);

namespace GNET
{

class BattleStart : public GNET::Protocol
{
	#include "battlestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		battleground_start( tag, map_id, end_time, apply_count, false, battle_type);
	}
};

};

#endif
