
#ifndef __GNET_PLAYERCHANGEGS_HPP
#define __GNET_PLAYERCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class PlayerChangeGS : public GNET::Protocol
{
	#include "playerchangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
