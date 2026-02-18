
#ifndef __GNET_TEAMRAIDACTIVEAPPLYNOTICE_HPP
#define __GNET_TEAMRAIDACTIVEAPPLYNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidActiveApplyNotice : public GNET::Protocol
{
	#include "teamraidactiveapplynotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//LOG_TRACE("TeamRaidActiveApplyNotice roleid=%d, isapplying=%d, active_roleid=%d", roleid, isapplying, active_roleid);
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
