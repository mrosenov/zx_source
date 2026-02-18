#ifndef __GNET_VIPInfoCache_HPP
#define __GNET_VIPInfoCache_HPP

#include <map.h>
#include "itimer.h"

namespace GNET
{

class VIPInfoCache : public IntervalTimer::Observer
{
public:
	friend class DebugCommand;
	static VIPInfoCache *GetInstance()
	{
		static VIPInfoCache instance;
		return &instance;
	}

	void Initialize();

	void UpdateVIPInfo(int userid, int currenttime, int starttime, int endtime, int viplevela, int status, int totalcash, int recenttotalcash, int infolack);

	void SendVIPInfoToGS(int userid, int roleid, int gsid) const;
	void SendVIPInfoToCentralDS(int userid) const;

	size_t GetOnlineVIPNum() const
	{
		return _vipinfos.size();
	}

	bool Update();
private:
	VIPInfoCache() : _now(0), _timebase(0), _seconds_perday(86400)
	{
	}

	~VIPInfoCache()
	{
	}

	bool IsAnotherPeriod()
	{
		_now = Timer::GetTime();
		// 为保证VIP系统信息已经更新完，游戏服务器每天5点更新到期的VIP信息
		return (_now - _timebase > _seconds_perday || _now < _timebase);
	}

	void SetTimeBase();
	void EraseOfflineVIPInfo();
	void GatherUserNeedToUpdate();
	void QueryVIPInfo();

	void SetIsOpen(int isopen)
	{
		_isopen = isopen;
	}

	enum
	{
		UPDATE_INTERVAL = 10,
		SECOND_TO_MICROSECOND = 1000000,
		QUERY_VIPINFO = 5,
		MAXNUM_PERUPDATE = 20,
	};

	struct VIPInfo
	{
		int currenttime;
		int starttime;
		int endtime;
		int viplevel;
		int deltatime; // delivery跟VIP服务器的时间差
		int status;
		int totalcash;
		int recenttotalcash;
		int infolack;
	};

	typedef std::map<int/*userid*/, VIPInfo> VIPInfoMap;
	VIPInfoMap _vipinfos; // 当前在线的VIP玩家的VIP信息

	time_t _now;
	time_t _timebase; 	// 每天的0点，用于更新在线VIP玩家的信息
	std::vector<int/*userid*/> _updating_vipinfos; 	// 0点需要向AU请求更新的VIP玩家，由于可能更新多次，_vipinfos在更新过程中可能改变，需要先存下来
	static bool _isopen; // VIP功能是否开启

	int _seconds_perday;
};

};
#endif
