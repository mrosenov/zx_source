
#ifndef __GNET_BATTLEENTERFAIL_HPP
#define __GNET_BATTLEENTERFAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleEnterFail : public GNET::Protocol
{
	#include "battleenterfail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
