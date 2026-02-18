
#ifndef __GNET_KINGDOMANNOUNCE_RE_HPP
#define __GNET_KINGDOMANNOUNCE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomAnnounce_Re : public GNET::Protocol
{
	#include "kingdomannounce_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid ,this);	
	}
};

};

#endif
