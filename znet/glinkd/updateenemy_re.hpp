
#ifndef __GNET_UPDATEENEMY_RE_HPP
#define __GNET_UPDATEENEMY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class UpdateEnemy_Re : public GNET::Protocol
{
	#include "updateenemy_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);	
	}
};

};

#endif
