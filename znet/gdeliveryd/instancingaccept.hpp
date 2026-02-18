
#ifndef __GNET_INSTANCINGACCEPT_HPP
#define __GNET_INSTANCINGACCEPT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "instancingaccept_re.hpp"

namespace GNET
{

class InstancingAccept : public GNET::Protocol
{
	#include "instancingaccept"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (roleid == requester)
			return;
		int retcode = ERR_SUCCESS;
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL==pinfo)
			return;
		PlayerInfo *ptarget = UserContainer::GetInstance().FindRoleOnline(requester);
		if (ptarget == NULL)
			retcode = ERR_BATTLE_ACCEPT_REQUESTERNOTFOUND;

		GDeliveryServer *dsm = GDeliveryServer::GetInstance(); 
		if (retcode != ERR_SUCCESS)
		{
			dsm->Send(sid, InstancingAccept_Re(retcode, accept, roleid, map_id, requester, localsid));
			return;
		}
		retcode = InstancingManager::GetInstance()->Accept(roleid, accept, pinfo->gameid, map_id, requester);
		dsm->Send(sid, InstancingAccept_Re(retcode, accept, roleid, map_id, requester, localsid));
	}
};

};

#endif
