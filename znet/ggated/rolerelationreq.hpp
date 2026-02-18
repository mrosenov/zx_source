
#ifndef __GNET_ROLERELATIONREQ_HPP
#define __GNET_ROLERELATIONREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtmanager.h"
namespace GNET
{

class RoleRelationReq : public GNET::Protocol
{
	#include "rolerelationreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RoleRelationReq: roleid=%lld userid=%lld ",roleid,userid);
		bool ret_err;
		if(manager == SNSPlatformClient::GetInstance())
		{
			ret_err = GTManager::Instance()->OnRoleRelationReq(userid,roleid,GGT_SNS_TYPE);
		}
		else
		{
			ret_err = GTManager::Instance()->OnRoleRelationReq(userid,roleid,GGT_GT_TYPE);
		}
		if(ret_err)	
			DEBUG_PRINT("RoleRelationReq:OnRoleRelationReq error");
	}
};

};

#endif
