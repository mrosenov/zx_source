
#ifndef __GNET_SENDINSTANCINGENTER_HPP
#define __GNET_SENDINSTANCINGENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

bool player_enter_battleground( int role_id, int battle_world_tag, int battle_id, int faction, int battle_type);

namespace GNET
{

class SendInstancingEnter : public GNET::Protocol
{
	#include "sendinstancingenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!player_enter_battleground( roleid, tag, map_id, team, 0))
		{
			InstancingEnterFail msg;
			msg.roleid = roleid;
			msg.map_id = map_id;
			GProviderClient::DispatchProtocol(0, msg);
		}
	}
};

};

#endif
