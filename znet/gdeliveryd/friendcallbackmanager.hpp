#ifndef __GNET_FRIENDCALLBACKMANAGER_HPP
#define __GNET_FRIENDCALLBACKMANAGER_HPP

#include <map>
#include "mutex.h"
#include "gcallbacklimit"
#include "mapuser.h"
#include "gcallbackinfo"
#include "gmail"

namespace GNET
{
class FriendCallbackInfo;
class FriendCallbackMail;
class FriendCallbackSubscribe;
// 测试
//#define CALLBACK_DEBUG
class FriendCallbackManager
{
	enum
	{
		TEMPLATEID_MIN = 0,
		TEMPLATEID_MAX = 3,
#ifdef CALLBACK_DEBUG
		SECONDS_PERDAY = 1800,
		COOLDOWN_TIME = 2, 			
#else
		SECONDS_PERDAY = 86400,
		COOLDOWN_TIME = 10, 			// 对同一好友发送召回邮件的时间间隔为10天
#endif
		RESETMAILNUM_TIME = 1, 			// 重置发送召回邮件数的时间间隔为1天
		MAX_CALLBACKMAIL_PERDAY = 5, 		// 每人每天发送召回邮件的上限为5封 
		MSG_INTERVAL = 3, 			// 服务器允许客户端连续两次发送召回邮件消息的时间间隔为3秒
	};

	typedef std::map<int, GCallbackLimit> LimitMap; 	// 角色ID与发送召回邮件限制的对应表
	typedef std::map<int, std::map<int, int> > RollbackMap; // <sender, <receiver, lastsend_time>>记录发送者给接收者上次发送的时间，用于回滚

	LimitMap _callback_limit;
	RollbackMap _rollback;
	Thread::Mutex _locker;
	static bool _isopen;
	static int _zoneoff;
public:
	static FriendCallbackManager *GetInstance()
	{
		static FriendCallbackManager instance;
		return &instance;
	}

	bool Initialize()
	{
		_isopen = true;
		return _isopen;
	}

	bool IsPlayerOnLine(int roleid, PlayerInfo *&playerinfo)
	{
		Thread::RWLock::RDScoped locker(UserContainer::GetInstance().GetLocker());
		playerinfo = UserContainer::GetInstance().FindRole(roleid);
		return (NULL == playerinfo) ? false : true;
	}

	void QueryCallbackInfo(const FriendCallbackInfo *msg, unsigned int sid);
	void ReplyCallbackInfo(unsigned char retcode, int roleid, unsigned int localsid, unsigned int sid);
	void LoadCallbackInfo(int roleid, const GCallbackInfo &callback_info);

	void SendCallbackMail(const FriendCallbackMail *msg, unsigned int sid);
	void ReplyCallbackMail(unsigned char retcode, int sender, unsigned int localsid, int receiver, unsigned int sid);
	void Rollback(int sender, int receiver);

	void PlayerLogout(int roleid);

	void PlayerLogin(int roleid);
	void NotifyAwardInfo(int roleid, int award_type, const vector<int> &summon_list);

	void Subscribe(const FriendCallbackSubscribe *msg, unsigned int sid);
	void ReplySubscribeResult(unsigned char retcode, char is_subscribe, int roleid, unsigned int localsid, unsigned int sid);
private:
	FriendCallbackManager() : _locker("FriendCallbackManager::Lock")
	{
		time_t now = Timer::GetTime();
		struct tm dt;
		localtime_r(&now, &dt);
		_zoneoff = dt.tm_gmtoff;
	}

	~FriendCallbackManager()
	{
		_callback_limit.clear();
		_rollback.clear();
	}

	std::string Identification() const
	{
		return "FriendCallback";
	}

	bool ExistCallbackLimit(int roleid)
	{
		Thread::Mutex::Scoped locker(_locker);
		return _callback_limit.find(roleid) == _callback_limit.end() ? false : true;
	}

	bool IsNDaysBefore(int time, int ndays)
	{
		int now = Timer::GetTime();
		return (now / SECONDS_PERDAY - time / SECONDS_PERDAY >= ndays) ? true : false;
	}

	bool IsTemplateIdValid(unsigned int template_id)
	{
		return ((template_id >= TEMPLATEID_MIN) && (template_id <= TEMPLATEID_MAX)) ? true : false;
	}

	bool IsCooldown(int checktime, int cooldown_time)
	{
		int now = Timer::GetTime();
		return (now - checktime > cooldown_time) ? false : true;
	}

	unsigned char CanSendCallbackMail(const FriendCallbackMail *msg, int &userid);
	
	bool IsFriend(const PlayerInfo *playerinfo, int roleid);
	bool IsReceiverInCooldown(int sender, int receiver);

	void GetCallbackLimit(int roleid, GCallbackInfo &callback_limit);
	void UpdateCallbackLimit(int sender, int receiver);

	void MakeMail(GMail &mail, int sender, int receiver, unsigned char template_id);

	bool NeedSync2DB(int roleid);

	void ReloadMaillist(int roleid, int localsid, int sid);
};

};

#endif
