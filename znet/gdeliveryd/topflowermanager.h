#ifndef __GNET_TOPFLOWERMGR_H
#define __GNET_TOPFLOWERMGR_H 

#include <vector>
#include <map>

#include "gtopflowerdata"
#include "localmacro.h"
//#include <sys/time.h>
#include "timer.h"
#include "thread.h"
#include "flowertoprecord"

namespace GNET
{

#define TOPFLOWER_CLIENT_COUNT 500 // 客户端榜单人
#define TOPFLOWER_ALL_COUNT 600 // 总榜单榜单人,多出100，防止角色被删
#define ONE_PAGE_COUNT 20 // 每一页20列

struct STopFlowerID
{
	int count;// 花的数量
	int stamp;// 时间戳
};

inline bool operator > ( const STopFlowerID& lhs, const STopFlowerID& rhs )
{
	return lhs.count > rhs.count ? true : (lhs.count == rhs.count ? lhs.stamp < rhs.stamp : false);
}


class TopFlowerManager : public IntervalTimer::Observer
{
	typedef std::multimap<STopFlowerID, GTopFlowerData, std::greater<STopFlowerID> > TMultiMap;
	typedef std::map<int, GTopFlowerData> TMap; // roleid is key

	TopFlowerManager() : locker("topflowermanager::locker_top"),id_load(0),status(TOPFLOWER_NOTREADY){}  
public:
	Thread::RWLock & GetLocker() { return locker; }
public:
	Thread::RWLock locker;
	int id_load;
	ETopFlowerStatus status;

	TMultiMap m_RecvTopMap;// 接收榜
	TMultiMap m_SendTopMap; // 前500名送花榜

	TMap m_RecvAllMap;// 接收花 未入榜集合
	TMap m_SendAllMap;// 未入榜单集合
	
	~TopFlowerManager() {}

	inline ETopFlowerStatus GetStatus() { return status; }

	bool	Initialize()
	{
		IntervalTimer::Attach(this,(10*1000000)/IntervalTimer::Resolution());
		return true;
	}

	static TopFlowerManager* Instance() { static TopFlowerManager instance; return &instance;}
	bool InitTop(bool isRecv, std::vector<GTopFlowerData>& data);
	bool InitAddTop(bool isRecv, std::vector<GTopFlowerData>& data);
	void OnDBConnect();
	void Open();
	ETopFlowerStatus UpdateTop(bool isRecv, int roleid, int userid, int add_count, int gender, const Octets& rolename);
	bool GetTop(bool isRecv, int page, std::vector<GTopFlowerData>& data, int& all_count);
	bool GetRolePos(bool isRecv, int roleid, int& pos, int& count);
	bool IsGiftTake(bool isRecv, int roleid); // 是否可以领取奖励
	bool UpdateGiftTake(bool isRecv, int roleid); // 标记已经领取奖励
	bool DelRoleTop(bool isRecv, int roleid);// 删除角色调用
	bool OnRolenameChange(bool isRecv, int roleid , const Octets& oldname, const Octets& newname); // 改名字调用	
	int GetTopForWeb(int type, std::vector<FlowerTopRecord> & top);	// 和网站的接口

	bool Update();
	
	bool CheckGetTop();// 获取排行榜阀门
	bool CheckOpenTop();// 送和接受阀门
	bool CheckOpenGift();// 领取奖励功能阀门

protected:
	int _GetTopMinCount(bool isRecv); // 排行榜最小值 
	GTopFlowerData* _GetKey(bool isRecv, int roleid); // 得到key从Map中
	bool _UpdateTop(bool isRecv, GTopFlowerData& oldData, GTopFlowerData& newData); // 更新榜，淘汰老数据
	inline bool _CheckTime(struct tm& start, struct tm& end)
	{
		time_t curr_sec;
		time(&curr_sec);
		//struct tm dt;
		//localtime_r(&curr_sec, &dt);
		time_t start_sec = mktime(&start);
		time_t end_sec = mktime(&end);
		LOG_TRACE("Start_sec: %d, end_sec: %d, curr_sec: %d\n", (int)start_sec, (int)end_sec, (int)curr_sec);
		return curr_sec >= start_sec && curr_sec <= end_sec;

	}
	inline int _Local_MillSecond()
	{
		/*struct timeval _tstart;
		gettimeofday(&_tstart, NULL);
		return (int)(_tstart.tv_sec*1000 + _tstart.tv_usec/1000);*/
		time_t curr_sec;
		time(&curr_sec);
		//localtime(&curr_sec);
		return curr_sec;
	}
};

};
#endif

