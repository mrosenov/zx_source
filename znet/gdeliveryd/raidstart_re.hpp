
#ifndef __GNET_RAIDSTART_RE_HPP
#define __GNET_RAIDSTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidStart_Re : public GNET::Protocol
{
	#include "raidstart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidStart_Re retcode=%d, raidroom_id=%d, localsid=%d, roleid=%d", retcode, raidroom_id, localsid, roleid);
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
