#ifndef __ONLINEGAME_GS_COUPLE_JUMP_H__
#define __ONLINEGAME_GS_COUPLE_JUMP_H__

#include "../item.h"
#include "../config.h"

class item_couple_jump : public item_body
{
public:
	item_couple_jump()
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_COUPLE_JUMP;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(10);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
};
#endif


