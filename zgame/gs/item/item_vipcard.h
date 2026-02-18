#ifndef __ONLINEGAME_GS_VIPCARD_ITEM_H__
#define __ONLINEGAME_GS_VIPCARD_ITEM_H__ 

#include "../item.h"
#include "../config.h"
#include <common/types.h>

class item_vipcard : public item_body
{
	int _duration;

public:
	item_vipcard(int dur)
	{
		_duration = dur;
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_VIPCARD;}
	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const;
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(3.0f);}
	virtual int OnUse(item::LOCATION l,size_t index, gactive_imp* imp,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index, gactive_imp*,item * parent) const;
	virtual void OnPutIn(item::LOCATION  l ,size_t index , gactive_imp* imp,item * parent) const;
};

#endif
