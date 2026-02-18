
#ifndef __GNET_BATTLEGETLIST_HPP
#define __GNET_BATTLEGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battlemanager.h"

namespace GNET
{

class BattleGetList : public GNET::Protocol
{
	#include "battlegetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleGetList: roleid=%d, battle_type=%d", roleid, battle_type);
		BattleManager::GetInstance()->GetMap(roleid, battle_type, sid, localsid);
	}
};

};

#endif
