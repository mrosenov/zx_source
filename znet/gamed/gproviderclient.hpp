#ifndef __GNET_GPROVIDERCLIENT_HPP
#define __GNET_GPROVIDERCLIENT_HPP

#include "protocol.h"
#include "thread.h"
#include "log.h"
#include "keepalive.hpp"

namespace GNET
{

class GProviderClient : public Protocol::Manager
{
	enum { BACKOFF_INIT = 2, BACKOFF_DEADLINE = 256 };
	enum { MAX_PROVIDERID = 256 };

	typedef std::vector<GProviderClient*> ClientArray;

	static ClientArray g_clientarray;
	static GProviderClient* g_clients[MAX_PROVIDERID];
	static int g_lineid;	
	static std::string g_name;
	static std::string g_edition;
	static int g_pksetting;
	
	Session::ID	sid;
	size_t		accumulate_limit;
	bool		conn_state;
	Thread::Mutex	locker_state;
	size_t		backoff;
	int	m_providerid;
	std::string m_identification;
	int 		server_attr;

	//用来检查gs和delivery的连接是否正常
	int		sender_counter;		//发送端心跳计时器
	int		receive_counter;	//接送端心跳计时器

	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	void OnAbortSession(Session::ID sid);
	void OnCheckAddress(SockAddr &) const;
	void Reconnect();
public:
	std::string Identification() const 
	{
		return m_identification;
	}
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GProviderClient() : accumulate_limit(0), conn_state(false), backoff(BACKOFF_INIT), server_attr(0), sender_counter(0), receive_counter(0) { }
	GProviderClient(const char* id) : 
		accumulate_limit(0),conn_state(false),backoff(BACKOFF_INIT), m_identification(id), server_attr(0), sender_counter(0), receive_counter(0) { }
	
	bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, protocol); }
	int  GetProviderServerID() { return m_providerid; }
	static bool Connect(int lineid,const char* name,const char* edition,int server_attr = 0, int pksetting=0);
	static bool Attach(int providerid,GProviderClient* manager);
	static bool DispatchProtocol(int providerid,const Protocol &protocol);
	static bool DispatchProtocol(int providerid,const Protocol *protocol);
	static void BroadcastProtocol(const Protocol &protocol);

	bool IsConnState() { return conn_state;}
	void SetConnState(bool val){conn_state = val;}
	int GetSenderCounter() { return ++sender_counter;}
	int GetReceiveCounter(){return receive_counter;}
	void SetReceiveCounter(int counter) { receive_counter = counter;} 
	void OnCheckConnect();

};
class KeepAliveTask : public Thread::Runnable
{
	unsigned int delay;
public:
	KeepAliveTask(unsigned int _delay,unsigned int priority=1) : Runnable(priority),delay(_delay) { } 
	~KeepAliveTask() { }
	void Run()
	{
		GProviderClient::BroadcastProtocol(KeepAlive((unsigned int)PROTOCOL_KEEPALIVE));
		Thread::HouseKeeper::AddTimerTask(this,delay);
		PollIO::WakeUp();
	}	
};


};
#endif
