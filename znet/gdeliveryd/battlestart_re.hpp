
#ifndef __GNET_BATTLESTART_RE_HPP
#define __GNET_BATTLESTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battlemanager.h"
#include "openbanquetmanager.h"
#include "battlemanager.h"

namespace GNET
{

class BattleStart_Re : public GNET::Protocol
{
	#include "battlestart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "BattleStart_Re: retcode=%d gs_id=%d, tag=%d map_id=%d battle_type=%d.\n", retcode, gs_id, tag, map_id, battle_type);
		if(GDeliveryServer::GetInstance()->IsCentralDS() && battle_type == BATTLEFIELD_TYPE_FLOW_CRSSVR)
		{
			OpenBanquetManager::GetInstance()->OnOpenBanquetStart(gs_id,tag,retcode);
		}
		else
		{
			BattleManager::GetInstance()->OnBattleStart(gs_id, tag, map_id, retcode);
		}
	}
};

};

#endif
