#ifndef __ONLINE_GAME_GS_ANTI_WALLOW_H__
#define __ONLINE_GAME_GS_ANTI_WALLOW_H__

#include <stdio.h>
#include <sys/types.h>

namespace anti_wallow
{

struct punitive_param
{
	bool active;
	float exp;
	float item;
	float money;
	float task_exp;
	float task_sp;
	float task_money;
};

enum 
{
	MAX_WALLOW_LEVEL = 4
};


const punitive_param & Get(size_t level);
void SetParam(size_t level, const punitive_param & param);

void AdjustNormalExp(size_t level, int64_t & exp);
void AdjustTaskExp(size_t level, int64_t & exp);
void AdjustNormalMoneyItem(size_t level, float & money, float & item);
void AdjustTaskMoney(size_t level, int & money);

}
#endif

