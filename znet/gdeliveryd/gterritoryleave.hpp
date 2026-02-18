
#ifndef __GNET_GTERRITORYLEAVE_HPP
#define __GNET_GTERRITORYLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "territoryleave_re.hpp"

namespace GNET
{

class GTerritoryLeave : public GNET::Protocol
{
	#include "gterritoryleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL)
			return;
		int retcode = TerritoryManager::GetInstance()->Leave(*this);
		if (retcode != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, TerritoryLeave_Re(retcode, pinfo->localsid));
		if (retcode == ERR_SUCCESS && pinfo->old_gs != _GAMESERVER_ID_INVALID)
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, pinfo->old_gs, map_tag, 0, scalex, scaley, scalez));
		LOG_TRACE("gterritoryleave:roleid=%d,tid=%d,map=%d,x=%f,y=%f,z=%f,retcode=%d",roleid, territoryid, map_tag, scalex, scaley, scalez, retcode);
	}
};

};

#endif
