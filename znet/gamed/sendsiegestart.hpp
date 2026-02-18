
#ifndef __GNET_SENDSIEGESTART_HPP
#define __GNET_SENDSIEGESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void battlefield_start(int battle_id,int world_tag,int attacker,int defender,int attacker_assistant,int end_timestamp,bool force);

namespace GNET
{

class SendSiegeStart : public GNET::Protocol
{
	#include "sendsiegestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		battlefield_start(battle_id,world_tag,attacker,owner,assistant,endtime,(bool)force);
	}
};

};

#endif
