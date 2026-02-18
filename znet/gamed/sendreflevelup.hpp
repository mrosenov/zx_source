
#ifndef __GNET_SENDREFLEVELUP_HPP
#define __GNET_SENDREFLEVELUP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendRefLevelUp : public GNET::Protocol
{
	#include "sendreflevelup"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
