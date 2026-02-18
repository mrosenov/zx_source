
#ifndef __GNET_OPENBANQUETQUEUECOUNT_HPP
#define __GNET_OPENBANQUETQUEUECOUNT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "openbanquetmanager.h"
#include "openbanquetqueuecount_re.hpp"
#include "mapuser.h"
#include "gdeliveryserver.hpp"

namespace GNET
{

class OpenBanquetQueueCount : public GNET::Protocol
{
	#include "openbanquetqueuecount"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();

		if(!iscentral)
		{
			iscross = true;
			LOG_TRACE("OpenBanquetCancel send to Central roleid=%d", roleid);
			CentralDeliveryClient::GetInstance()->SendProtocol(*this);	
		}
		else
		{
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
			if ( NULL==pinfo )
			{
				//LOG_TRACE("OpenBanquetQueueCount roleid=%d not online", roleid);
				return;
			}

			SOpenBanquetID* pOpenBanquetID = OpenBanquetManager::GetInstance()->FindRolesServer(roleid);	
			if(NULL == pOpenBanquetID)
			{
				//LOG_TRACE("OpenBanquetQueueCount roleid=%d FindRolesServer is NULL", roleid);
				return;
			}

			LOG_TRACE("OpenBanquetQueueCount gs_id=%d, map_id=%d", pOpenBanquetID->gs_id, pOpenBanquetID->map_tag);
			int queue_count = OpenBanquetManager::GetInstance()->GetQueuePos(roleid, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag);
			
			if(iscross)
			{
				int zoneid = CrossBattleManager::GetInstance()->GetSrcZoneID(roleid);
				CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, OpenBanquetQueueCount_Re(roleid, pinfo->gameid, pinfo->world_tag, queue_count, pinfo->localsid));
			}
			else
			{
				GDeliveryServer::GetInstance()->Send(pinfo->linksid, OpenBanquetQueueCount_Re(roleid, pinfo->gameid, pinfo->world_tag, queue_count, pinfo->localsid));
			}
		}
	}
};

};

#endif
