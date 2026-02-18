#ifndef __GNET_BATTLEFLAGMANAGER_HPP
#define __GNET_BATTLEFLAGMANAGER_HPP

#include <map>
#include "itimer.h"
#include "battleflagbuffscope.hpp"
#include "factionidbean"

namespace GNET
{

struct BattleFlagInfo
{
	int _flagid; // 决定战旗的类型
	int _endtime;
	int _gsid;
	int _worldtag; // 用于区分同一GS上的不同副本

	BattleFlagInfo() : _flagid(0), _endtime(0), _gsid(0), _worldtag(0)
	{
	}

	BattleFlagInfo(int flagid, int endtime, int gsid, int worldtag) : _flagid(flagid), _endtime(endtime), _gsid(gsid), _worldtag(worldtag)
	{
	}

	BattleFlagInfo(const BattleFlagInfo &rhs) : _flagid(rhs._flagid), _endtime(rhs._endtime), _gsid(rhs._gsid), _worldtag(rhs._worldtag)
	{
	}
};
/*
class LessFunctor
{
public:
	bool operator() (const FactionIDBean &fid1, const FactionIDBean &fid2) const
	{
		return (fid1.ftype < fid2.ftype || fid1.factionid < fid2.factionid);
	}
};*/
inline bool operator == ( const FactionIDBean& lhs, const FactionIDBean& rhs )
{
	        return lhs.ftype == rhs.ftype && lhs.factionid == rhs.factionid;
}

inline bool operator < ( const FactionIDBean& lhs, const FactionIDBean& rhs )
{
	return lhs.ftype < rhs.ftype ? true : (lhs.ftype == rhs.ftype ? lhs.factionid < rhs.factionid : false);
}

struct BattleFlagKey
{
	FactionIDBean factionid;
	int gsid;
	int worldtag;
	inline bool operator < ( const BattleFlagKey& rhs ) const
	{
		if(factionid < rhs.factionid)
		{
			return true;
		}
		else
		{
			if(factionid == rhs.factionid)
			{
				return gsid < rhs.gsid ? true : (gsid == rhs.gsid ? worldtag < rhs.worldtag : false);
		
			}
			else
			{
				return false;
			}
		}
	}


};

class BattleFlagManager : public IntervalTimer::Observer
{
public:
	typedef std::map<BattleFlagKey, BattleFlagInfo> BattleMap;

	static BattleFlagManager *GetInstance()
	{
		static BattleFlagManager instance;
		return &instance;
	}

	static void MakeFactionID(BattleFlagKey &fid, int roleid, int factionid, int gsid, int worldtag)
	{
		if(factionid != 0)
		{
			fid.factionid.ftype = FACTIONTYPE;
			fid.factionid.factionid = factionid;
			fid.gsid = gsid;
			fid.worldtag = worldtag;
		}
		else
		{
			// 如果角色没有帮派则用roleid作为一种特殊的帮派
			fid.factionid.ftype = NO_FACTION;
			fid.factionid.factionid = roleid;
			fid.gsid = gsid;
			fid.worldtag = worldtag;
		}
	}

	void BattleFlagStart(int flagid, int duration, const BattleFlagKey& bfkey)
	{
		AddBattleFlag(flagid, duration, bfkey);
		SendBuffScope(bfkey.gsid, flagid, bfkey.worldtag, duration, bfkey.factionid);
	}

	void BattleFlagEnd(int gsid, int worldtag)
	{
		for(BattleMap::iterator it = _battleflags.begin(); it != _battleflags.end();)
		{
			BattleFlagInfo &info = it->second;
			if(info._gsid == gsid && info._worldtag == worldtag)
			{
				_battleflags.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	void QueryBattleFlagBuff(const BattleFlagKey& bfkey)
	{
		BattleMap::const_iterator it = _battleflags.find(bfkey);
		if(it == _battleflags.end())
		{
			return;
		}
		if(it->second._gsid != bfkey.gsid || it->second._worldtag != bfkey.worldtag)
		{
			return;
		}
		SendBuffScope(bfkey.gsid, it->second._flagid, bfkey.worldtag, (it->second._endtime - Timer::GetTime()), bfkey.factionid);
	}

	bool Update()
	{
		time_t now = Timer::GetTime();
		for(BattleMap::iterator it = _battleflags.begin(); it != _battleflags.end();)
		{
			if(it->second._endtime <= now)
			{
				_battleflags.erase(it++);
			}
			else
			{
				++it;
			}
		}
		return true;
	}
private:
	enum
	{
		NO_FACTION = -1,
		UPDATE_INTERVAL = 10,
		SECOND_TO_MICROSECOND = 1000000,
	};

	BattleFlagManager()
	{
		IntervalTimer::Attach(this, UPDATE_INTERVAL * SECOND_TO_MICROSECOND / IntervalTimer::Resolution());
	}
	~BattleFlagManager()
	{
	}

	void AddBattleFlag(int flagid, int duration, const BattleFlagKey& bfkey)
	{
		time_t now = Timer::GetTime();
		_battleflags[bfkey] = BattleFlagInfo(flagid, now + duration, bfkey.gsid, bfkey.worldtag);
	}


	void SendBuffScope(int gsid, int flagid, int worldtag, int remaintime, const FactionIDBean &factionid)
	{
		BattleFlagBuffScope msg;
		msg.worldtag = worldtag;
		msg.flagid = flagid;
		msg.remaintime = remaintime;
		GetBuffScope(factionid, msg.roles, gsid);
		GProviderServer::GetInstance()->DispatchProtocol(gsid, msg);
		LOG_TRACE("Send BattleFlagBuffScope to gsid=%d,worldtag=%d,flagid=%d,roles.size=%d", gsid, worldtag, flagid, msg.roles.size());
	}

	void GetBuffScope(const FactionIDBean &factionid, std::vector<int> &roles, int gsid)
	{
		// 没有帮派的人，战旗作用范围只有自己
		if(factionid.ftype == NO_FACTION)
		{
			roles.push_back(factionid.factionid);
			return;
		}

		FactionManager::Instance()->GetOnlineMember(factionid.factionid, roles, gsid);
		return;
	}

	BattleMap _battleflags;
};

}

#endif
