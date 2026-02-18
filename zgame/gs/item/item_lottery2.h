#ifndef __ONLINEGAME_GS_LOTTERY2_H__
#define __ONLINEGAME_GS_LOTTERY2_H__

#include "../item.h"
#include "../config.h"
#include "item_lottery.h"
typedef struct lua_State luaState;


/*
 *  ¿É¶Ñµþ²ÊÆ±°æ±¾
 */
class item_lottery2 : public item_lottery
{

public:
	item_lottery2(int n) : item_lottery(n)
	{
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_LOTTERY2;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool LotteryCashing(gactive_imp* obj,item * parent, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period){return false;}
	
};

#endif //__ONLINEGAME_GS_LOTTERY2_H__
