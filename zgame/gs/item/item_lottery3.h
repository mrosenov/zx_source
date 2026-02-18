#ifndef __ONLINEGAME_GS_LOTTERY3_H__
#define __ONLINEGAME_GS_LOTTERY3_H__

#include "../item.h"
#include "../config.h"
#include "item_lottery.h"
typedef struct lua_State luaState;


/*
 *  ¿É¶Ñµþ²ÊÆ±°æ±¾
 */
class item_lottery3 : public item_lottery
{
private:
	int _award_item_id;

public:
	item_lottery3(int n, int award_item_id) : item_lottery(n), _award_item_id(award_item_id)
	{
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_LOTTERY3;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool LotteryCashing(gactive_imp* obj,item * parent, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period){return false;}
	
};

#endif //__ONLINEGAME_GS_LOTTERY3_H__
