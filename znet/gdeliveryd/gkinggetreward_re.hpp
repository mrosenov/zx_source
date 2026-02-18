
#ifndef __GNET_GKINGGETREWARD_RE_HPP
#define __GNET_GKINGGETREWARD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingGetReward_Re : public GNET::Protocol
{
	#include "gkinggetreward_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
