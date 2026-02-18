#include "item_giftbox_set.h"
#include "../template/elementdataman.h"
#include "arandomgen.h"
#include "../player_imp.h"
#include "../actsession.h"

int item_giftbox_set::OnUse(item::LOCATION l ,size_t index, gactive_imp* imp,item * parent) const
{
	if( l != item::INVENTORY) return -1;

	gplayer_imp *pImp = (gplayer_imp *)imp;

	int item_num_list[4];
	memset(item_num_list, 0, sizeof(item_num_list));
	giftbox_set_essense* ess = GetEssenece(parent);
	if(!ess)
	{
		pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);	
		return -1;
	}	
	int left = ess->apoint;
	size_t slotNeed = 0;
	for(int i = 0; i < 4; i ++)
	{
		if(_giftbox_list[i].item_id > 0 && _giftbox_list[i].apoint > 0)
		{
			item_num_list[i] = left / _giftbox_list[i].apoint;
			int pile_limit = gmatrix::GetDataMan().get_item_pile_limit(_giftbox_list[i].item_id);	
			slotNeed += item_num_list[i] / pile_limit;
			if(item_num_list[i] % pile_limit > 0)
			{
				slotNeed ++;
			}
			left = left % _giftbox_list[i].apoint;
		}
	}

	if(pImp->GetInventory().IsFull() || pImp->GetInventory().GetEmptySlotCount() < slotNeed)
	{
		pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);	
		return -1;
	}

	for(size_t i = 0; i < 4; ++i)
	{
		if(item_num_list[i] > 0)
		{
			pImp->DeliverItem(_giftbox_list[i].item_id, item_num_list[i], true, -1, ITEM_INIT_TYPE_TASK);
			GLog::log(GLOG_INFO, "用户%d开启礼包%d获得了物品%d个数为%d", pImp->_parent->ID.id, _tid, _giftbox_list[i].item_id, item_num_list[i]);  
		}
	}
	return  1;	
}

void item_giftbox_set::SetGiftBoxAccPoint(int p, item* parent)
{
	giftbox_set_essense* ess = GetEssenece(parent);
	if(!ess)
	{
		return;
	}	
	ess->apoint = p;	
}

int item_giftbox_set::GetGiftBoxAccPoint(item* parent)
{
	giftbox_set_essense* ess = GetEssenece(parent);
	return ess ? ess->apoint : 0;
}
