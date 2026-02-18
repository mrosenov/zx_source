
#ifndef __GNET_UPDATEENEMY_HPP
#define __GNET_UPDATEENEMY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "localmacro.h"

namespace GNET
{

class UpdateEnemy : public GNET::Protocol
{
	#include "updateenemy"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(oper!=ENEMY_REMOVE  && oper!= ENEMY_FREEZE && GLinkServer::ValidRole(sid,roleid))
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
