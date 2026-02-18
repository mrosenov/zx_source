
#ifndef __GNET_CONTESTANSWER_HPP
#define __GNET_CONTESTANSWER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ContestAnswer : public GNET::Protocol
{
	#include "contestanswer"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL==pinfo )
			return;

		ContestManager &cm = ContestManager::GetInstance();
		if(!cm.IsProcessState())
			return;
		cm.OnPlayerAnswer(roleid, questionid, answer, usestar, pinfo->name);
	}
};

};

#endif
