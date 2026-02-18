
#ifndef __GNET_PLAYERRECONNECT_HPP
#define __GNET_PLAYERRECONNECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerReconnect : public GNET::Protocol
{
	#include "playerreconnect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if(!lsm->RoleLogout(localsid, roleid))
			return;

		SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
		if (sinfo)
		{
			sinfo->userid = 0;
			sinfo->ingame = false;
		}

//		lsm->SetReadyCloseTime(localsid, 10);
		lsm->Close(localsid);
	}
};

};

#endif
