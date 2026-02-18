
#ifndef __GNET_GTERRITORYENTER_HPP
#define __GNET_GTERRITORYENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "territoryenter_re.hpp"
#include "playerchangegs.hpp"

namespace GNET
{

class GTerritoryEnter : public GNET::Protocol
{
	#include "gterritoryenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gterritoryenter:roleid=%d,tid=%d,fid=%d",roleid, territoryid, factionid);
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL)
			return;
		int dst_gs = 0, map_tag = 0;
		int retcode = TerritoryManager::GetInstance()->Enter(*this, dst_gs, map_tag, pinfo);
		if (retcode != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, TerritoryEnter_Re(retcode, pinfo->localsid));
		if (retcode == ERR_SUCCESS)
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, dst_gs, map_tag));
		LOG_TRACE("gterritoryenter:roleid=%d,map_tag=%d,retcode=%d",roleid, map_tag, retcode);
	}
};

};

#endif
