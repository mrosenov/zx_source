
#ifndef __GNET_PLAYERCHANGEGS_HPP
#define __GNET_PLAYERCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void player_change_gs( int localsid, int roleid, int target_gs , int tag, float x, float y, float z, int reason);
namespace GNET
{

class PlayerChangeGS : public GNET::Protocol
{
	#include "playerchangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		player_change_gs(localsid,roleid,gslineid,mapid,scalex,scaley,scalez,reason);
	}
};

};

#endif
