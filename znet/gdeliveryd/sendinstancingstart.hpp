
#ifndef __GNET_SENDINSTANCINGSTART_HPP
#define __GNET_SENDINSTANCINGSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendInstancingStart : public GNET::Protocol
{
	#include "sendinstancingstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
