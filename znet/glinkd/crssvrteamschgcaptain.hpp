
#ifndef __GNET_CRSSVRTEAMSCHGCAPTAIN_HPP
#define __GNET_CRSSVRTEAMSCHGCAPTAIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsChgCaptain : public GNET::Protocol
{
	#include "crssvrteamschgcaptain"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsChgCaptain master_new=%d, teamid=%d, localsid=%d", master_new, teamid, sid);

		localsid = sid;

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
