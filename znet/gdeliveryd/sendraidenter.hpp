
#ifndef __GNET_SENDRAIDENTER_HPP
#define __GNET_SENDRAIDENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendRaidEnter : public GNET::Protocol
{
	#include "sendraidenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
