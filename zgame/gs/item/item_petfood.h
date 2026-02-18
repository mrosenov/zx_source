#ifndef __ONLINEGAME_GS_PETFOOD_ITEM_H__
#define __ONLINEGAME_GS_PETFOOD_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../petman.h"
#include <crc.h>

class item_pet_food: public item_body
{
public:
	enum
	{
		MIN_USAGE_NUM = 200, //一次性消耗物品最少消耗量
	};
	struct use_arg_t
	{
		int index;
	};
public:
	int _pet_level_min;
	int _pet_level_max;
	unsigned int _pet_type_mask; //种族限制
	int _food_usage; // 0 补充饱食度 1 补充hp 2 补充mp 3 补充亲密度当前值 4 降低年龄 5 增加寿命 6 增加宠物经验
	unsigned int _food_type; // 0 多次使用的 只有_food_usage 为012时才有用 1 一次性消耗品
	int _food_value;

protected:
	//标记该类物品可以使用
	virtual bool IsItemCanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) const { return sizeof(int) == buf_size; }
	virtual bool IsItemBroadcastUse() const { return false; }
	virtual int OnUse(item::LOCATION l,size_t index,gactive_imp* imp,item* parent) const;
	virtual int OnUse(item::LOCATION l,size_t index,gactive_imp* imp,item* parent,const char* arg,size_t arg_size) const;

public:
	item_pet_food(int pet_level_min,int pet_level_max,unsigned int pet_type_mask,int food_usage,unsigned int food_type,int food_value):
		_pet_level_min(pet_level_min),_pet_level_max(pet_level_max),_pet_type_mask(pet_type_mask),_food_usage(food_usage),_food_type(food_type),_food_value(food_value)
	{}
	
public:
	virtual ITEM_TYPE GetItemType() const
	{
		return ITEM_TYPE_PET_FOOD;
	}

	void InitFromShop(gplayer_imp* pImp,item* parent,int value) const;
};

#endif

