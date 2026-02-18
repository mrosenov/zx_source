
#ifndef __GNET_GBATTLELEAVE_HPP
#define __GNET_GBATTLELEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GBattleLeave : public GNET::Protocol
{
	#include "gbattleleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
