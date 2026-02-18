#include "playermall.h"
#include "dbgprt.h"
#include <spinlock.h>


#define SECONDS_PER_DAY  (24 * 60 * 60)
#define SECONDS_PER_HOUR (60 * 60)
#define SECONDS_PER_MIN  (60)
#define SECONDS_ENDLESS  (-1)


namespace qgame{

bool
mall::AddGoods(const node_t & node)
{
	spin_autolock keeper(_lock);
	
	if(node.goods_id <= 0 || node.goods_count <= 0 || node.bonus_ratio < 0)
	{
		return false;
	}
	if(node.entry[0].cash_need <= 0) return false;
	_map.push_back(node);
	return true;
}

bool 
mall::QueryGoods(size_t index, node_t & n)
{
	spin_autolock keeper(_lock);

	if(index >= _map.size()) return false;
	n = _map[index];
/*	
	MAP::iterator it = _map.find(id);
	if(it == _map.end()) return false;
	n = it->second;
	*/
	return true;
}

bool 
mall::AddLimitGoods(const node_t & node, int index)
{
	_limit_goods.push_back(index_node_t(node, index));
	return true;
}

/*
size_t 
mall::QueryGoods(const int * id, node_t * n, size_t count)
{
	spin_autolock keeper(_lock);

	size_t index = 0;
	for(size_t i = 0; i < count; i ++)
	{
		MAP::iterator it = _map.find(id[i]);
		if(it == _map.end())  continue;
		n[index] = it->second;
		index ++;
	}
	return index;
}
*/

mall::sale_time::IDENTIFIECODE 
mall::sale_time::GetRemainingTime(time_t now, int & remain_secs)
{
	//先检查是否满足时间条件
	if(!CheckAvailable(now)) return RET_ERROR;

	switch(_type)
	{
		case TYPE_NOLIMIT:
		{
			remain_secs = SECONDS_ENDLESS;
			__PRINTF("\n\n\n\nERR：取永久物品的剩余时间！\n\n\n\n\n");
		}
		break;

		case TYPE_INTERZONE:
		{
			if(HAS_ENDTIME & _param)
			{
				remain_secs = _end_time - now;
			}
			else
			{
				remain_secs = SECONDS_ENDLESS;
			}
		}
		break;

		case TYPE_WEEK:
		case TYPE_MONTH:
		{
			struct tm tm1;
            localtime_r(&now, &tm1);
			int tmpsec = ConvertToSeconds(tm1);
			remain_secs = _end_time - tmpsec;
		}
		break;

		default:
		{
			__PRINTF("\n\n\n\nERR：商城限时功能type类型不对！\n\n\n\n\n");
		}
		break;
	}

	return RET_SUCCESS;
}

bool 
mall::sale_time::CheckAvailable(time_t t) const
{
	switch(_type)
	{
		case TYPE_NOLIMIT: 
			return true;

		case TYPE_INTERZONE:
		{
			if( (HAS_STARTTIME & _param) && (HAS_ENDTIME & _param) )
			{
				return (t >= _start_time && t < _end_time);
			}
			else if(HAS_STARTTIME & _param)
			{
				return (t >= _start_time);
			}
			else if(HAS_ENDTIME & _param)
			{
				return (t < _end_time);
			}
		}
		return false;

		case TYPE_WEEK:
		{
			struct tm tm1;
			localtime_r(&t, &tm1);
			if((1 << tm1.tm_wday) & _param)
			{
				int tmpsec = ConvertToSeconds(tm1);
				return (tmpsec >= _start_time && tmpsec < _end_time);
			}
		}
		return false;

		case TYPE_MONTH:
		{
			struct tm tm1;
			localtime_r(&t, &tm1);
			if( (1 << tm1.tm_mday) & _param )
			{
				int tmpsec = ConvertToSeconds(tm1);
				return (tmpsec >= _start_time && tmpsec < _end_time);
			}
		}
		return false;

		default: 
			return false;
	}

	return false;
}

int 
mall::sale_time::ConvertToSeconds(struct tm & tmtime) const
{
	//时间只涉及小时、分钟和秒转化为具体的秒数
	return ( (tmtime.tm_hour * SECONDS_PER_HOUR) + (tmtime.tm_min * SECONDS_PER_MIN) + (tmtime.tm_sec) );
}

bool
mall::sale_time::TrySetParam(int t, int start, int end, int p) 
{
	//检查参数是否有效
	if(!CheckParam(t, start, end, p)) return false;

	_type = t;
	_start_time = start;
	_end_time = end;
	_param = p;

	return true;
}

bool
mall::sale_time::CheckParam(int type, int start_time, int end_time, int param)
{
	switch(type)
	{
		case TYPE_NOLIMIT:
		{
			if(param) return false;
		}
		break;

		case TYPE_INTERZONE:
		{
			if( ((HAS_STARTTIME & param) && start_time < 0) 
			 || ((HAS_ENDTIME & param) && end_time < 0) ) 
			{
				return false;
			}
			if( !(HAS_STARTTIME & param) && !(HAS_ENDTIME & param) ) return false;
			if( (HAS_STARTTIME & param) && (HAS_ENDTIME & param) ) return (start_time < end_time);
		}
		break;

		case TYPE_WEEK:
		{
			return (param & 0x7f) && !(param & 0xffffff80) && (start_time < end_time);
		}
		break;

		case TYPE_MONTH:
		{
			return (param & 0xfffffffe) && !(param & 0x01) && (start_time < end_time);
		}
		break;

		default:
		{
			__PRINTF("\n\n\n\nERR：商城限时商品类型有误！\n\n\n\n");
		}
		return false;
	}

	return true;
}

}

