#ifndef __ONLINEGAME_GS_TOWNSCROLL_H__
#define __ONLINEGAME_GS_TOWNSCROLL_H__

#include "../item.h"
#include "../config.h"

class item_townscroll_paper : public item_body
{
public:
	item_townscroll_paper()
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_TOWN_SCROLL_PAPER;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(3);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
};

class item_townscroll : public item_body
{
	struct ts_pos
	{
		int tag;
		float x;
		float y;
		float z;
	};

public:
	item_townscroll()
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_TOWN_SCROLL;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
};
#endif



