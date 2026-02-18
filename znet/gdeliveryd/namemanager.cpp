#include "namemanager.h"
#include "dbrolenamelist.hrp"
#include "mapuser.h"

namespace GNET
{
bool NameManager::AddName(int id, const Octets & name)
{
	if (!m_blInit_)
		return false;
	name_map[id] = name;
	return true;
}

bool NameManager::DelName(int id)
{
	if (!m_blInit_)
		return false;
	name_map.erase(id);	
	return true;
}

bool NameManager::FindName(int id, Octets &name)
{
	if (!m_blInit_)
		return false;
	NameMap::iterator it = name_map.find(id);
	if (it == name_map.end())
		return false;
	name = it->second;
	return true;	
}

void NameManager::OnDBConnect(Protocol::Manager *manager, int sid)
{               
	manager->Send(sid, Rpc::Call(RPC_DBROLENAMELIST, DBRoleNameListArg(1)));
}

void NameManager::Init(const ID2NameVector& list, bool blFinish)
{
	if (m_blInit_)
	{
		ResetMap();
		m_blInit_=false;
	}
	ID2NameVector::const_iterator it = list.begin(), iv = list.end();
	for(; it!=iv; ++it)
		name_map[it->id] = it->name;
	if (blFinish) 
	{ 
		Log::formatlog("namemanager","init: total=%d", name_map.size()); 
		m_blInit_=true; 
	}
}

bool NameManager::IsInit()
{
	return m_blInit_;
}

bool NameManager::GetNameHistory(const Octets & name, std::vector<NameHistory> & list)
{
	if (query_cache.find(name, list))
	{
		bool dirty = false;
		std::vector<NameHistory>::iterator it, ite = list.end();
		for (it = list.begin(); it != ite; ++it)
		{
			GRoleInfo * info = RoleInfoCache::Instance().Get(it->roleid);
			if (info)
			{
				if (it->occupation != info->occupation || it->level != info->level)
				{
					it->occupation = info->occupation;
					it->level = info->level;
					it->reborn_cnt = info->reborn_cnt;
					dirty = true;
				}
			}
		}
		if (dirty)
			query_cache.put(name, list);
		return true;
	}
	else
		return false;
	
	/*
	CacheItem item;
	if (query_cache.find(name, item) && (Timer::GetTime() - item.first) < CACHE_TIME)
	{
		list = item.second;
		return true;
	}
	else
		return false;
		*/
}

void NameManager::CacheNameHistory(const Octets & name, const std::vector<NameHistory> & list)
{
	/*
	time_t uptime = Timer::GetTime();
	query_cache.put(name, std::make_pair(uptime, list));
	*/
	query_cache.put(name, list);
}

void NameManager::ClearNameHistoryCache(const Octets & name)
{
	query_cache.del(name);
}

//void Namemanager::OnLevelUp(int roleid, const Octets & rolename, int level)

}
