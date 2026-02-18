
#include "centraldeliveryserver.hpp"
#include "state.hxx"
#include "loadexchange.hpp"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "gproviderserver.hpp"
#include "crossbattlemanager.hpp"

namespace GNET
{

CentralDeliveryServer CentralDeliveryServer::instance;

const Protocol::Manager::Session::State* CentralDeliveryServer::GetInitState() const
{
	return &state_CentralDeliveryServer;
}

void CentralDeliveryServer::OnAddSession(Session::ID sid)
{
	//TODO
}

void CentralDeliveryServer::OnDelSession(Session::ID sid)
{
	int zoneid = -1;
	DSMap::iterator it = ds_map.begin();
	for (; it != ds_map.end(); ++it)
	{
		if (it->second.sid == sid)
		{
			zoneid = it->first;
			ds_map.erase(it);
			break;
		}
	}
	if (zoneid != -1)
	{
		int count = UserContainer::GetInstance().DisconnectZoneUsers(zoneid);
		Log::log(LOG_ERR, "CentralDeliveryServer erase deliveryd %d sid=%d drop %d users", zoneid, sid, count);
	}
}

void CentralDeliveryServer::SetLoad(int zoneid, int srv_limit, int srv_count, const LinePlayerLimitVector & lines)
{
	DSMap::iterator it = ds_map.find(zoneid);
	if (it == ds_map.end())
	{
		Log::log(LOG_ERR, "Central delivery set load, invalid zoneid %d", zoneid);
		return;
	}
	it->second.cache_server_load.SetLoad(srv_limit, srv_count, lines);
}

void LoadExchangeTask::Run()
{
	LOG_TRACE("CentralDeliveryServer start load exchange");
	LoadExchange exchg;
	GDeliveryServer * dsm = GDeliveryServer::GetInstance();
	exchg.zoneid = dsm->zoneid;
	exchg.version = dsm->GetVersion();
	exchg.edition = dsm->GetEdition();
	exchg.server_limit = UserContainer::GetInstance().GetPlayerLimit();
	exchg.server_count = UserContainer::GetInstance().Size();
	exchg.line_status = GProviderServer::GetInstance()->GetLimits();
	CentralDeliveryServer::GetInstance()->BroadcastProtocol(exchg);
	Thread::HouseKeeper::AddTimerTask(this, interval);
}

};
