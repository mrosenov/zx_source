
#ifndef __GNET_GBATTLEJOIN_HPP
#define __GNET_GBATTLEJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GBattleJoin : public GNET::Protocol
{
	#include "gbattlejoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
