
#ifndef __GNET_PLAYERCONSIGNOFFLINE_HPP
#define __GNET_PLAYERCONSIGNOFFLINE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void user_offline_consignrole(int cs_index, int sid, int uid);

namespace GNET
{

class PlayerConsignOffline : public GNET::Protocol
{
	#include "playerconsignoffline"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		user_offline_consignrole(provider_link_id, localsid, roleid);
	}
};

};

#endif
