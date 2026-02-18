
#ifndef __GNET_BATTLEFLAGBUFFSCOPE_HPP
#define __GNET_BATTLEFLAGBUFFSCOPE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleFlagBuffScope : public GNET::Protocol
{
	#include "battleflagbuffscope"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
