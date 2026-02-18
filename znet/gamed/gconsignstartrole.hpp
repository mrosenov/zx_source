
#ifndef __GNET_GCONSIGNSTARTROLE_HPP
#define __GNET_GCONSIGNSTARTROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GConsignStartRole : public GNET::Protocol
{
	#include "gconsignstartrole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
