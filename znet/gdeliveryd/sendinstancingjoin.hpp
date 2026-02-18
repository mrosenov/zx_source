
#ifndef __GNET_SENDINSTANCINGJOIN_HPP
#define __GNET_SENDINSTANCINGJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "instancingjoin_re.hpp"
#include "instancingmanager.h"

namespace GNET
{

class SendInstancingJoin : public GNET::Protocol
{
	#include "sendinstancingjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		InstancingRole role;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
			if (NULL==pinfo || prole==NULL)
			{
				manager->Send(sid, GMailEndSync(0, ERR_SIEGE_FACTIONLIMIT, roleid));
				return;
			}
			role.roleid = pinfo->roleid;
			role.level = pinfo->level;
			role.name = pinfo->name;
			role.occupation = pinfo->occupation;
			role.score = pinfo->battlescore;
			role.reborn_cnt = prole->reborn_cnt;
			role.localsid = pinfo->localsid;
			role.linksid = pinfo->linksid;
			role.cultivation = prole->cultivation;
		}

		int retcode = InstancingManager::GetInstance()->JoinTeam(role, gs_id, map_id, team, syncdata);
		if (retcode != ERR_SUCCESS)
		{
			InstancingJoin_Re re(retcode, gs_id, map_id, team, 0, role.localsid);
			GDeliveryServer::GetInstance()->Send(role.linksid, re);
			manager->Send(sid, GMailEndSync(0, retcode, roleid));
		}
		LOG_TRACE("SendInstancingJoin: retcode=%d, roleid=%d, gs_id=%d, map_id=%d, team=%d", retcode, roleid, gs_id, map_id, team);
	}
};

};

#endif
