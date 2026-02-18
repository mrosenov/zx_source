#ifndef __ONLINEGAME_GS_PETAUTOFOOD_ITEM_H__
#define __ONLINEGAME_GS_PETAUTOFOOD_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../petman.h"
#include <crc.h>

class item_pet_auto_food: public item_body
{
public:
	enum USE_MASK
	{
		HP = 0x01,
		VP = 0x02,
		HUNGER = 0x04,
	};
	struct use_arg_t
	{
		int index;
		int mask;
	};
	int _food_value;
	float _hp_gen;
	float _mp_gen;
	float _hunger_gen;

protected:
	//标记该类物品可以使用
	virtual bool IsItemCanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) const { return sizeof(int) * 2 == buf_size; }
	virtual bool IsItemBroadcastUse() const { return false; }
	virtual int OnUse(item::LOCATION l,size_t index,gactive_imp* imp,item* parent) const;
	virtual int OnUse(item::LOCATION l,size_t index,gactive_imp* imp,item* parent,const char* arg,size_t arg_size) const;

public:
	item_pet_auto_food(int food_value,float hp_gen,float mp_gen,float hunger_gen):
		_food_value(food_value),_hp_gen(hp_gen),_mp_gen(mp_gen),_hunger_gen(hunger_gen)
	{}
	
public:
	virtual ITEM_TYPE GetItemType() const
	{
		return ITEM_TYPE_PET_AUTO_FOOD;
	}

	void InitFromShop(gplayer_imp* pImp,item* parent,int value) const;
};

#endif

