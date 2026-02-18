#ifndef __ONLINEGAME_GS_PKVALUE_RESET_ITEM_H__
#define __ONLINEGAME_GS_PKVALUE_RESET_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_pk_reset : public item_body
{
public:
	item_pk_reset()
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_PK_RESET;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(4);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}
};
#endif


