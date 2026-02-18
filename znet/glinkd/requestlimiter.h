#ifndef __GNET_REQUESTLIMITER_H
#define __GNET_REQUESTLIMITER_H

#include <algorithm>
#include <map>
#include "timer.h"

namespace GNET
{

class LimitPolicy
{
public:
	virtual bool Update() { return false; }
	virtual ~LimitPolicy() { }
};
enum
{
	CHAT_POLICY      = 1,
	WHISPER_POLICY   = 2,
	FINDNAME_POLICY  = 3,
	GET_SSO_TICKET	 = 4,
	FAST_PAY	 = 5,
	QUERYNAME_POLICY = 6,
	KINGDOM_PRIVILEGE_POLICY = 7, //行驶国家权力
	REPORT_CHEATER	 = 8, //举报外挂
	GET_PASSPORT_CASHADD = 9,
	GET_TOUCH_POINT = 10,
	GET_KINGDOM_INFO = 11,
	GET_FAC_DYNAMIC = 12,
	FAC_BASE_DATA_SEND = 13,
	GET_KDC_TOKEN = 14,
};

class CounterPolicy : public LimitPolicy
{
	int _period;
	int _threshold;
	int _term;
	int _counter;
	time_t _last;
public:
	CounterPolicy(int period, int threshold, int term):_period(period),_threshold(threshold),_term(term)
	{
		_last = Timer::GetTime();
		_counter = 0;
	}

	bool Update() 
	{ 
		time_t now = Timer::GetTime();
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
};

class RequestLimiter
{
	typedef std::map<int, LimitPolicy*> PolicyMap;
	PolicyMap limiters;
public:
	RequestLimiter() 
	{
	}
	~RequestLimiter()
	{
		for(PolicyMap::iterator it=limiters.begin();it!=limiters.end();++it)
			delete it->second;
		limiters.clear();
	}
	void Initialize()
	{
		limiters.insert(std::make_pair((int)CHAT_POLICY,new CounterPolicy(10,5,30)));
		limiters.insert(std::make_pair((int)WHISPER_POLICY,new CounterPolicy(10,8,30)));
		limiters.insert(std::make_pair((int)FINDNAME_POLICY,new CounterPolicy(10,3,15)));
		limiters.insert(std::make_pair((int)GET_SSO_TICKET,new CounterPolicy(1,1,1)));
		limiters.insert(std::make_pair((int)FAST_PAY,new CounterPolicy(1,1,1)));
		limiters.insert(std::make_pair((int)QUERYNAME_POLICY,new CounterPolicy(1,1,1)));
		limiters.insert(std::make_pair((int)KINGDOM_PRIVILEGE_POLICY,new CounterPolicy(1,1,1)));
		//一小时内最多举报30次 惩罚时间半小时
		limiters.insert(std::make_pair((int)REPORT_CHEATER,new CounterPolicy(3600,30,1800)));
		limiters.insert(std::make_pair((int)GET_PASSPORT_CASHADD,new CounterPolicy(1,1,1)));
		limiters.insert(std::make_pair((int)GET_TOUCH_POINT,new CounterPolicy(1,1,1)));
		limiters.insert(std::make_pair((int)GET_KINGDOM_INFO,new CounterPolicy(1,4,1)));
		limiters.insert(std::make_pair((int)GET_FAC_DYNAMIC,new CounterPolicy(10,15,10)));
		limiters.insert(std::make_pair((int)FAC_BASE_DATA_SEND,new CounterPolicy(3,10,3)));
		limiters.insert(std::make_pair((int)GET_KDC_TOKEN,new CounterPolicy(3,10,3)));
	}
	bool Update(int requestid)
	{
		PolicyMap::iterator it = limiters.find(requestid);
		if(it==limiters.end())
			return false;
		return it->second->Update();
	}
};

}

#endif
