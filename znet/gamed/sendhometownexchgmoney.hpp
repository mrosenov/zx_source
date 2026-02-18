
#ifndef __GNET_SENDHOMETOWNEXCHGMONEY_HPP
#define __GNET_SENDHOMETOWNEXCHGMONEY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class SendHometownExchgMoney : public GNET::Protocol
{
	#include "sendhometownexchgmoney"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
