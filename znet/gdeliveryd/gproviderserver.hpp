#ifndef __GNET_GPROVIDERSERVER_HPP
#define __GNET_GPROVIDERSERVER_HPP

#include "protocol.h"
#include "macros.h"

#include "serverload.h"
#include "lineplayerlimit"

namespace GNET
{

class GProviderServer : public Protocol::Manager
{
public:	
	struct gameserver_t
	{
		unsigned int sid;
		int max_num;
		unsigned int attr;
	};
private:
	static GProviderServer instance;
	size_t		accumulate_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	void NotifyGShopScheme(Session::ID sid);
	
	int m_provider_server_id;
	int m_serialno;
public:
	static GProviderServer *GetInstance() { return &instance; }
	int SetLimits(const LinePlayerLimitVector &limits);
	LinePlayerLimitVector GetLimits();
	std::string Identification() const { return "GProviderServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GProviderServer() : 
		accumulate_limit(0),
		m_provider_server_id(_PROVIDER_ID_INVALID),
		locker_gameservermap("GProviderServer::locker_gameservermap")
   	{ 
		m_serialno = time(NULL);
	}
	Thread::RWLock locker_gameservermap;
	typedef std::map<int,gameserver_t> GameServerMap;
	GameServerMap gameservermap;
	LocalServerLoad cache_server_load;//load in each line of this deliver

	const LinePlayerLimitVector& GetLineLimits();
	void SetCachedLimits();
	int SelectLine(int & line) const{return cache_server_load.SelectLine(line);}
	
	int  SetLineLimit(int gsid,int limit){
		GameServerMap::iterator it = gameservermap.find(gsid);
		if(it!= gameservermap.end()){
			it->second.max_num = limit;
			return 0;
		}
		return -1;
	}
	void SetProviderServerID(int ps_id) { m_provider_server_id=ps_id; } 
	int  GetProviderServerID() { return m_provider_server_id; } 
	int  GetSerialno() { return m_serialno; }
	int  GetGSID(unsigned int sid)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator ite,it=gameservermap.begin();
		for (ite=gameservermap.end();it!=ite;++it)
		{
			if(it->second.sid == sid)
				return it->first;
		}
		return -1;
	}
	bool IsNormalLine(int game_id)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it=gameservermap.find(game_id);
		if (it==gameservermap.end()) return false;
		return !(it->second.attr&GS_ATTR_HIDE);
	}
	bool DispatchProtocol(int game_id,const Protocol *protocol)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it=gameservermap.find(game_id);
		if (it==gameservermap.end()) return false;
		return this->Send((*it).second.sid,protocol);
	}
	bool DispatchProtocol(int game_id,const Protocol &protocol)
	{
		return DispatchProtocol(game_id,&protocol);
	}
	bool DispatchProtocol(int game_id,		Protocol *protocol)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it=gameservermap.find(game_id);
		if (it==gameservermap.end()) return false;
		return this->Send((*it).second.sid,protocol);
	}
	bool DispatchProtocol(int game_id,		Protocol &protocol)
	{
		return DispatchProtocol(game_id,&protocol);
	}

	void BroadcastProtocol(const Protocol *protocol)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it=gameservermap.begin();
		for (;it!=gameservermap.end();it++)
			this->Send((*it).second.sid,protocol);
	}
	void BroadcastProtocol(const Protocol &protocol)
	{
		return BroadcastProtocol(&protocol);
	}
	void BroadcastProtocol(		Protocol *protocol)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it=gameservermap.begin();
		for (;it!=gameservermap.end();it++)
			this->Send((*it).second.sid,protocol);
	}
	void BroadcastProtocol(		Protocol &protocol)
	{
		return BroadcastProtocol(&protocol);
	}

	//find gameserver by session ID
	int FindGameServer(Session::ID sid)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it;
		for (it=gameservermap.begin();it!=gameservermap.end();it++)
		{
			if ((*it).second.sid == sid) return (*it).first;
		}
		return _GAMESERVER_ID_INVALID;
	}   
};
class LinePlayerTimer : public Thread::Runnable
{
	int update_time;
public:
	LinePlayerTimer(int _time,int _proir=1) : Runnable(_proir),update_time(_time) { }
        static LinePlayerTimer* GetInstance(int _time,int _proir=1) {
                static LinePlayerTimer instance(_time,_proir);
                return &instance;
        }
	void Run();
};

};
#endif
