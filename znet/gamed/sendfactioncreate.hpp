
#ifndef __GNET_SENDFACTIONCREATE_HPP
#define __GNET_SENDFACTIONCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SendFactionCreate : public GNET::Protocol
{
	#include "sendfactioncreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
