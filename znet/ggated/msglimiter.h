#ifndef __GNET_MSGLIMITER_H
#define __GNET_MSGLIMITER_H

#include <algorithm>
#include <map>
#include "localmacro.h"

namespace GNET
{

template <int _period,int _threshold>
class NoCooldownCounterPolicy//for global limits 
{
	int _counter;
	time_t _last;
public:
	NoCooldownCounterPolicy(){
		_last = Timer::GetTime();
		_counter = 1;
//		DEBUG_PRINT("NoCooldownCounterPolicy construct,last=%d,counter=%d",_last,_counter);
	}
	bool Check(){ 
		time_t now = Timer::GetTime();
//		DEBUG_PRINT("CounterPolicy NO_Cooldown_Check now=%d,last=%d,offset=%d",now,_last,now-_last);
		if(now-_last>_period)
		{
			_counter = 1;
			_last = now;
			return true;
		}
		return ++_counter<=_threshold;
	}
	void Rollback(){ --_counter;}
};

template <int _period,int _threshold,int _term >
class CounterPolicy
{
//	int _period;
//	int _threshold;
//	int _term;
	int _counter;
	time_t _last;
public:
/*	CounterPolicy(int period, int threshold, int term):_period(period),_threshold(threshold),_term(term)*/
	CounterPolicy(){
		_last = Timer::GetTime();
		_counter = 1;
//		DEBUG_PRINT("CounterPolicy construct,last=%d,counter=%d",_last,_counter);
	}

	bool Check(){ 
		time_t now = Timer::GetTime();
//		DEBUG_PRINT("CounterPolicy Check now=%d,last=%d,offset=%d",now,_last,now-_last);
		if(_counter<0)
		{
			if(now-_last<_term)
				return false;
			_counter = 1;
			_last = now;
			return true;
		}
		if(now-_last>_period)
		{
			_counter = 1;
			_last = now;
			return true;
		}
		if(_counter>=_threshold)
		{
			_counter = -1;
			_last = now;
			return false;
		}
		else
		{
			_counter++;
		}
		return true;
	}
	bool IsEntryExpired(time_t oldest_valid_time)const{ 
                return (_last<oldest_valid_time);
	}		
};

template <int period,int threshold,int term >
class MsgLimiter: public IntervalTimer::Observer
{
	typedef CounterPolicy<period,threshold,term> MsgPolicy;
	typedef std::map<int, MsgPolicy*> PolicyMap;
	PolicyMap limiters;
	int cursor;
	static const int UPDATE_INTERVAL=30;//second
	static const int UPDATE_COUNT=300;//update 300 entry each time
	//update the whole map, for example 18000 entries, in about 30 minutes
	static const time_t ENTRY_EXPIRE_INTERVAL=900;//second;
public:
	MsgLimiter():cursor(-1){
		Initialize();
	}
	virtual ~MsgLimiter()
	{
		for(typename PolicyMap::iterator it=limiters.begin();it!=limiters.end();++it)
			delete it->second;
		limiters.clear();
	}
//	static  MsgLimiter& GetInstance() { static MsgLimiter instance;return instance; }
	void Initialize(){
//		limiters.insert(std::make_pair((int)CHAT_POLICY,new CounterPolicy<10,5,30>()));
//		limiters.insert(std::make_pair((int)WHISPER_POLICY,new CounterPolicy>10,8,30>());
		IntervalTimer::Attach(this,UPDATE_INTERVAL*1000000/IntervalTimer::Resolution());
	}
	bool Check(int roleid)
	{
		typename PolicyMap::iterator it = limiters.find(roleid);
		if(it==limiters.end())
		{
			limiters.insert(std::make_pair(roleid,new MsgPolicy()));
			return true;
		}
		return it->second->Check();
	}
	void PartlyWalk( int& beginid, int count)
	{
		typename PolicyMap::iterator it;
		typename PolicyMap::iterator ite = limiters.end();
		if( -1 == beginid )
			it = limiters.begin();
		else
			it = limiters.lower_bound(beginid);
		time_t now = Timer::GetTime();
//		DEBUG_PRINT("PolicyMap update time=%d beginid=%d size=%d threshold=%d nodesize=%d",now,beginid,limiters.size(),threshold,sizeof(MsgPolicy));
		time_t entry_expire_time = now - ENTRY_EXPIRE_INTERVAL;
		for( int n=0; it != ite && n < count; ++n)
		{
			MsgPolicy& cp=*it->second;
			if(cp.IsEntryExpired(entry_expire_time))
			{	
				DEBUG_PRINT("PolicyMap erase entry roleid=%d",it->first);
				delete it->second;
				limiters.erase(it++);
			}
			else
				++it;
		}
		beginid = ( it != limiters.end() ? it->first : -1 );
	}
	bool Update()
	{
                PartlyWalk(cursor,UPDATE_COUNT);
                return true;
	}
};

};

#endif
