
#ifndef __GNET_GBATTLEJOIN_HPP
#define __GNET_GBATTLEJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GBattleJoin : public GNET::Protocol
{
	#include "gbattlejoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//与battlejoin.hpp保持一致
		LOG_TRACE("GBattleJoin: roleid=%d, gs_id=%d, map_id=%d, itemid=%d, itempos=%d", roleid, gs_id, map_id, itemid, item_pos);
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
					BattleJoin_Re(ERR_BATTLE_GAME_SERVER, roleid, gs_id, map_id, 0, 0, 0, 0, pinfo->localsid));
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
			role.linksid = pinfo->linksid;
			role.src_zoneid = pinfo->user->src_zoneid;
		}
		int starttime = 0;
		char battletype = 0;
		int cooltime = -1;
		int retcode = BattleManager::GetInstance()->JoinTeam(role, gs_id, map_id, 0, starttime, cooltime, battletype, b_gm, itemid, item_pos, syncdata); 	
		if (retcode != ERR_SUCCESS)
		{
			GDeliveryServer::GetInstance()->Send(role.linksid, BattleJoin_Re(retcode, roleid, gs_id, map_id, 0, 0, 0, starttime, role.localsid, cooltime));
			GProviderServer::GetInstance()->DispatchProtocol(gs_id, GMailEndSync(0, retcode, roleid));
		}
	}
};

};

#endif
