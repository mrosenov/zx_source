#include <stdio.h>
#include "timesegment.h"
#include "dbgprt.h"


time_segment::time_segment()
{
	_ts_type = NOTHING;
	_begin_index = -1;

	_atimeseg.clear();
	_timeseg_map.clear();
	ResetTimeMark();
}


time_segment::~time_segment()
{
	_ts_type = NOTHING;
	_begin_index = -1;

	_atimeseg.clear();
	_timeseg_map.clear();
	ResetTimeMark();
}


void
time_segment::Clear()
{
	_ts_type = NOTHING;
	_begin_index = -1;

	_atimeseg.clear();
	_timeseg_map.clear();
	ResetTimeMark();
}


bool 
time_segment::CheckValidity(const entry_t &start, const entry_t &end)
{
	bool ret = false;
	if( TS_SUCCESS == IsValid(start, end) )
	{
		ret = true;
	}

	return ret;
}


bool
time_segment::Init(std::vector<entry_t> & atimeseg, bool is_intersection)
{
	if( 0 == atimeseg.size() )
	{
		return false;
	}

	if(is_intersection)
	{
		_ts_type = INTERSECTION;
	}
	else
	{
		_ts_type = UNIONSET;
	}

	_atimeseg.swap(atimeseg);

	return true;
}


bool
time_segment::IsInit()
{
	if(NOTHING == _ts_type || 0 == _atimeseg.size())
	{
		return false;
	}

	return true;
}


bool
time_segment::HasTimeSeg()
{
	return IsInit();
}


void 
time_segment::FillTMTime(tm &tmtime, const entry_t &datetime)
{
	if(datetime.min >= 0)
	{
		tmtime.tm_min = datetime.min;
	}

	if(datetime.hour >= 0)
	{
		tmtime.tm_hour = datetime.hour;
	}

	if(datetime.day_of_months >= 0)
	{
		tmtime.tm_mday = datetime.day_of_months;
	}

	if(datetime.month >= 0)
	{
		tmtime.tm_mon  = datetime.month;
	}

	if(datetime.year >= 0)
	{
		tmtime.tm_year = datetime.year - 1900;
	}
}


bool
time_segment::ChangeToMap(const entry_t &start, const entry_t &end, const std::vector<entry_t> & atimeseg)
{
	_timeseg_map.clear();
	if(_timeseg_map.size() != 0)
	{
		//fprintf(stderr, "ChangeToMap error!\n");
		return false;
	}

	struct tm tmstart, tmend, cur;
	struct tm *pdate;
	time_t timenow, start_sec, end_sec; 

	time(&timenow);
	pdate = localtime(&timenow);
	memcpy(&cur, pdate, sizeof(tm));
	
	for(unsigned int i = 0; i < atimeseg.size(); i += 2)
	{
		entry_t start_time, end_time;
		start_time.SetParam(atimeseg[i]);
		end_time.SetParam(atimeseg[i+1]);
		
		memcpy(&tmstart, &cur, sizeof(tm));
		memcpy(&tmend, &cur, sizeof(tm));
		FillTMTime(tmstart, start_time);
		FillTMTime(tmend, end_time);

		start_sec = mktime(&tmstart);
		end_sec   = mktime(&tmend);

		int res_sec = end_sec - start_sec;
		if(res_sec < 0)
		{
			//fprintf(stderr, "start_sec greater than end_sec!!!! \n");
			_timeseg_map.clear();
			return false;
		}

		timeseg_pair tmptspair;
		tmptspair.start.SetParam(start_time);
		tmptspair.end.SetParam(end_time);
		_timeseg_map.insert( std::pair<int, timeseg_pair>(res_sec, tmptspair) );
	}

	memcpy(&tmstart, &cur, sizeof(tm));
	memcpy(&tmend, &cur, sizeof(tm));
	FillTMTime(tmstart, start);
	FillTMTime(tmend, end);

	start_sec = mktime(&tmstart);
	end_sec   = mktime(&tmend);

	int res_sec = end_sec - start_sec;
	if(res_sec < 0)
	{
		//fprintf(stderr, "start_sec greater than end_sec!!!! \n");
		_timeseg_map.clear();
		return false;
	}

	timeseg_pair tmptspair;
	tmptspair.start.SetParam(start);
	tmptspair.end.SetParam(end);
	_timeseg_map.insert( std::pair<int, timeseg_pair>(res_sec, tmptspair) );


	return true;
}


inline bool
time_segment::is_week_overlay(const int curweek, const entry_t &start_time, const entry_t &end_time)
{
	int timespan, cur_timespan;	

	timespan = (end_time.day_of_week + 7 - start_time.day_of_week) % 7;
	cur_timespan = (curweek + 7 - start_time.day_of_week) % 7;
	if(cur_timespan > timespan)
		return false;
	
	return true;
}


bool 
time_segment::IsWeekOverylay(const entry_t &curstart, const entry_t &curend, const entry_t &start, const entry_t &end)
{
	int curweek = curstart.day_of_week;
	if(is_week_overlay(curweek, start, end))
	{
		return true;
	}

	curweek = curend.day_of_week;
	if(is_week_overlay(curweek, start, end))
	{
		return true;
	}

	curweek = start.day_of_week;
	if(is_week_overlay(curweek, curstart, curend))
	{
		return true;
	}

	curweek = end.day_of_week;
	if(is_week_overlay(curweek, curstart, curend))
	{
		return true;
	}

	return false;
}


bool 
time_segment::IsTimeSegEqual(const entry_t& start_new, const entry_t& end_new, const entry_t& start_old, const entry_t& end_old)
{
	if( (start_new.year == start_old.year) 
	 && (start_new.month == start_old.month) 
	 && (start_new.day_of_months == start_old.day_of_months) 
	 && (start_new.hour == start_old.hour) 
	 && (start_new.min  == start_old.min) 
	 && (start_new.day_of_week == start_old.day_of_week) )
	{
		if( (end_new.year == end_old.year) 
		 && (end_new.month == end_old.month) 
		 && (end_new.day_of_months == end_old.day_of_months) 
		 && (end_new.hour == end_old.hour) 
		 && (end_new.min  == end_old.min) 
		 && (end_new.day_of_week == end_old.day_of_week) )
		{
			return true;
		}
	}

	return false;
}


int 
time_segment::CheckEqual(const entry_t &start_new, const entry_t &end_new, const std::vector<entry_t> & atimeseg, bool is_intersection)
{
	for(unsigned int i = 0; i < atimeseg.size(); i += 2)
	{
		entry_t start_time, end_time;
		start_time.SetParam(atimeseg[i]);
		end_time.SetParam(atimeseg[i+1]);

		if(IsTimeSegEqual(start_new, end_new, start_time, end_time))
		{
			if(is_intersection)
			{
				return INTERSECTION;
			}
			else
			{
				return UNIONSET;
			}
		}
	}

	return NOTHING;
}


bool
time_segment::IsInterOverlay()
{
	bool is_overlay = true;

	TSMAPITER testiter = _timeseg_map.begin();
	while( testiter != _timeseg_map.end() )
	{
		entry_t start_new, end_new;
		start_new.SetParam( (*testiter).second.start );
		end_new.SetParam( (*testiter).second.end );
		_timeseg_map.erase(testiter++);

		//从大到小进行Overlay检查
		RTSMAPITER tsmapiter;
		for(tsmapiter = _timeseg_map.rbegin(); tsmapiter != _timeseg_map.rend(); ++tsmapiter)
		{
			entry_t start_time, end_time;
			start_time.SetParam( (*tsmapiter).second.start );
			end_time.SetParam( (*tsmapiter).second.end );

			if(TS_SUCCESS != CheckTimeSegOverlay(start_new, start_time, end_time))
			{
				if( TS_SUCCESS != CheckTimeSegOverlay(start_time, start_new, end_new) 
				 && TS_SUCCESS != CheckTimeSegOverlay(end_time, start_new, end_new) )
				{
					is_overlay = false;
					break;
				}
				else
				{
					if(start_new.day_of_week >= 0 && start_time.day_of_week >= 0)
					{
						if(!IsWeekOverylay(start_new, end_new, start_time, end_time))
						{
							is_overlay = false;
							break;
						}
					}
				}
			}
			else
			{
				if(start_new.day_of_week >= 0 && start_time.day_of_week >= 0)
				{
					if(!IsWeekOverylay(start_new, end_new, start_time, end_time))
					{
						is_overlay = false;
						break;
					}
				}
			}
		}

		if(is_overlay)
		{
			continue;
		}

		is_overlay = true;
		for(tsmapiter = _timeseg_map.rbegin(); tsmapiter != _timeseg_map.rend(); ++tsmapiter)
		{
			entry_t start_time, end_time;
			start_time.SetParam( (*tsmapiter).second.start );
			end_time.SetParam( (*tsmapiter).second.end );

			if(TS_SUCCESS != CheckTimeSegOverlay(end_new, start_time, end_time))
			{
				if( TS_SUCCESS != CheckTimeSegOverlay(start_time, start_new, end_new) 
		         && TS_SUCCESS != CheckTimeSegOverlay(end_time, start_new, end_new) )
				{
					is_overlay = false;
					return false;
				}
				else
				{
					if(start_new.day_of_week >= 0 && start_time.day_of_week >= 0)
					{
						if(!IsWeekOverylay(start_new, end_new, start_time, end_time))
						{
							is_overlay = false;
							return false;
						}
					}
				}
			}
			else
			{
				if(end_new.day_of_week >= 0 && start_time.day_of_week >= 0)
				{
					if(!IsWeekOverylay(start_new, end_new, start_time, end_time))
					{
						is_overlay = false;
						return false;
					}
				}
			}
		}
	}

	return true;
}


bool
time_segment::IsUnionsetOverlay()
{
	TSMAPITER testiter = _timeseg_map.begin();
	while( testiter != _timeseg_map.end() )
	{
		entry_t start_new, end_new;
		start_new.SetParam( (*testiter).second.start );
		end_new.SetParam( (*testiter).second.end );
		_timeseg_map.erase(testiter++);

		//从大到小进行Overlay检查
		RTSMAPITER tsmapiter;
		for(tsmapiter = _timeseg_map.rbegin(); tsmapiter != _timeseg_map.rend(); ++tsmapiter)
		{
			entry_t start_time, end_time;
			start_time.SetParam( (*tsmapiter).second.start );
			end_time.SetParam( (*tsmapiter).second.end );

			if(TS_SUCCESS == CheckTimeSegOverlay(start_new, start_time, end_time))
			{
				if(start_new.day_of_week >= 0 && start_time.day_of_week >= 0)
				{
					if(IsWeekOverylay(start_new, end_new, start_time, end_time))
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			} 
			else //两个段同时出现任意的时候时间的大小范围无法计算时
			{
				if( TS_SUCCESS == CheckTimeSegOverlay(start_time, start_new, end_new) 
				 || TS_SUCCESS == CheckTimeSegOverlay(end_time, start_new, end_new) )
				{
					if(start_new.day_of_week >= 0 && start_time.day_of_week >= 0)
					{
						if(IsWeekOverylay(start_new, end_new, start_time, end_time))
						{
							return true;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}

		for(tsmapiter = _timeseg_map.rbegin(); tsmapiter != _timeseg_map.rend(); ++tsmapiter)
		{
			entry_t start_time, end_time;
			start_time.SetParam( (*tsmapiter).second.start );
			end_time.SetParam( (*tsmapiter).second.end );

			if(TS_SUCCESS == CheckTimeSegOverlay(end_new, start_time, end_time))
			{
				if(end_new.day_of_week >= 0 && start_time.day_of_week >= 0)
				{
					if(IsWeekOverylay(start_new, end_new, start_time, end_time))
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
			else //两个段同时出现任意的时候时间的大小范围无法计算时
			{
				if( TS_SUCCESS == CheckTimeSegOverlay(start_time, start_new, end_new) 
		         || TS_SUCCESS == CheckTimeSegOverlay(end_time, start_new, end_new) )
				{
					if(start_new.day_of_week >= 0 && start_time.day_of_week >= 0)
					{
						if(IsWeekOverylay(start_new, end_new, start_time, end_time))
						{
							return true;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool 
time_segment::IsTimeSegOverlay(const entry_t &start, const entry_t &end, const std::vector<entry_t> &atimeseg, bool is_intersection)
{
	bool is_overlay = true;
	entry_t start_new, end_new;
	start_new.SetParam( start );
	end_new.SetParam( end );

	if( NOTHING != CheckEqual(start_new, end_new, atimeseg, is_intersection) )
	{
		if(is_intersection)
			return false;
		else
			return true;
	}

	//要在IsInterOverlay(),IsUnionsetOverlay()之前调用
	ChangeToMap(start_new, end_new, atimeseg);

	if(is_intersection)
	{
		is_overlay = IsInterOverlay();
	}
	else
	{
		is_overlay = IsUnionsetOverlay();
	}

	return is_overlay;
}


bool 
time_segment::CheckTimeSegAvailable(const std::vector<entry_t> & atimeseg, bool is_intersection)
{
	std::vector<entry_t> temptimeseg;

	if( atimeseg.size() == 0 )
	{
		return false;
	}

	for(size_t i = 0; i < atimeseg.size(); i += 2)
	{
		if( is_intersection == IsTimeSegOverlay(atimeseg[i], atimeseg[i+1], temptimeseg, is_intersection) )
		{
			temptimeseg.push_back(atimeseg[i]);
			temptimeseg.push_back(atimeseg[i+1]);
		}
		else
		{
			return false;
		}
	}

	return true;
}


CHECK_TIMESEG
time_segment::IsValid(const entry_t &_start_time, const entry_t &_end_time)
{
	//标识是否存在
	bool year_flag  = false;
	bool month_flag = false;
	bool mday_flag  = false;
	bool wday_flag  = false;
	bool hour_flag  = false;
	bool min_flag   = false;

	//开始时间年不为任意，结束时间的年也不能为任意，月、日、周、小时、分钟情况一致
	if(_start_time.year >= 0)
	{
		if(_end_time.year < 0 || _start_time.year > _end_time.year)
			return TS_ERR_YEAR;

		year_flag = true;
	}

	//年为任意时，开始月不能小于结束月,其他时间也这样检查
	if(_start_time.month >= 0)
	{
		if( _end_time.month < 0 || (!year_flag && (_start_time.month > _end_time.month))
		 || _start_time.month > 11 || _end_time.month > 11)
			return TS_ERR_MONTH;

		//极端情况1
		if( (_start_time.year == _end_time.year) 
		 && (_start_time.month > _end_time.month) )
		{
			return TS_ERR_MONTH;
		}

		month_flag = true;
	}

	if(_start_time.day_of_months > 0)
	{
		if( _end_time.day_of_months < 0 || (!month_flag && (_start_time.day_of_months > _end_time.day_of_months))
		 || _start_time.day_of_months > 31 || _end_time.day_of_months > 31)
			return TS_ERR_MONTHDAY;

		//极端情况2
		if( (_start_time.year == _end_time.year)
		 && (_start_time.month == _end_time.month)
		 && (_start_time.day_of_months > _end_time.day_of_months) )
		{
			return TS_ERR_MONTHDAY;
		}

		 
		//检查具体月的天数是否符合逻辑，必须是在月有值的情况
		if(year_flag && month_flag)
		{
			int start_mdays = GetMDay(_start_time.year, _start_time.month);
			int end_mdays   = GetMDay(_end_time.year, _end_time.month);
			if(start_mdays < _start_time.day_of_months || end_mdays < _end_time.day_of_months)
				return TS_ERR_MONTHDAY;
		}
		else if(month_flag)
		{
			//没有具体的年时，2月按平年计算
			int start_mdays = GetMDay(2001, _start_time.month);
			int end_mdays   = GetMDay(2001, _end_time.month);
			if(start_mdays < _start_time.day_of_months || end_mdays < _end_time.day_of_months)
				return TS_ERR_MONTHDAY;
		}

		mday_flag = true;
	}

	//周是独立的，而且是循环的
	if(_start_time.day_of_week >= 0)
	{
		if(_end_time.day_of_week < 0 || _start_time.day_of_week > 6 || _end_time.day_of_week > 6)
			return TS_ERR_WEEKDAY;

		wday_flag = true;
	}

	if(_start_time.hour >= 0)
	{
		if( _end_time.hour < 0 || (!mday_flag && (_start_time.hour > _end_time.hour))
		 || _start_time.hour > 23 || _end_time.hour > 23)
			return TS_ERR_HOUR;

		//极端情况3
		if( (_start_time.year == _end_time.year)
		 && (_start_time.month == _end_time.month)
		 && (_start_time.day_of_months == _end_time.day_of_months) 
		 && (_start_time.hour > _end_time.hour) )
		{
			return TS_ERR_HOUR;
		}

		//极端情况4
		if(!month_flag && (_start_time.day_of_months == _end_time.day_of_months) 
		 && (_start_time.hour > _end_time.hour) )
			return TS_ERR_HOUR;

		//极端情况5
		if( wday_flag && (_start_time.day_of_week == _end_time.day_of_week)
	 	 && (_start_time.hour > _end_time.hour) )
			return TS_ERR_HOUR;

		hour_flag = true;
	}

	if(_start_time.min >= 0)
	{
		if( _end_time.min < 0 || (!hour_flag && (_start_time.min > _end_time.min))
		 || _start_time.min > 59 || _end_time.min > 59)
			return TS_ERR_MIN;

		//极端情况6，前面都相等，开始的分钟大于结束的分钟，悲催的程序员！
		if( (_start_time.year == _end_time.year)
		 && (_start_time.month == _end_time.month)
		 && (_start_time.day_of_months == _end_time.day_of_months) 
		 && (_start_time.hour == _end_time.hour)
		 && (_start_time.min > _end_time.min) )
			return TS_ERR_MIN;

		//极端情况7
		if( (!month_flag || !mday_flag)
		 && (_start_time.day_of_months == _end_time.day_of_months)
		 && (_start_time.hour == _end_time.hour) 
		 && (_start_time.min > _end_time.min) )
			return TS_ERR_MIN;

		//极端情况8
		if( wday_flag && (_start_time.day_of_week == _end_time.day_of_week)
		 && (_start_time.hour == _end_time.hour)
		 && (_start_time.min > _end_time.min) )
			return TS_ERR_MIN;

		min_flag = true;
	}

	//极端情况9
	if(!year_flag && !month_flag && !mday_flag && !wday_flag && !hour_flag && !min_flag)
		return TS_ERR_ALL;

	return TS_SUCCESS;
}


void 
time_segment::ResetTimeMark()
{
	_year_mark.flag_start  = -1;
	_year_mark.flag_end    = -1;

	_month_mark.flag_start = -1;
	_month_mark.flag_end   = -1;

	_monthday_mark.flag_start  = -1;
	_monthday_mark.flag_end    = -1;

	_weekday_mark.flag_start   = -1;
	_weekday_mark.flag_end     = -1;

	_hour_mark.flag_start  = -1;
	_hour_mark.flag_end    = -1;
}



//有一项为-1则这项的前面所有项都应该mark为-1
void 
time_segment::MarkBoundary(const tm &cur, const entry_t &start, const entry_t &end,  TIMESEG_MARK mark, int prev_mark)
{
	int year_real = cur.tm_year + 1900;
	switch(mark)
	{
		case TS_YEAR:
			{
				inline_markboundary(year_real, start.year, end.year, _year_mark);
			}
			break;

		case TS_MONTH:
			{
				inline_markboundary(cur.tm_mon, start.month, end.month, _month_mark);
			}
			break;

		case TS_MDAY:
			{
				inline_markboundary(cur.tm_mday, start.day_of_months, end.day_of_months, _monthday_mark);
				if(prev_mark)
				{
					_year_mark.flag_start = -1;
					_year_mark.flag_end   = -1;
				}
			}
			break;

		case TS_HOUR:
			{
				inline_markboundary(cur.tm_hour, start.hour, end.hour, _hour_mark);
				if(prev_mark)
				{
					_year_mark.flag_start = -1;
					_year_mark.flag_end   = -1;

					_month_mark.flag_start = -1;
					_month_mark.flag_end   = -1;
				}
			}
			break;

		case TS_WEEKDAY:
			{
				inline_markboundary(cur.tm_wday, start.day_of_week, end.day_of_week, _weekday_mark);
			}
			break;

		default:
			break;
	}
}


bool 
time_segment::IsOutofBoundary(const tm &cur, const entry_t &start, const entry_t &end, TIMESEG_MARK mark)
{
	switch(mark)
	{
		case TS_YEAR:
			{
				int year_real = cur.tm_year + 1900;
				if( (year_real < start.year) 
				 || (year_real > end.year) )
				{
					return true;
				}

			}
			break;

		case TS_MONTH:
			{
				//如果年在边界上用月份来判断
				if( _year_mark.flag_start && (cur.tm_mon < start.month) )
				{
					return true;
				}

				if( _year_mark.flag_end && (cur.tm_mon > end.month) )
				{
					return true;
				}

			}
			break;

		case TS_MDAY:
			{
				if( _year_mark.flag_start && _month_mark.flag_start
						&& (cur.tm_mday < start.day_of_months))
				{
					return true;
				}

				if( _year_mark.flag_end && _month_mark.flag_end
						&& (cur.tm_mday > end.day_of_months) )
				{
					return true;
				}
			}
			break;

		case TS_HOUR:
			{
				if( _year_mark.flag_start && _month_mark.flag_start && _monthday_mark.flag_start
				 && (cur.tm_hour < start.hour) )
				{
					return true;
				}

				if( _year_mark.flag_end && _month_mark.flag_end && _monthday_mark.flag_end 
				 && (cur.tm_hour > end.hour) )
				{
					return true;
				}
			}
			break;

		case TS_MIN:
			{
				if( _year_mark.flag_start && _month_mark.flag_start && _monthday_mark.flag_start
				 && _hour_mark.flag_start && (cur.tm_min < start.min) )
				{
					return true;
				}
				
				if( _year_mark.flag_end && _month_mark.flag_end && _monthday_mark.flag_end
				 && _hour_mark.flag_end && (cur.tm_min > end.min) )
				{
					return true;
				}

			}
			break;
			
		default:
			break;
	}

	return false;
}


CHECK_TIMESEG
time_segment::CheckTimeSegment(const tm &curtime, const entry_t &start_time, const entry_t &end_time)
{
	int timespan     = 0;
	int cur_timespan = 0;

	ResetTimeMark();

	if(start_time.year >= 0)
	{
		if(IsOutofBoundary(curtime, start_time, end_time, TS_YEAR))
		{
			return TS_ERR_YEAR;
		}

		MarkBoundary(curtime, start_time, end_time, TS_YEAR);
	}

	if(start_time.month >= 0)
	{
		if(start_time.year < 0) //一年内
		{
			timespan = (end_time.month + 12 - start_time.month) % 12;
			cur_timespan = (curtime.tm_mon + 12 - start_time.month) % 12;
			if(cur_timespan > timespan)
				return TS_ERR_MONTH;

			MarkBoundary(curtime, start_time, end_time, TS_MONTH, -1);
		}
		else if(_year_mark.flag)
		{
			//如果年在边界上用月份来判断
			if(IsOutofBoundary(curtime, start_time, end_time, TS_MONTH))
			{
				return TS_ERR_MONTH;
			}

			MarkBoundary(curtime, start_time, end_time, TS_MONTH);
		}
	}

	if(start_time.day_of_months > 0) //day_of_months不会等于零
	{
		if(start_time.month < 0) //一个月内
		{
			int ndays = GetMDay(curtime.tm_year + 1900, curtime.tm_mon);
			//对日进行调整，比如现在是二月，start-end是28~31号
			//如果不调整，1号也会被计算在范围之内
			int dayofmonths = end_time.day_of_months;
			if(curtime.tm_mday < start_time.day_of_months)
				return TS_ERR_MONTHDAY;
			if(dayofmonths > ndays)
				dayofmonths = ndays;

			timespan = (dayofmonths + ndays - start_time.day_of_months) % ndays;
			cur_timespan = (curtime.tm_mday + ndays - start_time.day_of_months) % ndays;
			if(cur_timespan > timespan)
				return TS_ERR_MONTHDAY;

			MarkBoundary(curtime, start_time, end_time, TS_MDAY, -1);
		}
		else if(_month_mark.flag)
		{
			if(IsOutofBoundary(curtime, start_time, end_time, TS_MDAY))
			{
				return TS_ERR_MONTHDAY;
			}

			MarkBoundary(curtime, start_time, end_time, TS_MDAY);
		}
	}

	if(start_time.day_of_week >= 0)
	{
		timespan = (end_time.day_of_week + 7 - start_time.day_of_week) % 7;
		cur_timespan = (curtime.tm_wday + 7 - start_time.day_of_week) % 7;
		if(cur_timespan > timespan)
			return TS_ERR_WEEKDAY;
	}

	if(start_time.hour >= 0)
	{
		//日和星期相比星期的优先级大
		if(start_time.day_of_months <= 0)//一天内
		{
			timespan = (end_time.hour + 24 - start_time.hour) % 24;
			cur_timespan = (curtime.tm_hour + 24 - start_time.hour) % 24;
			if(cur_timespan > timespan)
				return TS_ERR_HOUR;

			MarkBoundary(curtime, start_time, end_time, TS_HOUR, -1);
		}
		else if(_monthday_mark.flag)
		{
			if(IsOutofBoundary(curtime, start_time, end_time, TS_HOUR))
			{
				return TS_ERR_HOUR;
			}

			MarkBoundary(curtime, start_time, end_time, TS_HOUR);
		}
	}

	//分钟为-1的时候表示不关心分钟，比如每月3~5号激活，这里不需要分钟
	if(start_time.min >= 0)
	{
		if(start_time.hour < 0)//一小时内
		{
			timespan = (end_time.min + 60 - start_time.min) % 60;
			cur_timespan = (curtime.tm_min + 60 - start_time.min) % 60;
			if(cur_timespan > timespan)
				return TS_ERR_MIN;
		}
		else if(_hour_mark.flag)
		{
			if(IsOutofBoundary(curtime, start_time, end_time, TS_MIN))
			{
				return TS_ERR_MIN;
			}
		}
	}

	return TS_SUCCESS;
}


bool
time_segment::IsInTimeSegment(const tm &curtime)
{
	//交集
	if( INTERSECTION == _ts_type )
	{
		for(size_t i = 0; i < _atimeseg.size(); i += 2)
		{
			if(TS_SUCCESS !=  CheckTimeSegment(curtime, _atimeseg[i], _atimeseg[i+1]) )
			{
				return false;
			}
		}

		return true;
	}
	else if( UNIONSET == _ts_type ) //合集
	{
		if(_begin_index < 0)
		{
			for(size_t i = 0; i < _atimeseg.size(); i += 2)
			{
				if(TS_SUCCESS ==  CheckTimeSegment(curtime, _atimeseg[i], _atimeseg[i+1]) )
				{
					_begin_index = i;
					return true;
				}
			}

			return false;
		}
		else
		{
			if(TS_SUCCESS ==  CheckTimeSegment(curtime, _atimeseg[_begin_index], _atimeseg[_begin_index+1]) )
			{
				return true;
			}
			else
			{
				_begin_index = -1;
				return false;
			}
		}
	}

	__PRINTF("控制器时间段既不是交集也不是合集，不知道怎么处理！\n");

	return false;
}


void
time_segment::FillTimeSeg(tm &cur_fill, tm &start_fill, tm &end_fill, const entry_t &newtime, const entry_t &start, const entry_t &end, OVERLAY_FLAG &overlay_flag)
{
	struct tm *pdate;
	time_t timenow; 
	time(&timenow);
	pdate = localtime(&timenow);

	memcpy(&start_fill, pdate, sizeof(tm));
	mktime(&start_fill);
	memcpy(&end_fill, &start_fill, sizeof(tm));
	memcpy(&cur_fill, &start_fill, sizeof(tm));

	bool year_flag  = false;
	bool month_flag = false;
	bool mday_flag  = false;
	bool hour_flag  = false;
	bool min_flag   = false;

	bool cur_year_flag  = false;
	bool cur_month_flag = false;
	bool cur_mday_flag  = false;
	bool cur_hour_flag  = false;
	bool cur_min_flag   = false;


	//-------------------------填充newtime--------------------------------
	if(newtime.year >= 0)
	{
		cur_fill.tm_year = newtime.year - 1900;
		cur_year_flag = true;
	}
	else if(start.year >= 0)
	{
		cur_fill.tm_year = start.year - 1900;
	}

	if(newtime.month >= 0)
	{
		cur_fill.tm_mon = newtime.month;
		cur_month_flag = true;
	}
	else if(start.month >= 0)
	{
		cur_fill.tm_mon = start.month;
	}

	if(newtime.day_of_months >= 0)
	{
		cur_fill.tm_mday = newtime.day_of_months;
		cur_mday_flag = true;
	}
	else if(start.day_of_months >= 0)
	{
		cur_fill.tm_mday = start.day_of_months;
	}

	if(newtime.hour >= 0)
	{
		cur_fill.tm_hour = newtime.hour;
		cur_hour_flag = true;
	}
	else if(start.hour >= 0)
	{
		cur_fill.tm_hour = start.hour;
	}

	if(newtime.min >= 0)
	{
		cur_fill.tm_min = newtime.min;
		cur_min_flag = true;
	}
	else if(start.min >= 0)
	{
		cur_fill.tm_min = start.min;
	}

	//---------------------填充start和end-------------------------
	if(start.year >= 0)
	{
		start_fill.tm_year = start.year - 1900;
		end_fill.tm_year   = end.year - 1900;
		year_flag = true;
	}
	else
	{
		start_fill.tm_year = cur_fill.tm_year;
		end_fill.tm_year   = cur_fill.tm_year;
	}

	if(start.month >= 0)
	{
		start_fill.tm_mon = start.month;
		end_fill.tm_mon   = end.month;
		month_flag = true;
	}
	else
	{
		start_fill.tm_mon = cur_fill.tm_mon;
		end_fill.tm_mon   = cur_fill.tm_mon;
	}

	if(start.day_of_months >= 0)
	{
		start_fill.tm_mday = start.day_of_months;
		end_fill.tm_mday   = end.day_of_months;
		mday_flag = true;
	}
	else
	{
		start_fill.tm_mday = cur_fill.tm_mday;
		end_fill.tm_mday   = cur_fill.tm_mday;
	}

	if(start.hour >= 0)
	{
		start_fill.tm_hour = start.hour;
		end_fill.tm_hour   = end.hour;
		hour_flag = true;
	}
	else
	{
		start_fill.tm_hour = cur_fill.tm_hour;
		end_fill.tm_hour   = cur_fill.tm_hour;
	}

	if(start.min >= 0)
	{
		start_fill.tm_min = start.min;
		end_fill.tm_min   = end.min;
		min_flag = true;
	}
	else
	{
		start_fill.tm_min = cur_fill.tm_min;
		end_fill.tm_min   = cur_fill.tm_min;
	}

	//重新调整cur_fill
	if( (newtime.month < 0 || start.month < 0)
	 && (year_flag && cur_year_flag) )
	{
		if( (cur_fill.tm_year == end_fill.tm_year) )
		{
			cur_fill.tm_mon = end_fill.tm_mon;
			overlay_flag.mday_flag = 1;
		}
	}


	if( (newtime.day_of_months < 0 || start.day_of_months < 0)
	 && (  (year_flag && cur_year_flag)
	    || (month_flag && cur_month_flag)) )
	{
		if( (cur_fill.tm_year == end_fill.tm_year)
		 && (cur_fill.tm_mon  == end_fill.tm_mon) )
		{
			cur_fill.tm_mday = end_fill.tm_mday;
			overlay_flag.hour_flag = 1;
		}
	}


	if( (newtime.hour < 0 || start.hour < 0)
	 && (  (year_flag && cur_year_flag)
	    || (month_flag && cur_month_flag)
		|| (mday_flag && cur_mday_flag) ) )
	{
		if( (cur_fill.tm_year == end_fill.tm_year)
		 && (cur_fill.tm_mon  == end_fill.tm_mon)
		 && (cur_fill.tm_mday == end_fill.tm_mday) )
		{
			cur_fill.tm_hour = end_fill.tm_hour;
			overlay_flag.min_flag = 1;
		}
	}


	if( (newtime.min < 0 || start.min < 0)
	 && (  (year_flag && cur_year_flag)
	    || (month_flag && cur_month_flag)
	    || (mday_flag && cur_mday_flag)
	    || (hour_flag && cur_hour_flag)) )
	{
		if( (cur_fill.tm_year == end_fill.tm_year)
		 && (cur_fill.tm_mon  == end_fill.tm_mon)
		 && (cur_fill.tm_mday == end_fill.tm_mday)
		 && (cur_fill.tm_hour == end_fill.tm_hour) )
		{
			cur_fill.tm_min = end_fill.tm_min;
		}
	}
}


bool 
time_segment::IsOutofTimesegSeconds_old(tm &cur, tm &start, tm &end, tm &curfilled, TIMESEG_MARK flag)
{
	switch(flag)
	{
		case TS_MIN:
			{
				cur.tm_year = start.tm_year;
				cur.tm_mon  = start.tm_mon;
				cur.tm_mday = start.tm_mday;
				cur.tm_hour = start.tm_hour;

				if(check_outof_seconds(cur, start, end))
				{
					cur.tm_hour += 1;
					if(check_outof_seconds(cur, start, end))
					{
						return true;
					}
					curfilled.tm_hour += 1;
				}
			}
			break;

		case TS_HOUR:
			{
				cur.tm_year = start.tm_year;
				cur.tm_mon  = start.tm_mon;
				cur.tm_mday = start.tm_mday;

				if(check_outof_seconds(cur, start, end))
				{
					cur.tm_mday += 1;
					if(check_outof_seconds(cur, start, end))
					{
						return true;
					}
					curfilled.tm_mday += 1;
				}
			}
			break;

		case TS_MDAY:
			{
				cur.tm_year = start.tm_year;
				cur.tm_mon  = start.tm_mon;

				if(check_outof_seconds(cur, start, end))
				{
					cur.tm_mon += 1;
					if(check_outof_seconds(cur, start, end))
					{
						return true;
					}
					curfilled.tm_mon += 1;
				}
			}
			break;

		case TS_MONTH:
			{
				cur.tm_year = start.tm_year;

				if(check_outof_seconds(cur, start, end))
				{
					cur.tm_year += 1;
					if(check_outof_seconds(cur, start, end))
					{
						return true;
					}
					curfilled.tm_year += 1;
				}
			}
			break;

		case TS_YEAR:
			{
				if(check_outof_seconds(cur, start, end))
					return true;
			}
			break;

		default:
			break;
	}

	return false;
}


bool 
time_segment::IsOutofTimesegSeconds_once(tm &cur, tm &start, tm &end, TIMESEG_MARK flag, bool is_on_end)
{
	tm tempstart, tempend;

	switch(flag)
	{
		case TS_MIN:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;
					cur.tm_mon  = end.tm_mon;
					cur.tm_mday = end.tm_mday;
					cur.tm_hour = end.tm_hour;

					memcpy(&tempstart, &start, sizeof(tm));
					tempstart.tm_year = end.tm_year;
					tempstart.tm_mon  = end.tm_mon;
					tempstart.tm_mday = end.tm_mday;
					tempstart.tm_hour = end.tm_hour;


					if(check_outof_seconds(cur, tempstart, end))
					{
						return true;
					}
				}
				else
				{	
					cur.tm_year = start.tm_year;
					cur.tm_mon  = start.tm_mon;
					cur.tm_mday = start.tm_mday;
					cur.tm_hour = start.tm_hour;

					memcpy(&tempend, &end, sizeof(tm));
					tempend.tm_year = start.tm_year;
					tempend.tm_mon  = start.tm_mon;
					tempend.tm_mday = start.tm_mday;
					tempend.tm_hour = start.tm_hour;


					if(check_outof_seconds(cur, start, tempend))
					{
						return true;
					}
				}
			}
			break;

		case TS_HOUR:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;
					cur.tm_mon  = end.tm_mon;
					cur.tm_mday = end.tm_mday;

					memcpy(&tempstart, &start, sizeof(tm));
					tempstart.tm_year = end.tm_year;
					tempstart.tm_mon  = end.tm_mon;
					tempstart.tm_mday = end.tm_mday;


					if(check_outof_seconds(cur, tempstart, end))
					{
						return true;
					}
				}
				else
				{
					cur.tm_year = start.tm_year;
					cur.tm_mon  = start.tm_mon;
					cur.tm_mday = start.tm_mday;

					memcpy(&tempend, &end, sizeof(tm));
					tempend.tm_year = start.tm_year;
					tempend.tm_mon  = start.tm_mon;
					tempend.tm_mday = start.tm_mday;


					if(check_outof_seconds(cur, start, tempend))
					{
						return true;
					}
				}
			}
			break;

		case TS_MDAY:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;
					cur.tm_mon  = end.tm_mon;

					memcpy(&tempstart, &start, sizeof(tm));
					tempstart.tm_year = end.tm_year;
					tempstart.tm_mon  = end.tm_mon;


					if(check_outof_seconds(cur, tempstart, end))
					{
						return true;
					}
				}
				else
				{
					cur.tm_year = start.tm_year;
					cur.tm_mon  = start.tm_mon;

					memcpy(&tempend, &end, sizeof(end));
					tempend.tm_year = start.tm_year;
					tempend.tm_mon  = start.tm_mon;


					if(check_outof_seconds(cur, start, tempend))
					{
						return true;
					}
				}
			}
			break;

		case TS_MONTH:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;

					memcpy(&tempstart, &start, sizeof(tm));
					tempstart.tm_year = end.tm_year;

					if(check_outof_seconds(cur, tempstart, end))
					{
						return true;
					}
				}
				else
				{
					cur.tm_year = start.tm_year;

					memcpy(&tempend, &end, sizeof(tm));
					tempend.tm_year = start.tm_year;

					if(check_outof_seconds(cur, start, tempend))
					{
						return true;
					}
				}
			}
			break;

		case TS_YEAR:
			{
				if(check_outof_seconds(cur, start, end))
				{
					return true;
				}
			}
			break;

		default:
			break;
	}

	return false;
}



bool 
time_segment::IsOutofTimesegSeconds(tm &cur, tm &start, tm &end, tm &curfilled, TIMESEG_MARK flag, bool is_on_end)
{
	switch(flag)
	{
		case TS_MIN:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;
					cur.tm_mon  = end.tm_mon;
					cur.tm_mday = end.tm_mday;
					cur.tm_hour = end.tm_hour;
					
					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_hour -= 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_hour -= 1;
					}
				}
				else
				{	
					cur.tm_year = start.tm_year;
					cur.tm_mon  = start.tm_mon;
					cur.tm_mday = start.tm_mday;
					cur.tm_hour = start.tm_hour;

					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_hour += 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_hour += 1;
					}
				}
			}
			break;

		case TS_HOUR:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;
					cur.tm_mon  = end.tm_mon;
					cur.tm_mday = end.tm_mday;

					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_mday -= 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_mday -= 1;
					}
				}
				else
				{
					cur.tm_year = start.tm_year;
					cur.tm_mon  = start.tm_mon;
					cur.tm_mday = start.tm_mday;

					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_mday += 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_mday += 1;
					}
				}
			}
			break;

		case TS_MDAY:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;
					cur.tm_mon  = end.tm_mon;

					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_mon -= 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_mon -= 1;
					}
				}
				else
				{
					cur.tm_year = start.tm_year;
					cur.tm_mon  = start.tm_mon;

					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_mon += 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_mon += 1;
					}
				}
			}
			break;

		case TS_MONTH:
			{
				if(is_on_end)
				{
					cur.tm_year = end.tm_year;

					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_year -= 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_year -= 1;
					}
				}
				else
				{
					cur.tm_year = start.tm_year;

					if(check_outof_seconds(cur, start, end))
					{
						cur.tm_year += 1;
						if(check_outof_seconds(cur, start, end))
						{
							return true;
						}
						curfilled.tm_year += 1;
					}
				}
			}
			break;

		case TS_YEAR:
			{
				if(check_outof_seconds(cur, start, end))
				{
					return true;
				}
			}
			break;

		default:
			break;
	}

	return false;
}


CHECK_TIMESEG 
time_segment::CheckTimeSegOverlay(const entry_t &newtime, const entry_t &_start_time, const entry_t &_end_time)
{
	struct tm cur, start, end;
	struct tm curfilled, startfilled, endfilled;

	//填充时间段各个值
	OVERLAY_FLAG overlay_flag;
	overlay_flag.flag = 0;
	FillTimeSeg(curfilled, startfilled, endfilled, newtime, _start_time, _end_time, overlay_flag);

	
	//分钟
	if(_start_time.min >= 0)
	{
		reset_tm_time(cur, curfilled);
		reset_tm_time(start, startfilled);
		reset_tm_time(end, endfilled);

		if(_start_time.hour < 0 && newtime.hour < 0)
		{
			if(IsOutofTimesegSeconds_once(cur, start, end, TS_MIN, overlay_flag.min_flag))
			{
				return TS_ERR_MIN;
			}
		}
		else if(_start_time.hour < 0 || newtime.hour < 0)
		{
			if(IsOutofTimesegSeconds(cur, start, end, curfilled, TS_MIN, overlay_flag.min_flag))
			{
				return TS_ERR_MIN;
			}
		}
	}

	//小时
	if(_start_time.hour >= 0)
	{
		reset_tm_time(cur, curfilled);
		reset_tm_time(start, startfilled);
		reset_tm_time(end, endfilled);

		if(_start_time.day_of_months < 0 && newtime.day_of_months < 0)
		{
			if(IsOutofTimesegSeconds_once(cur, start, end, TS_HOUR, overlay_flag.hour_flag))
			{
				return TS_ERR_HOUR;
			}
		}
		else if(_start_time.day_of_months < 0 || newtime.day_of_months < 0)
		{
			if(IsOutofTimesegSeconds(cur, start, end, curfilled, TS_HOUR, overlay_flag.hour_flag))
			{
				return TS_ERR_HOUR;
			}
		}
	}

	//日
	if(_start_time.day_of_months >= 0)
	{
		reset_tm_time(cur, curfilled);
		reset_tm_time(start, startfilled);
		reset_tm_time(end, endfilled);

		if(_start_time.month < 0 && newtime.month < 0)
		{
			if(IsOutofTimesegSeconds_once(cur, start, end, TS_MDAY, overlay_flag.mday_flag))
			{
				return TS_ERR_MONTHDAY;
			}
		}
		else if(_start_time.month < 0 || newtime.month < 0)
		{
			if(IsOutofTimesegSeconds(cur, start, end, curfilled, TS_MDAY, overlay_flag.mday_flag))
			{
				return TS_ERR_MONTHDAY;
			}
		}
	}

	//月
	if(_start_time.month >= 0)
	{
		reset_tm_time(cur, curfilled);
		reset_tm_time(start, startfilled);
		reset_tm_time(end, endfilled);

		if(_start_time.year < 0 && newtime.year < 0)
		{
			if(IsOutofTimesegSeconds_once(cur, start, end, TS_MONTH, overlay_flag.month_flag))
			{
				return TS_ERR_MONTH;
			}
		}
		else if(_start_time.year < 0 || newtime.year < 0)
		{
			if(IsOutofTimesegSeconds(cur, start, end, curfilled, TS_MONTH, overlay_flag.month_flag))
			{
				return TS_ERR_MONTH;
			}
		}
	}

	//年
	/*if(_start_time.year >= 0 && newtime.year >= 0)
	{
		if(newtime.year > _start_time.year && newtime.year < _end_time.year)
		{
			return TS_SUCCESS;
		}
	}*/

	reset_tm_time(cur, curfilled);
	reset_tm_time(start, startfilled);
	reset_tm_time(end, endfilled);

	if(IsOutofTimesegSeconds(cur, start, end, curfilled, TS_YEAR, overlay_flag.year_flag))
	{
		return TS_ERR_YEAR;
	}

	return TS_SUCCESS;
}


int 
time_segment::TimeCalculation(const tm &tt, const entry_t &target_time)
{
	int offset = 0;

	int year = target_time.year;
	int month = target_time.month;
	int day_of_months = target_time.day_of_months;
	int day_of_week = target_time.day_of_week;
	int hour = target_time.hour;
	int min = target_time.min;

	if(year >= 0 && year != (tt.tm_year + 1900)) 
	{
		//年份不符合
		if(tt.tm_year + 1901 != year) return 3600;
		if(tt.tm_mon != 11) return 3600;
		if(tt.tm_mday != 31) return 3600;
		return (23 - tt.tm_hour)*3600 + (59-tt.tm_min)+(60-tt.tm_sec);

	}
	if(month >= 0 && month != tt.tm_mon) 
	{
		if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

		//如果可能是月末的23点50分以后，则等待到0点的时间
		return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);
	}


	//ASSERT(day_of_months < 0 || day_of_week < 0);
	if(day_of_months >= 0 && day_of_months != tt.tm_mday)
	{
		offset = (day_of_months - tt.tm_mday) * 24 * 3600;
	}
	if(day_of_week >= 0 && day_of_week != tt.tm_wday)
	{
		offset = (day_of_week - tt.tm_wday) * 24 * 3600;
	}

	if(hour >=0 && hour != tt.tm_hour)
	{
		offset += (hour - tt.tm_hour)*3600;
	}

	//ASSERT(min >= 0);
	if(min >=0 && min != tt.tm_min)
	{
		offset += (min - tt.tm_min)*60;
	}

	offset -= tt.tm_sec;

	if(offset <= -60)
	{
		//时间超出了一分钟，考虑计算下一次可能事件
		if(hour < 0)
		{
			for(int h = tt.tm_hour; h < 23 && offset < 0; h++)
			{
				offset += 3600;
			}
			if(offset > 0)
				return offset;
			else
			{
				//靠小时调整无法完成，则需要调整更大的数值
				//所以将小时归0
				offset -= 3600*23;
			}
		}

		if(day_of_months < 0 || day_of_week < 0)
		{
			if(day_of_week >= 0)
			{
				//day of_month 是 *
				//进行周调整 
				offset %= 3600*24*7;
				offset += 3600*24*7;
				if(offset > 600) offset = 600;
				return offset;

			}
			else if(day_of_months >= 0)
			{
				//定日不定周，需要进行月份调整 
				//直接跳过10分钟
				if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

				//如果可能是月末的23点50分以后，则等待到0点的时间
				return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);

			}
			else
			{
				//任意一天，需要进行日调整
				offset %= 3600*24;
				offset += 3600*24;
				if(offset > 600) offset = 600;
				return offset;
			}
		}

		//定日定周的 进行月份调整
		if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

		//如果可能是月末的23点50分以后，则等待到0点的时间
		return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);

	}

	return offset;

}

int  
time_segment::GetNextTimeToStart(const tm &curtime)
{
	int next_time = 0;
	int temp;

	//交集
	if( INTERSECTION == _ts_type )
	{
		for(size_t i = 0; i < _atimeseg.size(); i += 2)
		{
			temp = TimeCalculation(curtime, _atimeseg[i]);
			if(temp > next_time)
			{
				next_time = temp;
			}
		}
	}
	else if( UNIONSET == _ts_type )//合集
	{
		if(_begin_index < 0)
		{
			next_time = 60000;
			for(size_t i = 0; i < _atimeseg.size(); i += 2)
			{
				temp = TimeCalculation(curtime, _atimeseg[i]);
				if(temp < next_time)
				{
					next_time = temp;
				}
			}
		}
		else
		{
			__PRINTF("有误：GetNextTimeToStart(), _begin_index居然大于0");
		}
	}

	return next_time;
}

int  
time_segment::GetNextTimeToStop(const  tm &curtime)
{
	int next_time = 0;
	int temp = 0;

	//交集
	if( INTERSECTION == _ts_type )
	{
		next_time = 60000;
		for(size_t i = 0; i < _atimeseg.size(); i += 2)
		{
			temp = TimeCalculation(curtime, _atimeseg[i+1]);
			if(temp < next_time)
			{
				next_time = temp;
			}
		}
	}
	else if( UNIONSET == _ts_type )//合集
	{
		if(_begin_index >= 0)
		{
			next_time = TimeCalculation(curtime, _atimeseg[_begin_index+1]);
		}
		else
		{
			__PRINTF("有误：GetNextTimeToStop(), _begin_index居然小于0");
		}
	}

	return next_time;
}

