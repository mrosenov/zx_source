
#ifndef __GNET_DSANNOUNCEIDENTITY_HPP
#define __GNET_DSANNOUNCEIDENTITY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "centraldeliveryserver.hpp"
#include "centraldeliveryclient.hpp"
#include "crosscrssvrteamsmanager.h"

namespace GNET
{

class DSAnnounceIdentity : public GNET::Protocol
{
	#include "dsannounceidentity"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv DSAnnounceIdentity zoneid %d version %d edition.size %d", zoneid, version, edition.size());
		GDeliveryServer * dsm = GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			if (CentralDeliveryServer::GetInstance()->IsConnect(zoneid))
			{
				Log::log(LOG_ERR, "DSAnnounceIdentity Identical Zoneid %d", zoneid);
				manager->Close(sid);
				return;
			}
			if (!CentralDeliveryServer::GetInstance()->IsDebug() && (version != dsm->GetVersion() || edition != dsm->GetEdition()))
			{
				Log::log(LOG_ERR, "CentralDeliveryClient zoneid(%d) version-edition.size %d-%d does not match Server(local) %d-%d, disconnect connection", zoneid, version, edition.size(), dsm->GetVersion(), dsm->GetEdition().size());
				manager->Close(sid);
				return;
			}
			LOG_TRACE("Accept CentralDSClient zoneid(%d) and send back", zoneid);
			CentralDeliveryServer::GetInstance()->InsertZoneId(zoneid, sid);

			CrossCrssvrTeamsManager::Instance()->OnCrossTopUpdate(CRSSVRTEAMS_DAILY_TOP_TYPE);
			CrossCrssvrTeamsManager::Instance()->OnCrossTopUpdate(CRSSVRTEAMS_WEEKLY_TOP_TYPE);
			this->zoneid = GDeliveryServer::GetInstance()->zoneid;
			manager->Send(sid, this);
		}
		else
			CentralDeliveryClient::GetInstance()->SetServerZoneid(zoneid);
	}
};

};

#endif
