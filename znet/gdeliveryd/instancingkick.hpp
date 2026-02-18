
#ifndef __GNET_INSTANCINGKICK_HPP
#define __GNET_INSTANCINGKICK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "instancingkicknotify.hpp"

namespace GNET
{

class InstancingKick : public GNET::Protocol
{
	#include "instancingkick"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (roleid == target) return;
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL==pinfo)
			return;
		int retcode = InstancingManager::GetInstance()->Kick(roleid, pinfo->gameid, map_id, target);
		manager->Send(sid, InstancingKick_Re(retcode, roleid, map_id, target, localsid));
		if (retcode == ERR_SUCCESS)
		{
			PlayerInfo *ptargetinfo = UserContainer::GetInstance().FindRoleOnline(target);
			if (NULL != ptargetinfo)
				manager->Send(sid, InstancingKickNotify(roleid, map_id, target, ptargetinfo->localsid));
		}
	}
};

};

#endif
