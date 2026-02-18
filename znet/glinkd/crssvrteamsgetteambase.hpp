
#ifndef __GNET_CRSSVRTEAMSGETTEAMBASE_HPP
#define __GNET_CRSSVRTEAMSGETTEAMBASE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsGetTeamBase : public GNET::Protocol
{
	#include "crssvrteamsgetteambase"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsGetTeamBase roleid=%d, teamid=%d, localsid=%d", roleid, teamid, sid);

		localsid = sid;

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
