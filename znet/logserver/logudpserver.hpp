#ifndef __GNET_LOGUDPSERVER_HPP
#define __GNET_LOGUDPSERVER_HPP

#include "protocol.h"

namespace GNET
{

class LogUdpServer : public Protocol::Manager
{
	static LogUdpServer instance;
	size_t		accumulate_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
public:
	static LogUdpServer *GetInstance() { return &instance; }
	std::string Identification() const { return "LogUdpServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	LogUdpServer() : accumulate_limit(0) { }
	
};

};
#endif
