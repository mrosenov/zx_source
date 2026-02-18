#ifndef __ONLINEGAME_GS_BATTLEFLAG_ITEM_H__
#define __ONLINEGAME_GS_BATTLEFLAG_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_battleflag : public item_body
{
	int _cooltime;
public:
	item_battleflag(int cooltime)
	{
		_cooltime = cooltime;
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_BATTLEFLAG;}
	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true; }
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(0.f);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
};
#endif


