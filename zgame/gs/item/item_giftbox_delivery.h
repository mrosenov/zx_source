#ifndef __ONLINEGAME_GS_GIFTBOX_ITEM_H__
#define __ONLINEGAME_GS_GIFTBOX_ITEM_H__

#include "../item.h"
#include "../config.h"

struct gift_item
{
	unsigned int item_id;
	float prob;
	unsigned int num_min;
	unsigned int num_max;
	unsigned int is_bind;
	unsigned int effect_time;	
};

struct giftbox_delivery_essence
{
	int item_id;
	int item_cnt;
	bool bind;
	int effect_time;
};

class item_giftbox_delivery: public item_body
{
	int _open_time;
	abase::vector<gift_item> _gift_group_list[4];
	abase::vector<gift_item> _gift_random_list;
public:	
	item_giftbox_delivery(int time, abase::vector<gift_item>  gift_group_list[], abase::vector<gift_item> & gift_random_list) : _open_time(time)
	{
		_open_time = time;
		for(size_t i = 0; i < 4; ++i)
		{
			_gift_group_list[i] = gift_group_list[i];
		}
		_gift_random_list = gift_random_list;
	}

	giftbox_delivery_essence* GetEssenece(const item* parent) const
	{
		size_t len;
		const void* buf = parent->GetContent(len);
		if(len == sizeof(giftbox_delivery_essence))
		{
			return (giftbox_delivery_essence*) buf;
		}
		return NULL;
	}
	
	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_GIFTBOX_DELIVERY;}
	virtual	bool IsItemCanUse(item::LOCATION l, gactive_imp *pImp) const {return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(_open_time);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool DeliveryGiftBox(gactive_imp* obj, item* parent);
};


#endif
