
#ifndef __GNET_BATTLELEAVE_RE_HPP
#define __GNET_BATTLELEAVE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleLeave_Re : public GNET::Protocol
{
	#include "battleleave_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleLeave_Re::retcode=%d,roleid=%d,iscross=%d", retcode, roleid, iscross);
		RoleData *data = GLinkServer::GetInstance()->GetRoleInfo(roleid);
		if(data != NULL)
		{
			GLinkServer::GetInstance()->Send(data->sid, this);
		}
	}
};

};

#endif
