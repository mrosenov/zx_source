
#ifndef __GNET_CRSSVRTEAMSKICK_HPP
#define __GNET_CRSSVRTEAMSKICK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "crssvrteamsmanager.h"

namespace GNET
{

class CrssvrTeamsKick : public GNET::Protocol
{
	#include "crssvrteamskick"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("CrssvrTeamsKick teamid=%d, kick_roleid=%d, roleid=%d,localsid=%d", teamid, kick_roleid, roleid, localsid);
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		int linksid;
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to CrssvrTeamsKick on Central Delivery Server, refuse him!", roleid);
			return;
		}

		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo)
			{
				LOG_TRACE("CrssvrTeamsKick, teamid=%d, roleid=%d is not online", teamid, roleid);
				return;
			}
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;

			int can_kick = CrssvrTeamsManager::Instance()->CanKick(teamid, roleid);
			if(0 != can_kick)
			{
				LOG_TRACE("CrssvrTeamsKick, teamid=%d, roleid=%d can not kick", teamid, roleid);
				dsm->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_NOT_MASTER, roleid, teamid, localsid));
				return;

			}
		
			DBCrssvrTeamsLeaveArg arg(kick_roleid, teamid);
			DBCrssvrTeamsLeave* rpc = (DBCrssvrTeamsLeave*) Rpc::Call( RPC_DBCRSSVRTEAMSLEAVE,arg);
			rpc->linksid = linksid;
			rpc->localsid = localsid;
			rpc->name = pinfo->name;
			rpc->iskick = true;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
	}
};

};

#endif
