
#ifndef __GNET_GETFACTIONHOSTILE_HPP
#define __GNET_GETFACTIONHOSTILE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "hostilefaction"
#include "getfactionhostile_re.hpp"

namespace GNET
{

class GetFactionHostile : public GNET::Protocol
{
	#include "getfactionhostile"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		/*
		GetFactionHostile_Re re;
		re.roleid = roleid;
		re.localsid = localsid;
		if(FactionManager::Instance()->GetHostileInfo(factionid, re.hostiles))
		{
			if(!manager->Send(sid, re))
				return;
		}
		*/
	}
};

};

#endif
