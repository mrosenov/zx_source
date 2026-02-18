#ifndef __GNET_GPROVIDERSERVER_HPP
#define __GNET_GPROVIDERSERVER_HPP

#include "protocol.h"
#include "macros.h"
#include "thread.h"

#include "lineplayerlimit"
#include "linelist.hpp"
#include "localmacro.h"

namespace GNET
{
class DisconnectTimeoutTask : public Thread::Runnable
{
	int gid_discon;  //gameserver id that is disconnect from link
public:
	DisconnectTimeoutTask(int _gid) : Runnable(1),gid_discon(_gid) { }
	~DisconnectTimeoutTask() { }
	void Run();
};




class GProviderServer : public Protocol::Manager
{
	static GProviderServer instance;
	size_t	accumulate_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);

	int m_provider_server_id;

	string section_num;
public:
	static GProviderServer *GetInstance() { return &instance; }
	void SetSectionNum(const char* sn) { section_num=sn; }
	std::string Identification() const { return string("GProviderServer")+section_num; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GProviderServer() : 
		accumulate_limit(0),
		m_provider_server_id(_PROVIDER_ID_INVALID),
		locker_gameservermap("GProviderServer::locker_gameservermap")
   	{ }

	void SetLineLimits(const LinePlayerLimitVector &limits)
	{
		Thread::RWLock::WRScoped l(locker_gameservermap);
		linelist_cache.linelist.clear();
		for(LinePlayerLimitVector::const_iterator it = limits.begin(), ie = limits.end(); it != ie; ++it)
		{
			GameServerMap::const_iterator it2 = gameservermap.find((*it).line_id);
			if( it2 != gameservermap.end() )
			{
								const LinePlayerLimit &limit = (*it);
				LineInfo lineinfo = it2->second.info;
				lineinfo.id = limit.line_id;
				
				if (it2->second.IsHide())
				{
					lineinfo.ishide = true;
				}
				else
				{
					lineinfo.ishide = false;
				}

				Octets tos = lineinfo.attribute;
				lineinfo.attribute.resize(4);
				int iLoad=(limit.cur_num>=limit.max_num)?100:((int)(limit.cur_num*100/(float)(limit.max_num)));
				*(int*)(lineinfo.attribute.begin()) = iLoad;
				lineinfo.attribute.insert(lineinfo.attribute.end(), tos.begin(), tos.end());
				linelist_cache.linelist.push_back(lineinfo);
			}
		}
		DEBUG_PRINT("glinkd: received player limit of each line from delivery, limits=%d, gameservermap=%d, size %d\n", limits.size(), gameservermap.size(), linelist_cache.linelist.size());
	}
	int CheckLineLimit(int lineid, char flag = 0);

	void ClearLineLimits()
	{
		Thread::RWLock::WRScoped l(locker_gameservermap);
		linelist_cache.linelist.clear();
	}

	bool GetLineList(LineList &linelist);

	struct LineInfoEx 
	{
/*
		enum
		{
			GS_NORMAL,  // 普通gs
			GS_BATTLE,  // 城战gs
		};	
*/
		Session::ID sid; 
		unsigned int attr;
		LineInfo info;

		LineInfoEx(Session::ID _sid, unsigned int _attr, const LineInfo &_info)
			: sid(_sid), attr(_attr), info(_info)
			{
			}
//		bool IsHide() const { return attr == GS_BATTLE; }
		bool IsHide() const { return (attr & GS_ATTR_HIDE); }
	};

	Thread::RWLock locker_gameservermap;
	typedef std::map<int, LineInfoEx > GameServerMap;
	GameServerMap gameservermap;
	LineList linelist_cache;
	void SetProviderServerID(int ps_id) { m_provider_server_id=ps_id; } 
	static int GetProviderServerID() { return instance.m_provider_server_id; } 

	bool DispatchProtocol(int game_id,const Protocol *protocol)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it=gameservermap.find(game_id);
		if (it==gameservermap.end()) return false;
		return this->Send(it->second.sid, protocol);
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
		return this->Send(it->second.sid, protocol);
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
			this->Send(it->second.sid, protocol);
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
			this->Send(it->second.sid, protocol);
	}
	void BroadcastProtocol(		Protocol &protocol)
	{
		return BroadcastProtocol(&protocol);
	}

	int FindGameServer(Session::ID sid)
	{
		Thread::RWLock::RDScoped l(locker_gameservermap);
		GameServerMap::const_iterator it;
		for (it=gameservermap.begin();it!=gameservermap.end();it++)
		{
			if (it->second.sid == sid) return it->first;
		}
		return _GAMESERVER_ID_INVALID;
	}	
};

};
#endif
