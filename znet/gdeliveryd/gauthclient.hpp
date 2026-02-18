#ifndef __GNET_GAUTHCLIENT_HPP
#define __GNET_GAUTHCLIENT_HPP

#include "protocol.h"
#include "thread.h"
//#include "timersender.h"
#include "callid.hxx"
namespace GNET
{

class GAuthClient : public Protocol::Manager
{
public:
	class ConnState
	{
		bool avail;
		std::set<unsigned int> pro_set;
		public:
		ConnState(bool _avail) : avail(_avail){ }
		ConnState(unsigned int* first, size_t size) : avail(false), pro_set(first, first+size) { }
		bool Policy(unsigned int pro)
		{
			if (GAuthClient::GetInstance()->GetVersion()!=1 && pro==PROTOCOL_GAME2AU)
			{
				Log::log(LOG_ERR, "try to send game2au, but version not support");
				return false;
			}
			return (avail) || (pro_set.find(pro) != pro_set.end());
		}
		bool Available() { return avail; }
	};
	static ConnState _TState_Null;
	static ConnState _TState_HandShaking;
	static ConnState _TState_Available;
private:
	static GAuthClient instance;
	size_t		accumulate_limit;
	Session::ID	sid;
	ConnState * conn_state;
	Thread::Mutex	locker_state;
	enum { BACKOFF_INIT = 2, BACKOFF_DEADLINE = 256 };
	size_t		backoff;
	bool need2reconnect;
	int version;

	void Reconnect();
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid) {}
	void OnDelSession(Session::ID sid, int status);
	void OnAbortSession(Session::ID sid);
	void OnCheckAddress(SockAddr &) const;
	Octets GetIPDigest(unsigned int addr);
	void OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer);
	//TimerSender timersender;

public:
	static GAuthClient *GetInstance() { return &instance; }
	std::string Identification() const { return "GAuthClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GAuthClient() : 
		accumulate_limit(0),
	   	conn_state(&_TState_Null), 
		locker_state("GAuthClient::locker_state"),
		backoff(BACKOFF_INIT),
		need2reconnect(true),
		version(0)
		//timersender(this)
	{
		blreset=true;
		local_ip = 0;
	} 

	Octets shared_key;
	Octets osec_key;
	Octets authd_cert;	
	bool blreset; //whether clear all onlineuser of this zoneid on Auth server
	int local_ip;
	
	void Need2Reconnect(bool reconnect)
	{
		need2reconnect = reconnect;
	}
	size_t GetActiveSid() { return sid; }
//	void RunTimerSender(size_t ticks=1) { timersender.Run(ticks); }
//	void AccumulateSend(Protocol *protocol) { if (conn_state) timersender.SendProtocol(sid,protocol); }
	bool SendProtocol(const Protocol &protocol) { return conn_state->Policy(protocol.GetType()) && Send(sid, protocol); }
	bool SendProtocol(const Protocol *protocol) { return conn_state->Policy(protocol->GetType()) && Send(sid, protocol); }
	bool SendProtocol(Protocol &protocol) { return conn_state->Policy(protocol.GetType()) && Send(sid, protocol); }
	bool SendProtocol(Protocol *protocol) { return conn_state->Policy(protocol->GetType()) && Send(sid, protocol); }
	void SetState(ConnState * state) { conn_state = state; }
	bool IsConnAvailable() { return conn_state->Available(); }
	void SetVersion(int v) { version = v; }
	int GetVersion() { return version; }
};
};
#endif
