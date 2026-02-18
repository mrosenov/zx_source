
#ifndef __GNET_KINGDOMBATTLESTART_RE_HPP
#define __GNET_KINGDOMBATTLESTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomBattleStart_Re : public GNET::Protocol
{
	#include "kingdombattlestart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
