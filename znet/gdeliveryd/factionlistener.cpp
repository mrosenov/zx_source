#include "factionmanager.h"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "getfactionbaseinfo_re.hpp"
#include "siegemanager.h"
#include "dbfactionget.hrp"

namespace GNET
{
bool RefListener::OnEvent(int event, FactionDetailInfo * faction)
{
	if (event == E_ONLISTEN)
	{
		faction->SetKeepAlive();
		return true;
	}
	return false;
}

bool BaseInfoListener::OnEvent(int event, FactionDetailInfo * faction)
{
	if (event == E_LOADFINISH)
	{
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL==pinfo)
		{
			DEBUG_PRINT("BaseInfoListener, roleid=%d not found", roleid);
			return true;
		}
		GetFactionBaseInfo_Re re;
		re.roleid = roleid;
		re.localsid = pinfo->localsid;
		re.faction_info.fid = faction->info.fid;
		re.faction_info.name = faction->info.name;
		re.faction_info.level = faction->info.level;
		GDeliveryServer::GetInstance()->Send(pinfo->linksid,re);
		return true;
	}
	return false;
}

bool LoginListener::OnEvent(int event, FactionDetailInfo * faction)
{
	if (event == E_LOADFINISH)
	{
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (NULL==pinfo || pinfo->roleid!=roleid) 
		{
			DEBUG_PRINT("LoginListener, roleid=%d not found", roleid);
			return true;
		}
		FactionManager::Instance()->OnLogin(roleid,faction->info.fid, factiontitle, familyid, 
					contribution,devotion,pinfo->gameid,pinfo->linksid,pinfo->localsid);
		return true;
	}
	return false;
}

bool FamilyInfoListener::OnEvent(int event, FactionDetailInfo * faction)
{
	return true;
}

bool SiegeListener::OnEvent(int event, FactionDetailInfo * faction)
{
	if (event == E_ONLISTEN)
	{
		faction->SetKeepAlive();
		SiegeManager::GetInstance()->SyncFaction(faction->info.fid, faction->info.master);
		return true;
	}
	return false;
}

bool TerritoryListener::OnEvent(int event, FactionDetailInfo * faction)
{
	if (event == E_ONLISTEN)
	{
		faction->SetKeepAlive();
		return true;
	}
	return false;
}

void LoadRefFaction(int fid)
{
	LOG_TRACE("Load and reference faction %d", fid);
	FactionDetailInfo * faction = FactionManager::Instance()->Find(fid);
	if (!faction)
	{
		DBFactionGet* rpc = (DBFactionGet*) Rpc::Call( RPC_DBFACTIONGET,FactionId(fid));
		rpc->listener = new RefListener();
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return;
	}
	faction->SetKeepAlive();
}

void ReleaseFaction(int fid)
{
	LOG_TRACE("Release faction %d", fid);
	FactionDetailInfo * faction = FactionManager::Instance()->Find(fid);
	if (faction)
		faction->ClrKeepAlive();
}

}; // end namespace
