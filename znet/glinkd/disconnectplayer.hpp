
#ifndef __GNET_DISCONNECTPLAYER_HPP
#define __GNET_DISCONNECTPLAYER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
namespace GNET
{

class DisconnectPlayer : public GNET::Protocol
{
	#include "disconnectplayer"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		Thread::RWLock::RDScoped l(lsm->locker_map);
		RoleData * uinfo = lsm->GetRoleInfo(roleid);
		if (!uinfo || uinfo->roleid != roleid ) 
			return;
		localsid = uinfo->sid;
		lsm->Close(localsid);
		Log::log(LOG_DEBUG,"DisconnectPlayer roleid=%d,localsid=%d,pid=%d,gameid=%d", roleid, localsid, provider_link_id, gameid);
	}
};

};

#endif
