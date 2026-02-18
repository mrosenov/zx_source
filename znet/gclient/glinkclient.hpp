#ifndef __GNET_GLINKCLIENT_HPP
#define __GNET_GLINKCLIENT_HPP

#include "protocol.h"
#include "thread.h"

namespace GNET
{

class GLinkClient : public Protocol::Manager
{
	static GLinkClient instance;
	size_t		accumulate_limit;
	Session::ID	sid;
	bool		conn_state;
	Thread::Mutex	locker_state;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	void OnAbortSession(const SockAddr &sa);
	void OnCheckAddress(SockAddr &) const;
public:
	static GLinkClient *GetInstance() { return &instance; }
	std::string Identification() const { return "GLinkClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GLinkClient() : accumulate_limit(0), conn_state(false), locker_state("GLinkClient::locker_state") { printf("bbbbbbbbbbbb\n"); }

	bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(const Protocol *protocol) { return conn_state && Send(sid, protocol); }
};

};
#endif
