#ifndef __ONLINEGAME_GS_LOTTERY_H__
#define __ONLINEGAME_GS_LOTTERY_H__

#include "../item.h"
#include "../config.h"
typedef struct lua_State luaState;

class item_lottery : public item_body
{
protected:
	int _number_limit;		//可以扔几次骰子
	enum 
	{
		MAX_DICE_COUNT = 32
	};
	bool GenerateTicket(lua_State * L, char * ticktes, int index) const ;
	bool GetLotteryBonus(lua_State * L,char * tickets,int n[6]) const ;

	bool CheckBonusLevel(int level) const
	{
		return level >=0; 
	}

	bool CheckBonusMoney(int money) const
	{
		return money>= 0 && money <= 200000000;
	}

	bool CheckBonusItem(int item, int count) const
	{
		return count >= 0;
	}

	bool CheckBonusBindPeriod(int bind, int period) const
	{
		return (bind ==1 || bind == 0) && (period >= 0 && period <= 3600*24*365*10);
	}
	
public:
	item_lottery(int n):_number_limit(n)
	{
		//前面已经检查过了 
		ASSERT(n > 0 && n <= MAX_DICE_COUNT);
	}

	static int CheckScript(int id, int n);

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_LOTTERY;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return false;}
	virtual bool LotteryCashing(gactive_imp* obj,item * parent, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period);
};
#endif


