
#ifndef __GNET_GTSYNCTEAMS_HPP
#define __GNET_GTSYNCTEAMS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTSyncTeams : public GNET::Protocol
{
	#include "gtsyncteams"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gtsyncteams size=%d, sid=%d", teams.size(), sid);
		if (GTPlatformClient::GetInstance()->SendProtocol(this))
		{
			int gsid = GProviderServer::GetInstance()->GetGSID(sid);
			if (gsid == _GAMESERVER_ID_INVALID)
			{
				Log::log(LOG_ERR, "gtsyncteams from unknown gs, sid=%d", sid);
				return;
			}
			TeamCache::GetInstance()->OnTeamSync(gsid, teams);
		}
	}
};

};

#endif
