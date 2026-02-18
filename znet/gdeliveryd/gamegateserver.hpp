#ifndef __GNET_GAMEGATESERVER_HPP
#define __GNET_GAMEGATESERVER_HPP

#include "protocol.h"

namespace GNET
{

class GameGateServer : public Protocol::Manager
{
	static GameGateServer instance;
	static bool gtopen;
	size_t		accumulate_limit;
	typedef std::set<Session::ID> GateSidSet;
	GateSidSet gate_sid_set;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
public:
	static GameGateServer *GetInstance() { return &instance; }
	std::string Identification() const { return "GameGateServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GameGateServer() : accumulate_limit(0) { }
	static void OpenGT(){gtopen=true;}
	bool GTSend(const Protocol *protocol){
		if (gtopen==false || gate_sid_set.empty()) 
			return false;
		return this->Send(*gate_sid_set.begin(),protocol);
	}
	bool GTSend(const Protocol &protocol){
		return GTSend(&protocol);
	}
	bool GTSend(Protocol *protocol){
		if (gtopen==false || gate_sid_set.empty()) 
			return false;
		return this->Send(*gate_sid_set.begin(),protocol);
	}
	bool GTSend( Protocol &protocol){
		return GTSend(&protocol);
	}
	bool GateSend(const Protocol *protocol){
		if (gate_sid_set.empty()) 
			return false;
		return this->Send(*gate_sid_set.begin(),protocol);
	}
	bool GateSend(Protocol *protocol){
		if (gate_sid_set.empty()) 
			return false;
		return this->Send(*gate_sid_set.begin(),protocol);
	}
};

};
#endif
