#ifndef __GNET_MAPPASSWD_H
#define __GNET_MAPPASSWD_H

#include <map>
#include <ext/hash_map>

#include "hashstring.h"
#include "thread.h"

namespace GNET
{
class MPassword
{
        int    userid;
        int    algorithm;
        Octets password;
        Octets matrix;
public:
        MPassword() { }
        MPassword(int u, int algo, const Octets& p,const Octets& m) : userid(u),algorithm(algo),password(p),matrix(m)  {}
        MPassword(const MPassword& r) : userid(r.userid),algorithm(r.algorithm),password(r.password),matrix(r.matrix)  {}

        void GetPassword(int& uid, int& algo, Octets& p, Octets& m)
        {
                uid  = userid;
                algo = algorithm;
                p = password;
                m = matrix;
        }
};

class Passwd
{
	typedef __gnu_cxx::hash_map<Octets/*username*/, MPassword> Map;
	typedef std::map<int/*userid*/, int/*algo*/> AlgoCache;//用于缓存补填提示与奖励信息
	Map map;
	AlgoCache algo_cache;
	Thread::Mutex locker;
	static Passwd	instance;
public:
	Passwd() : locker("Passwd::locker") { }
	~Passwd() { }
	static Passwd & GetInstance() { return instance; }
	size_t Size() { Thread::Mutex::Scoped l(locker);	return map.size();	}

	bool GetPasswd( Octets username, int& userid, Octets& passwd, Octets& matrix, int& algorithm )
        {
                Thread::Mutex::Scoped l(locker);
                Map::iterator it = map.find( username );
                if (it != map.end())
                {
                        it->second.GetPassword(userid, algorithm, passwd, matrix);
                        return true;
                }
                return false;
        }

	bool GetPasswd( Octets username, int& userid, Octets& passwd )
	{
		Thread::Mutex::Scoped l(locker);
		Map::iterator it = map.find( username );
		if (it != map.end())
		{
			Octets matrix;
                        int algorithm;
                        it->second.GetPassword(userid, algorithm, passwd, matrix);
			return true;
		}
		return false;
	}

	void SetPasswd( Octets username, int userid, Octets passwd )
	{
		Thread::Mutex::Scoped l(locker);
		map[username] = MPassword(userid, 0, passwd, Octets());
	}

	void SetPasswd( Octets username, int userid, Octets& passwd, Octets& matrix, int algorithm )
        {
                Thread::Mutex::Scoped l(locker);
                map[username] = MPassword(userid, algorithm, passwd, matrix);
        }

        void ClearPasswd(Octets username)
	{
		Thread::Mutex::Scoped l(locker);
		map.erase(username);
	}
	void CacheAlgo(int userid, int algo)
	{
		algo_cache[userid] = algo;
	}
	int GetAlgo(int userid)
	{
		AlgoCache::iterator it = algo_cache.find(userid);
		if (it != algo_cache.end())
			return it->second;
		else
			return 0;
	}
};

};

#endif

