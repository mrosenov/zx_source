
#ifndef __GNET_GOPENBANQUETROLEDEL_HPP
#define __GNET_GOPENBANQUETROLEDEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


extern void flow_battle_clear_leave(int world_tag, int role_id, char faction);
namespace GNET
{

class GOpenBanquetRoleDel : public GNET::Protocol
{
	#include "gopenbanquetroledel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		flow_battle_clear_leave(map_id, roleid, faction);
	}
};

};

#endif
