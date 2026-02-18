
#ifndef __GNET_TEAMRAIDMAPPINGSUCCESS_HPP
#define __GNET_TEAMRAIDMAPPINGSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidMappingSuccess : public GNET::Protocol
{
	#include "teamraidmappingsuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("TeamRaidMappingSuccess iscross=%d, raidroom_id=%d, roleid=%d", iscross, raidroom_id, roleid);
		PlayerInfo *info = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(info != NULL && info->ingame)
		{
			localsid = info->localsid;
			GDeliveryServer::GetInstance()->Send(info->linksid, this);
		}
	}
};

};

#endif
