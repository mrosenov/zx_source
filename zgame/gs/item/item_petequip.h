#ifndef __ONLINEGAME_GS_PETEQUIP_ITEM_H__
#define __ONLINEGAME_GS_PETEQUIP_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../petman.h"
#include <crc.h>

class item_pet_equip: public item_body
{
public:
	item_pet_equip()
	{}
	
	virtual void InitFromShop(gplayer_imp* pImp,item* parent,int value) const;
public:
	virtual ITEM_TYPE GetItemType() const
	{
		return ITEM_TYPE_PET_EQUIP;
	}
};
#endif

