
#ifndef __GNET_BATTLESTARTNOTIFY_HPP
#define __GNET_BATTLESTARTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleStartNotify : public GNET::Protocol
{
	#include "battlestartnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleStartNotify::result=%d,roleid=%d,gs_id=%d,map_id=%d,starttime=%d,iscross=%d", start_result, roleid, gs_id, map_id, starttime, iscross);
		RoleData *data = GLinkServer::GetInstance()->GetRoleInfo(roleid);
		if(data != NULL)
		{
			GLinkServer::GetInstance()->Send(data->sid, this);
		}
	}
};

};

#endif
