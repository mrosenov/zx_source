
#ifndef __GNET_QUERYUSERFORBID2_HPP
#define __GNET_QUERYUSERFORBID2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueryUserForbid2 : public GNET::Protocol
{
	#include "queryuserforbid2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
