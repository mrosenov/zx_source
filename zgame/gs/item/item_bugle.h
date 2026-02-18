#ifndef __ONLINEGAME_GS_BUGLE_H__
#define __ONLINEGAME_GS_BUGLE_H__

#include <stddef.h>
#include "../item.h"
#include "../config.h"
#include "item_equip.h"
#include <crc.h>

class item_bugle : public item_body             
{               
	int _emote_id;
public:
	item_bugle(int emote):_emote_id(emote)
	{}

	virtual void OnPutIn(item::LOCATION  l ,size_t index , gactive_imp* obj,item * parent) const
	{
		if(l == item::BODY)
		{
			if(!parent->IsActive())
			{
				OnActivate(index,obj,parent);
				parent->SetActive(true);
			}

		}
	}

	virtual void OnTakeOut(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
	{
		if(l == item::BODY)
		{
			Deactivate(index,obj,parent);
		}
	}

	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_BUGLE; }
	virtual bool VerifyRequirement(item_list & list,gactive_imp*,const item * parent) const 
	{
		return true;
	}

	virtual void OnActivate(size_t index,gactive_imp*, item * parent) const;
	virtual void OnDeactivate(size_t index,gactive_imp*, item * parent) const;
	virtual int OnGetEquipMask() const { return item::EQUIP_MASK_BUGLE;}
};

#endif
