
#ifndef __GNET_SENDSNSPRESSMESSAGE_HPP
#define __GNET_SENDSNSPRESSMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gapplycondition"

namespace GNET
{

class SendSNSPressMessage : public GNET::Protocol
{
	#include "sendsnspressmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
