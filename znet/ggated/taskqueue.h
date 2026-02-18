#ifndef __GNET_TASKQUEUE_H
#define __GNET_TASKQUEUE_H
#include "localmacro.h"
namespace GNET
{
template <class TaskNode,int tasktype>
class TaskQueue: public IntervalTimer::Observer
{
//class TaskNode should have TryQuery member function,
//otherwise specialised RunTask template function is needed. 
public:
	const static int NO_FREQUENCY_LIMIT=-1;
public:
	void RunQueue(){
		while(tqueue.size() && CheckLimit()){
			RunTask<TaskNode,tasktype>(tqueue.front());
			AddCount();
			tqueue.pop_front();
			DEBUG_PRINT("RunQueue one node queuesize=%d",tqueue.size());
		}
	}
	int TryRunTask(const TaskNode& node){
		RunQueue();//old task first
		if(CheckLimit()){
			if(RunTask<TaskNode,tasktype>(node)==0)
			{
				AddCount();
				return 0;
			}
			else
				return -1;
		}
		else
		{
//			DEBUG_PRINT("TryRunTask, CheckLimit fail");
			if(AddQueue(node))
				return 1;
			else
				return 2;
		}
	}
	bool AddQueue(const TaskNode& node){
		if(tqueue.size()<queue_limit){
			tqueue.push_back(node);
			DEBUG_PRINT("AddQueue ok");
			return true;
		}
//		DEBUG_PRINT("AddQueue fail");
		return false;	
	}
	TaskQueue(int frequency_limit_=30,unsigned int queue_limit_=10,int period_= 60,int interval_=15):frequency_limit(frequency_limit_),queue_limit(queue_limit_),period(period_),interval(interval_)
	{
	//	DEBUG_PRINT("TaskQueue Construct tasktype=%d",tasktype);
		ticks=0;
		task_count=0;
		if(queue_limit==0)queue_limit=1;
		if(frequency_limit<=0)frequency_limit=1;
		if(period<=0)period=1;
		if(interval<=0)interval=1;
		IntervalTimer::Attach(this,interval*1000000/IntervalTimer::Resolution());
	}
	~TaskQueue(){}
	int GetPeriod(){return period;}
	bool Update(){
	//	DEBUG_PRINT("Taskqueue Update ticks=%d,task_count=%d,tasktype=%d",ticks,task_count,tasktype);
		ticks+=interval;
		if(ticks>=period){
			ticks=0;
			task_count=0;
			RunQueue();
		}
		return true;
	}
	void set_frequency_limit(int flimit){
		if(flimit>0)frequency_limit = flimit;
	}
private:
	bool CheckLimit(){
	//	DEBUG_PRINT("Taskqueue CheckLimit frequency_limit=%d,task_count=%d,tasktype=%d",frequency_limit,task_count,tasktype);
		if(frequency_limit==NO_FREQUENCY_LIMIT)
			return true;
		return task_count<frequency_limit;
	}
	//give default implement for simplequeries
	template<class Tasknd,int ttype>
	int RunTask(const TaskNode& node){
//		DEBUG_PRINT("RunTask ");
		return node.TryQuery();
	}
	void AddCount(){task_count++;}
private:
	std::list<TaskNode> tqueue;
	int frequency_limit;//permitted task number in each period
	unsigned int queue_limit;//limits of pending task number
	int period;//task_count is limited to be no more than frequency_limit, in every period. value are seconds.
	int interval;//try to run task every 'interval' seconds
	int ticks;//seconds have passed in current period
	int task_count;//processed task number in current period
};
const static int RoleListReqType = 1;
const static int RoleRelationReqType = 2;
const static int FactionInfoReqType = 3;
const static int FamilyInfoReqType = 4;
const static int RoleInfoReqType = 5;
const static int RoleStatusUpdateType = 6;

};
#endif
