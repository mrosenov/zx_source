
#include "log2tcpclient.hpp"
#include "state.hxx"
#include "timertask.h"
namespace GNET
{

Log2TcpClient Log2TcpClient::instance;

void Log2TcpClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	backoff *= 2;
	std::cout << "backoff = "  << backoff << std::endl;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* Log2TcpClient::GetInitState() const
{
	return &state_LoggerNull;
}

void Log2TcpClient::OnAddSession(Session::ID sid)
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
	std::cout << "Connected! " << std::endl;
	//TODO
}

void Log2TcpClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	std::cout << "Closed! " << std::endl;
	//TODO
}

void Log2TcpClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	std::cout << "Connect Faild! " << std::endl;
	//TODO
}

void Log2TcpClient::OnCheckAddress(SockAddr &sa) const
{
	struct sockaddr_in * s = sa;
	std::cout << "Connect to " << inet_ntoa(s->sin_addr) << ":" << ntohs(s->sin_port) << std::endl;
	//TODO
}

};
