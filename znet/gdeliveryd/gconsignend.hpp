
#ifndef __GNET_GCONSIGNEND_HPP
#define __GNET_GCONSIGNEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GConsignEnd : public GNET::Protocol
{
	#include "gconsignend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
