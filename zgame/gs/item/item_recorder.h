#ifndef __ONLINEGAME_GS_ITEM_RECORDER_H__
#define __ONLINEGAME_GS_ITEM_RECORDER_H__

#include "../item.h"
#include "../config.h"

class item_recorder : public item_body
{
public:
	item_recorder()
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_RECORDER;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return false;}
};
#endif


