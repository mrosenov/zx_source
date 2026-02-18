#include "antiwallow.h"

namespace anti_wallow
{

static punitive_param list[MAX_WALLOW_LEVEL] = {{0}};



const punitive_param & GetParam(size_t level)
{
	if(level >= MAX_WALLOW_LEVEL)
	{
		level = MAX_WALLOW_LEVEL - 1;
	}
	return list[level];
}

void SetParam(size_t level, const punitive_param & param)
{
	if(level >= MAX_WALLOW_LEVEL)
	{
		level = MAX_WALLOW_LEVEL - 1;
	}
	list[level] = param;
	list[level].active = true;
}

void AdjustNormalExp(size_t level, int64_t & exp)
{
	const punitive_param & param = GetParam(level);
	exp = (int64_t)(exp * param.exp + 0.5f);
}

void AdjustTaskExp(size_t level, int64_t & exp)
{
	const punitive_param & param = GetParam(level);
	exp = (int64_t)(exp * param.task_exp + 0.5f);
}

void AdjustNormalMoneyItem(size_t level, float & money, float & item)
{
	const punitive_param & param = GetParam(level);
	money *= param.money;
	item *= param.item;
}

void AdjustTaskMoney(size_t level, int & money)
{
	const punitive_param & param = GetParam(level);
	money = (int)(money * param.task_money + 0.5f);
}
}

