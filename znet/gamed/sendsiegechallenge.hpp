
#ifndef __GNET_SENDSIEGECHALLENGE_HPP
#define __GNET_SENDSIEGECHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class SendSiegeChallenge : public GNET::Protocol
{
	#include "sendsiegechallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
