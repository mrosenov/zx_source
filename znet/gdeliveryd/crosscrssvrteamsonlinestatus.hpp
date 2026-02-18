
#ifndef __GNET_CROSSCRSSVRTEAMSONLINESTATUS_HPP
#define __GNET_CROSSCRSSVRTEAMSONLINESTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "crssvrteamsmanager.h"


namespace GNET
{

class CrossCrssvrTeamsOnlineStatus : public GNET::Protocol
{
	#include "crosscrssvrteamsonlinestatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrossCrssvrTeamsOnlineStatus, roleid=%d, teamid=%d, online=%d", roleid, teamid, online);
		CrssvrTeamsManager::Instance()->UpdateRoleOnline(teamid, roleid, online);
	}
};

};

#endif
