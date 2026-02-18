
#ifndef __GNET_SYNCROLEDATA_HPP
#define __GNET_SYNCROLEDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SyncRoleData : public GNET::Protocol
{
	#include "syncroledata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if(pinfo && pinfo->roleid==(int)roleid)
		{
			pinfo->level = level;
			pinfo->occupation = occupation;
			pinfo->battlescore = battlescore;
		}
	}
};

};

#endif
