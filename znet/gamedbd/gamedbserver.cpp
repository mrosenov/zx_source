
#include "gamedbserver.hpp"
#include "state.hxx"

namespace GNET
{

GameDBServer GameDBServer::instance;

const Protocol::Manager::Session::State* GameDBServer::GetInitState() const
{
	return &state_GameDBServer;
}

void GameDBServer::OnAddSession(Session::ID sid)
{
	Log::log( LOG_INFO, "GameDBServer::OnAddSession, sid=%d.\n", sid );
}

void GameDBServer::OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer)
{
	Log::formatlog("addsession","sid=%d:ip=%s", sid, inet_ntoa(((const struct sockaddr_in*)peer)->sin_addr));
	SetSessionIP(sid,((const struct sockaddr_in*)peer)->sin_addr.s_addr);
}


void GameDBServer::OnDelSession(Session::ID sid)
{
	Log::log( LOG_INFO, "GameDBServer::OnDelSession, sid=%d.\n", sid );
	if ( sid==delivery_sid )
	{
		delivery_sid=0;
		Log::log( LOG_ERR, "GameDBServer::disconnect from delivery, sid=%d.\n", sid );
	}
}

};
