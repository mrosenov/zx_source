
#ifndef __GNET_SENDRAIDKICKOUT_HPP
#define __GNET_SENDRAIDKICKOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendRaidKickout : public GNET::Protocol
{
	#include "sendraidkickout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
