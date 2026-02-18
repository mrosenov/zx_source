
#ifndef __GNET_GTTEAMCREATE_HPP
#define __GNET_GTTEAMCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTTeamCreate : public GNET::Protocol
{
	#include "gtteamcreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gtteamcreate teamid %lld master %lld member size %d, sid=%d", team.teamid, team.captain, team.members.size(), sid);
		if (GTPlatformClient::GetInstance()->SendProtocol(this))
		{
			int gsid = GProviderServer::GetInstance()->GetGSID(sid);
			if (gsid == _GAMESERVER_ID_INVALID)
			{
				Log::log(LOG_ERR, "gtteamcreate from unknown gs, sid=%d", sid);
				return;
			}
			TeamCache::GetInstance()->OnTeamCreate(gsid, team.teamid);
		}
	}
};

};

#endif
