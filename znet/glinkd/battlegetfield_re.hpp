
#ifndef __GNET_BATTLEGETFIELD_RE_HPP
#define __GNET_BATTLEGETFIELD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gbattlerole"

namespace GNET
{

class BattleGetField_Re : public GNET::Protocol
{
	#include "battlegetfield_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleGetField_Re::retcode=%d,roleid=%d,starttime=%d,battle_type=%d,red_team.size=%d,blue_team.size=%d,localsid=%d,iscross=%d", retcode, roleid, starttime, battle_type, red_team.size(), blue_team.size(), iscross);
		RoleData *data = GLinkServer::GetInstance()->GetRoleInfo(roleid);
		if(data != NULL)
		{
			GLinkServer::GetInstance()->Send(data->sid, this);
		}
	}
};

};

#endif
