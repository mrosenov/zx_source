
#ifndef __GNET_GTTEAMDISMISS_HPP
#define __GNET_GTTEAMDISMISS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTTeamDismiss : public GNET::Protocol
{
	#include "gtteamdismiss"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gtteamdismiss teamid %lld, sid=%d", teamid, sid);
		if (GTPlatformClient::GetInstance()->SendProtocol(this))
		{
			int gsid = GProviderServer::GetInstance()->GetGSID(sid);
			if (gsid == _GAMESERVER_ID_INVALID)
			{
				Log::log(LOG_ERR, "gtteamdismiss from unknown gs, sid=%d", sid);
				return;
			}
			TeamCache::GetInstance()->OnTeamDismiss(gsid, teamid);
		}
	}
};

};

#endif
