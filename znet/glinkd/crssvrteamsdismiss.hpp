
#ifndef __GNET_CRSSVRTEAMSDISMISS_HPP
#define __GNET_CRSSVRTEAMSDISMISS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsDismiss : public GNET::Protocol
{
	#include "crssvrteamsdismiss"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsDismiss roleid=%d, teamid=%d, localsid=%d", roleid, teamid, sid);

		localsid = sid;

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
