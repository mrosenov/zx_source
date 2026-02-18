#include "familycache.h"
#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbbuffer.h"
#include "gfolk"
#include "gfamilyskill"
#include "familyid"
#include "hostilefaction"
#include "hostileinfo"
#include "gfactioninfo"
#include "gfamily"

namespace GNET
{
FamilyCache::CACHEMAP        FamilyCache::cache;
FamilyCache::FAMILYLISTMAP   FamilyCache::familylist;
Thread::Mutex   FamilyCache::locker;

void FamilyCache::InsertFamilies(unsigned int factionid, std::vector<FamilyId>& vec)
{
	Thread::Mutex::Scoped lock(locker);
	familylist.erase(factionid);
	for(std::vector<FamilyId>::iterator it = vec.begin(), ie=vec.end(); it!=ie;++it)
		familylist.insert(std::make_pair(factionid, it->fid));

}
void FamilyCache::InsertFamily(unsigned int familyid, unsigned int factionid, int jointime)
{
	Thread::Mutex::Scoped lock(locker);
	cache[familyid] = CacheItem(factionid, jointime);
}
void FamilyCache::InsertFaction(unsigned int familyid, unsigned int factionid)
{
	Thread::Mutex::Scoped lock(locker);
	cache[familyid] = CacheItem(factionid, Timer::GetTime());
	if(factionid)
		familylist.insert(std::make_pair(factionid, familyid));
}
bool FamilyCache::GetFaction(unsigned int familyid, unsigned int& factionid, int& jointime)
{
	factionid = 0;
	jointime  = 0;
	if(!familyid)
		return false;
	{
		Thread::Mutex::Scoped lock(locker);
		CACHEMAP::iterator it = cache.find(familyid);
		if(it!=cache.end())
		{
			factionid = it->second.factionid;
			jointime = it->second.jointime;
			return true;
		}
	}
	try
	{
		StorageEnv::Storage *pfamily = StorageEnv::GetStorage("family");
		StorageEnv::Storage *pfaction = StorageEnv::GetStorage("faction");
		StorageEnv::CommonTransaction txn;
		GFamily family;
		try{
			Marshal::OctetsStream key_family;
			Marshal::OctetsStream value,vfaction;
			key_family << familyid;
			if(pfamily->find(key_family, value, txn))
			{
				value >> family;
				if(!family.factionid)
				{
					FamilyCache::InsertFamily(familyid, 0, 0);
					return true;
				}
				bool found = true;
				{
					Thread::Mutex::Scoped lock(locker);
					FAMILYLISTMAP::iterator it = familylist.find(factionid);
					if(it==familylist.end())
						found = false;
				}

				if(!found)
				{
					if(!pfaction->find(Marshal::OctetsStream()<<family.factionid, vfaction, txn))
					{
						Log::log(LOG_ERR, "familycache, familyid=%d, faction %d does not exist.", 
								familyid, family.factionid);
						family.factionid = 0;
						pfamily->insert(key_family, Marshal::OctetsStream()<<family, txn);
						FamilyCache::InsertFamily(familyid, 0, 0);
						return false;
					}
					else
					{
						GFactionInfo info;
						vfaction >> info;
						FamilyCache::InsertFamilies(family.factionid, info.member);
					}
				}

				{
					Thread::Mutex::Scoped lock(locker);
					FAMILYLISTMAP::iterator ie = familylist.upper_bound(family.factionid);
					FAMILYLISTMAP::iterator it = familylist.lower_bound(family.factionid);

					found = true;
					for(;it!=ie&&it->second!=familyid;++it);
					if(it==ie)
					{
						found = false;
						cache[familyid] = CacheItem(0, 0);
					}
					else
					{
						factionid = family.factionid;
						cache[familyid] = CacheItem(factionid, family.jointime);
					}
				}
				if(!found)
				{
					Log::log(LOG_ERR, "familycache, family=%d does not belongs to faction=%d",
							familyid, family.factionid);
					family.factionid = 0;
					family.jointime  = 0;
					pfamily->insert(key_family, Marshal::OctetsStream()<<family, txn);
				}
				return true;
			}
			FamilyCache::InsertFamily(familyid, 0, 0);
			return true;

		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException e( DB_OLD_VERSION );
			txn.abort( e );
			throw e;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "FamilyCache, familyid=%d what=%s.", familyid,e.what());
	}
	return false;
}
};
