
#ifndef __GNET_GRAIDEND_HPP
#define __GNET_GRAIDEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GRaidEnd : public GNET::Protocol
{
	#include "graidend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
