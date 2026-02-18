
#ifndef __GNET_CRSSVRTEAMSTEAMBASE_RE_HPP
#define __GNET_CRSSVRTEAMSTEAMBASE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsTeamBase_Re : public GNET::Protocol
{
	#include "crssvrteamsteambase_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsTeamBase_Re retcode=%d, master=%d, teamid=%d, localsid=%d", retcode, master, teamid, localsid);
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
