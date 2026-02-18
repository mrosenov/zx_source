
#include "snsplatformclient.hpp"
#include "state.hxx"
#include "timertask.h"
#include "announcezoneidtoim.hpp"
namespace GNET
{

SNSPlatformClient SNSPlatformClient::instance;

void SNSPlatformClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* SNSPlatformClient::GetInitState() const
{
	return &state_SNSPlatformClient;
}

void SNSPlatformClient::OnAddSession(Session::ID sid)
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

	AnnounceZoneidToIM msg(aid, zoneid, Timer::GetTime());
	SendProtocol(msg);
	LOG_TRACE("SNSPlatformClient::OnAddSession sid=%d aid=%d zoneid=%d", sid, aid, zoneid);
}

void SNSPlatformClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	//TODO
}

void SNSPlatformClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	//TODO
}

void SNSPlatformClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
}

};
