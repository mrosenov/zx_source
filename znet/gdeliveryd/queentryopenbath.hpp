
#ifndef __GNET_QUEENTRYOPENBATH_HPP
#define __GNET_QUEENTRYOPENBATH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueenTryOpenBath : public GNET::Protocol
{
	#include "queentryopenbath"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo)
			return;
		int retcode = KingdomManager::GetInstance()->TryOpenBath(roleid);
		if (retcode != ERR_SUCCESS)
			manager->Send(sid, QueenOpenBath_Re(retcode, roleid, localsid));
		LOG_TRACE("queentryopenbath roleid %d, ret %d", roleid, retcode);
	}
};

};

#endif
