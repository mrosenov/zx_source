
#ifndef __GNET_SENDCRSSVRTEAMSCREATE_HPP
#define __GNET_SENDCRSSVRTEAMSCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "precrssvrteamscreate.hrp"
#include "gmailsyncdata"
#include "localmacro.h"
#include "uniquenameclient.hpp"
#include "crssvrteamsnotify_re.hpp"

namespace GNET
{

class SendCrssvrTeamsCreate : public GNET::Protocol
{
	#include "sendcrssvrteamscreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "SendCrssvrTeamsCreate, roleid=%d", roleid);

		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to create crssvrteams on Central Delivery Server, refuse him!", roleid);
			GMailEndSync sync(0,ERR_CRSSVRTEAMS_CHECKCONDITION,roleid);
			manager->Send(sid,sync);
			return;
		}
		int linksid, localsid, gameid;

		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	                if ( !pinfo )
				return;
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;
			gameid = sid;
		}

		if(!CrssvrTeamsManager::Instance()->ValidName(teamname))
		{
			LOG_TRACE("roleid %d try to create crssvrteams, but name not valid", roleid);
			GDeliveryServer::GetInstance()->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_INVALIDNAME,roleid,0,localsid));
			GMailEndSync sync(0,ERR_CRSSVRTEAMS_INVALIDNAME,roleid);
			manager->Send(sid,sync);
			return;
		}
	
		int ret = ERR_SUCCESS;
		if(ERR_SUCCESS != (ret=CrssvrTeamsManager::Instance()->CanCreate(roleid)))
		{
			LOG_TRACE("SendCrssvrTeamsCreate roleid %d try to create crssvrteams, but not cancreate", roleid);
			GDeliveryServer::GetInstance()->Send(linksid, CrssvrTeamsNotify_Re(ret,roleid,0,localsid));
			GMailEndSync sync(0,ERR_CRSSVRTEAMS_INVALIDNAME,roleid);
			manager->Send(sid,sync);
			return;
		}
		
		if(syncdata.inventory.money < 100000000)// ะกำฺ1wj
		{
			LOG_TRACE("SendCrssvrTeamsCreate roleid %d try to create crssvrteams, but money not full", roleid);
			GDeliveryServer::GetInstance()->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_CHECKCONDITION,roleid,0,localsid));
			GMailEndSync sync(0,ERR_CRSSVRTEAMS_CHECKCONDITION,roleid);
			manager->Send(sid,sync);
			return;
		}
		syncdata.inventory.money -= 100000000;

		PreCrssvrTeamsCreate* rpc = (PreCrssvrTeamsCreate*)Rpc::Call(RPC_PRECRSSVRTEAMSCREATE,
				PreCrssvrTeamsCreateArg(GDeliveryServer::GetInstance()->zoneid,teamname));
		rpc->linksid = linksid;
		rpc->localsid = localsid;
		rpc->roleid = roleid;
		rpc->gamesid = sid;
		Utility::SwapSyncData(rpc->syncdata, syncdata);
		UniqueNameClient::GetInstance()->SendProtocol(rpc );
	}
};

};

#endif
