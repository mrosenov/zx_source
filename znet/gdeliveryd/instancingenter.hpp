
#ifndef __GNET_INSTANCINGENTER_HPP
#define __GNET_INSTANCINGENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "instancingenter_re.hpp"


namespace GNET
{

class InstancingEnter : public GNET::Protocol
{
	#include "instancingenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL==pinfo || pinfo->gameid!= gs_id)
			return;

		InstancingEnter_Re res;
		res.localsid = localsid;
		res.retcode = InstancingManager::GetInstance()->OnBattleEnter(roleid, gs_id, map_id, res.endtime);
		LOG_TRACE( "InstancingEnter:Roleid=%d retcode=%d", roleid, res.retcode);
		GDeliveryServer::GetInstance()->Send(sid, res);

		return;
	}
};

};

#endif
