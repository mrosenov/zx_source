#ifndef __ONLINEGAME_GS_RECIPE_ITEM_H__
#define __ONLINEGAME_GS_RECIPE_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_recipe : public item_body
{
	int _recipe_id;
	int _require_produce_level;
public:
	item_recipe(int id,int require_level):_recipe_id(id),_require_produce_level(require_level)
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_RECIPE;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(3);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
};
#endif


