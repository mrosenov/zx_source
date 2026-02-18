#ifndef __GNET_NAME_MANAGER_H
#define __GNET_NAME_MANAGER_H

#include <map>
#include "octets.h"
#include "id2name"
#include "namehistory"
#include "simplelru.hpp"

namespace GNET
{
class NameManager
{
public:
	struct CompareOctets
	{       
		bool operator() ( const GNET::Octets &o1, const GNET::Octets &o2 ) const
		{       
			size_t s1 = o1.size();
			size_t s2 = o2.size();
			if ( int r = memcmp( o1.begin(), o2.begin(), std::min(s1, s2) ) )
				return r < 0;
			return s1 < s2;
		}
	};
	typedef std::map<int, Octets> NameMap;
//	typedef std::pair<int/*uptime*/, std::vector<NameHistory> > CacheItem;
	//typedef simplelru<Octets, CacheItem, CompareOctets> HistoryCache;
	typedef simplelru<Octets, std::vector<NameHistory>, CompareOctets> HistoryCache;
	static NameManager * GetInstance() {static NameManager instance; return &instance; }
	bool AddName(int id, const Octets &name);
	bool DelName(int id);
	bool FindName(int id, Octets &name);
	void Init(const ID2NameVector& list, bool blFinish);
	bool IsInit();
	void OnDBConnect(Protocol::Manager *manager, int sid);
	bool GetNameHistory(const Octets & name, std::vector<NameHistory> & list);
	void CacheNameHistory(const Octets & name, const std::vector<NameHistory> & list);
	void ClearNameHistoryCache(const Octets & name);
//	void OnLevelUp(int roleid, const Octets & rolename, int level);
private:
	NameManager() : query_cache(2000){};
	void ResetMap()
	{
		name_map.clear();
	}

private:
	bool		m_blInit_;
	NameMap		name_map;
	HistoryCache	query_cache;
	/*
	enum
	{
		CACHE_TIME = 1800,
	};
	*/
};

}

#endif

