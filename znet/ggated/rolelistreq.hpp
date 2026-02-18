
#ifndef __GNET_ROLELISTREQ_HPP
#define __GNET_ROLELISTREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtmanager.h"

namespace GNET
{

class RoleListReq : public GNET::Protocol
{
	#include "rolelistreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleListReq: userid=%lld", userid);
		if(manager == SNSPlatformClient::GetInstance())
		{
			GTManager::Instance()->OnRoleListReq(userid, GGT_SNS_TYPE);
		}
		else
		{
			GTManager::Instance()->OnRoleListReq(userid, GGT_GT_TYPE);
		}
	}
};

};

#endif
