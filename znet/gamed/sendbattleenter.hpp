
#ifndef __GNET_SENDBATTLEENTER_HPP
#define __GNET_SENDBATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

bool player_enter_battleground( int role_id, int battle_world_tag, int battle_id, int faction, int battle_type);

namespace GNET
{

class SendBattleEnter : public GNET::Protocol
{
	#include "sendbattleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!player_enter_battleground( roleid, tag, map_id, team, field_type))
		{
			BattleEnterFail msg;
			msg.roleid = roleid;
			msg.map_id = map_id;
			GProviderClient::DispatchProtocol(0, msg);
		}
	}
};

};

#endif
