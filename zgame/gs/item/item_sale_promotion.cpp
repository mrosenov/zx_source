
#include "item_sale_promotion.h"
#include "../player_imp.h"

extern abase::timer      g_timer;

std::set<int> item_sale_promotion::_pre_task_ids;			// 前置任务id集合
std::set<int> item_sale_promotion::_pre_achieve_ids;		// 前置成就id集合


int 
item_sale_promotion::OnUse(item::LOCATION l, size_t index, gactive_imp * imp, item * parent, const char* arg, size_t arg_size) const
{
	gplayer_imp* pImp = (gplayer_imp*)imp;
	if(l != item::INVENTORY) return -1;

	if (arg_size != sizeof(sale_promotion_item_use_args)) return -1;;
	sale_promotion_item_content* pContent = GetDataEssence(parent);
	if (pContent == NULL) return -1; // 版本???
	sale_promotion_item_use_args* pArgs = (sale_promotion_item_use_args*)arg;

	if (pArgs->index == -1) { // pArgs->index = -1表示领取额外奖励
		if (pContent->is_get_extra_award) return -1; // 已领取过了
		if (pContent->extra_count < _extra_award_need_count) return -1; // 次数不够
		if (!pImp->InventoryHasSlot(1)) return -1; 

		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(_extra_award_id);
		if (pItem == NULL) return -1;

		int expire_date = 0;
		if (_extra_award_valid_time) {
			expire_date = g_timer.get_systime() + _extra_award_valid_time;
		} 
		pImp->DeliverItem(_extra_award_id, 	_extra_award_num, _extra_award_is_bind > 0, expire_date, ITEM_INIT_TYPE_TASK);
		pContent->is_get_extra_award = 1;
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, index);
		return 0;	
	} else {
		if (pArgs->index < 0 || pArgs->index >= 20) return -1;

		sale_promotion cur = _sale_promotions[pArgs->index];
		if (cur._valid_start == 0 && cur._valid_end == 0) return -1; // 未填充的，应该必须有时间区间
	
		int cur_time = g_timer.get_systime();
		if (cur_time < cur._valid_start || cur_time > cur._valid_end) return -1; // 时间不符合区间

		if (cur._cond_type != COND_TYPE_LOTTERY) { // 抽奖无次数
			if (pContent->usedcount[pArgs->index] >= cur._get_count) return -1; // 使用次数已满
		}
		// 检查奖励物品规则
		for (int i = 0; i < 2; ++i) {	
			if (cur._award_items[i].award_item_id == 0 || cur._award_items[i].award_item_count == 0) continue;

			element_data::item_tag_t tag = {element_data::IMT_CREATE, 0};
			const item_data * pItem = gmatrix::GetDataMan().generate_item(cur._award_items[i].award_item_id, &tag, sizeof(tag));
			if (pItem == NULL || pItem->pile_limit <= 0) return -1;
			if (cur._award_items[i].award_item_count > (int)pItem->pile_limit)	return -1; // 大于堆叠上限，错误
			if (cur._award_items[i].award_item_valid_time && pItem->pile_limit > 1) return -1; // 时间属性，堆叠上限需为1
		}	

		// 直接使用后绑定
		parent->Bind();

		// 检查包裹空间
		int award_item_num = 0;
		if (cur._award_items[0].award_item_id > 0) {
			award_item_num += 1;	
		}	
		if (cur._award_items[1].award_item_id > 0) {
			award_item_num += 1;	
		}	
		if (award_item_num != 0) { // 抽奖就没有奖励，奖励放在抽奖页面上做
			if (!pImp->InventoryHasSlot(award_item_num)) return -1; 
		}
		// 检查并消耗前置条件
		switch (cur._cond_type) {
		case COND_TYPE_DIRECT: 
			break;
		case COND_TYPE_PRE_TASK:
			if (!pImp->isSalePromotionTaskCompleted(cur._cond_arg1)) { // 前提任务未完成
				return -1;	
			}
			break;
		case COND_TYPE_PRE_ACHIEVEMENT:
			if (!pImp->IsAchievementFinish(cur._cond_arg1)) { // 前提成就未完成
				return -1;	
			}
			break;
		case COND_TYPE_USE_INGOT: 
		{
			if (cur._award_items[0].award_item_id == 0) return -1; // RMB类，需要第一个物品存在，记日志必须
			if (pImp->MallInfo().GetCash() < cur._cond_arg1) { // 元宝不足
				return -1;	
			}

			// 实现RMB商城购买日志
			// 跨服代币记录formatlog的时候做特殊处理, cash_need字段记为0(财务统计需求) 
			int expire_date = 0;
			if (cur._award_items[0].award_item_valid_time) {
				expire_date = g_timer.get_systime() + cur._award_items[0].award_item_valid_time;
			}

			int order_id = pImp->MallInfo().GetOrderID(); 
			pImp->PlayerUseCash(cur._award_items[0].award_item_id, cur._award_items[0].award_item_count, expire_date, cur._cond_arg1, order_id);
			break;
		}
		case COND_TYPE_USE_MONEY:
			if ((int)pImp->GetMoney() < cur._cond_arg1) { // 钱不足
				return -1;		
			}

			pImp->SpendMoney(cur._cond_arg1);
			pImp->_runner->spend_money(cur._cond_arg1);
			break;
		case COND_TYPE_USE_ITEM:
			if (pImp->GetInventory().CountItemByID(cur._cond_arg1) < cur._cond_arg2) { // 物品不足
				return -1;	
			}
			pImp->TakeOutItem(cur._cond_arg1, cur._cond_arg2);
			break;
		case COND_TYPE_USE_AREA_YHD:
			if (pImp->GetRegionReputation(cur._cond_arg1) < cur._cond_arg2) { // 区域声望不足
				return -1;	
			}
			pImp->ModifyRegionReputation(cur._cond_arg1, -cur._cond_arg2);
			break;
		case COND_TYPE_LOTTERY: // 抽奖，点击按钮，仅仅统计完成
			break;
		default: // 未知类型
			return -1;
		}

		// 加上次数
		pContent->usedcount[pArgs->index] += 1;

		int extra_count = 0;
		for (int i = 0; i < 20; i++) {
			if (pContent->usedcount[i] > 0)	
				extra_count++;
		}
		pContent->extra_count = extra_count;

		// 通告物品内容变化
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, index);

		// 加奖励物品
		for (int i = 0; i < 2; ++i) {	
			if (cur._award_items[i].award_item_id == 0 || cur._award_items[i].award_item_count == 0) continue;

			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(cur._award_items[i].award_item_id);
			if (pItem == NULL) continue;

			pImp->DeliverItem(cur._award_items[i].award_item_id, 
					cur._award_items[i].award_item_count, 
					cur._award_items[i].award_item_is_bind > 0, 
					cur._award_items[i].award_item_valid_time, ITEM_INIT_TYPE_TASK);
		}	
		// XXX 全部领取后，是否要销毁??

		return 0;
	}
}

void 
item_sale_promotion::InitFromShop(gplayer_imp* pImp, item* parent, int value) const
{
	GetDataEssence(parent); // 构造数据
}

void
item_sale_promotion::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	GetDataEssence(parent); // 并不是所有都会走InitFromShop，所以这里检查下格式，如果没有，则构造下数据
}
