
#ifndef __GNET_FRIENDCALLBACKMAIL_HPP
#define __GNET_FRIENDCALLBACKMAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "friendcallbackmanager.hpp"

namespace GNET
{

class FriendCallbackMail : public GNET::Protocol
{
	#include "friendcallbackmail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		FriendCallbackManager::GetInstance()->SendCallbackMail(this, sid);
	}
};

};

#endif
