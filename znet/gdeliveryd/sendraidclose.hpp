
#ifndef __GNET_SENDRAIDCLOSE_HPP
#define __GNET_SENDRAIDCLOSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendRaidClose : public GNET::Protocol
{
	#include "sendraidclose"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
