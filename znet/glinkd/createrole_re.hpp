
#ifndef __GNET_CREATEROLE_RE_HPP
#define __GNET_CREATEROLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "roleinfo"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "statusannounce.hpp"
namespace GNET
{

class CreateRole_Re : public GNET::Protocol
{
	#include "createrole_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (!lsm->ValidSid(localsid))
			return;
		lsm->ChangeState(localsid,&state_GSelectRoleServer);
		lsm->Send(localsid,this);
	}
};

};

#endif
