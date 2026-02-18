
#ifndef __GNET_OPENBANQUETENTER_HPP
#define __GNET_OPENBANQUETENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "playerchangegs.hpp"
#include "battleenter_re.hpp"
#include "crossbattlemanager.hpp"
#include "openbanquetmanager.h"


namespace GNET
{

class OpenBanquetEnter : public GNET::Protocol
{
	#include "openbanquetenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetEnter roleid=%d, gs_id=%d, map_id=%d, iscross=%d, localsid=%d", roleid, gs_id, map_id, iscross, localsid);

		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		bool b_gm = false;
		if(iscross && !iscentral) // 在原服同意进入战场，需要将消息转发给跨服服务器
		{
			CrossBattleManager::GetInstance()->EnterOpenBanquetBattle(OPENBANQUET_BATTLE, roleid);
			return;
		}
		else if(!iscross) // 角色在所在服务器同意进入战场，走原战场逻辑
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
			if ( NULL==pinfo )
			{
				return;
			}
			if (pinfo->IsGM())
				b_gm = true;
		}

		BattleEnter_Re res;
		res.roleid = roleid;
		res.localsid = localsid;
		res.endtime = 0;
		res.iscross = iscross;
		res.battle_type = OPENBANQUET_BATTLE;
		if(iscentral)
		{
			// 跨服 - 跨服
			res.retcode = OpenBanquetManager::GetInstance()->OnTryEnter(roleid, gs_id, map_id, res.endtime, res.battle_type, iscross, b_gm);	
		}
		LOG_TRACE( "openbanquetenter:roleid=%d retcode=%d gs_id=%d map_id=%d iscross=%d endtime=%d enter battle endtime =%s\n", roleid, res.retcode, gs_id, map_id, iscross, res.endtime, ctime((time_t*)&res.endtime));
		if(res.retcode != ERR_SUCCESS)
		{
			CrossBattleManager::GetInstance()->EraseFightingTag(roleid);
		}
		else
		{
			CrossBattleManager::GetInstance()->ActiveFightingTag(roleid);
		}
		if(!iscross)
		{
			GDeliveryServer::GetInstance()->Send(sid, res);
			// 进入所在服务器战场的，首先回复是否进入成功，然后如果是进入跨服战场，则还需要通知原服
			if(iscentral)
			{
				int zoneid = CrossBattleManager::GetInstance()->GetSrcZoneID(roleid);
				if(res.retcode == ERR_SUCCESS)
				{
					res.retcode = ERR_BATTLE_ENTERCROSSBATTLE;
				}

				CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, res);
				CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, OPENBANQUET_BATTLE);
			}
			
		}
		else // 收到玩家在原服进入跨服战场的消息
		{
			CrossBattleManager::GetInstance()->EnterBattle(OPENBANQUET_BATTLE, res);
		}

		return;
	}
};

};

#endif
