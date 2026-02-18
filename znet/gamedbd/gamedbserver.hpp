#ifndef __GNET_GAMEDBSERVER_HPP
#define __GNET_GAMEDBSERVER_HPP

#include "protocol.h"

namespace GNET
{

class GameDBServer : public Protocol::Manager
{
	static GameDBServer instance;
	size_t		accumulate_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	Thread::Mutex	lockerip;
	std::map<Session::ID, int> ipmap;
	bool is_central_db;
	bool activate_kill;
public:
	static GameDBServer *GetInstance() { return &instance; }
	std::string Identification() const { return "GameDBServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GameDBServer() : accumulate_limit(0), is_central_db(false), activate_kill(false)
       	{ delivery_sid=0; zoneid=0;aid=0;delivery_zoneid=-1;}
	bool Send2Delivery(const Protocol* p) {
		if ( !delivery_sid ) return false;
		return Send(delivery_sid,p);
	}
	bool Send2Delivery(      Protocol* p) {
		if ( !delivery_sid ) return false;
		return Send(delivery_sid,p);
	}
	bool Send2Delivery(const Protocol& p) {
		return Send2Delivery(&p);
	}
	bool Send2Delivery(      Protocol& p) {
		return Send2Delivery(&p);
	}
	int GetSessionIP(Session::ID sid)
	{
		Thread::Mutex::Scoped l(lockerip);
		return ipmap[sid];
	}
        int SetSessionIP(Session::ID sid, int ip)
	{       
		Thread::Mutex::Scoped l(lockerip);
		ipmap[sid] = ip;
		return ipmap[sid];
	}
	void OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer);
	unsigned int delivery_sid;
	int delivery_zoneid;
	int zoneid;
	char aid;
	bool IsCentralDB() { return is_central_db; }
	void SetCentralDB(bool b) { is_central_db = b; }
	void SetActivateKill() { activate_kill = true; }
	bool IsActivateKill() const { return activate_kill; }
};

};
#endif
