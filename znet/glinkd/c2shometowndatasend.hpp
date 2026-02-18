
#ifndef __GNET_C2SHOMETOWNDATASEND_HPP
#define __GNET_C2SHOMETOWNDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class C2SHometowndataSend : public GNET::Protocol
{
	#include "c2shometowndatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
