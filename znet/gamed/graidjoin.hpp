
#ifndef __GNET_GRAIDJOIN_HPP
#define __GNET_GRAIDJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GRaidJoin : public GNET::Protocol
{
	#include "graidjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
