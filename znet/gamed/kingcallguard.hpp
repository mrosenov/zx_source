
#ifndef __GNET_KINGCALLGUARD_HPP
#define __GNET_KINGCALLGUARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

bool guard_call_by_king(int roleid, int lineid, int mapid, float pos_x, float pos_y, float pos_z);

namespace GNET
{

class KingCallGuard : public GNET::Protocol
{
	#include "kingcallguard"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		guard_call_by_king(roleid, lineid, mapid, posx, posy, posz);
	}
};

};

#endif
