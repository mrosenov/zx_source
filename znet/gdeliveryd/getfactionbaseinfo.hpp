
#ifndef __GNET_GETFACTIONBASEINFO_HPP
#define __GNET_GETFACTIONBASEINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "getfactionbaseinfo_re.hpp"
#include "factionmanager.h"

namespace GNET
{

class GetFactionBaseInfo : public GNET::Protocol
{
	#include "getfactionbaseinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GetFactionBaseInfo_Re re;
		re.roleid = roleid;
		re.localsid = localsid;
		re.scale = scale;
		switch (scale)
		{
		case 0 : // faction
			for(IntVector::iterator it=factionlist.begin();it!=factionlist.end();it++)
			{
				if(FactionManager::Instance()->GetBaseInfo(*it, re.faction_info))
				{
					if(!manager->Send(sid, re))
						return;
				}
			}
			break;
		case 1 : // family
			for(IntVector::iterator it=factionlist.begin();it!=factionlist.end();it++)
			{
				if(FamilyManager::Instance()->GetBaseInfo(*it, re.faction_info))
				{
					if(!manager->Send(sid, re))
						return;
				}
			}
			break;
		}
	}
};

};

#endif
