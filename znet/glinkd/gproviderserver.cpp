
#include "gproviderserver.hpp"
#include "state.hxx"
#include "announceproviderid.hpp"
#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "getlineplayerlimit.hrp"
#include "maperaser.h"
namespace GNET
{

GProviderServer GProviderServer::instance;

const Protocol::Manager::Session::State* GProviderServer::GetInitState() const
{
	return &state_GProviderLinkServer;
}

void GProviderServer::OnAddSession(Session::ID sid)
{
}

void GProviderServer::OnDelSession(Session::ID sid)
{
	Thread::RWLock::WRScoped l(locker_gameservermap);
	GameServerMap::iterator itg=gameservermap.begin();
	MapEraser<GameServerMap> del_keys(gameservermap);
	for (;itg!=gameservermap.end();itg++)
	{
		if (itg->second.sid == sid)
		{
			Log::log(LOG_ERR,"glinkd::disconnect from gameserver. gameserver %d, sid=%d\n",itg->first,itg->second.sid);
			Thread::HouseKeeper::AddTimerTask(new DisconnectTimeoutTask((*itg).first/*gameserver id*/),15);
			del_keys.push((*itg).first);
		}
	}
}
void DisconnectTimeoutTask::Run()
{
	//find whether the gameserver is reconnect
	bool blconnect=false;
	{
		GProviderServer* psm=GProviderServer::GetInstance();
		Thread::RWLock::RDScoped l(psm->locker_gameservermap);
		blconnect= psm->gameservermap.find(gid_discon)!=psm->gameservermap.end();
	}
	if (!blconnect) //if connection is not revive,disconnect all players that belongs to this gameserver
	{
		DEBUG_PRINT("glinkd:: disconnect from gamesever %d for 15 sec, drop all players on this gameserver.",gid_discon);
		GLinkServer* gsm=GLinkServer::GetInstance();
		Thread::RWLock::WRScoped l(gsm->locker_map);
		for (GLinkServer::RoleInfoMap::iterator it=gsm->roleinfomap.begin();it!=gsm->roleinfomap.end();++it)
		{
			if (it->second.gs_id==gid_discon) 
				gsm->Close(it->second.sid);
		}
	}
}

bool GProviderServer::GetLineList(LineList &linelist)
{ 
    Thread::RWLock::RDScoped l(locker_gameservermap);
    if( linelist_cache.linelist.size() == 0 ) 
    {
        GDeliveryClient::GetInstance()->SendProtocol(
            (GetLinePlayerLimit*)Rpc::Call(RPC_GETLINEPLAYERLIMIT, GetLinePlayerLimitArg(GLinkServer::GetInstance()->GetVersion())));
        return false;
    }

    linelist = linelist_cache;
    linelist.linelist.clear();
    //DEBUG_PRINT("glinkd:: GetLineList tlist.size=%d", tlist.linelist.size());

    for(LineInfoVector::iterator it = linelist_cache.linelist.begin(),
        ie = linelist_cache.linelist.end(); it != ie; ++it)
    {
	    DEBUG_PRINT("glinkd:: GetLineList line_id=%d, it.ishide=%d", (*it).id, (*it).ishide);
	
	if(!(*it).ishide)
	{
		linelist.linelist.push_back(*it);
	}
    }

    //DEBUG_PRINT("glinkd:: GetLineList linelist.size=%d", linelist.linelist.size());

    return true;
}

int GProviderServer::CheckLineLimit(int lineid, char flag)
{
    Thread::RWLock::RDScoped l(locker_gameservermap);
    if( linelist_cache.linelist.size() == 0 )
    {
        GDeliveryClient::GetInstance()->SendProtocol(
            (GetLinePlayerLimit*)Rpc::Call(RPC_GETLINEPLAYERLIMIT, GetLinePlayerLimitArg(GLinkServer::GetInstance()->GetVersion())));
        return -1;
    }
    for(LineInfoVector::const_iterator it = linelist_cache.linelist.begin(),
        ie = linelist_cache.linelist.end(); it != ie; ++it)
    {
	DEBUG_PRINT("glinkd:: CheckLineLimit flag=%d, lineid=%d, it.id=%d, it.ishide=%d.", flag, lineid, (*it).id, (*it).ishide);
	
	if((flag == DS_TO_CENTRALDS || flag == DS_TO_BATTLEMAP) && (*it).ishide && (*it).id == lineid)// 如果从原服到跨服，可以去隐藏线
	{
		return (*(int*)((*it).attribute.begin())) < 100 ? 0 : 1;
	}

        if(!(*it).ishide && (*it).id == lineid )
            return (*(int*)((*it).attribute.begin())) < 100 ? 0 : 1;
            //return (*it).load < 100 ? 0 : 1;
    }
    return -2;
}

};
