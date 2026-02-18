#include "playeronlineaward.h"
#include "dbgprt.h"
#include "timer.h"
#include <algorithm>


//在player心跳中最长的下次检查时间，单位为秒
#define MAX_CHECKTIME_INTERVAL 601
#define SECONDS_PER_DAY  (24 * 60 * 60)
#define SECONDS_PER_HOUR (60 * 60)
#define SECONDS_PER_MIN  (60)
#define SECONDS_ENDLESS  (-1)

#define SECONDS_OF_ALL_DAY   (24*3600)

//版本信息
#define DATA_VERSION_0 ((int)0)

//当前存盘的数据版本，也是最新的版本
#define CUR_ONLINEAWARD_DATA_VERSION (DATA_VERSION_0)

extern abase::timer g_timer;



player_online_award::player_online_award()
{
	time_t t1 = time(NULL);
	struct tm tm1;
	localtime_r(&t1, &tm1);
	_tz_adjust = -tm1.tm_gmtoff;
	_check_counter_down = 0;
}

player_online_award::~player_online_award()
{
	_check_counter_down = 0;
	_info_map.clear();
}

bool player_online_award::CheckTimeToActive(time_t t, const player_info & playerinfo, bool & force_flag)
{
	force_flag			= false;
	bool ret			= false;
	int next_acttime	= MAX_CHECKTIME_INTERVAL;
	const player_template::OnlineGiftbagConfigMap & config_list			= player_template::GetOnlineGiftbagConfigMap();
	player_template::OnlineGiftbagConfigMap::const_iterator it_config	= config_list.begin();
	ONLINEAWARDINFO_MAP::iterator it_info;	

	//去除过期的倒计时包,去除已经不满足条件的倒计时包
	ret = CheckOverdueOnlineAward(t, config_list, playerinfo);
	if(ret)
	{
		force_flag = true;
	}

	//获取下一个检查点
	for(; it_config != config_list.end(); ++it_config)
	{
		if(!ValidPlayer(playerinfo, it_config->second))
		{
			continue;
		}

		int tmpret = MAX_CHECKTIME_INTERVAL;
		if(CheckNextTimeToActive(t, it_config->second, tmpret) && !IsActive(it_config->second.id))
		{
			onlineawardinfo data;
			InitOnlineAwardInfo(it_config->second.id, data, t);
			_info_map[it_config->second.id] = data; 
			ret = true;
		}
		
		if(tmpret >= 0 && tmpret < next_acttime)
		{
			next_acttime = tmpret;
		}
	}

	_check_counter_down = next_acttime;

	return ret;
}

bool player_online_award::FinishOnlineAwardObtain(int roleid, int id, int index, time_t t, bool & is_closed)
{
	if(!IsActive(id))
	{
		//成功领取还没激活的奖励，有错误！
		//可能是因为还没调用TryGetOnlineAward()函数就直接调用FinishOnlineAwardObtain()
		//这里的处理方法是不让玩家领取该奖励
		onlineawardinfo data_err;
		InitOnlineAwardInfo(id, data_err, t);
		data_err.index				= player_template::MAX_SMALL_ONLINE_GB_COUNT - 1;
		data_err.is_receive_awards	= true;
		_info_map[id]				= data_err;

		__PRINTF("玩家%d还没激活在线奖励%d就成功领取奖励---index=%d", roleid, id, index);
		GLog::log(GLOG_ERR, "玩家%d还没激活在线奖励%d就成功领取奖励---index=%d", roleid, id, index);

		return false;
	}

	const player_template::OnlineGiftbagConfigMap & config_list = player_template::GetOnlineGiftbagConfigMap();
	int next_index = player_template::MAX_SMALL_ONLINE_GB_COUNT - 1;
	//如果还有后续的小礼包可以领取
	if(GetSmallGiftBagsNext(id, config_list, index, next_index))
	{
		_info_map[id].index				= next_index;
		_info_map[id].is_receive_awards = false;
	}
	else //没有后续的小礼包
	{
		_info_map[id].is_receive_awards = true;
		_info_map[id].is_closed			= true;
		is_closed						= true;
	}

	//设置下一轮的起始时间
	_info_map[id].SetStartTime(t);

	return true;
}

player_online_award::TRYGET_CODE 
player_online_award::TryGetOnlineAward(int id, int index, time_t t, int small_giftbag_id)
{
	//没有激活的在线奖励不能领取
	ONLINEAWARDINFO_MAP::iterator it = _info_map.find(id);	
	if(_info_map.end() == it) 
	{
		return ONLINE_ERR_NOT_ACTIVE;
	}

	//是否已经领完所有奖励
	if(it->second.is_closed)
	{
		return ONLINE_ERR_EXHAUST;
	}

	//是否已经领取
	if(it->second.is_receive_awards)
	{
		return ONLINE_ERR_HAS_OBTAINED;
	}

	//---------配置表信息检查-----------
	//获取配置表
	const player_template::OnlineGiftbagConfigMap & config_list			= player_template::GetOnlineGiftbagConfigMap();
	player_template::OnlineGiftbagConfigMap::const_iterator it_config	= config_list.find(id);
	if(config_list.end() == it_config)
	{
		return ONLINE_ERR_NOT_FOUND;
	}

	if((size_t)index >= it_config->second.small_gift_bags.size())
	{
		return ONLINE_ERR_NOT_FOUND;
	}	


	//检查是否是无效的index（小礼包id为0）,或者与客户端数据版本不一致
	if( (it_config->second.small_gift_bags[index].gift_bag_id <= 0)
	 || (small_giftbag_id != it_config->second.small_gift_bags[index].gift_bag_id) )
	{
		return ONLINE_ERR_INVALID_SMALL_GIFT;
	}

	//是否已经超过领奖的最后期限
	if(it_config->second.overdue_time <= t)
	{
		return ONLINE_ERR_OVER_DEADLINE;
	}

	//---------在线信息检查------------
	//index领取顺序是单增的，范围是0 ~ (MAX_SMALL_ONLINE_GB_COUNT - 1)
	if(index != it->second.index  || index >= player_template::MAX_SMALL_ONLINE_GB_COUNT) 
	{
		return ONLINE_ERR_INVALID_INDEX;
	}

	//检查倒计时是否已经结束
	int interval_secs = it_config->second.small_gift_bags[index].deliver_interval;
	if( t < (it->second.GetStartTime() + interval_secs) )
	{
		return ONLINE_ERR_COUNTDOUWN;
	}

	return ONLINE_SUCCESS;
}

/**
 * @brief Save 
 *
 * 版本号为0的存储顺序：size(), 奖励的模板id, index倒计时到第几轮, timepassed本轮已倒计时的秒数, is_receive_awards, tomorrow, is_closed
 *
 * @param ar
 */
void player_online_award::Save(archive & ar)
{
	ar << CUR_ONLINEAWARD_DATA_VERSION;
	ar << (size_t)_info_map.size();

	ONLINEAWARDINFO_MAP::iterator it = _info_map.begin();
	while(_info_map.end() != it)
	{
		//计算本轮已倒计时多少秒
		int time_passed = g_timer.get_systime() - it->second.GetStartTime();
		ar << it->first;
		//由于数据版本的原因，增加字段时最好不要打乱顺序
		ar << it->second.index << time_passed << it->second.is_receive_awards << it->second.GetTomorrow(_tz_adjust) << it->second.is_closed;
		++it;
	}
}

void player_online_award::Load(archive & ar, int roleid)
{
	int version;	
	size_t count;
	ar >> version;
	ar >> count;

	switch(version)
	{
	case DATA_VERSION_0:
		{
			for(size_t i = 0; i < count; i++)
			{
				int id;
				int tmptomorrow;
				int tmpstarttime;
				onlineawardinfo data;
				ar >> id;
				ar >> data.index;
				ar >> data.time_passed;
				ar >> data.is_receive_awards;
				ar >> tmptomorrow;
				ar >> data.is_closed;
				data.SetTomorrow(tmptomorrow, _tz_adjust);
				tmpstarttime = g_timer.get_systime() - data.time_passed; 
				data.SetStartTime(tmpstarttime);
				_info_map[id] = data;
			}
		}
		break;

	default:
		{
			__PRINTF("\n\n\nERROR: 从DB读到错误的在线倒计时奖励数据版本！version:%d count:%d \n\n", version, count);
			GLog::log(GLOG_ERR, "玩家id=%d从DB读到错误的在线倒计时奖励数据版本！version:%d count:%d", roleid, version, count);
		}
		break;
	}
}

void player_online_award::Swap(player_online_award & rhs)
{
	_tz_adjust			= rhs._tz_adjust;
	_check_counter_down = rhs._check_counter_down;

	_info_map.swap(rhs._info_map);
}

bool player_online_award::CheckOverdueOnlineAward(time_t t, const player_template::OnlineGiftbagConfigMap & config_list, const player_info & playerinfo)
{
	bool ret = false;
	ONLINEAWARDINFO_MAP::iterator it = _info_map.begin();
	while(it != _info_map.end())
	{
		player_template::OnlineGiftbagConfigMap::const_iterator it_config = config_list.find(it->first);
		if(config_list.end() == it_config)
		{
			_info_map.erase(it++);
			ret = true;
			continue;
		}

		//检查时间
		if(PER_DAY_AWARD == it_config->second.award_type)
		{
			if(t >= it->second.GetTomorrow(_tz_adjust))
			{
				_info_map.erase(it++);
				ret = true;
				continue;
			}

			struct tm tm1;
			localtime_r(&t, &tm1);
			int tmpsec = ConvertToSeconds(tm1);
			if(tmpsec >= it_config->second.end_time_per_day)
			{
				_info_map.erase(it++);
				ret = true;
				continue;
			}
		}

		//检查是否过期
		if(t >= it_config->second.overdue_time)
		{
			_info_map.erase(it++);
			ret = true;
			continue;
		}

		++it;
	}

	return ret;
}

/**
 * @brief CheckNextTimeToActive 
 *
 * @param t:	  当前时间
 * @param config: 在线倒计时奖励配置信息
 * @param next_acttime: 下一个检查点
 *
 * @return true:表示立刻激活倒计时
 */
bool player_online_award::CheckNextTimeToActive(time_t t, const player_template::online_giftbag_config & config, int & next_acttime)
{
	next_acttime = 0;
	//先检查时间段是否满足条件
	if(t >= config.overdue_time)
	{
		next_acttime = MAX_CHECKTIME_INTERVAL;
		return false;
	}

	switch(config.award_type)
	{
		//时间段
		case TIME_INTERVAL_AWARD:
		{
			if(t >= config.start_time_per_interval && t < config.end_time_per_interval)
			{
				next_acttime = std::min<int>(config.end_time_per_interval - t, MAX_CHECKTIME_INTERVAL);
				return true;
			}
			else if(t < config.start_time_per_interval)
			{
				next_acttime = std::min<int>(config.start_time_per_interval - t, MAX_CHECKTIME_INTERVAL); 
				return false;
			}
			else if(t >= config.end_time_per_interval)
			{
				next_acttime = MAX_CHECKTIME_INTERVAL;
				return false;
			}
		}
		break;

		//每日
		case PER_DAY_AWARD:
		{
			//先检查时间段是否满足
			if(t < config.start_time_per_interval)
			{
				next_acttime = std::min<int>(config.start_time_per_interval - t, MAX_CHECKTIME_INTERVAL); 
				return false;
			}
			else if(t >= config.end_time_per_interval)
			{
				next_acttime = MAX_CHECKTIME_INTERVAL;
				return false;
			}

			//每日的时间段是否满足条件
			struct tm tm1;
			localtime_r(&t, &tm1);
			int tmpsec = ConvertToSeconds(tm1);
			if(tmpsec >= config.start_time_per_day && tmpsec < config.end_time_per_day)
			{
				next_acttime = std::min<int>(config.end_time_per_day - tmpsec, MAX_CHECKTIME_INTERVAL);
				return true;
			}
			else if(tmpsec < config.start_time_per_day)
			{
				next_acttime = std::min<int>(config.start_time_per_day - tmpsec, MAX_CHECKTIME_INTERVAL);
				return false;
			}
			else if(tmpsec >= config.end_time_per_day)
			{
				next_acttime = std::min<int>(SECONDS_OF_ALL_DAY - tmpsec, MAX_CHECKTIME_INTERVAL);
				return false;
			}
		}
		break;

		default:
			__PRINTF("在线倒计时类型出错!");
			break;
	}

	return false;
}


/**
 * @brief ValidPlayer 检查等级、是否飞升、性别、职业、种族
 *
 * @param playerinfo
 * @param config
 *
 * @return 
 */
inline bool player_online_award::ValidPlayer(const player_info & playerinfo, const player_template::online_giftbag_config & config)
{
	if(!__CheckLevel(playerinfo.level, config.require_min_level, config.require_max_level))
		return false;

	if(!__CheckRebornCnt(playerinfo.rebornCnt, config.renascence_count))
		return false;

	if(!__CheckGender(playerinfo.gender, config.require_gender))
		return false;

	if(!__CheckCls(playerinfo.cls, config.character_combo_id, config.character_combo_id2))
		return false;

	if(!__CheckRace(playerinfo.cls, config.require_race))
		return false;

	return true;
}

int player_online_award::ConvertToSeconds(struct tm & tmtime) const
{
	//时间只涉及小时、分钟和秒转化为具体的秒数
	return ( (tmtime.tm_hour * SECONDS_PER_HOUR) + (tmtime.tm_min * SECONDS_PER_MIN) + (tmtime.tm_sec) );
}

void player_online_award::InitOnlineAwardInfo(int id, onlineawardinfo & info, time_t t)
{
	time_t tmptomorrow;
	struct tm tm1;
    localtime_r(&t, &tm1);
	tm1.tm_mday += 1;
	tm1.tm_hour  = 0;
	tm1.tm_min	 = 0;
	tm1.tm_sec   = 0;
	tmptomorrow  = mktime(&tm1);

	const player_template::OnlineGiftbagConfigMap & config_list = player_template::GetOnlineGiftbagConfigMap();
	info.index				= GetSmallGiftBagsBegin(id, config_list);
	info.is_receive_awards	= false;
	info.time_passed		= 0;
	info.SetTomorrow(tmptomorrow, _tz_adjust);
	info.SetStartTime(t);
}

int player_online_award::GetSmallGiftBagsBegin(int id, const player_template::OnlineGiftbagConfigMap & config_list)
{
	player_template::OnlineGiftbagConfigMap::const_iterator it_config = config_list.find(id);
	if(config_list.end() == it_config)
	{                   
		return player_template::MAX_SMALL_ONLINE_GB_COUNT + 1;    
	} 

	for(size_t i = 0; i < it_config->second.small_gift_bags.size(); i++)
	{
		if(it_config->second.small_gift_bags[i].gift_bag_id > 0)
		{
			return i;
		}
	}

	return player_template::MAX_SMALL_ONLINE_GB_COUNT + 1;
}

bool player_online_award::GetSmallGiftBagsNext(int id, const player_template::OnlineGiftbagConfigMap & config_list, int index, int & nextindex) const
{
	//检查参数的合法性
	if(index < 0 || index >= player_template::MAX_SMALL_ONLINE_GB_COUNT)
	{
		return 0;
	}

	player_template::OnlineGiftbagConfigMap::const_iterator it_config = config_list.find(id);
	if(config_list.end() == it_config)
	{
		return 0;
	}

	int end_index  = index;
	int next_index = index;
	for(size_t i = index; i < it_config->second.small_gift_bags.size(); i++)
	{
		if(it_config->second.small_gift_bags[i].gift_bag_id > 0)
		{
			if(next_index == index)
			{
				next_index = i;
			}

			end_index = i;
		}
	}

	nextindex = next_index;
	return (index == end_index) ? false : true;
}

bool player_online_award::GetNextIndex(int id, int index, int & nextindex) const
{
	const player_template::OnlineGiftbagConfigMap & config_list = player_template::GetOnlineGiftbagConfigMap();
	return (GetSmallGiftBagsNext(id, config_list, index, nextindex));
}

int player_online_award::GetCurOnlineAwardInfo(std::vector<S2C::CMD::online_award_info::award_data> & info_list)
{
	const player_template::OnlineGiftbagConfigMap & config_list = player_template::GetOnlineGiftbagConfigMap();
	player_template::OnlineGiftbagConfigMap::const_iterator it_config;

	ONLINEAWARDINFO_MAP::iterator it;
	for(it = _info_map.begin(); it != _info_map.end(); ++it)
	{
		//已经全部领取的礼包不在发给客户端
		if(it->second.is_closed)
		{
			continue;
		}

		it_config = config_list.find(it->first);
		int interval_secs = it_config->second.small_gift_bags[it->second.index].deliver_interval;

		S2C::CMD::online_award_info::award_data tmpinfo;
		tmpinfo.award_id			= it->first;
		tmpinfo.index				= it->second.index;
		tmpinfo.small_giftbag_id	= it_config->second.small_gift_bags[it->second.index].gift_bag_id;
		tmpinfo.remaining_time		= it->second.GetStartTime() + interval_secs - g_timer.get_systime();

		info_list.push_back(tmpinfo);
	}

	return info_list.size();
}

bool player_online_award::GetSmallGiftbagID(int awardid, int index, int & id)
{
	//获取配置表
	const player_template::OnlineGiftbagConfigMap & config_list			= player_template::GetOnlineGiftbagConfigMap();
	player_template::OnlineGiftbagConfigMap::const_iterator it_config	= config_list.find(awardid);
	if(config_list.end() == it_config)
	{
		return false;
	}

	id = it_config->second.small_gift_bags[index].gift_bag_id;
	return true;
}

void player_online_award::Clear(void)
{
	_info_map.clear();
	_check_counter_down = 0;
}


void player_online_award::DebugCmdClearOnlineAward(player_online_award & rhs)
{
	rhs.Clear();
}

bool player_online_award::DebugIsActive(int id, player_online_award & rhs)
{
	return (rhs.IsActive(id));
}

void player_online_award::DebudDumpActiveAward(std::vector<int> & active_list, player_online_award & rhs)
{
	for(ONLINEAWARDINFO_MAP::iterator it = rhs._info_map.begin(); it != rhs._info_map.end(); ++it)
	{
		active_list.push_back(it->first);
	}
}



bool player_online_award::GetTimePassed(int awardid, time_t t, int & index, int & time_passed)
{
	ONLINEAWARDINFO_MAP::iterator it = _info_map.find(awardid);
	if(it == _info_map.end())
	{
		return false;
	}

	index		= it->second.index;
	time_passed = t - it->second.GetStartTime();

	return true;
}
