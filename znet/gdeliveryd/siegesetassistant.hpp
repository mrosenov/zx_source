
#ifndef __GNET_SIEGESETASSISTANT_HPP
#define __GNET_SIEGESETASSISTANT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbsiegesetassistant.hrp"
#include "siegesetassistant_re.hpp"
#include "factionmanager.h"

namespace GNET
{

class SiegeSetAssistant : public GNET::Protocol
{
	#include "siegesetassistant"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SiegeSetAssistant roleid=%d,battle_id=%d,factionname=%d", roleid,battle_id,faction_name.size());
		int linksid, localsid;
		int factionid;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factiontitle>TITLE_VICEMASTER)
				return;
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;
			factionid = pinfo->factionid;
		}
		FactionDetailInfo * faction = FactionManager::Instance()->Find(factionid);
		if (!faction)
			return;
		int rslt = SiegeManager::GetInstance()->CanSetAssistant(battle_id, factionid);

		if (rslt == ERR_SUCCESS)
		{
			DBSiegeSetAssistant *rpc = (DBSiegeSetAssistant *) Rpc::Call(RPC_DBSIEGESETASSISTANT,
					DBSiegeSetAssistantArg(battle_id, factionid, faction_name));
			rpc->localsid = localsid;
			rpc->link_sid = linksid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			return;
		}
		GDeliveryServer::GetInstance()->Send(linksid, SiegeSetAssistant_Re(rslt,battle_id, 0, localsid));
	}
};

};

#endif
