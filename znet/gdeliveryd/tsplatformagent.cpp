#include "tsplatformagent.h"
#include "gamegateserver.hpp"
#include "gdeliveryserver.hpp"
#include "removefaction.hpp"
#include "syncroleevent2sns.hpp"

namespace GNET
{

bool TSPlatformAgent::tsopen = false;
/*
int TSPlatformAgent::AddFriend(int roleid, int friendid, const Octets& name)
{
	if(!IsTSOpen())
		return -1;
	LOG_TRACE("TSPlatformAgent::AddFriend friendid=%d roleid=%d",friendid,roleid);
	SyncRoleEvent2SNS msg(roleid, TS_ADDFRIEND_TYPE, name);
	GameGateServer::GetInstance()->GateSend(msg);
	return 0;
}*/

int TSPlatformAgent::AddFamily(int roleid, int fid)
{
	if(!IsTSOpen())
		return -1;
	LOG_TRACE("TSPlatformAgent::AddFamily roleid=%d, fid=%d", roleid, fid);

	SyncRoleEvent2SNS msg(roleid, TS_ADD_FAMILY_TYPE, Octets(fid));
	GameGateServer::GetInstance()->GateSend(&msg);

	return 0;
}

int TSPlatformAgent::AddFaction(int familyid, int factionid)
{
	if(!IsTSOpen())
		return -1;
	LOG_TRACE("TSPlatformAgent::AddFaction familyid=%d, fid=%d", familyid, factionid);

	SyncRoleEvent2SNS msg(familyid, TS_ADD_FACTION_TYPE, Octets(factionid));
	GameGateServer::GetInstance()->GateSend(&msg);

	return 0;
}

int TSPlatformAgent::LeaveFamily(int roleid, int fid)
{
	if(!IsTSOpen())
		return -1;
	LOG_TRACE("TSPlatformAgent::LeaveFamily roleid=%d, fid=%d", roleid, fid);

	SyncRoleEvent2SNS msg(roleid, TS_DELETE_FAMILY_TYPE, Octets(fid));
	GameGateServer::GetInstance()->GateSend(&msg);

	return 0;
}

int TSPlatformAgent::LeaveFaction(int familyid, int factionid)
{
	if(!IsTSOpen())
		return -1;
	LOG_TRACE("TSPlatformAgent::LeaveFaction familyid=%d, factionid=%d", familyid, factionid);

	SyncRoleEvent2SNS msg(familyid, TS_DELETE_FACTION_TYPE, Octets(factionid));
	GameGateServer::GetInstance()->GateSend(&msg);

	return 0;
}





};
