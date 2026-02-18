
#ifndef __GNET_BATTLEENTER_RE_HPP
#define __GNET_BATTLEENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "crossbattlemanager.hpp"
#include "centraldeliveryclient.hpp"
#include "trychangeds.hpp"
#include "protocolexecutor.h"

namespace GNET
{

class BattleEnter_Re : public GNET::Protocol
{
	#include "battleenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleEnter_Re::retcode=%d,roleid=%d,endtime=%d,battle_type=%d,iscross=%d", retcode, roleid, endtime, battle_type, iscross);
		
		if(retcode == ERR_SUCCESS)
		{
			ProtocolExecutor *task = new ProtocolExecutor(GProviderServer::GetInstance(), 0, new TryChangeDS(roleid, DS_TO_BATTLEMAP));
			Thread::Pool::AddTask(task);
		}
		else if(retcode == ERR_BATTLE_ENTERCROSSBATTLE) // 角色已经在跨服进入战场
		{
			if(battle_type == OPENBANQUET_BATTLE)
				CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, OPENBANQUET_BATTLE);
			else
				CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, NORMAL_BATTLE);
		}
		else
		{
			if(battle_type == OPENBANQUET_BATTLE)
				CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, OPENBANQUET_BATTLE);
			else
				CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, NORMAL_BATTLE);
			PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
			if(info != NULL)
			{
				GDeliveryServer::GetInstance()->Send(info->linksid, this);
			}
		}
	}
};

};

#endif
