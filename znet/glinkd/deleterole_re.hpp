
#ifndef __GNET_DELETEROLE_RE_HPP
#define __GNET_DELETEROLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "statusannounce.hpp"

namespace GNET
{

class DeleteRole_Re : public GNET::Protocol
{
	#include "deleterole_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (result!=ERR_SUCCESS)
		{
			Log::log(LOG_ERR,"glinkd::deleterole_re: failed. result=%d,roleid=%d\n",result,roleid);
		}
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (!lsm->ValidSid(localsid))
			return;
		lsm->ChangeState(localsid,&state_GSelectRoleServer);
		lsm->Send(localsid,this);

	}
};

};

#endif
