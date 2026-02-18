
#ifndef __GNET_GETPASSPORTCASHADD_HPP
#define __GNET_GETPASSPORTCASHADD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetPassportCashAdd : public GNET::Protocol
{
	#include "getpassportcashadd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if( NULL == pinfo )
			return;
		Game2AU req(pinfo->userid, AU_REQ_PASSPORT_CASHADD, Octets(), 0);
		if (!GAuthClient::GetInstance()->SendProtocol(req)) 
			manager->Send(sid, GetPassportCashAdd_Re(ERR_TOUCH_COMMUNICATION, roleid, localsid, 0));
		LOG_TRACE("getpassportcashadd roleid %d userid %d", roleid, pinfo->userid);
	}
};

};

#endif
