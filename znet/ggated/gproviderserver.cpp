
#include "gproviderserver.hpp"
#include "state.hxx"

namespace GNET
{

GProviderServer GProviderServer::instance;

const Protocol::Manager::Session::State* GProviderServer::GetInitState() const
{
	return &state_GProviderGateServer;
}

void GProviderServer::OnAddSession(Session::ID sid)
{
	LOG_TRACE("GProviderServer OnAddSession sid=%d", sid);
}

void GProviderServer::OnDelSession(Session::ID sid)
{
	GameServerMap::iterator itg=gameservermap.begin();
	MapEraser<GameServerMap> del_keys(gameservermap);
	for (;itg!=gameservermap.end();itg++)
	{
		if ((*itg).second == sid)
		{
			Log::log(LOG_ERR,"Erase gameserver %d,sid=%d\n",(*itg).first, sid);
//			SiegeManager::GetInstance()->OnDisconnect(itg->first);
			del_keys.push( (*itg).first );
			TeamCache::GetInstance()->OnGSDisconnect((*itg).first);
		}
	}
}

};
