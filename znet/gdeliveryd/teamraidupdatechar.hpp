
#ifndef __GNET_TEAMRAIDUPDATECHAR_HPP
#define __GNET_TEAMRAIDUPDATECHAR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"

#include "teamraidroledata"

namespace GNET
{

class TeamRaidUpdateChar : public GNET::Protocol
{
	#include "teamraidupdatechar"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//int ret=RaidManager::GetInstance()->OnTeamRaidUpdateChar(*this);
		//LOG_TRACE("TeamRaidUpdateChar roleid=%d roomid=%d ret=%d",roledata.roleid,raidroom_id,ret);
	}
};

};

#endif
