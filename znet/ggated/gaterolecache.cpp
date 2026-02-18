#include "gaterolecache.h"
#include "gamegateclient.hpp"
#include "gtmanager.h"
#include "gateonlinelist_re.hpp"
#include "ggateonlineinfo"
#include "gtreconnect.hpp"
#include "gtteamdismiss.hpp"
namespace GNET
{

RoleContainer::~RoleContainer()
{
	RoleMap::iterator it;
	RoleMap::iterator ite = rolemap.end();
	it = rolemap.begin();
	for( ; it != ite; ++it)
	{
		delete it->second;
	}
	rolemap.clear();
}

//	bool RoleContainer::Update(){return true;}

int RoleContainer::GameOnlineSync(char start,char finish,const std::vector<GGateOnlineInfo>& data)
{

/*	switch(status)
	{
	case SYNC_START:
		break;
	case SYNC_CONTINUE:
		break;
	case SYNC_FINISH:
		break;
	default:
		return -1;
		break;
	}*/
	if(start)
		ClearGameOnlineStatus();

	std::vector<GGateOnlineInfo>::const_iterator it,ite=data.end();
	for(it=data.begin();it!=ite;++it)
	{
		RoleInfo* role = FindRole(it->roleid);
		if(role != NULL)
		{
			role->SetGameStatus(true);
		}
		else
		{
			AddRole(it->roleid,true,0);
		}
	}

	if(finish==0 && data.size()!=0)
	{
		GateOnlineList_Re re(data.back().roleid);
		GameGateClient::GetInstance()->SendProtocol(re);
	}
	else
	{
		GTManager::Instance()->OnGameGateSync();
	}
	return 0;
}

void RoleContainer::PartlyWalk( int& beginid, int count)
{
	RoleMap::iterator it;
	RoleMap::iterator ite = rolemap.end();
	if( -1 == beginid )
		it = rolemap.begin();
	else
		it = rolemap.lower_bound(beginid);
	time_t now = Timer::GetTime();
//	DEBUG_PRINT("RoleContainer update time=%d beginid=%d size=%d",now,beginid,rolemap.size());
	time_t data_expire_time = now - DATA_EXPIRE_INTERVAL; 
	time_t entry_expire_time = now - ENTRY_EXPIRE_INTERVAL;
	for( int n=0; it != ite && n < count; ++n)
	{
		RoleInfo& role=*it->second;
		if(role.IsDataExpired(data_expire_time))
		{
			DEBUG_PRINT("RoleContainer FreeData roleid=%d",it->first);
			role.FreeData();
		}
		if(role.IsEntryExpired(entry_expire_time))
		{
			DEBUG_PRINT("RoleContainer erase entry roleid=%d",it->first);
			delete it->second;
			rolemap.erase(it++);
		}
		else
			++it;
	//	UpdateEntry( it->first,*it->second, data_expire_time, entry_expire_time);
	}
	beginid = ( it != rolemap.end() ? it->first : -1 );
}

int RoleContainer::ClearGameOnlineStatus()
{
	RoleMap::iterator it;
	RoleMap::iterator ite = rolemap.end();
	it = rolemap.begin();
	time_t tvalue = Timer::GetTime();
	for( ; it != ite; ++it)
	{
		it->second->ClearGameOnline(tvalue);
	}
	return 0;
}

int RoleContainer::GetOnlineRolelist(int startrole,int count,std::vector<int>& rolelist,char & finish)
{
	if(count<=0)
		return -1;
	RoleMap::iterator it = rolemap.upper_bound(startrole);
	RoleMap::iterator ite = rolemap.end();
	rolelist.reserve(count);
	for(int i=0;i<count && it!=ite;++it)
	{
		if(it->second && it->second->GameOnline())
		{
			rolelist.push_back(it->first);
			i++;
		}
	}
	if(it==ite)
		finish=1;
	else
		finish=0;

	return 0;
}

void TeamCache::OnTeamSync(int gsid, const std::vector<GTTeamBean> & teams)
{
	TeamSet & teamset = teammap[gsid];
	std::vector<GTTeamBean>::const_iterator it, ite = teams.end();
	for (it = teams.begin(); it != ite; ++it)
		teamset.insert(it->teamid);
	LOG_TRACE("after teamsync, gsid %d size %d", gsid, teamset.size());
}

void TeamCache::OnTeamCreate(int gsid, int64_t teamid)
{
	TeamSet & teamset = teammap[gsid];
	teamset.insert(teamid);
	LOG_TRACE("after teamcreate, gsid %d size %d", gsid, teamset.size());
}

void TeamCache::OnTeamDismiss(int gsid, int64_t teamid)
{
	TeamSet & teamset = teammap[gsid];
	teamset.erase(teamid);
	LOG_TRACE("after teamdismiss, gsid %d size %d", gsid, teamset.size());
}

void TeamCache::OnGTDisconnect()
{
	teammap.clear();
	LOG_TRACE("clear all team cache");
}

void TeamCache::OnGTConnect()
{
	GProviderServer::GetInstance()->BroadcastProtocol(GTReconnect(0));
	LOG_TRACE("broadcast gtreconnect to all gs");
}

void TeamCache::OnGSDisconnect(int gsid)
{
	if (GTPlatformClient::GetInstance()->IsGTSessionOK())
	{
		TeamSet & teamset = teammap[gsid];
		TeamSet::const_iterator it, ite = teamset.end();
		for (it = teamset.begin(); it != ite; ++it)
			GTPlatformClient::GetInstance()->SendProtocol(GTTeamDismiss(*it));
		LOG_TRACE("tell gt server to dismiss %d teams", teamset.size());
	}
	teammap.erase(gsid);
}

};
