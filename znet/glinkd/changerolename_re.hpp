
#ifndef __GNET_CHANGEROLENAME_RE_HPP
#define __GNET_CHANGEROLENAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ChangeRolename_Re : public GNET::Protocol
{
	#include "changerolename_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(localsid, roleid)) 
			return;
		GLinkServer::GetInstance()->Send(localsid, this);	
	}
};

};

#endif
