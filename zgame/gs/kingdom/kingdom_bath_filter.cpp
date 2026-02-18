#include "kingdom_bath_filter.h"
#include "../clstab.h"
#include "../player_imp.h"
#include "../statedef.h"

DEFINE_SUBSTANCE(kingdom_bath_filter,filter,CLS_FILTER_KINGDOM_BATH)

void 
kingdom_bath_filter::OnAttach()
{
//	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
//	pImp->_runner->start_general_operation(S2C::GOP_BATH, SECOND_TO_TICK(120), false);
}



void 
kingdom_bath_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->OnBathEnd(_is_leader);
//	pImp->_runner->stop_general_operation(S2C::GOP_BATH,false);
}

void 
kingdom_bath_filter::Heartbeat(int tick)
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	
	if(_bath_count <=5 && _award_period != 0 && (++_counter % _award_period) == 0)
	{
		int item_id = 0; 
		int item_count = 0;
		player_template::GetBathAwardItem(item_id, item_count);

		if(item_id > 0 && item_count > 0)
		{
			if(pImp->GetInventory().GetEmptySlotCount() == 0)
			{
				pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
				return; 
			}
			pImp->DeliverItem(item_id, item_count, false, 0, ITEM_INIT_TYPE_TASK);
			pImp->_runner->bath_gain_item(item_id, item_count);
		}
	}

	if(--_timeout <=0 )
	{
		_is_deleted = true;
		return;
	}

	if(!pImp->IsBindState())
	{
		_is_deleted = true;
		return;
	}
}

