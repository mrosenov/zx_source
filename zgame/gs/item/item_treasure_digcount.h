#ifndef __ONLINEGAME_GS_TREASURE_DIGCOUNT__
#define __ONLINEGAME_GS_TREASURE_DIGCOUNT__

#include "../item.h"

class item_treasure_digcount : public item_body
{
	int _dig_count;

public:
	item_treasure_digcount(int count) : _dig_count(count)
	{}
	

	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_TREASURE_DIGCOUNT;}
	virtual	bool IsItemCanUse(item::LOCATION l, gactive_imp *pImp) const {return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(3);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;

};


#endif
