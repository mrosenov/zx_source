
#ifndef __GNET_SENDRAIDSTART_RE_HPP
#define __GNET_SENDRAIDSTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendRaidStart_Re : public GNET::Protocol
{
	#include "sendraidstart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
