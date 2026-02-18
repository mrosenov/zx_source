
#ifndef __GNET_PLAYERLOGOUT_HPP
#define __GNET_PLAYERLOGOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "log.h"
#include "mapuser.h"
namespace GNET
{

class PlayerLogout : public GNET::Protocol
{
	#include "playerlogout"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::log(LOG_INFO,"gdelivery::PlayerLogout roleid=%d, result=%d", roleid, result);
		if(result!=_PLAYER_LOGOUT_HALF && result!=_PLAYER_LOGOUT_FULL)
		{
			Log::log(LOG_ERR,"gdelivery::playerlogout, retcode is invalid.\n");
			return;
		}
		UserContainer::GetInstance().OnPlayerLogout(*this);
	}
};

};

#endif
