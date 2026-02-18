
#ifndef __GNET_COMBATTOP_HPP
#define __GNET_COMBATTOP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "combattop_re.hpp"

namespace GNET
{

class CombatTop : public GNET::Protocol
{
	#include "combattop"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("CombatTop: roleid=%d, mapid=%d", roleid, mapid);
		CombatTop_Re re;
		re.handle = handle;
		re.localsid = localsid;
		re.mapid = mapid;
		CombatManager::Instance()->GetTop(factionid,mapid,re.handle,re.list,re.attacker,re.defender); 
		manager->Send(sid, re);
	}
};

};

#endif
