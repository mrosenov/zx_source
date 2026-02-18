#include "localmacro.h"
#include "gamegateserver.hpp"
#include "gtplatformagent.h"
#include "state.hxx"

namespace GNET
{

GameGateServer GameGateServer::instance;
bool GameGateServer::gtopen = false;

const Protocol::Manager::Session::State* GameGateServer::GetInitState() const
{
	return &state_GameGateServer;
}

void GameGateServer::OnAddSession(Session::ID sid)
{
	DEBUG_PRINT("GameGateServer::OnAddSession sid=%d",sid);
	if(gate_sid_set.size()!=0)
        {
                Close(sid);
		Log::log(LOG_ERR, "GameGateServer::OnAddSession already connected,gate_sid_set size=%d",gate_sid_set.size());
		return;
        }
	gate_sid_set.insert(sid);
	GTPlatformAgent::SyncTOGate(0,true);
}

void GameGateServer::OnDelSession(Session::ID sid)
{
	DEBUG_PRINT("GameGateServer::OnDelSession sid=%d",sid);
	if(gate_sid_set.erase(sid)==0)
		Log::log(LOG_ERR, "GameGateServer::OnDelSession not found sid=%d",sid);
}

};
