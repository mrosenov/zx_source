
#ifndef __GNET_GTERRITORYCHALLENGE_HPP
#define __GNET_GTERRITORYCHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GTerritoryChallenge : public GNET::Protocol
{
	#include "gterritorychallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo==NULL)
			return;
		int retcode = TerritoryManager::GetInstance()->Challenge(*this);
		if (retcode != ERR_SUCCESS)
		{
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, TerritoryChallenge_Re(retcode, roleid, territoryid, 0, pinfo->localsid));
			manager->Send(sid, GMailEndSync(0, retcode, roleid));
		}
		LOG_TRACE("GTerritoryChallenge roleid=%d,tid=%d,factionid=%d,itemid=%d,item_count=%d,retcode=%d", roleid,territoryid,factionid,itemid,item_count, retcode);
	}
};

};

#endif
