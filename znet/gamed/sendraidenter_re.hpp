
#ifndef __GNET_SENDRAIDENTER_RE_HPP
#define __GNET_SENDRAIDENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendRaidEnter_Re : public GNET::Protocol
{
	#include "sendraidenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
