
#ifndef __GNET_COMBATSTATUS_HPP
#define __GNET_COMBATSTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class CombatStatus : public GNET::Protocol
{
	#include "combatstatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
