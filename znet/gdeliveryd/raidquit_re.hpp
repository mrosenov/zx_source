
#ifndef __GNET_RAIDQUIT_RE_HPP
#define __GNET_RAIDQUIT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidQuit_Re : public GNET::Protocol
{
	#include "raidquit_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RaidQuit_Re retcode=%d, roleid=%d", retcode, roleid);
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
