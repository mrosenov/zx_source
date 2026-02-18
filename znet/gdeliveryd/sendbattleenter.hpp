
#ifndef __GNET_SENDBATTLEENTER_HPP
#define __GNET_SENDBATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SendBattleEnter : public GNET::Protocol
{
	#include "sendbattleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
