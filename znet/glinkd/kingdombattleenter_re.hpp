
#ifndef __GNET_KINGDOMBATTLEENTER_RE_HPP
#define __GNET_KINGDOMBATTLEENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomBattleEnter_Re : public GNET::Protocol
{
	#include "kingdombattleenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
