#ifndef __GNET_MAPTASKDATA_H
#define __GNET_MAPTASKDATA_H

#include <map>

#include "thread.h"

#include "gtaskdata"

namespace GNET
{

class TaskData
{
	typedef std::map<int/*task id*/,GTaskData/*task data*/> Map;
	Map map;
	Thread::Mutex locker;
	static TaskData	instance;
public:
	TaskData() : locker("TaskData::locker") { }
	~TaskData() { }
	static TaskData & GetInstance() { return instance; }
	size_t Size() { Thread::Mutex::Scoped l(locker);	return map.size();	}

	bool GetTaskData(int taskid, GTaskData &data);

	void GameSetTaskData(int taskid, const Octets &td);
	void SetTaskData(int taskid, const Octets &td );
	void GameGetTaskData(int taskid, unsigned int sid,int playerid, const Octets &env);

	bool Valid(time_t settime) const 
    {
		return settime != 0;
		/* 20070727 不再校验时间
        struct tm tm1,tm2;
        time_t now = time(NULL);
        localtime_r(&now, &tm1);
        localtime_r(&settime, &tm2);
        return tm1.tm_year == tm2.tm_year && tm1.tm_yday == tm2.tm_yday;
		*/
    }

};

};

#endif

