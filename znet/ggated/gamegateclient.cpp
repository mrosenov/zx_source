#include "gtmanager.h"
#include "gamegateclient.hpp"
#include "state.hxx"
#include "timertask.h"
namespace GNET
{

GameGateClient GameGateClient::instance;

void GameGateClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* GameGateClient::GetInitState() const
{
	return &state_GameGateClient;
}

void GameGateClient::OnAddSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	DEBUG_PRINT("GameGateClient::OnAddSession");
	if (conn_state)
	{
	DEBUG_PRINT("GameGateClient::OnAddSession conn_state = true,close");
		Close(sid);
		return;
	}
	conn_state = true;
	this->sid = sid;
	backoff = BACKOFF_INIT;
	//TODO
}

void GameGateClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	DEBUG_PRINT("GameGateClient::OnDelSession");
	conn_state = false;
	GTManager::Instance()->OnGameGateDisconnect();
	Reconnect();
	//TODO
}

void GameGateClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	DEBUG_PRINT("GameGateClient::OnAbortSession");
	conn_state = false;
	GTManager::Instance()->OnGameGateDisconnect();
	Reconnect();
	//TODO
}

void GameGateClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
}

};
