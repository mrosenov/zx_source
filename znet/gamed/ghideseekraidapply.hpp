
#ifndef __GNET_GHIDESEEKRAIDAPPLY_HPP
#define __GNET_GHIDESEEKRAIDAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "teamraidapplyingrole"

namespace GNET
{

class GHideSeekRaidApply : public GNET::Protocol
{
	#include "ghideseekraidapply"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
