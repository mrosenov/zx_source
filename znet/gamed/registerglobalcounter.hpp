
#ifndef __GNET_REGISTERGLOBALCOUNTER_HPP
#define __GNET_REGISTERGLOBALCOUNTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "globalcounterinfo"

namespace GNET
{

class RegisterGlobalCounter : public GNET::Protocol
{
	#include "registerglobalcounter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
