
#ifndef __GNET_SENDINSTANCINGENTER_HPP
#define __GNET_SENDINSTANCINGENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendInstancingEnter : public GNET::Protocol
{
	#include "sendinstancingenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
			}
};

};

#endif
