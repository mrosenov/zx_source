
#include "gdeliveryclient.hpp"
#include "state.hxx"
#include "timertask.h"
namespace GNET
{

GDeliveryClient GDeliveryClient::instance;

void GDeliveryClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	//backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* GDeliveryClient::GetInitState() const
{
	return &state_GDeliverClient;
}

void GDeliveryClient::OnAddSession(Session::ID sid)
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
}

void GDeliveryClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log(LOG_ERR,"glinkd::Disconnect from delivery.\n");
}

void GDeliveryClient::OnAbortSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log(LOG_ERR,"glinkd::Connect to delivery failed.\n");
}

void GDeliveryClient::OnCheckAddress(SockAddr &sa) const
{
}

};
