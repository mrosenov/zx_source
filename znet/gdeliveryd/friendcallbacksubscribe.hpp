
#ifndef __GNET_FRIENDCALLBACKSUBSCRIBE_HPP
#define __GNET_FRIENDCALLBACKSUBSCRIBE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "friendcallbackmanager.hpp"


namespace GNET
{

class FriendCallbackSubscribe : public GNET::Protocol
{
	#include "friendcallbacksubscribe"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("GDelivery::FriendCallbackSubscribe, roleid=%d, is_subscribe=%d\n", roleid, is_subscribe);
		FriendCallbackManager::GetInstance()->Subscribe(this, sid);
	}
};

};

#endif
