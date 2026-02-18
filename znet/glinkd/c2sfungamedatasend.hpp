
#ifndef __GNET_C2SFUNGAMEDATASEND_HPP
#define __GNET_C2SFUNGAMEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class C2SFunGamedataSend : public GNET::Protocol
{
	#include "c2sfungamedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
