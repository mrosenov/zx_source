
#ifndef __GNET_CRSSVRTEAMSRECRUIT_HPP
#define __GNET_CRSSVRTEAMSRECRUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsRecruit : public GNET::Protocol
{
	#include "crssvrteamsrecruit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsRecruit roleid=%d, teamid=%d, invitee=%d, name=%.*s, localsid=%d", roleid, teamid, invitee, name.size(), (char*)name.begin(), sid);

		localsid = sid;

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
