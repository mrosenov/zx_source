
#ifndef __GNET_GRAIDLEAVE_HPP
#define __GNET_GRAIDLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "playerchangegs.hpp"
#include "raidmanager.h"
namespace GNET
{

class GRaidLeave : public GNET::Protocol
{
	#include "graidleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("graidleave:roleid=%d,index=%d,oldmap=%d,x=%f,y=%f,z=%f reason=%d",roleid, index, map_id, scalex, scaley, scalez, reason);
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL)
		{
			return;
		}
			
//		int retcode = RaidManager::GetInstance()->OnGSRoleLeave(gs_id,roleid,index);
//		if (retcode == ERR_SUCCESS && pinfo->old_gs != _GAMESERVER_ID_INVALID)
//		RaidManager::GetInstance()->TryLeaveRaid(gs_id, roleid, reason);
		//LOG_TRACE("graidleave roleid=%d, pinfo->old_gs=%d, _GAMESERVER_ID_INVALID=%d", roleid, pinfo->old_gs, _GAMESERVER_ID_INVALID); 

		if (pinfo->old_gs != _GAMESERVER_ID_INVALID)
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, pinfo->old_gs, map_id, reason, scalex, scaley, scalez));
		else
		{
			int dst_lineid = 0;
			const CrossBattleInfo* battleinfo = CrossBattleManager::GetInstance()->GetCrossBattleInfo(roleid, CRSSVRTEAMS_BATTLE);
			if(battleinfo != NULL)
			{
				//LOG_TRACE("GRaidLeave roleid=%d, src_gsid=%d", roleid, battleinfo->_gsid);
				dst_lineid = battleinfo->_gsid;
			}
			ProtocolExecutor *task = new ProtocolExecutor(GProviderServer::GetInstance(), 0, new TryChangeDS(roleid, CENTRALDS_TO_DS, dst_lineid));
			Thread::Pool::AddTask(task);

                	LOG_TRACE("graidleave roleid=%d, dst_lineid=%d", roleid,  dst_lineid); 
		}
		
		CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, CRSSVRTEAMS_BATTLE);
	}
};

};

#endif
