#ifndef __GNET_CENTRALDELIVERYCLIENT_HPP
#define __GNET_CENTRALDELIVERYCLIENT_HPP

#include "protocol.h"
#include "thread.h"
#include "serverload.h"

namespace GNET
{
class CentralDeliveryClient : public Protocol::Manager, public Timer::Observer
{
	enum
	{
		SESSION_CHECK_INTERVAL = 15,
	};
	static CentralDeliveryClient instance;
	size_t		accumulate_limit;
	Session::ID	sid;
	bool		conn_state;
	Thread::Mutex	locker_state;
	enum { BACKOFF_INIT = 15, BACKOFF_DEADLINE = 256 };
	size_t		backoff;
	void Reconnect();
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	void OnAbortSession(const SockAddr &sa);
	void OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer);
	void OnCheckAddress(SockAddr &) const;

	int server_zoneid;
	SockAddr server_addr;
	ServerLoad cache_server_load;
public:
	static CentralDeliveryClient *GetInstance() { return &instance; }
	std::string Identification() const { return "CentralDeliveryClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	CentralDeliveryClient() : accumulate_limit(0), conn_state(false), locker_state("CentralDeliveryClient::locker_state"), backoff(BACKOFF_INIT), server_zoneid(0){ }

	bool IsConnect() { Thread::Mutex::Scoped l(locker_state); return conn_state; }
	bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(const Protocol *protocol) { return conn_state && Send(sid, protocol); }
	void SetServerZoneid(int zoneid)
	{
		server_zoneid = zoneid;
	}

	int GetServerZoneid()
	{
		return server_zoneid;
	}

	void SetLoad(int srv_limit, int srv_count, const LinePlayerLimitVector & lines)
	{
		cache_server_load.SetLoad(srv_limit, srv_count, lines);
	}
	int CheckLimit(int line)
	{
		return cache_server_load.CheckLimit(line);
	}
	int SelectLine(int & line) const
	{
		return cache_server_load.SelectLine(line);
	}
	void Update()
	{
		static int checktimer = SESSION_CHECK_INTERVAL;
		if (checktimer--) return;
		CheckSessionTimePolicy();
		checktimer = SESSION_CHECK_INTERVAL;
	}
	void CloseAll()
	{
		Close(sid);
	}
/*
	int GetServerAddr(int & zoneid, int & ip, int & port) 
	{
		zoneid = server_zoneid;
		ip = ((struct sockaddr_in*)server_addr)->sin_addr.s_addr;
		port = ((struct sockaddr_in*)server_addr)->sin_port;
	}
*/
};
};
#endif
