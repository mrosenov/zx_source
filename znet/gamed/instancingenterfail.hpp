
#ifndef __GNET_INSTANCINGENTERFAIL_HPP
#define __GNET_INSTANCINGENTERFAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingEnterFail : public GNET::Protocol
{
	#include "instancingenterfail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
