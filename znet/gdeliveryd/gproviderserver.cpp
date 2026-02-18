
#include "gproviderserver.hpp"
#include "state.hxx"
#include "announceproviderid.hpp"
#include "maperaser.h"
#include "mapgameattr.h"
#include "querygameserverattr_re.hpp"
#include "battlemanager.h"
#include "siegemanager.h"
#include "factionmanager.h"
#include "syncfactionlevel.hpp"
#include "commondata.h"
#include "gshopsetdiscountscheme.hpp"
#include "gshopsetsalescheme.hpp"
#include "gdeliveryserver.hpp"
#include "territorymanager.h"
#include "fungamemanager.h"
#include "raidmanager.h"
#include "kingdommanager.h"
#include "iwebcmd.h"
#include "facbasemanager.h"

namespace GNET
{

GProviderServer GProviderServer::instance;

const Protocol::Manager::Session::State* GProviderServer::GetInitState() const
{
	return &state_GProviderDeliveryServer;
}

const LinePlayerLimitVector& GProviderServer::GetLineLimits()
{
	return cache_server_load.GetLineLimit();
}
void GProviderServer::SetCachedLimits()
{
	Thread::RWLock::RDScoped l(locker_gameservermap);
	LinePlayerLimitVector v;
	for(GameServerMap::const_iterator it = gameservermap.begin(), ie = gameservermap.end(); it != ie; ++it)
	{
		LinePlayerLimit limit;
		limit.max_num = (*it).second.max_num;
		limit.line_id = (*it).first;
		limit.cur_num = UserContainer::GetInstance().GetLinePlayerNumber(limit.line_id);
		limit.attr = (*it).second.attr;
		v.push_back(limit);
	}
	cache_server_load.SetLoad(UserContainer::GetInstance().GetPlayerLimit(),UserContainer::GetInstance().Size(),v);
}
LinePlayerLimitVector GProviderServer::GetLimits()
{
/*	Thread::RWLock::RDScoped l(locker_gameservermap);
	LinePlayerLimitVector v;
	for(GameServerMap::const_iterator it = gameservermap.begin(), ie = gameservermap.end(); it != ie; ++it)
	{
		LinePlayerLimit limit;
		limit.max_num = (*it).second.max_num;
		limit.line_id = (*it).first;
		limit.cur_num = UserContainer::GetInstance().GetLinePlayerNumber(limit.line_id);
		limit.attr = (*it).second.attr;
		v.push_back(limit);
	}
	return v;*/
	return GetLineLimits(); 
}

int GProviderServer::SetLimits(const LinePlayerLimitVector &limits)
{
	Thread::RWLock::WRScoped l(locker_gameservermap);
	for(LinePlayerLimitVector::const_iterator it = limits.begin(), ie = limits.end(); it != ie; ++it)
	{
		const LinePlayerLimit &limit = (*it);
		GameServerMap::iterator it2 = gameservermap.find(limit.line_id);
		if( it2 != gameservermap.end() )
			(*it2).second.max_num = limit.max_num;
	}
	return 0;
}

void GProviderServer::NotifyGShopScheme(Session::ID sid)
{
	GShopScheme &scheme = GDeliveryServer::GetInstance()->gshop_scheme;
	Send(sid, GShopSetDiscountScheme(scheme.discount));
	Send(sid, GShopSetSaleScheme(scheme.sale));
}

void GProviderServer::OnAddSession(Session::ID sid)
{
	//announce gameserver attribute to gameserver
	QueryGameServerAttr_Re qgsa_re;
	GameAttrMap::Get(qgsa_re.attr);
	Send(sid,qgsa_re);
	SyncFactionLevel data;
	if(FactionManager::Instance()->GetLevels(data.list))
		Send(sid,data);
	SiegeManager::GetInstance()->SendStatus(sid);
	TerritoryManager::GetInstance()->SyncTerritoryGS(sid);
	KingdomManager::GetInstance()->OnGSConnect(sid);
	FacBaseManager::GetInstance()->OnGSConnect(sid);
	IwebCmd::GetInstance()->OnGSConnect(sid);
	/*
	SyncFactionHostiles sync;
	if(FactionManager::Instance()->GetHostiles(sync))
		Send(sid,data);
	*/
	CommonDataMan::Instance()->SyncGs(sid);
	NotifyGShopScheme(sid);
}

void GProviderServer::OnDelSession(Session::ID sid)
{
	Thread::RWLock::WRScoped l(locker_gameservermap);
	GameServerMap::iterator itg=gameservermap.begin();
	MapEraser<GameServerMap> del_keys(gameservermap);
	for (;itg!=gameservermap.end();itg++)
	{
		if ((*itg).second.sid==sid)
		{
			Log::log(LOG_ERR,"gdelivery::Erase gameserver %d,sid=%d\n",(*itg).first,(*itg).second.sid);
			SiegeManager::GetInstance()->OnDisconnect(itg->first);
			BattleManager::GetInstance()->OnDisconnect(itg->first);
			FunGameManager::GetInstance().OnGSLeave(itg->first);
			TerritoryManager::GetInstance()->OnDisconnect(itg->first);
			KingdomManager::GetInstance()->OnDisconnect(itg->first);
			RaidManager::GetInstance()->OnDisconnect(itg->first);
			FacBaseManager::GetInstance()->OnDisconnect(itg->first);
			del_keys.push( (*itg).first );
		}
	}
	SetCachedLimits();
}

void LinePlayerTimer::Run()
{
	GProviderServer::GetInstance()->SetCachedLimits();
	Thread::HouseKeeper::AddTimerTask(this,update_time);
}

};
