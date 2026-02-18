
#ifndef __GNET_INSTANCINGAPPOINT_HPP
#define __GNET_INSTANCINGAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAppoint : public GNET::Protocol
{
	#include "instancingappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int gs_id ;
		int retcode = ERR_SUCCESS;
		int target_localsid;
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * src_info = UserContainer::GetInstance().FindRoleOnline(roleid);
			if ( NULL==src_info )
				return;
			gs_id = src_info->gameid;
		}
		PlayerInfo *target_info = NULL;
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			target_info = UserContainer::GetInstance().FindRoleOnline(target);
			if ( NULL==target_info )
				retcode = ERR_BATTLE_APPOINT_TARGETNOTFOUND;
			else
				target_localsid = target_info->localsid;
		}
		GDeliveryServer *dsm = GDeliveryServer::GetInstance(); 
		if (retcode != ERR_SUCCESS)
		{
			dsm->Send(sid, InstancingAppoint_Re(retcode, roleid, map_id, target, localsid));
			return;
		}

		retcode = InstancingManager::GetInstance()->Appoint(roleid, gs_id, map_id, target);
		dsm->Send(sid, InstancingAppoint_Re(retcode, roleid, map_id, target, localsid));
		dsm->Send(target_info->linksid, InstancingAppointNotify(roleid, map_id, target_localsid));
	}
};

};

#endif
