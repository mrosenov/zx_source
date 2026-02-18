
#ifndef __GNET_BATTLEJOIN_RE_HPP
#define __GNET_BATTLEJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleJoin_Re : public GNET::Protocol
{
	#include "battlejoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleJoin_Re::retcode=%d,roleid=%d,gs_id=%d,map_id=%d,battle_type=%d,iscross=%d,team=%d,starttime=%d,cooltime=%d", retcode, roleid, gs_id, map_id, battle_type, iscross, team, starttime, cooltime);
		RoleData *data = GLinkServer::GetInstance()->GetRoleInfo(roleid);
		if(data != NULL)
		{
			GLinkServer::GetInstance()->Send(data->sid, this);
		}
	}
};

};

#endif
