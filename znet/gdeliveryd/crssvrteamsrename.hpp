
#ifndef __GNET_CRSSVRTEAMSRENAME_HPP
#define __GNET_CRSSVRTEAMSRENAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "precrssvrteamsrename.hrp"
#include "gmailsyncdata"
#include "localmacro.h"
#include "uniquenameclient.hpp"
#include "crssvrteamsnotify_re.hpp"


namespace GNET
{

class CrssvrTeamsRename : public GNET::Protocol
{
	#include "crssvrteamsrename"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "CrssvrTeamsRename, roleid=%d", roleid);
		
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to CrssvrTeamsRename on Central Delivery Server, refuse him!", roleid);
			return;
		}
		int linksid, localsid;
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	                if ( !pinfo )
				return;
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;
		}

		// teamidÑéÖ¤
		int role_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
		if(role_teamid <= 0)
		{
			LOG_TRACE("CrssvrTeamsRename,roleid=%d, not find teamid",roleid, role_teamid);
			GDeliveryServer::GetInstance()->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_TEAMID_INVAILD, roleid, 0, localsid));
			return;
		}

		if(!CrssvrTeamsManager::Instance()->ValidName(new_teamsname))
		{
			LOG_TRACE("roleid %d try to rename crssvrteams, but name not valid", roleid);
			GDeliveryServer::GetInstance()->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_INVALIDNAME,roleid,0,localsid));
			return;
		}

		int ret = CrssvrTeamsManager::Instance()->CanChangeName(teamid, roleid, new_teamsname);
		if(ERR_SUCCESS != ret)
		{
			LOG_TRACE("CrssvrTeamsRename,roleid=%d, teamid=%d, CanChangeName false, ret=%d",roleid, role_teamid, ret);
			GDeliveryServer::GetInstance()->Send(linksid, CrssvrTeamsNotify_Re(ret,roleid,0,localsid));
			return;
		}

		PreCrssvrTeamsRename* rpc = (PreCrssvrTeamsRename*)Rpc::Call(RPC_PRECRSSVRTEAMSRENAME,
				PreCrssvrTeamsRenameArg(GDeliveryServer::GetInstance()->zoneid,teamid,new_teamsname));
		rpc->linksid = linksid;
		rpc->localsid = localsid;
		rpc->roleid = roleid;
		//rpc->oldname = old_teamsname;
		UniqueNameClient::GetInstance()->SendProtocol(rpc );
	}
};

};

#endif
