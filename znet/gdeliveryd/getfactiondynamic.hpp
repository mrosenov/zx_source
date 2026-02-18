
#ifndef __GNET_GETFACTIONDYNAMIC_HPP
#define __GNET_GETFACTIONDYNAMIC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "getfactiondynamic_re.hpp"

namespace GNET
{

class GetFactionDynamic : public GNET::Protocol
{
	#include "getfactiondynamic"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("getfactiondynamic roleid=%d pageid=%d", roleid, pageid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole((roleid));
		if (pinfo == NULL || pinfo->factionid == 0)
			return;
		GetFactionDynamic_Re re(ERR_SUCCESS, roleid, pageid, localsid, pinfo->factionid);
		FactionManager::Instance()->GetDynamic(pinfo->factionid, pageid, re.totalsize, re.dynamic);
		manager->Send(sid, re);
	}
};

};

#endif
