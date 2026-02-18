
#ifndef __GNET_FRIENDCALLBACKAWARD_HPP
#define __GNET_FRIENDCALLBACKAWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "friendcallbackmanager.hpp"


namespace GNET
{

class FriendCallbackAward : public GNET::Protocol
{
	#include "friendcallbackaward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		FriendCallbackManager::GetInstance()->PlayerLogin(roleid);
	}
};

};

#endif
