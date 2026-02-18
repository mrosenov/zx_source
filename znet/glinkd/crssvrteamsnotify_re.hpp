
#ifndef __GNET_CRSSVRTEAMSNOTIFY_RE_HPP
#define __GNET_CRSSVRTEAMSNOTIFY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsNotify_Re : public GNET::Protocol
{
	#include "crssvrteamsnotify_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsNotify_Re retcode=%d, roleid=%d, teamid=%d, localsid=%d, name.size=%d", retcode, roleid, teamid, localsid, name.size());
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
