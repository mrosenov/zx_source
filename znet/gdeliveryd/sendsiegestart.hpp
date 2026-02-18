
#ifndef __GNET_SENDSIEGESTART_HPP
#define __GNET_SENDSIEGESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendSiegeStart : public GNET::Protocol
{
	#include "sendsiegestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
