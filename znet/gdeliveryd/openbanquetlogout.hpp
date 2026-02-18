
#ifndef __GNET_OPENBANQUETLOGOUT_HPP
#define __GNET_OPENBANQUETLOGOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "openbanquetmanager.h"

namespace GNET
{

class OpenBanquetLogout : public GNET::Protocol
{
	#include "openbanquetlogout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();

		if(!iscentral)
		{
			LOG_TRACE("OpenBanquetLogout send to Central roleid=%d", roleid);
			CentralDeliveryClient::GetInstance()->SendProtocol(*this);	
		}
		else
		{
			SOpenBanquetID* pOpenBanquetID = OpenBanquetManager::GetInstance()->FindRolesServer(roleid);	
			if(NULL == pOpenBanquetID)
			{
				//LOG_TRACE("OpenBanquetLogout roleid=%d FindRolesServer is NULL");
				return;
			}

			LOG_TRACE("OpenBanquetLogout gs_id=%d, map_id=%d, roleid=%d", pOpenBanquetID->gs_id, pOpenBanquetID->map_tag, roleid);
			OpenBanquetManager::GetInstance()->OnLogout(roleid, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag);
		}
	}
};

};

#endif
