
#ifndef __GNET_GATEONLINELIST_HPP
#define __GNET_GATEONLINELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggateonlineinfo"

namespace GNET
{

class GateOnlineList : public GNET::Protocol
{
	#include "gateonlinelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
