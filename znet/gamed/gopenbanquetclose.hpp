
#ifndef __GNET_GOPENBANQUETCLOSE_HPP
#define __GNET_GOPENBANQUETCLOSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


void close_flow_battle(int gs_id, int map_id);
namespace GNET
{

class GOpenBanquetClose : public GNET::Protocol
{
	#include "gopenbanquetclose"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		close_flow_battle(gs_id, map_id);
	}
};

};

#endif
