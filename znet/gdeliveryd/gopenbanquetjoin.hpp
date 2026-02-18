
#ifndef __GNET_GOPENBANQUETJOIN_HPP
#define __GNET_GOPENBANQUETJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "openbanquetmanager.h"


namespace GNET
{

class GOpenBanquetJoin : public GNET::Protocol
{
	#include "gopenbanquetjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int roleid=0;
		if(roles.size()>0)
		{
			roleid = roles[0];
		}
		else
		{
			LOG_TRACE("OpenBanquetJoin is_team=%d, role.size=%d, gs_id=%d", is_team, roles.size(), gs_id);
			return;
		}

		LOG_TRACE("GOpenBanquetJoin is_team=%d, role.size=%d, gs_id=%d", is_team, roles.size(), gs_id);
		OpenBanquetManager::GetInstance()->OnJoin(roles, is_team, gs_id);
	}
};

};

#endif
