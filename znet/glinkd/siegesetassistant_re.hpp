
#ifndef __GNET_SIEGESETASSISTANT_RE_HPP
#define __GNET_SIEGESETASSISTANT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeSetAssistant_Re : public GNET::Protocol
{
	#include "siegesetassistant_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
