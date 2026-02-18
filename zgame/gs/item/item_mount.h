#ifndef __ONLINEGAME_GS_MOUNT_ITEM_H__
#define __ONLINEGAME_GS_MOUNT_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_mount : public item_body
{
	int 	_level_required;
	int 	_mount_id;
	float 	_mount_speed;
	bool 	_battle_ground_only;
	char	_mount_type; // 0 -普通坐骑 1-多人骑乘坐骑 
	char 	_reborn_required;
	int 	_kingdom_title_required;
	bool 	_loli_only; //只有萝莉(牵机)能做
public:
	item_mount(int mount_id, float mount_speed, int level_required, bool battle_ground_flag, char mount_type = 0, char reborn_required = 0, int kingdom_title_required = 0, bool loli_only = false)
	{
		_mount_id = mount_id;
		_mount_speed = mount_speed;
		_level_required = level_required;
		_battle_ground_only = battle_ground_flag;
		_mount_type = mount_type;
		_reborn_required = reborn_required;
		_kingdom_title_required = kingdom_title_required;
		_loli_only = loli_only;
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_MOUNT;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const;
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(0.f);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index, gactive_imp*,item * parent) const;
	virtual bool IsNeedCRC() { return false;}
	static void CollectAddons(item & parent, int * addons, size_t count);
};
#endif


