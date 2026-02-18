
#ifndef __GNET_QUERYUSERPRIVILEGE2_HPP
#define __GNET_QUERYUSERPRIVILEGE2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueryUserPrivilege2 : public GNET::Protocol
{
	#include "queryuserprivilege2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
