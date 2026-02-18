
#ifndef __GNET_GCHANGEFACTIONNAME_RE_HPP
#define __GNET_GCHANGEFACTIONNAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GChangeFactionName_Re : public GNET::Protocol
{
	#include "gchangefactionname_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
