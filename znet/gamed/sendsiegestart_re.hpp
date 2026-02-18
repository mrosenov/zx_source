
#ifndef __GNET_SENDSIEGESTART_RE_HPP
#define __GNET_SENDSIEGESTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendSiegeStart_Re : public GNET::Protocol
{
	#include "sendsiegestart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
