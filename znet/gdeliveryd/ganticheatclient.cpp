
#include "ganticheatclient.hpp"
#include "state.hxx"
#include "timertask.h"

#include "acstatusannounce.hpp"
#include "gdeliveryserver.hpp"
#include "acwhoami.hpp"
#include "mapuser.h"
#include "acstatusannounce2.hpp"
namespace GNET
{

GAntiCheatClient GAntiCheatClient::instance;

void GAntiCheatClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* GAntiCheatClient::GetInitState() const
{
	return &state_ACClient;
}

class AntiCheatClientQueryUser : public UserContainer::IQueryUser
{   
public:
	ACStatusAnnounce2 acsa;
	bool Update( int userid, UserInfo & info )
	{
		acsa.status=_STATUS_ONGAME;
		if (info.status==_STATUS_ONGAME)
		{
			acsa.info_list.push_back( ACOnlineStatus2(info.roleid, userid, info.ip) );
		}
		return true;
	}
};

void GAntiCheatClient::OnAddSession(Session::ID sid)
{
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

	DEBUG_PRINT("ganticheatclient:: connect gacd successfully!");
	SendProtocol( ACWhoAmI(_DELIVERY_CLIENT,0/* for delivery, id is always 0 */) );
	//Announce all online users to gFactionServer
	AntiCheatClientQueryUser	q;
	UserContainer::GetInstance().WalkUser( q );
	if (q.acsa.info_list.size() > 0)
		SendProtocol(q.acsa);
}

void GAntiCheatClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	DEBUG_PRINT("ganticheatclient:: disconnect from GACD!\n");
}

void GAntiCheatClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	DEBUG_PRINT("ganticheatclient:: connect to GACD failed!\n");
}

void GAntiCheatClient::OnCheckAddress(SockAddr &sa) const
{
}

};
