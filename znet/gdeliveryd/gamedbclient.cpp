
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp" //for zoneid,aid

#include "state.hxx"
#include "timertask.h"
#include "log.h"
#include "dbauctionlist.hrp"
#include "announcezoneid2.hpp"
#include "stockexchange.h"
#include "topmanager.h"
#include "siegemanager.h"
#include "commondata.h"
#include "snsmessagemanager.h"
#include "territorymanager.h"
#include "uniqueauctionmanager.h"
#include "consignmanager.h"
#include "namemanager.h"
#include "kingdommanager.h"
#include "topflowermanager.h"
#include "iwebcmd.h"

namespace GNET
{

GameDBClient GameDBClient::instance;

void GameDBClient::Reconnect()
{
	if (!need2reconnect)
		return;
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	//backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* GameDBClient::GetInitState() const
{
	return &state_GameDBClient;
}

void GameDBClient::OnAddSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	if (conn_state)
	{
		Close(sid);
		return;
	}
	conn_state = true;
	this->sid = sid;
	backoff = BACKOFF_INIT;
	// get auction list
	// Send(sid,Rpc::Call(RPC_DBAUCTIONLIST,DBAuctionListArg()));
	NameManager::GetInstance()->OnDBConnect(this, sid);
	TopManager::Instance()->OnDBConnect(this, sid);
	StockExchange::Instance()->OnDBConnect(this, sid);
	SiegeManager::GetInstance()->OnDBConnect();
	//TopFlowerManager::Instance()->OnDBConnect();
	CommonDataMan::Instance()->OnDBConnect(this, sid);
	SNSMessageManager::GetInstance()->OnDBConnect(this, sid);
	TerritoryManager::GetInstance()->OnDBConnect(this, sid);
	UniqueAuctionManager::GetInstance()->OnDBConnect(this, sid);
	KingdomManager::GetInstance()->OnDBConnect(this, sid);
	IwebCmd::GetInstance()->OnDBConnect(this, sid);
	// send zoneid,aid, to announce self as Delivery to gamedbd
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	Send( sid,AnnounceZoneid2(dsm->zoneid,dsm->aid,(char)(dsm->IsCentralDS())) );
	ConsignManager::GetInstance()->OnDBConnect(this, sid);
}

void GameDBClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log(LOG_ERR,"gdelivery:: disconnect from GameDB\n");
}

void GameDBClient::OnAbortSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log(LOG_ERR,"gdelivery:: connect GameDB failed\n");
}

void GameDBClient::OnCheckAddress(SockAddr &sa) const
{
}

};
