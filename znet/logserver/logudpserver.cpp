
#include "logudpserver.hpp"
#include "state.hxx"

namespace GNET
{

LogUdpServer LogUdpServer::instance;

const Protocol::Manager::Session::State* LogUdpServer::GetInitState() const
{
	return &state_LoggerNull;
}

void LogUdpServer::OnAddSession(Session::ID sid)
{
	//TODO
}

void LogUdpServer::OnDelSession(Session::ID sid)
{
	//TODO
}

};
