
#ifndef __GNET_OPENBANQUETJOIN_HPP
#define __GNET_OPENBANQUETJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "openbanquetmanager.h"

namespace GNET
{

class OpenBanquetJoin : public GNET::Protocol
{
	#include "openbanquetjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetJoin gs_id=%d, map_id=%d, is_team=%d, role.size=%d", gs_id, map_id, is_team, roles.size());
		OpenBanquetManager::GetInstance()->OnJoin(gs_id,map_id,roles,is_team,localsid);
	}
};

};

#endif
