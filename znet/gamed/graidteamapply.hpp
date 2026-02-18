
#ifndef __GNET_GRAIDTEAMAPPLY_HPP
#define __GNET_GRAIDTEAMAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "teamraidapplyingrole"

namespace GNET
{

class GRaidTeamApply : public GNET::Protocol
{
	#include "graidteamapply"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
