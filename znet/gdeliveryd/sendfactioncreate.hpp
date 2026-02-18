
#ifndef __GNET_SENDFACTIONCREATE_HPP
#define __GNET_SENDFACTIONCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "precreatefaction.hrp"
#include "precreatefamily.hrp"
#include "uniquenameclient.hpp"
#include "factioncreate_re.hpp"
#include "localmacro.h"

namespace GNET
{

class SendFactionCreate : public GNET::Protocol
{
	#include "sendfactioncreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to create family or faction on Central Delivery Server, refuse him!", roleid);
			GMailEndSync sync(0,ERR_FC_CHECKCONDITION,roleid);
			manager->Send(sid,sync);
			return;
		}
		int linksid, localsid, gameid;
		unsigned char level, occupation;

		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	                if ( !pinfo )
				return;
			if(scale && pinfo->familyid)
				return;
			if(!scale && pinfo->factionid)
				return;
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;
			gameid = pinfo->gameid;
			level = pinfo->level;
			occupation = pinfo->occupation;
			if(!scale && pinfo->factiontitle!=TITLE_HEADER)
				return;
		}

		if(!FactionManager::Instance()->ValidName(factionname))
		{
			GDeliveryServer::GetInstance()->Send(linksid, FactionCreate_Re(ERR_FC_INVALIDNAME,roleid,0,
						factionname,scale,localsid));
			GMailEndSync sync(0,ERR_FC_INVALIDNAME,roleid);
			manager->Send(sid,sync);
			return;
		}
		LOG_TRACE( "SendCreateFaction, roleid=%d, scale=%d", roleid, scale);
		switch (scale)
		{
		case 0: // create faction
			{
				syncdata.inventory.money -= 300000;
				if(syncdata.inventory.money<0)
					return;

				PreCreateFaction* rpc = (PreCreateFaction*)Rpc::Call(RPC_PRECREATEFACTION,
						PreCreateFactionArg(GDeliveryServer::GetInstance()->zoneid,factionname));
				rpc->linksid = linksid;
				rpc->localsid = localsid;
				rpc->roleid = roleid;
				rpc->gamesid = sid;
				rpc->level = level;
				rpc->occupation = occupation;
				Utility::SwapSyncData(rpc->syncdata, syncdata);
				UniqueNameClient::GetInstance()->SendProtocol(rpc );
			}
			break;
		case  1: // create family	
			{
				syncdata.inventory.money -= 50000;
				if(syncdata.inventory.money<0)
					return;

				PreCreateFamily* rpc = (PreCreateFamily*)Rpc::Call(RPC_PRECREATEFAMILY,
						PreCreateFamilyArg(GDeliveryServer::GetInstance()->zoneid,factionname));
				rpc->linksid = linksid;
				rpc->localsid = localsid;
				rpc->roleid = roleid;
				rpc->gamesid = sid;
				rpc->level = level;
				rpc->occupation = occupation;
				Utility::SwapSyncData(rpc->syncdata, syncdata);
				UniqueNameClient::GetInstance()->SendProtocol(rpc );
				break;
			}
		}
	}
};

};

#endif
