
#ifndef __GNET_GETTOUCHPOINT_HPP
#define __GNET_GETTOUCHPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetTouchPoint : public GNET::Protocol
{
	#include "gettouchpoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if( NULL == pinfo )
			return;
		Game2AU req(pinfo->userid, AU_REQ_TOUCH_POINT, Octets(), 0);
		if (!GAuthClient::GetInstance()->SendProtocol(req)) 
			manager->Send(sid, GetTouchPoint_Re(ERR_TOUCH_COMMUNICATION, roleid, localsid, 0));
		LOG_TRACE("gettouchpoint roleid %d userid %d", roleid, pinfo->userid);
	}
};

};

#endif
