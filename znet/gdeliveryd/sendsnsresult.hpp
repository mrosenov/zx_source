
#ifndef __GNET_SENDSNSRESULT_HPP
#define __GNET_SENDSNSRESULT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendSNSResult : public GNET::Protocol
{
	#include "sendsnsresult"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
