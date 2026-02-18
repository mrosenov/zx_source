
#ifndef __GNET_TEAMRAIDAPPLY_RE_HPP
#define __GNET_TEAMRAIDAPPLY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidApply_Re : public GNET::Protocol
{
	#include "teamraidapply_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("TeamRaidApply_Re retcode=%d, err_roleid=%d, roleid=%d", retcode, err_roleid, roleid);
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
