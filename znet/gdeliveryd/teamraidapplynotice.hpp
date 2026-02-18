
#ifndef __GNET_TEAMRAIDAPPLYNOTICE_HPP
#define __GNET_TEAMRAIDAPPLYNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidApplyNotice : public GNET::Protocol
{
	#include "teamraidapplynotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("TeamRaidApplyNotice roleid=%d, isapplying=%d", roleid, isapplying);
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
