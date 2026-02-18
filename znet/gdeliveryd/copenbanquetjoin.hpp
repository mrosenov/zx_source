
#ifndef __GNET_COPENBANQUETJOIN_HPP
#define __GNET_COPENBANQUETJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "openbanquetmanager.h"
#include "openbanquetjoin_re.hpp"

namespace GNET
{

class COpenBanquetJoin : public GNET::Protocol
{
	#include "copenbanquetjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();

		if(iscentral)
		{
			// ¿ç·ş
			SOpenBanquetID* pOpenBanquetID = OpenBanquetManager::GetInstance()->FindRolesServer(roleid);	
			if(NULL == pOpenBanquetID)
			{
				LOG_TRACE("COpenBanquetJoin roleid=%d FindRolesServer is NULL");
				return;
			}

			int count = OpenBanquetManager::GetInstance()->GetQueuePos(roleid, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag);
			if(count == 0)
			{
				LOG_TRACE("COpenBanquetJoin count is 0, roleid=%d", roleid);
				return;
			}
			
			int zoneid = CrossBattleManager::GetInstance()->GetSrcZoneID(roleid);
			LOG_TRACE("COpenBanquetJoin zoneid=%d", zoneid);
			CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, OpenBanquetJoin_Re(roleid, S2C_OPENBANQUET_QUEUE, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag, count, 0));
		}		
	}
};

};

#endif
