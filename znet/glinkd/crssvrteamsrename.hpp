
#ifndef __GNET_CRSSVRTEAMSRENAME_HPP
#define __GNET_CRSSVRTEAMSRENAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsRename : public GNET::Protocol
{
	#include "crssvrteamsrename"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsRename roleid=%d, teamid=%d, new_name=%.*s, localsid=%d", roleid, teamid,new_teamsname.size(), (char*)new_teamsname.begin(), sid);

		localsid = sid;

		GDeliveryClient::GetInstance()->SendProtocol(this);

	}
};

};

#endif
