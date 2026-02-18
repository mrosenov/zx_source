
#ifndef __GNET_BATTLELEAVE_RE_HPP
#define __GNET_BATTLELEAVE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "crossbattlemanager.hpp"

namespace GNET
{

class BattleLeave_Re : public GNET::Protocol
{
	#include "battleleave_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleLeave_Re::retcode=%d, roleid=%d, localsid=%d, iscross=%d", retcode, roleid, localsid, iscross);
		PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
		int linksid = (info != NULL) ? info->linksid : 0;
		CrossBattleManager::GetInstance()->LeaveBattle(NORMAL_BATTLE, roleid, linksid);
	}
};

};

#endif
