
#ifndef __GNET_ONLINEANNOUNCE_HPP
#define __GNET_ONLINEANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
#include "statusannounce.hpp"
namespace GNET
{

class OnlineAnnounce : public GNET::Protocol
{
	#include "onlineannounce"
	
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("glinkd::onlineannounce:user=%d,sid=%d,zoneid=%d,timeleft=%d,free_left=%d,free_end=%d\n",
			userid,localsid,zoneid,remain_time,free_time_left,free_time_end);
		GLinkServer* lsm=GLinkServer::GetInstance();
		
		if (!lsm->ValidUser(localsid,userid))
		{
			DEBUG_PRINT("linkd:: receive onlineannoucne from delivery,user(%d) is invalid.\n",userid);
			manager->Send(sid,StatusAnnounce(userid,localsid,_STATUS_OFFLINE));
		}
		else
		{
			lsm->ChangeState(localsid, &state_GRoleList);
			lsm->Send(localsid,this);
		}
	}
};

};

#endif
