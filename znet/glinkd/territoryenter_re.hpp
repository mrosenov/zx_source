
#ifndef __GNET_TERRITORYENTER_RE_HPP
#define __GNET_TERRITORYENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TerritoryEnter_Re : public GNET::Protocol
{
	#include "territoryenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
