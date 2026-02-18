
#ifndef __GNET_KINGDOMDISCHARGE_RE_HPP
#define __GNET_KINGDOMDISCHARGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomDischarge_Re : public GNET::Protocol
{
	#include "kingdomdischarge_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid ,this);	
	}
};

};

#endif
