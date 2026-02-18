
#ifndef __GNET_GCONSIGNSTART_HPP
#define __GNET_GCONSIGNSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class GConsignStart : public GNET::Protocol
{
	#include "gconsignstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
