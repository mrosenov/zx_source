
#include "logtcpserver.hpp"
#include "state.hxx"

namespace GNET
{

LogTcpServer LogTcpServer::instance;

const Protocol::Manager::Session::State* LogTcpServer::GetInitState() const
{
	return &state_LoggerNull;
}

void LogTcpServer::OnAddSession(Session::ID sid)
{
	//TODO
}

void LogTcpServer::OnDelSession(Session::ID sid)
{
	//TODO
}

};
