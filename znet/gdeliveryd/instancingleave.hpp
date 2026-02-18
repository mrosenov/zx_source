
#ifndef __GNET_INSTANCINGLEAVE_HPP
#define __GNET_INSTANCINGLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingLeave : public GNET::Protocol
{
	#include "instancingleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int gs_id ;
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if ( NULL==pinfo )
				return;
			gs_id = pinfo->gameid;
		}
		int retcode = InstancingManager::GetInstance()->LeaveTeam(roleid, gs_id, map_id);
		GDeliveryServer::GetInstance()->Send(sid, InstancingLeave_Re(retcode, map_id, localsid));
	}
};

};

#endif
