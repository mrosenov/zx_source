#include "item_giftbox.h"
#include "../template/elementdataman.h"
#include "arandomgen.h"
#include "../player_imp.h"
#include "../actsession.h"

int item_giftbox::OnUse(item::LOCATION l ,size_t index, gactive_imp* imp,item * parent) const
{
	if( l != item::INVENTORY) return -1;

	gplayer_imp *pImp = (gplayer_imp *)imp;

	DATA_TYPE dt;
	const GIFT_BAG_ESSENCE & ess = *(GIFT_BAG_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);

	if(ess.level_everLimit > 0 && pImp->GetMaxLevel() < ess.level_everLimit)
	{
		pImp->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -1;
	}
	if(ess.level_upperLimit > 0 && pImp->GetObjectLevel() > ess.level_upperLimit)
	{
		pImp->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -1;
	}
	if(pImp->GetObjectLevel() < ess.level_required)
	{
		pImp->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -1;
	}

	if(!((pImp->IsObjectFemale()?0x02:0x01) & ess.require_gender+1) || pImp->GetRebornCount() < ess.renascence_count || 
			!object_base_info::CheckCls(pImp->GetObjectClass(), ess.character_combo_id, ess.character_combo_id2) || (ess.god_devil_mask && !(pImp->GetCultivation() & ess.god_devil_mask)))
	{
		pImp->_runner->error_message(S2C::ERR_FATAL_ERR);	
		return -1;
	}


	if(pImp->GetInventory().IsFull() || pImp->GetInventory().GetEmptySlotCount() < ess.num_object)
	{
		pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);	
		return -1;
	}

	if( (ess.id1_object_need != 0 && !pImp->CheckItemExist(ess.id1_object_need,ess.id1_object_num))
			||(ess.id2_object_need != 0 && !pImp->CheckItemExist(ess.id2_object_need,ess.id2_object_num)) )
	{
		pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);	
		return -1;
	}

	if(ess.money_need > 0 && pImp->GetMoney() < (size_t)ess.money_need)
	{
		pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);	
		return -1;
	}

	//检查是否需要记录消费值
	pImp->CheckSpecialConsumption(ess.id1_object_need, ess.id1_object_num);
	pImp->CheckSpecialConsumption(ess.id2_object_need, ess.id2_object_num);

	if(ess.id1_object_need != 0) pImp->TakeOutItem(ess.id1_object_need, ess.id1_object_num);
	if(ess.id2_object_need != 0) pImp->TakeOutItem(ess.id2_object_need, ess.id2_object_num);

	pImp->SpendMoney(ess.money_need);
	pImp->_runner->spend_money(ess.money_need);

	bool need_consume = false;

	if(abase::Rand(0.0f, 1.0f) < ess.probability)
	{
		if(ess.success_disappear)
		{
			need_consume = true;
		}

		for(size_t i = 0; i < _gift_random_list.size(); ++i)
		{
			if(abase::Rand(0.0f, 1.0f) < _gift_random_list[i].prob)
			{
				int pile_limit = gmatrix::GetDataMan().get_item_pile_limit(_gift_random_list[i].item_id);	
				int count = abase::Rand((int)_gift_random_list[i].num_min, (int)_gift_random_list[i].num_max);
				if(count > pile_limit) count = pile_limit; 
				if(count <= 0) continue;
				if(_gift_random_list[i].is_bind) count =1;
				pImp->DeliverItem(_gift_random_list[i].item_id, count, _gift_random_list[i].is_bind, _gift_random_list[i].effect_time, ITEM_INIT_TYPE_TASK);
				GLog::log(GLOG_INFO, "用户%d开启礼包%d获得了物品%d个数为%d", pImp->_parent->ID.id, _tid, _gift_random_list[i].item_id, count);  
			}
		}

		for(size_t i = 0; i < 4; ++i)
		{
			if(!_gift_group_list[i].empty())
			{
				int index = abase::RandSelect(&(_gift_group_list[i][0].prob), sizeof(gift_item), _gift_group_list[i].size());

				int pile_limit = gmatrix::GetDataMan().get_item_pile_limit(_gift_group_list[i][index].item_id);	
				int count = abase::Rand((int)_gift_group_list[i][index].num_min, (int)_gift_group_list[i][index].num_max);
				if(count > pile_limit) count = pile_limit; 
				if(count <= 0) continue;
				if(_gift_group_list[i][index].is_bind) count =1;
				pImp->DeliverItem(_gift_group_list[i][index].item_id, count, _gift_group_list[i][index].is_bind, _gift_group_list[i][index].effect_time, ITEM_INIT_TYPE_TASK);
				GLog::log(GLOG_INFO, "用户%d开启礼包%d获得了物品%d个数为%d", pImp->_parent->ID.id, _tid, _gift_random_list[i].item_id, count);  
			}
		}

	}	
	else
	{
		if(ess.faliure_disappear)
		{
			need_consume = true;
		}
	}

	// 活跃度更新, 化咒签 id = 13347
	if (ess.id1_object_need == 13347 || ess.id2_object_need == 13347) {
		pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_HUAZHOU_QIAN);
	}

	return need_consume ? 1 : 0;	
}
