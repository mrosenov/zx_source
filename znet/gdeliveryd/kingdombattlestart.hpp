
#ifndef __GNET_KINGDOMBATTLESTART_HPP
#define __GNET_KINGDOMBATTLESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomBattleStart : public GNET::Protocol
{
	#include "kingdombattlestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
