
#ifndef __GNET_GKINGDOMBATTLEREGISTER_HPP
#define __GNET_GKINGDOMBATTLEREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "kingdombattleinfo"

namespace GNET
{

class GKingdomBattleRegister : public GNET::Protocol
{
	#include "gkingdombattleregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gkingdombattleregister gsid %d fields count %d", gsid, fields.size());
		KingdomManager::GetInstance()->RegisterField(gsid, fields);
	}
};

};

#endif
