
#ifndef __GNET_CRSSVRTEAMSGETROLELIST_HPP
#define __GNET_CRSSVRTEAMSGETROLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "crssvrteamsmanager.h"
#include "crosscrssvrteamsmanager.h"

namespace GNET
{

class CrssvrTeamsGetRoleList : public GNET::Protocol
{
	#include "crssvrteamsgetrolelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo;
		CrssvrTeamsRoleList_Re list_re;
		int linksid;
		LOG_TRACE("CrssvrTeamsGetRoleList roleid=%d, teamid=%d", roleid, teamid);
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		/*if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to CrssvrTeamsGetRoleList on Central Delivery Server, refuse him!", roleid);
			return;
		}*/

		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL==pinfo)
		{
			LOG_TRACE("CrssvrTeamsGetRoleList, roleid=%d is not online", roleid);
			return;
		}
		localsid = pinfo->localsid;
		linksid  = pinfo->linksid;

		// teamidÑéÖ¤
		int role_teamid = 0;
		if (!dsm->IsCentralDS())
		{
			role_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
		}
		else
		{
			role_teamid = CrossCrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
		}

		if(role_teamid <= 0)
		{
			list_re.retcode = ERR_CRSSVRTEAMS_ROLE_NOT_IN;
			list_re.localsid = localsid;
			dsm->Send(linksid, list_re);

			LOG_TRACE("CrssvrTeamsGetRoleList, roleid=%d, role_teamid=%d, teamid=%d", roleid, role_teamid, teamid);
			return;
		}
		
		int ret;
		if (!dsm->IsCentralDS())
		{
			ret = CrssvrTeamsManager::Instance()->GetRoleList(roleid, role_teamid, list_re);
		}
		else
		{
			ret = CrossCrssvrTeamsManager::Instance()->GetRoleList(roleid, role_teamid, list_re);
		}
		list_re.retcode = ret;
		list_re.localsid = localsid;
		dsm->Send(linksid, list_re);
	}
};

};

#endif
