
#ifndef __GNET_ACHIEVEMENTMESSAGE_HPP
#define __GNET_ACHIEVEMENTMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class AchievementMessage : public GNET::Protocol
{
	#include "achievementmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GRoleInfo* pinfo = RoleInfoCache::Instance().Get(roleid);
		if (pinfo == NULL)
			return;
		rolename = pinfo->name;	
		switch (localsid)
		{
		case 1:
			break;
		case 2:
			LinkServer::GetInstance().BroadcastProtocol(*this);
			break;
		default:
			break;
		}
	}
};

};

#endif
