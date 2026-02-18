
#ifndef __GNET_CRSSVRTEAMSROLENOTIFY_RE_HPP
#define __GNET_CRSSVRTEAMSROLENOTIFY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsRoleNotify_Re : public GNET::Protocol
{
	#include "crssvrteamsrolenotify_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsRoleNotify_Re roleid=%d,teamid=%d,ct_last_max_score=%d,ct_last_battle_count=%d, localsid=%d", roleid, teamid, ct_last_max_score, ct_last_battle_count, localsid);
		GLinkServer::GetInstance()->Send(localsid,this);

	}
};

};

#endif
