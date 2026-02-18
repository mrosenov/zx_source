#ifndef __ONLINEGAME_GS_LOTTERY_TY_H__
#define __ONLINEGAME_GS_LOTTERY_TY_H__

#include "../item.h"
#include "../config.h"
typedef struct lua_State luaState;

#pragma pack(1)
struct lottery_ty_essence
{
	int exp;
	char ticket;
};
#pragma pack()

class item_lottery_ty : public item_body
{
protected:
	int _exp_get;
	int _exp_level[7];
	int _cost_item_tid;
	int _cost_item_cnt;
	
	bool GenerateTicket(lua_State * L, int& exp, char& ticket) const ;
	bool GetLotteryBonus(lua_State * L, int n[6], char& ticket) const ;

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
	item_lottery_ty(int exp_get, const int exp_level[7], int item_tid, int item_cnt)
	{
		_exp_get = exp_get;
		memcpy(_exp_level, exp_level, sizeof(_exp_level));
		_cost_item_tid = item_tid;
		_cost_item_cnt = item_cnt;
	}

	static int CheckScript(int id);

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_LOTTERY_TY;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return false;}
	virtual bool LotteryCashing(gactive_imp* obj,item * parent, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period);

	lottery_ty_essence* GetEssenece(const item* parent) const
	{
		size_t len;
		const void* buf = parent->GetContent(len);
		if(len == sizeof(lottery_ty_essence))
		{
			return (lottery_ty_essence*) buf;
		}
		return NULL;
	}

	int GetLevelByExp(int exp) const;
};
#endif


