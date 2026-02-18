#ifndef __GNET_MAPREMAINTIME_H
#define __GNET_MAPREMAINTIME_H

#include <map>

#include "thread.h"

namespace GNET
{

class RemainTime
{
public:	
	struct play_time_t {
		int remain_time;
		int free_time_left;
		int free_time_end;
		play_time_t(int r=0,int left=0,int end=0) : remain_time(r),free_time_left(left),free_time_end(end) { }
	};
private:		
	typedef std::map<int/*userid*/,play_time_t> Map;
	Map map;
	Thread::Mutex locker;
	static RemainTime	instance;
public:
	RemainTime() : locker("RemainTime::locker") { }
	~RemainTime() { }
	static RemainTime & GetInstance() { return instance; }
	size_t Size() { Thread::Mutex::Scoped l(locker);	return map.size();	}
	bool IsUserInFreeState( int userid )
	{
		Thread::Mutex::Scoped l(locker);
		Map::iterator it = map.find(userid);
        if (it!=map.end())
			return (*it).second.free_time_left>0;
		return false;	
	}
	void UpdateRemainTime( int userid, int update_interval )
	{
		Thread::Mutex::Scoped l(locker);
		Map::iterator it = map.find(userid);
		if (it!=map.end())
		{
			(*it).second.remain_time -= update_interval;
			if ( (*it).second.remain_time<0 )
				(*it).second.remain_time=0;
		}
	}
	void UpdateFreeTime( int userid,int update_interval )
	{
		Thread::Mutex::Scoped l(locker);
		Map::iterator it=map.find(userid);
		if ( it!=map.end() )
		{
			(*it).second.free_time_left -= update_interval;
			if ( (*it).second.free_time_left<0 )
				(*it).second.free_time_left=0;
		}
	}
	void SetPlayTime( int userid, const play_time_t& playtime )
	{
		Thread::Mutex::Scoped l(locker);
		map[userid] = playtime;
	}
	play_time_t GetPlayTime( int userid )
	{
		return play_time_t(0, 0, -1); // means can not find user
	}
	bool IsValidTime( const play_time_t& time )
	{
		return !(time.remain_time==-3 && time.free_time_left==-2 && time.free_time_end==-1);
	}
};

};

#endif

