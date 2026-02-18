#ifndef __ONLINEGAME_GS_BOTTLE_ITEM_H_
#define __ONLINEGAME_GS_BOTTLE_ITEM_H_

#include "../item.h"
#include "../config.h"
#include "../cooldowncfg.h"
#include <common/types.h>

struct bottle_essence
{
	size_t capacity;
};

class item_bottle : public item_body
{
	enum
	{
		BOTTLE_TYPE_HP = 0,
		BOTTLE_TYPE_MP,
		BOTTLE_TYPE_DP,
	};

	int _bottle_type;
	int _potion_tids[20];
	int _require_level;
	int _require_reborn_count;
	int _cooldown_time;

public:
	item_bottle(int bottle_type, const int potion_tids[20], int level, int reborn_cnt, int cooldown) : _bottle_type(bottle_type), _require_level(level), _require_reborn_count(reborn_cnt), _cooldown_time(cooldown)
	{
		memcpy(_potion_tids,potion_tids, sizeof(_potion_tids));
	}
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_BOTTLE;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}

	int GetCoolDownIndex() const;
	void SetCoolDown(gactive_imp *imp) const;

	int FillBottle(gactive_imp* obj, item* parent, size_t bottle_index, int potion_type, int potion_tid, int potion_value, size_t potion_canuse_zone, size_t potion_cannotuse_zone, size_t potion_cantrade_zone, int& potion_left);
	bool IsItemCanUse(item::LOCATION l,gactive_imp* imp) const;
};

#endif /*__ONLINEGAME_GS_BOTTLE_ITEM_H_*/
