#ifndef __GNET_GAMEGATECLIENT_HPP
#define __GNET_GAMEGATECLIENT_HPP

#include "protocol.h"
#include "thread.h"

namespace GNET
{

class GameGateClient : public Protocol::Manager
{
	static GameGateClient instance;
	size_t		accumulate_limit;
	Session::ID	sid;
	bool		conn_state;
	Thread::Mutex	locker_state;
	enum { BACKOFF_INIT = 2, BACKOFF_DEADLINE = 256 };
	size_t		backoff;
	void Reconnect();
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	void OnAbortSession(const SockAddr &sa);
	void OnCheckAddress(SockAddr &) const;
	bool game_sync_ok;
public:
	static GameGateClient *GetInstance() { return &instance; }
	std::string Identification() const { return "GameGateClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GameGateClient() : accumulate_limit(0), conn_state(false), locker_state("GameGateClient::locker_state"), backoff(BACKOFF_INIT),game_sync_ok(false) { }
	bool IsSync(){return game_sync_ok;}
	bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(const Protocol *protocol) { return conn_state && Send(sid, protocol); }
};

};
#endif
