
#ifndef __GNET_ROLEMSG_HPP
#define __GNET_ROLEMSG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "rolemsgbean"
#include "gtmanager.h"
namespace GNET
{

class RoleMsg : public GNET::Protocol
{
	#include "rolemsg"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleMsg: receiver=%lld",receiver);
		if(GTManager::Instance()->OnGTRoleMsg(receiver,message))
		{
			DEBUG_PRINT("RoleMsg:OnGTRoleMsg error");
		}
	}
};

};

#endif
