
#include "glinkclient.h"
#include "state.hxx"
namespace GNET
{

GLinkClient GLinkClient::instance;

const Protocol::Manager::Session::State* GLinkClient::GetInitState() const
{
	return &state_GLoginClient;
}

void GLinkClient::OnAddSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	if (conn_state)
	{
		Close(sid);
		return;
	}
	conn_state = true;
	this->sid = sid;
	//TODO
}

void GLinkClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	//TODO
}

void GLinkClient::OnAbortSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
}

void GLinkClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	//TODO
}

void GLinkClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
}

};
