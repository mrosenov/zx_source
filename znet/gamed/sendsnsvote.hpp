
#ifndef __GNET_SENDSNSVOTE_HPP
#define __GNET_SENDSNSVOTE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendSNSVote : public GNET::Protocol
{
	#include "sendsnsvote"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
