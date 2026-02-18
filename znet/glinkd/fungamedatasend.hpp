
#ifndef __GNET_FUNGAMEDATASEND_HPP
#define __GNET_FUNGAMEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gproviderserver.hpp"
#include "c2sfungamedatasend.hpp"
namespace GNET
{

class FunGamedataSend : public GNET::Protocol
{
	#include "fungamedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		GLinkServer* lsm = GLinkServer::GetInstance();

		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if (sinfo) 
		{
			RoleData * uinfo = lsm->GetRoleInfo(sinfo->roleid);
			if (!uinfo) return;
			GDeliveryClient::GetInstance()->SendProtocol(C2SFunGamedataSend(sinfo->roleid,sid,data));
		}

	}
};

};

#endif
