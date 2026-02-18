
#include "kdsclient.hpp"
#include "state.hxx"
#include "timertask.h"
#include "tokengenerator.h"

namespace GNET
{

KdsClient KdsClient::instance;

void KdsClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* KdsClient::GetInitState() const
{
	return &state_KdsClient;
}

void KdsClient::OnAddSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	DEBUG_PRINT("KdsClient::OnAddSession");
	if (conn_state)
	{
		Close(sid);
		DEBUG_PRINT("KdsClient::OnAddSession already connect");
		return;
	}
	conn_state = true;
	this->sid = sid;
	backoff = BACKOFF_INIT;
	if(TokenGenerator::GetInstance().GetNewKey())
		DEBUG_PRINT("KdsClient::OnAddSession GetNewKey err");
}

void KdsClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	DEBUG_PRINT("KdsClient::OnDelSession");
}

void KdsClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	DEBUG_PRINT("KdsClient::OnAbortSession");
}

void KdsClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
}

};
