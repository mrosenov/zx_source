#ifndef __ONLINE_GAME_GS_COOLDOWN_H__
#define __ONLINE_GAME_GS_COOLDOWN_H__

#include <sys/time.h>
#include "staticmap.h"
namespace GNET
{

class cd_manager
{
	struct cd_ent
	{
		timeval t;
		int	interval;
		cd_ent(){}
		cd_ent(const timeval & t1, int i):t(t1),interval(i){}
	};
	typedef static_multimap<int,cd_ent>  COOLDOWN_LIST;
	COOLDOWN_LIST _list;
public:
	cd_manager()
	{
		_list.reserve(5);
	}

	void Swap(cd_manager & cd)
	{
		_list.swap(cd._list);
	}

	bool TestCoolDown(int id)
	{
		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it != _list.end())
		{
			timeval tv;
			gettimeofday(&tv, NULL);
			timeval & ct = it->second.t;
			if(ct.tv_sec > tv.tv_sec) 
				return false;
			else if(ct.tv_sec == tv.tv_sec && ct.tv_usec > tv.tv_usec) 
				return false;
			_list.erase(it);
		}
		return true;
	}


	bool SetCoolDown(int id, int cooldown_msec)
	{
		if(cooldown_msec <= 0) return true;
		//计算冷却的到期时间
		timeval tv;
		gettimeofday(&tv,NULL);
		int sec = cooldown_msec / 1000;
		int usec = (cooldown_msec % 1000)*1000;
		tv.tv_sec += sec;
		if((tv.tv_usec += usec) > 1000000)
		{
			tv.tv_sec += 1;
			tv.tv_usec -= 1000000;
		}

		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it == _list.end())
		{
			_list[id] = cd_ent(tv,cooldown_msec);
		}
		else
		{
			it->second.t = tv;
			it->second.interval = cooldown_msec;
		}
		return true;
	}

	bool ClrCoolDown(int id)
	{
		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it == _list.end()) return false;

		_list.erase(it);
		return true;
	}
	void GetCurCoolDown(vector<int> & vec)
	{
		vec.reserve(_list.size());
		COOLDOWN_LIST::iterator it = _list.begin();
		while(it != _list.end())
		{
			vec.push_back(it->first);
			++it;
		}
	}

	void ClearExpiredCoolDown()
	{
		timeval tv;
		gettimeofday(&tv,NULL);
		COOLDOWN_LIST::iterator it = _list.end();
		while(it != _list.begin())
		{
			--it;
			timeval & ct = it->second.t;
			if(ct.tv_sec > tv.tv_sec)  
				continue;
			else if(ct.tv_sec == tv.tv_sec && ct.tv_usec > tv.tv_usec) 
				continue;
			_list.erase(it);
		}
	}
};

} // namespace GNET
#endif

