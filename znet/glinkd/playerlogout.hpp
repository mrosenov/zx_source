
#ifndef __GNET_PLAYERLOGOUT_HPP
#define __GNET_PLAYERLOGOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "statusannounce.hpp"
namespace GNET
{

class PlayerLogout : public GNET::Protocol
{
	#include "playerlogout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if(!lsm->RoleLogout(localsid, roleid))
			return;
		lsm->AccumulateSend(localsid,this);

		//change state of linkserver
		if (result==_PLAYER_LOGOUT_FULL)
		{
			// todo check intention of readyclosetime
			lsm->SetReadyCloseTime(localsid, 30);
		}
	}
};

};

#endif
