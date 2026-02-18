
#ifndef __GNET_KICKOUTUSER_HPP
#define __GNET_KICKOUTUSER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryclient.hpp"
#include "glinkserver.hpp"
namespace GNET
{

class KickoutUser : public GNET::Protocol
{
	#include "kickoutuser"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		Thread::RWLock::RDScoped l(lsm->locker_map);
		Log::log(LOG_DEBUG,"KickoutUser userid=%d,localsid=%d,manager=%p,sid=%d,cause=%d",userid,localsid,manager,sid,cause);
		if ( cause==0 ) //means let the users disconnect directly
		{
			lsm->Close(localsid);
			return;
		}
		if (lsm->ValidUser(localsid,userid))
			lsm->SessionError(localsid,cause,"");
	}
};

};

#endif
