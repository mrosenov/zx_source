
#ifndef __GNET_BATTLEENTER_RE_HPP
#define __GNET_BATTLEENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleEnter_Re : public GNET::Protocol
{
	#include "battleenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleEnter_Re::retcode=%d,roleid=%d,endtime=%d,battle_type=%d,iscross=%d", retcode, roleid, endtime, battle_type, iscross);
		RoleData *data = GLinkServer::GetInstance()->GetRoleInfo(roleid);
		if(data != NULL)
		{
			GLinkServer::GetInstance()->Send(data->sid, this);
		}
	}
};

};

#endif
