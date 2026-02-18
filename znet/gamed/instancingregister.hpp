
#ifndef __GNET_INSTANCINGREGISTER_HPP
#define __GNET_INSTANCINGREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "instancingfieldinfo"

namespace GNET
{

class InstancingRegister : public GNET::Protocol
{
	#include "instancingregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
