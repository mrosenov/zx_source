
#ifndef __GNET_CRSSVRTEAMSGETROLENOTIFY_HPP
#define __GNET_CRSSVRTEAMSGETROLENOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "crssvrteamsmanager.h"
#include "crosscrssvrteamsmanager.h"

namespace GNET
{

class CrssvrTeamsGetRoleNotify : public GNET::Protocol
{
	#include "crssvrteamsgetrolenotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int linksid;
		LOG_TRACE("CrssvrTeamsGetRoleNotify roleid=%d", roleid);
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();

		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL==pinfo)
		{
			LOG_TRACE("CrssvrTeamsGetRoleNotify, roleid=%d is not online", roleid);
			return;
		}
		localsid = pinfo->localsid;
		linksid  = pinfo->linksid;

		int ret;
		CrssvrTeamsRoleNotify_Re re;
		if (!dsm->IsCentralDS())
		{
			ret = CrssvrTeamsManager::Instance()->GetRoleNotify(roleid, re);
		}
		else
		{
			ret = CrossCrssvrTeamsManager::Instance()->GetRoleNotify(roleid, re);
		}
		re.retcode = ret;
		re.localsid = localsid;
		dsm->Send(linksid, re);
		LOG_TRACE("CrssvrTeamsGetRoleNotify roleid=%d, send CrssvrTeamsRoleNotify_Re, retcode=%d,role_score=%d, ct_last_max_score=%d, ct_last_battle_count=%d", 
				roleid, re.retcode, re.role_score, re.ct_last_max_score, re.ct_last_battle_count);
	}
};

};

#endif
