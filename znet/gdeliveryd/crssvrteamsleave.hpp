
#ifndef __GNET_CRSSVRTEAMSLEAVE_HPP
#define __GNET_CRSSVRTEAMSLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsLeave : public GNET::Protocol
{
	#include "crssvrteamsleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("CrssvrTeamsLeave teamid=%d,roleid=%d,localsid=%d", teamid, roleid, localsid);
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		int linksid;
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to CrssvrTeamsLeave on Central Delivery Server, refuse him!", roleid);
			return;
		}

		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo)
			{
				LOG_TRACE("CrssvrTeamsLeave, roleid=%d is not online", roleid);
				return;
			}
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;

			// teamidÑéÖ¤
			int role_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
			if(role_teamid <= 0 || role_teamid != teamid)
			{
				LOG_TRACE("CrssvrTeamsLeave, roleid=%d, role_teamid=%d, teamid=%d not equal", roleid, role_teamid, teamid);
				dsm->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_TEAMID_INVAILD, roleid, teamid, localsid));
				return;
			}

			DBCrssvrTeamsLeaveArg arg(roleid, teamid);
			DBCrssvrTeamsLeave* rpc = (DBCrssvrTeamsLeave*) Rpc::Call( RPC_DBCRSSVRTEAMSLEAVE,arg);
			rpc->linksid = linksid;
			rpc->localsid = localsid;
			rpc->name = pinfo->name;
			rpc->iskick = false;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
	}
};

};

#endif
