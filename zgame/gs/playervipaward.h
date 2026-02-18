#ifndef __ONLINEGAME_GS_PLAYERVIPAWARD_H__
#define __ONLINEGAME_GS_PLAYERVIPAWARD_H__

#include "common/base_wrapper.h"
#include "common/protocol.h"
#include "vector.h"
#include <vector>
#include <map>
#include <deque>


namespace vipgame{

struct node_t
{
	int		award_id;			//编辑器里每个奖励物品对应的id，此id是唯一的，单增的
	short	award_type;			//奖品类型：0是普通，1是vip

	int		award_item_id;		//每个奖励的具体物品id，用于进包裹时声称物品
								//可能重复(送5个飞天符和送10个飞天符是不同的奖励)
	int		award_item_count;	//物品数量

	short	obtain_level;		//获取奖品所需的档次，vip档次从1开始，普通玩家档次根据等级分级，从1开始
	short	obtain_type;		//获取奖品的类型：0是每日领取，1是特殊领取

	int		expire_time;		//时限，奖品有单独的时限
};

enum AWARD_TYPE
{
	ORDINARY_AWARD_TYPE = 0,
	VIP_AWARD_TYPE      = 1,
};

enum OBTAIN_TYPE
{
	DAILY_AWARD			= 0,	//每日奖励，该值不能改，因为保存的时候map[0]对应每日
	SPECIAL_AWARD		= 1,	//特殊奖励，该值不能改，因为普通玩家保存在map[1]的位置
								//普通玩家不能越级领奖，所以只存一个level的物品即可
};

//用于调试命令
enum DEBUG_OPCODE
{
	CLEAR_ALL_AWARD		= 0,
	CLEAR_ALL_VIP_INFO	= 1,
};


class award_data
{
	typedef std::map<int, std::vector<int> >		LEVELMAP;
	typedef std::map<int, node_t>					TOTALMAP;

	
public:
	award_data();
	~award_data();

	//调用全部的数据添加完成后，需要调用CheckLevelAwardCnt检查是否超过最大数量
	bool		AddAward(const node_t & node);
	bool		CheckLevelAwardCnt(void);

	bool		QueryAward(int award_id, node_t & n);
	bool		QueryAwardByTypeLevel(int obtainlevel,OBTAIN_TYPE obtaintype, AWARD_TYPE awardtype, std::vector<node_t> &node_list);

private:
	//按档次、类型分类的map
	LEVELMAP	_vip_specialmap;		
	LEVELMAP	_vip_dailymap;
	LEVELMAP	_ordinary_specialmap;
	LEVELMAP	_ordinary_dailymap;
	
	//按照award_id查找
	TOTALMAP	_totalmap;				

	int			_lock;

	bool		CheckParam(const node_t & node) const;
	void		AddAwardByLevel(const node_t & node, LEVELMAP & levelmap);
	bool		QeryByLevel(int obtainlevel, LEVELMAP & levelmap, std::vector<node_t> & node_list);
	bool		CheckLevelAwardCnt(int minlevel, int maxlevel, LEVELMAP & levelmap);

	inline void NodeCopy(node_t & data, const node_t & node)
	{
		data.award_id			= node.award_id;
		data.award_type			= node.award_type;
		data.award_item_id		= node.award_item_id;
		data.award_item_count	= node.award_item_count;
		data.obtain_level		= node.obtain_level;
		data.obtain_type		= node.obtain_type;
		data.expire_time		= node.expire_time;
	}
};


class player_vip_award
{
	friend void DebugCmdClearAllVipAward(player_vip_award & rhs, DEBUG_OPCODE opcode);

	typedef std::map<int, std::deque<int> >		AWARDGOT_MAP;
	typedef std::deque<int>						AWARD_DEQUE;

	class award_info
	{
	public:
		award_info();
		virtual ~award_info() {}

		/***********************保存玩家奖励信息时*******************
		 * @brief: map[DAILY_AWARD]保存每日领过的奖励，map[SPECIAL_AWARD]保存领过的特殊奖励
		 *			每个档次只记录最近领取的MAX_COUNT_PER_LEVEL个奖励
		 */
		AWARDGOT_MAP	award_has_got;	

		
		//获取次日零时零分，需加上时区
		inline  int		GetTomorrow(int tz_adjust) const { return (tomorrow + tz_adjust); }
		inline  int		SetTomorrow(int tomorrow_secs, int tz_adjust) { return (tomorrow = tomorrow_secs - tz_adjust); }
		inline  bool	IsOutOfToday(int t, int tz_adjust) const { return (t >= GetTomorrow(tz_adjust)); }

		virtual bool	IsAwardExist(OBTAIN_TYPE level, int award_id);
		virtual void	ClearDeque(OBTAIN_TYPE level);
		virtual void	PushBackDeque(OBTAIN_TYPE level, int award_id);

		virtual void	Swap(award_info & rhs);
		virtual void	Clear() 
		{
			award_has_got.clear();
			tomorrow = 0;
		}

		virtual void	ClearAllAward()
		{
			award_has_got.clear();
			tomorrow = 0;
		}
	
	protected:
		virtual void	PopFrontDeque(OBTAIN_TYPE level);

	private:
		int				tomorrow;		//记录上次领取每日奖励的次日凌晨零分零秒，存盘为UTC时间

	};

	class vip_info : public award_info
	{
	public:
		vip_info();
		~vip_info();

		int				time_diff;		//收到协议时get_systime的时间和AU传来的curtime的差值
		int				start_time;		//AU传过来的开始时间，使用时需要time_diff
		int				end_time;		//AU传过来的结束时间，使用时需要time_diff
		int				level;			//vip档次

		virtual void	PushBackDeque(OBTAIN_TYPE level, int award_id);
		virtual void	Swap(vip_info & rhs);

		virtual void	Clear()
		{
			time_diff       = 0;
			start_time      = 0;
			end_time        = 0;
			level           = 0;

			award_info::Clear();
		}
		
		//vip每日奖励在6:00 AM后可以重新领取，0点到6点之间是不能领取的
		bool IsOverTomorrowInValidTime(int t, int tz_adjust);
		bool CheckValidTime(int t, int tz_adjust);

		//发给客户端的是UTC时间
		inline int		GetEndTimeUTC(int tz_adjust)
		{
			return (end_time - tz_adjust);
		}

		inline bool		IsOverdue(int curtime)
		{
			//调整到每天零点进行检查
			time_t t1 = end_time + time_diff;
			struct tm tm1;
			localtime_r(&t1, &tm1);
			tm1.tm_mday += 1;
			tm1.tm_hour  = 0;
			tm1.tm_min   = 0;
			tm1.tm_sec   = 0;
			int tmptime  = mktime(&tm1); 

			if(curtime < (start_time + time_diff) || curtime >= tmptime)
			{
				return true;
			}

			return false;
		}

	};


	class ordinary_info : public award_info
	{
	public:
		ordinary_info();
		~ordinary_info();

		virtual void    Swap(ordinary_info & rhs);
		virtual void	Clear()
		{
			award_info::Clear();
		}
	};


public:
	player_vip_award();
	~player_vip_award();

	//-------------------对外接口------------------------
	/**
	 * @brief TryGetAward FinishAwardObtention:
	 *         获取奖励分两个阶段:
	 *         (1) 首先是调用TryGetAward检查条件是否满足
	 *         (2) 然后等待进包裹成功的时候在调用FnishAwardObtention
	 *         FinishAwardObtention() 不管在什么情况都能把id添进领取队列里，防止在错误的情况下玩家能重复领取
	 */
	bool			TryGetAward(int curtime, const node_t & node, int playerlevel, int rebornCnt);
	bool			FinishAwardObtention(int curtime, const node_t & node);

	void			GetVipAwardEndtime(int & end_time);

	// 检查是否为终身VIP，定义为满级并且剩余时间大于2年
	bool IsLifeLongVip(int curtime);

	// 终身VIP，对VIP特殊类型的奖励，看时间戳，而不看award_has_got，根据新的30天重置规则
	// 30天重置VIP SPECIAL奖励规则:
	//					(a)无时间戳 --> 可领奖励，且领取奖励后，时间戳设置为当前。
	//                  (b)有时间戳且跟当前时间差，大于30天 --> 可领奖励，且领取奖励后，时间戳+30天
	//                  (c)有时间戳且跟当前时间差，小于30天 --> 不可领奖励
	bool IsLifeLongVipSpecialAwardAvailable(int curtime, int award_id);

	/**
	 * @brief IsVipAwardPlayer 由1级降成0级，AU不会再发消息过来，所以该函数会在vip过期的时候把_vip_info.level设为0
	 */
	bool			IsVipAwardPlayer(unsigned char & vipaward_level);
	
	/**
	 * @brief CheckVipLevelChange: 
	 *         delivery转发过来的vip信息需要判断，vip等级是否有变（升、降、保级）新周期开始可以重新领取奖励
	 */
	bool			CheckVipLevelChange(int roleid, int curlevel, int start_time, int end_time, int vipserver_curtime);

	int				GetVipAvailableAward(int curtime, std::vector<S2C::CMD::vip_award_info::award_data> & award_list);
	int				GetOrdinaryAvailableAward(int curtime, std::vector<S2C::CMD::vip_award_info::award_data> & award_list, int playerlevel, int rebornCnt);

	//存盘读盘
	void			Save(archive & ar);
	void			Load(archive & ar, int roleid);

	//player_imp swap
	void			Swap(player_vip_award & rhs);

	//保存AU传过来的recenttotalcash值，用于客户端显示
	void			SetRecentTotalCash(int recent_total_cash) { _recent_total_cash = recent_total_cash; }
	int				GetRecentTotalCash() const { return _recent_total_cash; }

	bool			SetVipLevelHide(char is_hide);
	bool			IsHideVipLevel() { return _is_hide; }



protected:
	//用于调试命令
	void			ClearAllAward();
	void			ClearAllVipInfo();



private:
	//time-zone
	int				_tz_adjust;
	int				_recent_total_cash;
	bool			_is_hide; //玩家是否关闭vip等级的显示，默认为打开,即false


	//info-data
	vip_info		_vip_info;
	ordinary_info	_ordinary_info;
	std::map<int , int> _special_award_time_stamps;


	bool			TryGetVipAward(int curtime, const node_t & node, int obtain_type, int level);
	bool			TryGetOrdinaryAward(int curtime, const node_t & node, int obtain_type, int playerlevel, int rebornCnt);

	bool			FinishGetVipAward(int curtime, const node_t & node, int obtain_type);
	bool			FinishGetOrdinaryAward(int curtime, const node_t & node, int obtain_type);

	int				CalcOrdinaryLevel(int playerlevel, int rebornCnt);
	int				CalcTomorrow(time_t t);

	bool			IsVipPlayer(int curtime);
};

/**
 * @brief DebugCmdClearAllVipAward 用于处理调试命令
 *
 * @param rhs
 * @param opcode: 0 表示清除玩家身上所有领取过的奖品的记录
 *                1 表示清除所有vip奖励信息--包括vip等级，每日到期时间，vip到期时间等等
 */
void DebugCmdClearAllVipAward(player_vip_award & rhs, DEBUG_OPCODE opcode);

}

#endif
