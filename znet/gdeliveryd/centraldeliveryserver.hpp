#ifndef __GNET_CENTRALDELIVERYSERVER_HPP
#define __GNET_CENTRALDELIVERYSERVER_HPP

#include "protocol.h"
#include "serverload.h"

namespace GNET
{

class CentralDeliveryServer : public Protocol::Manager
{
public:
	struct delivery_t
	{
		unsigned int sid;
		ServerLoad cache_server_load;
	};
	typedef std::map<int, delivery_t> DSMap;
private:
	static CentralDeliveryServer instance;
	size_t		accumulate_limit;
	bool		isdebug;
	DSMap ds_map;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
public:
	static CentralDeliveryServer *GetInstance() { return &instance; }
	std::string Identification() const { return "CentralDeliveryServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	CentralDeliveryServer() : accumulate_limit(0), isdebug(false) { }
	bool IsDebug() { return isdebug; }
	void SetDebug(bool debug) { isdebug = debug; LOG_TRACE("CentralDeliveryServer setdebug=%d", debug); }
	bool IsConnect(int zoneid)
	{
		return (ds_map.find(zoneid) != ds_map.end());
	}
	int CheckLimit(int zoneid, int gsid)
	{
		DSMap::const_iterator it = ds_map.find(zoneid);
		if (it == ds_map.end())
			return ERR_COMMUNICATION;
		return it->second.cache_server_load.CheckLimit(gsid);
	}
	int SelectLine(int zoneid, int & gsid) const
	{
		DSMap::const_iterator it = ds_map.find(zoneid);
		if (it == ds_map.end())
			return ERR_COMMUNICATION;
		return it->second.cache_server_load.SelectLine(gsid);
	}
	void InsertZoneId(int zoneid, unsigned int sid)
	{
		ds_map[zoneid].sid = sid;
	}
	void SetLoad(int zoneid, int srv_limit, int srv_count, const LinePlayerLimitVector & lines);
	void BroadcastProtocol(const Protocol *protocol)
	{
		DSMap::const_iterator it=ds_map.begin();
		for (;it!=ds_map.end();it++)
			this->Send((*it).second.sid,protocol);
	}
	void BroadcastProtocol(const Protocol &protocol)
	{
		return BroadcastProtocol(&protocol);
	}
	void BroadcastProtocol(		Protocol *protocol)
	{
		DSMap::const_iterator it=ds_map.begin();
		for (;it!=ds_map.end();it++)
			this->Send((*it).second.sid,protocol);
	}
	void BroadcastProtocol(		Protocol &protocol)
	{
		return BroadcastProtocol(&protocol);
	}
	bool DispatchProtocol(int zoneid, const Protocol *protocol)
	{
		DSMap::const_iterator it=ds_map.find(zoneid);
		if (it==ds_map.end()) return false;
		return this->Send((*it).second.sid, protocol);
	}
	bool DispatchProtocol(int zoneid, const Protocol &protocol)
	{
		return DispatchProtocol(zoneid, &protocol);
	}
	bool DispatchProtocol(int zoneid, Protocol *protocol)
	{
		DSMap::const_iterator it=ds_map.find(zoneid);
		if (it==ds_map.end()) return false;
		return this->Send((*it).second.sid, protocol);
	}
	bool DispatchProtocol(int zoneid, Protocol &protocol)
	{
		return DispatchProtocol(zoneid, &protocol);
	}
	void CloseAll()
	{
		DSMap::const_iterator it=ds_map.begin();
		for (;it!=ds_map.end();it++)
			this->Close((*it).second.sid);
	}
	int GetZoneId(unsigned int sid)
	{
		LOG_TRACE("GetZoneId sid=%d, size=%d", sid, ds_map.size());
		DSMap::const_iterator it=ds_map.begin();
		for (;it!=ds_map.end();it++)
		{
			LOG_TRACE("GetZoneId sid=%d, second.sid=%d", sid, (*it).second.sid);
			if((*it).second.sid == sid)
			{
				return it->first;
			}
		}
		return -1;
	}
};

class LoadExchangeTask : public Thread::Runnable
{
	int interval;
public:
	LoadExchangeTask(int _interval, int _prior=1) : Runnable(_prior), interval(_interval) {}
	void Run();
};
};
#endif
