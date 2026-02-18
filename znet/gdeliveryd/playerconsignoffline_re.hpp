
#ifndef __GNET_PLAYERCONSIGNOFFLINE_RE_HPP
#define __GNET_PLAYERCONSIGNOFFLINE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"
#include "blockedrole.h"
namespace GNET
{

class PlayerConsignOffline_Re : public GNET::Protocol
{
	#include "playerconsignoffline_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(result == ERR_SUCCESS && RoleTaskManager::GetInstance()->ConsignRoleOfflineAddData(roleid,gsroleinfo)==0)
			BlockedRole::GetInstance()->CompleteRoleLock(roleid);
//		elsewise, role lock status would be modified on update

	//	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	//	if(pinfo && pinfo->user)
	//		UserContainer::GetInstance().RoleLogout(pinfo->user, true);
		DEBUG_PRINT("PlayerConsignOffline_Re, roleid=%d, result=%d",roleid,result);
	}
};

};

#endif
