
#ifndef __GNET_SIEGEKILL_HPP
#define __GNET_SIEGEKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeKill : public GNET::Protocol
{
	#include "siegekill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int factionid;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(killer);
			if (NULL==pinfo)
				return;
			factionid = pinfo->factionid;
		}
		SiegeManager::GetInstance()->UpdateKill(mapid, killer, factionid, victim, victim_faction);
	}
};

};

#endif
