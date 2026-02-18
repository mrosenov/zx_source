
#ifndef __GNET_SENDREFADDEXP_HPP
#define __GNET_SENDREFADDEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendRefAddExp : public GNET::Protocol
{
	#include "sendrefaddexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
