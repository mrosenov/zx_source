#ifndef __GNET_SNSPLATFORMCLIENT_HPP
#define __GNET_SNSPLATFORMCLIENT_HPP

#include "protocol.h"
#include "thread.h"

namespace GNET
{

class SNSPlatformClient : public Protocol::Manager
{
	static SNSPlatformClient instance;
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
public:
	int zoneid;
	int aid;

	static SNSPlatformClient *GetInstance() { return &instance; }
	std::string Identification() const { return "SNSPlatformClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	SNSPlatformClient() : accumulate_limit(0), conn_state(false), locker_state("SNSPlatformClient::locker_state"), backoff(BACKOFF_INIT) { }

	bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(const Protocol *protocol) { return conn_state && Send(sid, protocol); }
};

};
#endif
