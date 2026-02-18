
#ifndef __GNET_BATTLEJOIN_HPP
#define __GNET_BATTLEJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battlejoin_re.hpp"
#include "battlemanager.h"

namespace GNET
{

class BattleJoin : public GNET::Protocol
{
	#include "battlejoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//与gbattlejoin.hpp保持一致
		LOG_TRACE("BattleJoin: roleid=%d, gs_id=%d, map_id=%d, team=%d, isreenter=%d", roleid, gs_id, map_id, team, isreenter);
		BattleRole role;
		bool b_gm = false;
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if ( NULL==pinfo)
				return;
			GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
			if (prole == NULL)
				return;
			if (gs_id != pinfo->gameid)
			{
				GDeliveryServer::GetInstance()->Send(pinfo->linksid, 
					BattleJoin_Re(ERR_BATTLE_GAME_SERVER, roleid, gs_id, map_id, 0, 0, 0, pinfo->localsid,0));
				return;
			}
			if (pinfo->IsGM())
				b_gm = true;
			role.roleid = pinfo->roleid;
			role.level = pinfo->level;
			role.name = pinfo->name;
			role.occupation = pinfo->occupation;
			role.score = pinfo->battlescore;
			role.reborn_cnt = prole->reborn_cnt;
			role.localsid = pinfo->localsid;
			role.linksid = sid;
			role.cultivation = prole->cultivation; 
			role.src_zoneid = pinfo->user->src_zoneid;
			role.is_reenter = isreenter;
		}
		int starttime = 0;
		char battletype = 0;
		int cooltime = -1;
		int retcode = BattleManager::GetInstance()->JoinTeam(role, gs_id, map_id, team, starttime, cooltime, battletype, b_gm); 	
		if (retcode != ERR_SUCCESS)
		{
			GDeliveryServer::GetInstance()->Send(sid, BattleJoin_Re(retcode, roleid, gs_id, map_id, battletype, 0, team, starttime, role.localsid, cooltime));
		}
	}
};

};

#endif
