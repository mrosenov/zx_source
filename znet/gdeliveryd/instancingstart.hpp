
#ifndef __GNET_INSTANCINGSTART_HPP
#define __GNET_INSTANCINGSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "instancingstart_re.hpp"


namespace GNET
{

class InstancingStart : public GNET::Protocol
{
	#include "instancingstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL==pinfo) 
			return;

		InstancingStart_Re re;
		re.localsid = localsid;
		re.roleid = roleid;
		re.map_id = map_id;
		re.retcode = InstancingManager::GetInstance()->BattleStart(roleid, pinfo->gameid, map_id);
		LOG_TRACE( "InstancingEnter:Roleid=%d retcode=%d", roleid, re.retcode);
		manager->Send(sid, re);
	}
};

};

#endif
