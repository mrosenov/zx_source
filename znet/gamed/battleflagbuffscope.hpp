
#ifndef __GNET_BATTLEFLAGBUFFSCOPE_HPP
#define __GNET_BATTLEFLAGBUFFSCOPE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

extern void battle_flag_buff_scope(const std::vector<int>& role_list, int flagid, int worldtag, int remaintime);

namespace GNET
{

class BattleFlagBuffScope : public GNET::Protocol
{
	#include "battleflagbuffscope"

	void Process(Manager *manager, Manager::Session::ID sid)
	{	
		battle_flag_buff_scope(roles, flagid, worldtag, remaintime);
	}
};

};

#endif
