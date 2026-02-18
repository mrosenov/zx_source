
#ifndef __GNET_CONTESTEXIT_HPP
#define __GNET_CONTESTEXIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestExit : public GNET::Protocol
{
	#include "contestexit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL==pinfo )
			return;

		ContestManager::GetInstance().OnPlayerExit(roleid);
	}
};

};

#endif
