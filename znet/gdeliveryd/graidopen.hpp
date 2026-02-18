
#ifndef __GNET_GRAIDOPEN_HPP
#define __GNET_GRAIDOPEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"
#include "raidmanager.h"
namespace GNET
{

class GRaidOpen : public GNET::Protocol
{
	#include "graidopen"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int gs_id = GProviderServer::GetInstance()->GetGSID(sid);
		LOG_TRACE("GRaidOpen: roleid=%d, gs_id=%d, map_id=%d, itemid=%d, itemcount=%d", roleid, gs_id, map_id, itemid, item_count);
		int retcode = RaidManager::GetInstance()->OpenRaid(roleid, gs_id, map_id, itemid, item_count,syncdata, roomname, can_vote, difficulty);
		if(retcode)
		{
			GProviderServer::GetInstance()->DispatchProtocol(gs_id, GMailEndSync(0, retcode, roleid, syncdata));
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if(pinfo)
				GDeliveryServer::GetInstance()->Send(pinfo->linksid,RaidOpen_Re(retcode,0,map_id,Octets(),0,pinfo->localsid, difficulty));
		}

	}
};

};

#endif
