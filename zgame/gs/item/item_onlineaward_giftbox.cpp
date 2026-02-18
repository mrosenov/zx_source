#include "item_onlineaward_giftbox.h"
#include "../template/elementdataman.h"
#include "../player_imp.h"
#include "../actsession.h"

int item_onlineaward_giftbox::OnUse(item::LOCATION l ,size_t index, gactive_imp* imp,item * parent) const
{
	if( l != item::INVENTORY) return -1;

	gplayer_imp *pImp = (gplayer_imp *)imp;

	DATA_TYPE dt;
    const SMALL_ONLINE_GIFT_BAG_ESSENCE & ess = *(SMALL_ONLINE_GIFT_BAG_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);

	if(pImp->GetInventory().IsFull() || pImp->GetInventory().GetEmptySlotCount() < ess.item_num)
	{
		pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return -1;
	} 

	for(size_t i = 0; i < ess.item_num; ++i)
	{
		int pile_limit = gmatrix::GetDataMan().get_item_pile_limit(ess.item_configs[i].item_id);	
		int count = ess.item_configs[i].item_count;
		if(count > pile_limit) count = pile_limit; 
		if(count <= 0) continue;
		if(ess.item_configs[i].is_bind) count =1;

		pImp->DeliverItem(ess.item_configs[i].item_id, count, ess.item_configs[i].is_bind, ess.item_configs[i].effect_time, ITEM_INIT_TYPE_TASK);
	}

	//该物品消失，return 1
	return 1;
}
