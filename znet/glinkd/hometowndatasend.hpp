
#ifndef __GNET_HOMETOWNDATASEND_HPP
#define __GNET_HOMETOWNDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "c2shometowndatasend.hpp"


namespace GNET
{

class HometowndataSend : public GNET::Protocol
{
	#include "hometowndatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm = GLinkServer::GetInstance();
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if (sinfo) 
		{
			RoleData * uinfo = lsm->GetRoleInfo(sinfo->roleid);
			if (!uinfo) return;
			GDeliveryClient::GetInstance()->SendProtocol(C2SHometowndataSend(sinfo->roleid,sid,data));
		}

	}
};

};

#endif
