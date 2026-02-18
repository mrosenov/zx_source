#ifndef __ONLINE_GAME_GS_ITEM_AIRCRAFT_H__
#define __ONLINE_GAME_GS_ITEM_AIRCRAFT_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include <crc.h>

class item_aircraft : public item_body
{
	float _speed;
	int   _level_required;
	int64_t   _class_required;
	int64_t	  _class_required1;
public:
	item_aircraft(float speed, int level_require, int64_t class_require, int64_t class_require1)
	{}

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_AIRCRAFT;}
	virtual void OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const;
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const;
	virtual void OnActivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void OnDeactivate(size_t index,gactive_imp* obj, item * parent) const;

	virtual int OnGetEquipMask() const { return item::EQUIP_MASK_WING; }
};
#endif

class item_aircraft_stamina_potion : public item_body
{
	int _min_cost;
	int _max_stamina;
public:
	item_aircraft_stamina_potion(int min_cost, int max_stamina):_min_cost(min_cost),_max_stamina(max_stamina)
	{
		ASSERT(min_cost >= 0);
	}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return false;}

};

