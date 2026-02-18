
#ifndef __GNET_GATEONLINELIST_RE_HPP
#define __GNET_GATEONLINELIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GateOnlineList_Re : public GNET::Protocol
{
	#include "gateonlinelist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
