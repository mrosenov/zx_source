#ifndef __GNET_GPROVIDERSERVER_HPP
#define __GNET_GPROVIDERSERVER_HPP

#include "protocol.h"
#include "macros.h"
#include "gaterolecache.h"

namespace GNET
{
class GProviderServer : public Protocol::Manager
{
	static GProviderServer instance;
	size_t		accumulate_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);

	int m_provider_server_id;
public:
	static GProviderServer *GetInstance() { return &instance; }
	std::string Identification() const { return "GProviderServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GProviderServer() : accumulate_limit(0), m_provider_server_id(_PROVIDER_ID_INVALID) {}

	typedef std::map<int, unsigned int/*sid*/> GameServerMap;
	GameServerMap gameservermap;

	void SetProviderServerID(int ps_id) { m_provider_server_id=ps_id; } 
	int GetProviderServerID() { return m_provider_server_id; } 

	bool DispatchProtocol(int game_id,const Protocol *protocol)
	{
		GameServerMap::const_iterator it=gameservermap.find(game_id);
		if (it==gameservermap.end()) return false;
		return this->Send((*it).second,protocol);
	}
	bool DispatchProtocol(int game_id,const Protocol &protocol)
	{
		return DispatchProtocol(game_id,&protocol);
	}
	bool DispatchProtocol(int game_id,		Protocol *protocol)
	{
		GameServerMap::const_iterator it=gameservermap.find(game_id);
		if (it==gameservermap.end()) return false;
		return this->Send((*it).second,protocol);
	}
	bool DispatchProtocol(int game_id,		Protocol &protocol)
	{
		return DispatchProtocol(game_id,&protocol);
	}

	void BroadcastProtocol(const Protocol *protocol)
	{
		GameServerMap::const_iterator it=gameservermap.begin();
		for (;it!=gameservermap.end();it++)
			this->Send((*it).second,protocol);
	}
	void BroadcastProtocol(const Protocol &protocol)
	{
		return BroadcastProtocol(&protocol);
	}
	void BroadcastProtocol(		Protocol *protocol)
	{
		GameServerMap::const_iterator it=gameservermap.begin();
		for (;it!=gameservermap.end();it++)
			this->Send((*it).second,protocol);
	}
	void BroadcastProtocol(		Protocol &protocol)
	{
		return BroadcastProtocol(&protocol);
	}
	int  GetGSID(unsigned int sid)
	{
		GameServerMap::const_iterator ite,it=gameservermap.begin();
		for (ite=gameservermap.end();it!=ite;++it)
		{
			if(it->second == sid)
				return it->first;
		}
		return _GAMESERVER_ID_INVALID;
	}
};

};
#endif
