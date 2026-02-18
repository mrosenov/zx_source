
#ifndef __GNET_QUERYROLENAMEHISTORY_RE_HPP
#define __GNET_QUERYROLENAMEHISTORY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "namehistory"

namespace GNET
{

class QueryRolenameHistory_Re : public GNET::Protocol
{
	#include "queryrolenamehistory_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
