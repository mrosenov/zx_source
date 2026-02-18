
#ifndef __GNET_BATTLESTARTNOTIFY_HPP
#define __GNET_BATTLESTARTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"
#include "gdeliveryserver.hpp"
#include "battleleave.hpp"

namespace GNET
{

class BattleStartNotify : public GNET::Protocol
{
	#include "battlestartnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleStartNotify::result=%d, roleid=%d, gs_id=%d, map_id=%d, iscross=%d", start_result, roleid, gs_id, map_id, iscross);
		PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
		if(info != NULL && info->ingame)
		{
			GDeliveryServer::GetInstance()->Send(info->linksid, this);
		}
		else
		{
			// 角色不在线，删除跨服战场中保存的信息，并通知跨服服务器
			CrossBattleManager::GetInstance()->LeaveBattle(NORMAL_BATTLE, roleid, 0);
			CentralDeliveryClient::GetInstance()->SendProtocol(BattleLeave(roleid, 0, true));
		}
	}
};

};

#endif
