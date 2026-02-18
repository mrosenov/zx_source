
#ifndef __GNET_CONTESTINVITEBROADCAST_HPP
#define __GNET_CONTESTINVITEBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestInviteBroadcast : public GNET::Protocol
{
	#include "contestinvitebroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
