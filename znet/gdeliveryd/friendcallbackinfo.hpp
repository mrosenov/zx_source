
#ifndef __GNET_FRIENDCALLBACKINFO_HPP
#define __GNET_FRIENDCALLBACKINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "friendcallbackmanager.hpp"


namespace GNET
{

class FriendCallbackInfo : public GNET::Protocol
{
	#include "friendcallbackinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//LOG_TRACE("GDelivery::FriendCallbackInfo, roleid=%d\n", roleid);
		FriendCallbackManager::GetInstance()->QueryCallbackInfo(this, sid);
	}
};

};

#endif
