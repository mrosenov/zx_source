
#ifndef __GNET_CRSSVRTEAMSSEARCH_RE_HPP
#define __GNET_CRSSVRTEAMSSEARCH_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcrssvrteamsdata"
#include "gdeliveryserver.hpp"

namespace GNET
{

class CrssvrTeamsSearch_Re : public GNET::Protocol
{
	#include "crssvrteamssearch_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsSearch_Re roleid=%d", roleid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL != pinfo )
		{
			GDeliveryServer * dsm = GDeliveryServer::GetInstance();
			(*this).localsid = pinfo->localsid;
			dsm->Send(pinfo->linksid, this);
		}
	}
};

};

#endif
