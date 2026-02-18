
#ifndef __GNET_CRSSVRTEAMSCHGCAPTAIN_HPP
#define __GNET_CRSSVRTEAMSCHGCAPTAIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "crssvrteamsmanager.h"

namespace GNET
{

class CrssvrTeamsChgCaptain : public GNET::Protocol
{
	#include "crssvrteamschgcaptain"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to CrssvrTeamsChgCaptain on Central Delivery Server, refuse him!", roleid);
			return;
		}

		PlayerInfo * pinfo;
		int linksid;
		LOG_TRACE("CrssvrTeamsChgCaptain roleid=%d, master_new=%d, teamid=%d", roleid, master_new, teamid);
	
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL==pinfo)
		{
			LOG_TRACE("CrssvrTeamsChgCaptain, NULL==pinfo, roleid=%d", roleid);
			return;
		}
		localsid = pinfo->localsid;
		linksid  = pinfo->linksid;

		// teamid验证
		int role_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
		if(role_teamid <= 0 || role_teamid != teamid)
		{
			LOG_TRACE("CrssvrTeamsChgCaptain,roleid=%d, role_teamid=%d, teamid=%d not equal \n",roleid, role_teamid, teamid);
			dsm->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_TEAMID_INVAILD, roleid, teamid, localsid));
			return;
		}

		// 是否可以换队长
		int ret = CrssvrTeamsManager::Instance()->CanChgCaptain(teamid, roleid, master_new);
		if(ret == 0)
		{
			DBCrssvrTeamsChgCaptain* rpc = (DBCrssvrTeamsChgCaptain*) Rpc::Call( RPC_DBCRSSVRTEAMSCHGCAPTAIN,DBCrssvrTeamsChgCaptainArg(master_new,teamid));
			rpc->localsid = localsid;
			rpc->linksid = linksid;
			rpc->master_old = roleid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else
			dsm->Send(linksid, CrssvrTeamsNotify_Re(ret, roleid, teamid, localsid));
	}
};

};

#endif
