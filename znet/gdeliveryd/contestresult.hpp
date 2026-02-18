
#ifndef __GNET_CONTESTRESULT_HPP
#define __GNET_CONTESTRESULT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestResult : public GNET::Protocol
{
	#include "contestresult"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
