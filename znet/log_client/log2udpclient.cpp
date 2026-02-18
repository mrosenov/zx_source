
#include "log2udpclient.hpp"
#include "state.hxx"
#include "timertask.h"
namespace GNET
{

Log2UdpClient Log2UdpClient::instance;

void Log2UdpClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* Log2UdpClient::GetInitState() const
{
	return &state_LoggerNull;
}

void Log2UdpClient::OnAddSession(Session::ID sid)
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
	//TODO
}

void Log2UdpClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	//TODO
}

void Log2UdpClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	//TODO
}

void Log2UdpClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
}

};
