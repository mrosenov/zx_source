
#ifndef __GNET_CRSSVRTEAMSLEAVE_HPP
#define __GNET_CRSSVRTEAMSLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsLeave : public GNET::Protocol
{
	#include "crssvrteamsleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsLeave roleid=%d, teamid=%d, localsid=%d", roleid, teamid, sid);

		localsid = sid;

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
