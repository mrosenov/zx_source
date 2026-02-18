
#ifndef __GNET_S2CHOMETOWNDATASEND_HPP
#define __GNET_S2CHOMETOWNDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class S2CHometowndataSend : public GNET::Protocol
{
	#include "s2chometowndatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
