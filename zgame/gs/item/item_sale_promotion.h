
#ifndef __ONLINEGAME_GS_SALE_PROMOTION_ITEM_H__
#define __ONLINEGAME_GS_SALE_PROMOTION_ITEM_H__ 

#include "../item.h"
#include "../config.h"
#include "../template/exptypes.h"
#include <common/types.h>
#include <set>

enum
{
	COND_TYPE_DIRECT = 0,			// 直接给奖励
	COND_TYPE_PRE_TASK,				// 前提任务
	COND_TYPE_PRE_ACHIEVEMENT,		// 前提成就
	COND_TYPE_USE_INGOT,			// 消耗元宝
	COND_TYPE_USE_MONEY,			// 消耗虚拟币
	COND_TYPE_USE_ITEM,				// 消耗物品
	COND_TYPE_USE_AREA_YHD,			// 消耗区域友好度
	COND_TYPE_LOTTERY,				// 打开抽奖页面
};

struct award_item {
	int award_item_id;			// id
	int award_item_count;		// 数目
	int award_item_valid_time;	// 有效期
	int award_item_is_bind;		// 是否绑定
};

struct sale_promotion {
	int _valid_start;
	int _valid_end;
	int _get_count;		// 可重复使用次数
	int _cond_type;		// 前提或消耗类型
	int _cond_arg1;		// 参数1
	int _cond_arg2;		// 参数2

	award_item _award_items[2];
};

struct sale_promotion_item_use_args {
	int index;					// 使用的索引项 0 ~ 19, -1表示领取额外奖励
};

struct sale_promotion_item_content {
	int usedcount[20]; 		// 已使用的次数	
	int extra_count;		// 完成的次数
	int is_get_extra_award; // 是否领取了额外奖励
};

class item_sale_promotion : public item_body
{
	static std::set<int> _pre_task_ids;		// 前置任务id集合
	static std::set<int> _pre_achieve_ids;	// 前置成就id集合

	sale_promotion _sale_promotions[20];
	int _extra_award_need_count; // 获取额外奖励所需领取次数
	int _extra_award_id;         // 额外奖励
	int _extra_award_num;        // 额外奖励
	int _extra_award_valid_time; // 额外奖励
	int _extra_award_is_bind;    // 额外奖励

public:
	static bool isRelatedTask(int taskid) {
		return _pre_task_ids.find(taskid) != _pre_task_ids.end();	
	}

	static bool isRelatedAchievement(int achieveid) {
		return _pre_achieve_ids.find(achieveid) != _pre_achieve_ids.end();	
	}

	sale_promotion_item_content* GetDataEssence(item* parent) const
	{
		size_t len;
		void* buf = parent->GetContent(len);	
		if (len == 0) { // 第一次，初始化
			sale_promotion_item_content content;
			for (int i = 0; i < 20; ++i) {
				content.usedcount[i] = 0;
			}
			content.extra_count = 0;
			content.is_get_extra_award = 0;

			parent->SetContent(&content, sizeof(content));	

			buf = parent->GetContent(len);	
		}

		if (len == sizeof(sale_promotion_item_content)) 
		{
			return (sale_promotion_item_content*) buf;	
		}
		return NULL;
	}

	item_sale_promotion(const SALE_PROMOTION_ITEM_ESSENCE& ess)
	{
		for (int i = 0; i < 20; ++i) {
			_sale_promotions[i]._valid_start = ess.element_list[i].valid_time_start;
			_sale_promotions[i]._valid_end = ess.element_list[i].valid_time_end;
			_sale_promotions[i]._get_count = ess.element_list[i].get_count;
			_sale_promotions[i]._cond_type = ess.element_list[i].condition_type;
			_sale_promotions[i]._cond_arg1 = ess.element_list[i].condition_arg1;
			_sale_promotions[i]._cond_arg2 = ess.element_list[i].condition_arg2;

			_sale_promotions[i]._award_items[0].award_item_id = ess.element_list[i].award_item1;
			_sale_promotions[i]._award_items[0].award_item_count = ess.element_list[i].award_item1_count;
			_sale_promotions[i]._award_items[0].award_item_valid_time = ess.element_list[i].award_item1_valid_time;
			_sale_promotions[i]._award_items[0].award_item_is_bind = ess.element_list[i].award_item1_is_bind;
			_sale_promotions[i]._award_items[1].award_item_id = ess.element_list[i].award_item2;
			_sale_promotions[i]._award_items[1].award_item_count = ess.element_list[i].award_item2_count;
			_sale_promotions[i]._award_items[1].award_item_valid_time = ess.element_list[i].award_item2_valid_time;
			_sale_promotions[i]._award_items[1].award_item_is_bind = ess.element_list[i].award_item2_is_bind;

			if (ess.element_list[i].condition_type == COND_TYPE_PRE_TASK) {
				_pre_task_ids.insert(ess.element_list[i].condition_arg1);				
			} else if (ess.element_list[i].condition_type == COND_TYPE_PRE_ACHIEVEMENT) {
				_pre_task_ids.insert(ess.element_list[i].condition_arg1);				
			}
		}

		_extra_award_need_count = ess.extra_award_need_count;
		_extra_award_id = ess.extra_award_id;
		_extra_award_num = ess.extra_award_num;
		_extra_award_valid_time = ess.extra_award_valid_time;
		_extra_award_is_bind = ess.extra_award_is_bind;
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_SALE_PROMOTION;}
	virtual bool IsItemCanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) const { return true; }
	virtual int OnUse(item::LOCATION l, size_t index, gactive_imp* imp, item * parent, const char* arg, size_t arg_size) const;
	virtual void InitFromShop(gplayer_imp* pImp, item* parent, int value) const;
	virtual void OnPutIn(item::LOCATION l, size_t index, gactive_imp* obj, item* parent) const;
};

#endif
