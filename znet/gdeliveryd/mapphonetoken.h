#ifndef _GNET_MAPPHONETOKEN_H
#define _GNET_MAPPHONETOKEN_H

#include "simplelru.hpp"
namespace GNET 
{

class PhoneTokenInfo
{
public:
	Octets		seed;
	int		rtime;
public:
	PhoneTokenInfo() { }
	PhoneTokenInfo(const Octets & s, int r) : seed(s),rtime(r)  {}
	PhoneTokenInfo(const PhoneTokenInfo & r) : seed(r.seed), rtime(r.rtime)  {}
	void GetKeyInfo(Octets & s, int & r)
	{
		s  = seed;
		r = rtime;
	}
};

class PhoneTokenCache
{
private:
	enum
	{
		CACHE_MAX_FOR_USER	= 10,	//最多为一个账号缓存10个最近用过的电子码 用于验证电子码是否重复使用
	};
	typedef std::list<Octets>	UsedElecList;
	typedef	simplelru<int, PhoneTokenInfo, std::less<int> >	CacheLRUMap;
	typedef simplelru<int, UsedElecList, std::less<int> >	UsedElecMap;
	CacheLRUMap	phonemap;
	UsedElecMap	usedelecmap;
public:
	static 	int time_diff;//au.currenttime - deliver.time(NULL).ignore network delay
	PhoneTokenCache() : phonemap(200000), usedelecmap(20000) { }
	static PhoneTokenCache * GetInstance()
	{
		static PhoneTokenCache instance;
		return &instance;
	}

/*
	void SetMaxSize(size_t size) 
	{
		if(size <= 200000)
		{
			phonemap.setMaxSize(200000);
			lastelecmap.setMaxSize(200000);
		}
		else 
		{
			phonemap.setMaxSize(size); 
			lastelecmap.setMaxSize(size);	
		}
	}
*/
	bool GetPhoneTokenInfo(int userid, Octets & seed, int & rtime, int & currenttime )
	{
		PhoneTokenInfo info;
		if(phonemap.find(userid, info))
		{
			info.GetKeyInfo(seed, rtime);
			currenttime = time_diff + (int)time(NULL);//time_t is int64_t in x64
			return true;
		}
		return false;
	}

	void SetPhoneTokenInfo(int userid, Octets & seed, int rtime, int currenttime)
	{
		time_diff = currenttime - (int)time(NULL);
		phonemap.put(userid, PhoneTokenInfo(seed, rtime));
	}

	void SetUsedElecNumber(int userid, const Octets & used_elec_number)
	{
		UsedElecList list;
		usedelecmap.find(userid, list);
		list.push_back(used_elec_number);
		if (list.size() > CACHE_MAX_FOR_USER)
			list.pop_front();
		usedelecmap.put(userid, list);	
		//int warning; LOG_TRACE("userid %d cache %d elec_nums", userid, list.size());
	}

	bool CheckUsedElecNumber(int userid, const Octets & elec_number)//电子码不重复则返回成功
	{
		UsedElecList list;
		if (!usedelecmap.find(userid, list))
			return true;
		return std::find(list.begin(), list.end(), elec_number) == list.end();
	}
};

};
#endif


