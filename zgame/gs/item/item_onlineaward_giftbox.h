#ifndef __ONLINEGAME_GS_ONLINEAWARD_GIFTBOX_ITEM_H__
#define __ONLINEGAME_GS_ONLINEAWARD_GIFTBOX_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_onlineaward_giftbox : public item_body
{
	int _open_time;

public:
	item_onlineaward_giftbox(int time) : _open_time(time)
	{}

	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_ONLINEAWARD_GIFTBOX;}
	virtual bool IsItemCanUse(item::LOCATION l, gactive_imp *pImp) const {return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(_open_time);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
};

#endif

