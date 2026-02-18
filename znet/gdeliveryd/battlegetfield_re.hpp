
#ifndef __GNET_BATTLEGETFIELD_RE_HPP
#define __GNET_BATTLEGETFIELD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gbattlerole"
#include "mapuser.h"
#include "gdeliveryserver.hpp"

namespace GNET
{

class BattleGetField_Re : public GNET::Protocol
{
	#include "battlegetfield_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleGetField_Re::retcode=%d, starttime=%d, red_team.size=%d, blue_team.size=%d", retcode, starttime, red_team.size(), blue_team.size());
		PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
		if(info != NULL)
		{
			GDeliveryServer::GetInstance()->Send(info->linksid, this);
		}
	}
};

};

#endif
