
#ifndef __GNET_GRAIDMEMBERCHANGE_HPP
#define __GNET_GRAIDMEMBERCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

extern void raid_member_change(int role_id, int gs_id, int map_id, int index, unsigned char operation);

namespace GNET
{

class GRaidMemberChange : public GNET::Protocol
{
	#include "graidmemberchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		raid_member_change(roleid, gs_id, map_id, index, operation);
	}
};

};

#endif
