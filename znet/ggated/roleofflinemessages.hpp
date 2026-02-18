
#ifndef __GNET_ROLEOFFLINEMESSAGES_HPP
#define __GNET_ROLEOFFLINEMESSAGES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "rolemsgbean"
#include "gtmanager.h"
namespace GNET
{

class RoleOfflineMessages : public GNET::Protocol
{
	#include "roleofflinemessages"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleOfflineMessages: receiver=%lld",receiver);
		if(GTManager::Instance()->OnGTOfflineRoleMsg(receiver,messages))
		{
			DEBUG_PRINT("RoleOfflineMessages:OnGTOfflineRoleMsg error");
		}
	}
};

};

#endif
