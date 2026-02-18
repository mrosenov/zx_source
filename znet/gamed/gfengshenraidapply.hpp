
#ifndef __GNET_GFENGSHENRAIDAPPLY_HPP
#define __GNET_GFENGSHENRAIDAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "teamraidapplyingrole"

namespace GNET
{

class GFengShenRaidApply : public GNET::Protocol
{
	#include "gfengshenraidapply"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
