
#ifndef __GNET_CRSSVRTEAMSGETROLENOTIFY_HPP
#define __GNET_CRSSVRTEAMSGETROLENOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsGetRoleNotify : public GNET::Protocol
{
	#include "crssvrteamsgetrolenotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsGetRoleNotify roleid=%d, localsid=%d", roleid, sid);

		localsid = sid;

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
