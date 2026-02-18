
#ifndef __GNET_KINGCALLGUARD_HPP
#define __GNET_KINGCALLGUARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingCallGuard : public GNET::Protocol
{
	#include "kingcallguard"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
