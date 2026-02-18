
#ifndef __GNET_SYNCPLAYERFAMILY_HPP
#define __GNET_SYNCPLAYERFAMILY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SyncPlayerFamily : public GNET::Protocol
{
	#include "syncplayerfamily"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
