
#include "gamedbclient.hpp"
#include "state.hxx"
#include "timertask.h"
#include "log.h"
namespace GNET
{

GameDBClient GameDBClient::instance;

void GameDBClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	backoff *= 2;
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
	Log::log( LOG_INFO, "GameDBClient::OnAddSession, sid=%d.", sid );
}

void GameDBClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log( LOG_INFO, "GameDBClient::OnDelSession, sid=%d.", sid );
}

void GameDBClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log( LOG_INFO, "GameDBClient::OnAbortSession." );
}

void GameDBClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
}

};
