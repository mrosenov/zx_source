
#ifndef __GNET_OPENBANQUETCANCEL_HPP
#define __GNET_OPENBANQUETCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "openbanquetmanager.h"

namespace GNET
{

class OpenBanquetCancel : public GNET::Protocol
{
	#include "openbanquetcancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();

		if(!iscentral)
		{

			LOG_TRACE("OpenBanquetCancel send to Central roleid=%d", roleid);
			CentralDeliveryClient::GetInstance()->SendProtocol(*this);	
		}
		else
		{
			SOpenBanquetID* pOpenBanquetID = OpenBanquetManager::GetInstance()->FindRolesServer(roleid);	
			if(NULL == pOpenBanquetID)
			{
				LOG_TRACE("OpenBanquetCancel roleid=%d FindRolesServer is NULL");
				return;
			}

			LOG_TRACE("OpenBanquetCancel gs_id=%d, map_id=%d, roleid=%d", pOpenBanquetID->gs_id, pOpenBanquetID->map_tag, roleid);
			OpenBanquetManager::GetInstance()->OnCancel(roleid, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag, isqueue);
		}
	}
};

};

#endif
