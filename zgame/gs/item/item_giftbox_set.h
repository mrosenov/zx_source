#ifndef __ONLINEGAME_GS_GIFTBOX_ITEM_SET_H__
#define __ONLINEGAME_GS_GIFTBOX_ITEM_SET_H__

#include "../item.h"
#include "../config.h"

struct giftbox_item
{
	unsigned int item_id;
	unsigned int apoint;
};

struct giftbox_set_essense
{
	int apoint;
};


class item_giftbox_set : public item_body
{
	int _open_time;
	giftbox_item _giftbox_list[4];

public:	
	item_giftbox_set(int time, giftbox_item items[4]) : _open_time(time)
	{
		_open_time = time;
		
		memcpy(_giftbox_list, items, sizeof(_giftbox_list));
		for(int i = 1; i < 4; i ++)
		{
			for(int j = i; j > 0; j --)
			{
				if(_giftbox_list[j].apoint > _giftbox_list[j - 1].apoint)
				{
					unsigned int item_id = _giftbox_list[j].item_id;
					unsigned int ap = _giftbox_list[j].apoint;
					_giftbox_list[j].item_id = _giftbox_list[j - 1].item_id;
					_giftbox_list[j].apoint = _giftbox_list[j - 1].apoint;
					_giftbox_list[j - 1].item_id = item_id;
					_giftbox_list[j - 1].apoint = ap;
				}
			}
		}
	}
	
	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_GIFTBOX_SET;}
	virtual	bool IsItemCanUse(item::LOCATION l, gactive_imp *pImp) const {return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(_open_time);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual void SetGiftBoxAccPoint(int p, item* parent);
	virtual int GetGiftBoxAccPoint(item* parent);

	giftbox_set_essense* GetEssenece(const item* parent) const
	{
		size_t len;
		const void* buf = parent->GetContent(len);
		if(len == sizeof(giftbox_set_essense))
		{
			return (giftbox_set_essense*) buf;
		}
		return NULL;
	}
};


#endif
