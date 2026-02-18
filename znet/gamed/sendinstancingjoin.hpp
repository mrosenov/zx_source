
#ifndef __GNET_SENDINSTANCINGJOIN_HPP
#define __GNET_SENDINSTANCINGJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class SendInstancingJoin : public GNET::Protocol
{
	#include "sendinstancingjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
