#ifndef __FAMILYCACHE_H
#define __FAMILYCACHE_H

#include <map>

#include "familyid"

namespace GNET
{

#define INVALID_FACTION 0xFFFFFFFF
class FamilyCache
{
	struct CacheItem
	{       
		unsigned int factionid;
	        int jointime;
		CacheItem() : factionid(0), jointime(0){} 
		CacheItem(unsigned int fid, int time) : factionid(fid), jointime(time){} 
		CacheItem(const CacheItem &r) : factionid(r.factionid), jointime(r.jointime) {}
	};
	typedef std::map<unsigned int,CacheItem> CACHEMAP;   
	typedef std::multimap<unsigned int,unsigned int> FAMILYLISTMAP;  // 列出每个帮派相关联的所有家族ID
	static CACHEMAP        cache;
	static FAMILYLISTMAP   familylist;
	static Thread::Mutex   locker;
public:
	static void InsertFaction(unsigned int familyid, unsigned int factionid);
	static void InsertFamilies(unsigned int factionid, std::vector<FamilyId>& vec);
	static void InsertFamily(unsigned int familyid, unsigned int factionid, int jointime);
	// Warning: NEVER call GetFaction within a transaction
	static bool GetFaction(unsigned int familyid, unsigned int& factionid, int& jointime);
};

};
#endif
