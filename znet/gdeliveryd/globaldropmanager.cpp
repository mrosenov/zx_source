#include "globaldropmanager.h"
#include "getglobalcounter.hrp"
#include "gamedbclient.hpp"
namespace GNET
{
class CounterTask : public Thread::Runnable
{
	GlobalCounterInfo cinfo;
public:
	CounterTask(const GlobalCounterInfo& data):cinfo(data){}
	void Run()
	{
		if(TaskRunning())
		{
			GetGlobalCounterArg arg(cinfo.template_id,cinfo.start_time,cinfo.start_time);
			GetGlobalCounter *rpc = (GetGlobalCounter *)Rpc::Call(RPC_GETGLOBALCOUNTER, arg);
			rpc->max_count = cinfo.max_count;
			rpc->itemid = cinfo.itemid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			Thread::HouseKeeper::AddTimerTask(this,GetDelay());
		}
	}
	int GetDelay()
	{
		int now=Timer::GetTime();
		return cinfo.speak_interval - (now - cinfo.start_time) % (cinfo.speak_interval);
	}
private:
	bool TaskRunning()
	{
		int now=Timer::GetTime();
		return cinfo.start_time <= now  &&  cinfo.start_time + cinfo.duration*3600 > now;
	}
};

bool GlobalDropManager::Initialize()
{
        IntervalTimer::Attach(this,60*1000000/IntervalTimer::Resolution());
        return true;
}

void GlobalDropManager::AddCounterTask(const GlobalCounterInfo & item)
{
	GetGlobalCounterArg arg(item.template_id,item.start_time,item.start_time);
	GetGlobalCounter *rpc = (GetGlobalCounter *)Rpc::Call(RPC_GETGLOBALCOUNTER, arg);
	rpc->max_count = item.max_count;
	rpc->itemid = item.itemid;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	CounterTask *ctask = new CounterTask(item);
	Thread::HouseKeeper::AddTimerTask(ctask,ctask->GetDelay()/* delay */);
}

void GlobalDropManager::RegisterNewCounter(const std::vector<GlobalCounterInfo>& newcounter)
{
	int64_t key;
	std::vector<GlobalCounterInfo>::const_iterator it,ite=newcounter.end();
	for(it=newcounter.begin();it!=ite;++it)
	{
		key = (((int64_t)(it->template_id))<<32) + it->start_time;
		if(register_map.find(key) == register_map.end())
		{
			register_map.insert(std::make_pair(key,*it));
			HandleNewCounter(key,*it);
		}
	}
}

void GlobalDropManager::HandleNewCounter(int64_t key, const GlobalCounterInfo & item)
{
	LOG_TRACE("GlobalDropManager::HandleNewCounter itemid=%d starttime=%d speak_interval=%d", item.itemid, item.start_time, item.speak_interval);
	int now=Timer::GetTime();
	if(item.start_time <= now && item.start_time+item.duration*3600 > now)
	{
		AddCounterTask(item);
	}
	else if(item.start_time > now)
	{
		future_counter_map.insert(std::make_pair(item.start_time,key));
	}
}

bool GlobalDropManager::Update()
{
	const std::map<int64_t,GlobalCounterInfo>::iterator mite = register_map.end();
	int now=Timer::GetTime();
	std::multimap<int,int64_t>::iterator it;
	for(it=future_counter_map.begin();it!=future_counter_map.end();)
	{
		if(it->first <= now)
		{
			AddCounterTask(it->second);
			future_counter_map.erase(it++);
		}
		else
		{
			++it;
		}
	}
	return true;
}

}
