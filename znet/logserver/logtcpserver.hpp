#ifndef __GNET_LOGTCPSERVER_HPP
#define __GNET_LOGTCPSERVER_HPP

#include "protocol.h"

namespace GNET
{

class LogTcpServer : public Protocol::Manager, public Timer::Observer
{
	static LogTcpServer instance;
	size_t		accumulate_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
public:
	static LogTcpServer *GetInstance() { return &instance; }
	std::string Identification() const { return "LogTcpServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	LogTcpServer() : accumulate_limit(0) { Timer::Attach(this); }
        void Update( ) { CheckSessionTimePolicy(); }
};

};
#endif
