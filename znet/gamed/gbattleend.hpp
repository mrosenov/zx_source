
#ifndef __GNET_GBATTLEEND_HPP
#define __GNET_GBATTLEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class GBattleEnd : public GNET::Protocol
{
	#include "gbattleend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
