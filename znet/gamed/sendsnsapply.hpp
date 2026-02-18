
#ifndef __GNET_SENDSNSAPPLY_HPP
#define __GNET_SENDSNSAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendSNSApply : public GNET::Protocol
{
	#include "sendsnsapply"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
