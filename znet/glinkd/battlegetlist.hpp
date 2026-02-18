
#ifndef __GNET_BATTLEGETLIST_HPP
#define __GNET_BATTLEGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gfieldinfo"
#include "battlegetlist_re.hpp"

namespace GNET
{

class BattleGetList : public GNET::Protocol
{
	#include "battlegetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
		{
			GLinkServer::GetInstance()->SessionError(sid,ERR_INVALID_ACCOUNT,"Error userid or roleid. BattleGetList");
			return;
		}       
		DEBUG_PRINT("BattleGetMap: %d\n", roleid);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
