#ifndef _GNET_NET_MARBLE_H_
#define _GNET_NET_MARBLE_H_

#include "itimer.h"
#include <map>

namespace GNET
{

struct NetMarbleInfo
{
	int ip;
	time_t last_deduct_time;
	time_t update_time;
	NetMarbleInfo(): ip(0),last_deduct_time(0),update_time(0){}
};
class UserInfo;
class NetMarble : public IntervalTimer::Observer
{
	enum {
		DEFAULT_UPDATE_INTERVAL_PER_USER = 60,
	};
	typedef std::map<int, NetMarbleInfo> NetMarbleInfoMap;
	NetMarbleInfoMap _usermap;
	bool _is_active;
	int _cursor;
	int _update_interval_per_user;
	NetMarble() : _is_active(false),_cursor(0),_update_interval_per_user(DEFAULT_UPDATE_INTERVAL_PER_USER) {}

public:
	static NetMarble *GetInstance()
	{
		static NetMarble instance;
		return &instance;
	}
	
	bool Initialize(int interval);
	bool Update();
	void OnLogin(int userid, int ip);
	void OnLogout(UserInfo * user);
	void OnNetMarbleLogin(int retcode, int userid, int ip, int deducttype);
	void OnNetMarbleDeduct(int retcode, int userid, int deduct_time);
};

};

#endif
