#ifndef __NETGAME_GS_TIMESEGMENT_H__
#define __NETGAME_GS_TIMESEGMENT_H__

#include <vector>
#include <map>
#include <time.h>


const int ERR_NEW_ALLOC    = -10;
const int ERR_START_THREAD = -11;
const int ERR_PARAM        = -12;
const int ERR_OPEN_FILE    = -13;

enum CHECK_TIMESEG{TS_SUCCESS = 0, TS_ERR_YEAR, TS_ERR_MONTH, TS_ERR_MONTHDAY, TS_ERR_WEEKDAY, TS_ERR_HOUR, TS_ERR_MIN, TS_ERR_ALL};



class time_segment
{
	
	enum TS_TYPE{NOTHING, INTERSECTION, UNIONSET};

public:
	struct entry_t
	{
		int min;
		int hour;
		int month;
		int year;
		int day_of_months;
		int day_of_week;

		bool is_inited;
		entry_t()
		{
			min    = -1;
			hour   = -1;
			month  = -1;
			year   = -1;
			day_of_months = -1;
			day_of_week   = -1;
			is_inited = false;
		}

		entry_t(const entry_t &rhs)
		{
			SetParam(rhs);
		}

		void SetParam(int __year, int __month, int __day_of_months, int __hour, int __min, int __day_of_week)
		{
			min	    	= __min	;
			hour		= __hour;	
			month		= __month;	
			year		= __year;	
			day_of_months	= __day_of_months;	
			day_of_week	= __day_of_week;

			is_inited   = true;
		}

		void SetParam(const entry_t & param)
		{
			min		= param.min;
			hour		= param.hour;	
			month		= param.month;	
			year		= param.year;	
			day_of_months	= param.day_of_months;	
			day_of_week	= param.day_of_week;

			is_inited = true;
		}

		void Clear()
		{
			min		    = -1;
			hour		= -1;	
			month		= -1;	
			year		= -1;	
			day_of_months	= -1;	
			day_of_week	= -1;

			is_inited   = false;
		}
	};

private:
	union FLAG
	{
		unsigned int flag;
		struct
		{
			char flag_start;
			char flag_end;
		};
	};
	
	union OVERLAY_FLAG
	{
		int64_t flag;
		struct
		{
			char year_flag;
			char month_flag;
			char mday_flag;
			char hour_flag;
			char min_flag;
		};
	};

	enum TIMESEG_MARK{TS_NULL = 0, TS_YEAR, TS_MONTH, TS_MDAY, TS_HOUR, TS_MIN, TS_WEEKDAY};

	struct timeseg_pair
	{
		entry_t start;
		entry_t end;
	};
	
public:
	time_segment();
	~time_segment();	

	//--------------------------------对外接口---------------------------------
	bool CheckValidity(const entry_t & start, const entry_t & end); 

	bool CheckTimeSegAvailable(const std::vector<entry_t> & atimeseg, bool is_intersection);

	bool IsInTimeSegment(const tm &curtime);

	int  GetNextTimeToStart(const tm &curtime);

	int  GetNextTimeToStop(const  tm &curtime);

	bool Init(std::vector<entry_t> & atimeseg, bool is_intersection);

	void Clear();

	bool HasTimeSeg();
	

private:
	//用于标记边界
	FLAG _year_mark, _month_mark, _monthday_mark, _weekday_mark, _hour_mark;

	//------------------------------通用计算函数---------------------------------
	inline bool IsLeapYear(int year)
	{
		if((year % 4) != 0)  return false;
		if((year % 400) == 0) return true;
		if((year % 100) == 0) return false;
		return true;
	}

	inline int GetMDay(int year, int mon)
	{
		static int mday[] = { 31,28,31,30,31,30,31,31,30,31,30,31};
		int d = mday[mon];
		if(mon == 1 && IsLeapYear(year)) d = d+1;
		return d;
	}

	//gs中使用的函数
	CHECK_TIMESEG IsValid(const entry_t &_start_time, const entry_t &_end_time);


	//------------------------判断时间是否在时间段范围内-------------------------
	void ResetTimeMark();

	inline void inline_markboundary(int cur, int start, int end, FLAG &mark)
	{
		if(cur == start)
			mark.flag_start = 1;
		else
			mark.flag_start = 0;

		if(cur == end)
			mark.flag_end = 1;
		else
			mark.flag_end = 0;
	}

	void MarkBoundary(const tm &cur, const entry_t &start, const entry_t &end,  TIMESEG_MARK mark, int prev_mark = 0);
	bool IsOutofBoundary(const tm &cur, const entry_t &start, const entry_t &end, TIMESEG_MARK mark);
	CHECK_TIMESEG CheckTimeSegment(const tm &curtime, const entry_t &start_time, const entry_t &end_time);


	//-----------判断新的输入和原来的时间段是否有交集----------
	void FillTimeSeg(tm &cur_fill, tm &start_fill, tm &end_fill, const entry_t &newtime, const entry_t &start, const entry_t &end, OVERLAY_FLAG &overlay_flag);
	
	inline void reset_tm_time(tm &desttime, tm &srctime)
	{
		memcpy(&desttime, &srctime, sizeof(tm));
	}

	inline bool check_outof_seconds(tm &cur, tm &start, tm &end)
	{
		time_t cur_sec, start_sec, end_sec;

		cur_sec   = mktime(&cur);
		start_sec = mktime(&start);
		end_sec   = mktime(&end);

		if(cur_sec < start_sec || cur_sec > end_sec)
		{
			return true;
		}

		return false;
	}

	bool IsOutofTimesegSeconds_old(tm &cur, tm &start, tm &end, tm &curfilled, TIMESEG_MARK flag);
	bool IsOutofTimesegSeconds_once(tm &cur, tm &start, tm &end, TIMESEG_MARK flag, bool is_on_end);
	bool IsOutofTimesegSeconds(tm &cur, tm &start, tm &end, tm &curfilled, TIMESEG_MARK flag, bool is_on_end);
	CHECK_TIMESEG CheckTimeSegOverlay(const entry_t &newtime, const entry_t &_start_time, const entry_t &_end_time);

	typedef std::multimap<int, timeseg_pair, std::less<int> > TSMAP;
	typedef std::multimap<int, timeseg_pair, std::less<int> >::iterator TSMAPITER;
	typedef std::multimap<int, timeseg_pair, std::less<int> >::reverse_iterator RTSMAPITER;
	
	//老接口，在新接口中调用
private:
	bool IsTimeSegOverlay(const entry_t &start, const entry_t &end, const std::vector<entry_t> &atimeseg, bool is_intersection);

//data
private:
	std::vector<entry_t> _atimeseg;
	TSMAP _timeseg_map;
	TS_TYPE _ts_type;
	int  _begin_index;

//func
private:
	inline bool IsInit();
	bool ChangeToMap(const entry_t &start, const entry_t &end, const std::vector<entry_t> & atimeseg);
	void FillTMTime(tm &tmtime, const entry_t &datetime);

	bool IsInterOverlay();
	bool IsUnionsetOverlay();
	inline bool is_week_overlay(const int curweek, const entry_t &start_time, const entry_t &end_time);
	bool IsWeekOverylay(const entry_t &curstart, const entry_t &curend, const entry_t &start, const entry_t &end);
    bool IsTimeSegEqual(const entry_t& start_new, const entry_t& end_new, const entry_t& start_old, const entry_t& end_old);
	int  CheckEqual(const entry_t &start_new, const entry_t &end_new, const std::vector<entry_t> & atimeseg, bool is_intersection);
    int  TimeCalculation(const tm &tt, const entry_t &target_time);

};

#endif
