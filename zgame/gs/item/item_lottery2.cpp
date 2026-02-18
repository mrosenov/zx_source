#include "item_lottery2.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"
#include "../item_manager.h"


int
item_lottery2::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *) obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_LOTTERY))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}

	if(pImp->GetInventory().IsFull())
	{
		pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);	
		return -1;
	}

	char ticket[] = {0};

	//抽奖
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_LOTTERY);
	lua_State * L = keeper.GetState();
	ASSERT(L);	//之前应当检查过了
	
	if(!GenerateTicket(L, ticket, 0))
	{
		pImp->_runner->error_message(S2C::ERR_FATAL_ERR);
		pImp->SetCoolDown(COOLDOWN_INDEX_LOTTERY,15000);
		return 0;
	}

	//发奖
	int result[6];
	int bonus_index = 0, bonus_level = 0, bonus_money = 0, bonus_id = -1, bonus_count = 0, bonus_bind = 0, bonus_period = 0 ;
	if(!GetLotteryBonus(L,ticket,result))
	{
		pImp->_runner->error_message(S2C::ERR_FATAL_ERR);
		pImp->SetCoolDown(COOLDOWN_INDEX_LOTTERY,15000);
		return 0;
	}

	bonus_index	= ticket[0];
	bonus_level	= result[0];
	bonus_money	= result[1];
	bonus_id	= result[2];
	bonus_count	= result[3];
	bonus_bind 	= result[4];
	bonus_period	= result[5];
	
	if(bonus_count > 0 && bonus_id != -1)
	{
		//发放物品奖励 以任务方式发放， 会调用InitFromShop
		pImp->DeliverItem(bonus_id,bonus_count,bonus_bind,bonus_period,ITEM_INIT_TYPE_LOTTERY);
	}

	if(bonus_money > 0)
	{
		//发放金钱奖励
		pImp->GainMoneyWithDrop(bonus_money);
		pImp->_runner->task_deliver_money(bonus_money,pImp->GetMoney());
	}
	
	pImp->_runner->lottery2_bonus(GetTID(), bonus_index, bonus_level, bonus_id, bonus_count, bonus_money);
	GLog::log(GLOG_INFO,"用户%d兑换彩票%d等级%d奖品(%d,%d)奖金%d", pImp->_parent->ID.id,  GetTID(), bonus_level, bonus_id, bonus_count, bonus_money);
	GLog::formatlog("formatlog:lottery:userid=%d:lottery_item=%d:bonus=%d:bonus_item=%d:bonus_count=%d:bonus_money=%d", pImp->_parent->ID.id,  GetTID(), bonus_level, bonus_id, bonus_count, bonus_money);
	
	pImp->SetCoolDown(COOLDOWN_INDEX_LOTTERY,LOTTERY_COOLDOWN_TIME);

	// 更新活跃度[炼丹炉-彩票]
	if (item_manager::IsCashItem(_tid)) {
		pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_LIANDANLU);
	}
	return 1;
}

