
#ifndef __GNET_GOPENBANQUETLEAVE_HPP
#define __GNET_GOPENBANQUETLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "openbanquetmanager.h"

namespace GNET
{

class GOpenBanquetLeave : public GNET::Protocol
{
	#include "gopenbanquetleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "GOpenBanquetLeave gs_id=%d, map_id=%d roleid=%d.\n", gs_id, map_id, roleid);
		OpenBanquetManager::GetInstance()->OnBattleLeave(roleid, gs_id, map_id);
	}
};

};

#endif
