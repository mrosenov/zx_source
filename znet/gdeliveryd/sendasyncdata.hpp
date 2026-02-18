
#ifndef __GNET_SENDASYNCDATA_HPP
#define __GNET_SENDASYNCDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SendAsyncData : public GNET::Protocol
{
	#include "sendasyncdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
