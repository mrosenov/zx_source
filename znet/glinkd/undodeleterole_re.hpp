
#ifndef __GNET_UNDODELETEROLE_RE_HPP
#define __GNET_UNDODELETEROLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class UndoDeleteRole_Re : public GNET::Protocol
{
	#include "undodeleterole_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (result!=ERR_SUCCESS)
		{
			Log::log(LOG_ERR,"glinkd:: undodeleterole_re: failed. result=%d\n",result);
		}
		GLinkServer* lsm=GLinkServer::GetInstance();
		lsm->ChangeState(localsid,&state_GSelectRoleServer);
		lsm->Send(localsid,this);
	}
};

};

#endif
