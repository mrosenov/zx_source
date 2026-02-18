
#ifndef __GNET_TRYRECONNECT_HPP
#define __GNET_TRYRECONNECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"


namespace GNET
{

class TryReconnect : public GNET::Protocol
{
	#include "tryreconnect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		UserContainer &container = UserContainer::GetInstance();
		{
			Thread::RWLock::RDScoped l(container.GetLocker());
			UserInfo *pInfo = container.FindUser(userid);

			UserContainer::GetInstance().UserReconnect(pInfo, userid, roleid, localsid, sid, checksum, loginip);
		}
	}
};

};

#endif
