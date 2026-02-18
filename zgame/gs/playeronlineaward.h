#ifndef __ONLINEGAME_GS_PLAYERONLINEAWARD_H__
#define __ONLINEGAME_GS_PLAYERONLINEAWARD_H__

#include "actobject.h"
#include <map>


class player_online_award
{
	enum
	{
		TIME_INTERVAL_AWARD = 0,
		PER_DAY_AWARD		= 1,
	};

	struct onlineawardinfo
	{
		short	index;				//标识已经倒计时到第几轮,从0开始。

		int		time_passed;		//记录本轮倒计时已过了多少秒，只在存盘的时候做计算
		bool	is_receive_awards;	//标识本轮的奖品是否已经领取
		bool	is_closed;			//已经领取所有的奖励,但还不能从列表里删除，标记为closed


		//获取次日零时零分，需加上时区
		inline int  GetTomorrow(int tz_adjust) const { return (tomorrow + tz_adjust); } 
		inline int  SetTomorrow(int tomorrow_secs, int tz_adjust) { return (tomorrow = tomorrow_secs - tz_adjust); }

		//获取本轮倒计时的开始时间，不存盘不需要调整时区
		inline int  GetStartTime() const { return (start_time); }
		inline int  SetStartTime(int starttime) { return (start_time = starttime); }

		onlineawardinfo()
		{
			index				= 0;
			time_passed			= 0;
			is_receive_awards	= false;
			tomorrow			= 0;
			start_time			= 0;
			is_closed			= false;
		}

		~onlineawardinfo()
		{
			index				= 0;
			time_passed			= 0;
			is_receive_awards	= false;
			tomorrow			= 0;
			start_time			= 0;
			is_closed			= false;
		}
	
	private:
		int		tomorrow;			//每日倒计时奖励每天要清零奖励，该值
									//该值保存次日零时零分零秒的UTC时间，使用时需调整时区

		int		start_time;			//本轮倒计时的开始时间，为绝对时间，该值登录时根据time_passed重新计算
									//该值保存为UTC时间，使用时需要调整时区,该值不存盘20111212
	};

	typedef std::map<int, onlineawardinfo> ONLINEAWARDINFO_MAP;


public:
	struct player_info
	{
		int		rebornCnt;		//转生次数
		int		level;			//等级
		bool	gender;			//性别
		short	cls;			//职业
	};

	enum TRYGET_CODE
	{
		ONLINE_ERR_OVER_DEADLINE		= -9,
		ONLINE_ERR_EXHAUST				= -8,
		ONLINE_ERR_COUNTDOUWN			= -7,
		ONLINE_ERR_INVALID_INDEX		= -6,
		ONLINE_ERR_INVALID_PLAYER		= -5,
		ONLINE_ERR_INVALID_SMALL_GIFT	= -4,
		ONLINE_ERR_NOT_FOUND			= -3,
		ONLINE_ERR_HAS_OBTAINED			= -2,
		ONLINE_ERR_NOT_ACTIVE			= -1, 
		ONLINE_SUCCESS					= 0,
		ONLINE_OBTAINED					= 1,	//物品最终进包裹了
	};


public:
	player_online_award();
	~player_online_award();

	//-----------------------对外接口----------------------
	/**
	 * @brief CheckTimeToActive: 获取下一个检查的时间点，在player的Heartbeat中调用 
	 * force_flag: 标志是否需要强制客户端更新状态
	 * @return true: 表示玩家的在线奖励有变化，需要向客户端发送当前的在线奖励信息
	 */
	bool		CheckTimeToActive(time_t t, const player_info & playerinfo, bool & force_flag);
	inline bool IsTimeToCheck(void) { return (--_check_counter_down <= 0); }

	/**
	 * @brief TryGetOnlineAward FinishOnlineAwardObtain: 
	 *         获取奖励分两个阶段
	 *         (1) 首先是调用TryGetOnlineAward检查条件是否满足
	 *         (2) 然后等待进包裹成功的时候在调用FinishOnlineAwardObtain
	 *         FinishOnlineAwardObtain不管在什么情况都能把id添加到激活队列中，防止在错误的情况下玩家能重复领取
	 */
	TRYGET_CODE	TryGetOnlineAward(int id, int index, time_t t, int small_giftbag_id);
	bool		FinishOnlineAwardObtain(int roleid, int id, int index, time_t t, bool & is_closed);

	/**
	 * @brief GetCurOnlineAwardInfo: 获取玩家当前所有已激活的在线奖励
	 */
	int 		GetCurOnlineAwardInfo(std::vector<S2C::CMD::online_award_info::award_data> & info_list);

	//DB存取
	void		Save(archive & ar);
	void		Load(archive & ar, int roleid);

	//player_imp swap
	void		Swap(player_online_award & rhs);

	bool		GetSmallGiftbagID(int awardid, int index, int & id);

	//调试用
	static void DebugCmdClearOnlineAward(player_online_award & rhs);
	static bool DebugIsActive(int id, player_online_award & rhs);
	static void DebudDumpActiveAward(std::vector<int> & active_list, player_online_award & rhs);
	bool		GetTimePassed(int awardid, time_t t, int & index, int & time_passed);
	bool		GetNextIndex(int id, int index, int & nextindex) const;


private:
	ONLINEAWARDINFO_MAP _info_map;
	int			_tz_adjust;				//时区信息
	int			_check_counter_down;	//检查倒计时

	int			ConvertToSeconds(struct tm & tmtime) const;
	bool		CheckNextTimeToActive(time_t t, const player_template::online_giftbag_config & config, int & next_acttime);
	bool		CheckOverdueOnlineAward(time_t t, const player_template::OnlineGiftbagConfigMap & config_list, const player_info & playerinfo);
	void		InitOnlineAwardInfo(int id, onlineawardinfo & info, time_t t);

	//取生效的小礼包的index
	int			GetSmallGiftBagsBegin(int id, const player_template::OnlineGiftbagConfigMap & config_list);
	bool		GetSmallGiftBagsNext(int id, const player_template::OnlineGiftbagConfigMap & config_list, int index, int & nextindex) const;

	inline bool ValidPlayer(const player_info & playerinfo, const player_template::online_giftbag_config & config);
	inline bool IsActive(int id) { return (_info_map.find(id) != _info_map.end()); }

	//clear
	void		Clear(void);

	//检查是否过期
	inline bool	IsPerDayAwardPastDue(time_t t, onlineawardinfo & info)
	{
		return (t >= info.GetTomorrow(_tz_adjust));
	}

	//等级
	inline bool __CheckLevel(int curlevel, int minlevel, int maxlevel)
	{
		if(minlevel > 0 && curlevel < minlevel)
		{
			return false;
		}
		
		if(maxlevel > 0 && curlevel > maxlevel)
		{
			return false;
		}

		return true;
	}

	//转生次数
	inline bool __CheckRebornCnt(int cur_rebornCnt, int rebornCnt_required)
	{
		return (cur_rebornCnt >= rebornCnt_required);
	}

	//性别
	inline bool __CheckGender(bool cur_gender, unsigned int gender_required)
	{
		if(2 == gender_required) 
			return true;
		if(cur_gender && gender_required) 
			return true;
		if(!cur_gender && !gender_required) 
			return true;

		return false;
	}

	//职业
	inline bool __CheckCls(short cur_cls, int64_t mask1, int64_t mask2)
	{
		return (object_base_info::CheckCls(cur_cls, mask1, mask2));
	}

	//种族：人族、神裔
	inline bool __CheckRace(short cur_cls, int required_race)
	{
		if( required_race == 0 && !(cur_cls <= 0x1F || (cur_cls > 0x3F && cur_cls <= 0x5F)) )
			return false;

		if( required_race == 1 && !((cur_cls > 0x1F && cur_cls <= 0x3F) || (cur_cls > 0x5F && cur_cls <= 0x7F)) )
			return false;

		return true;
	}
};


#endif
