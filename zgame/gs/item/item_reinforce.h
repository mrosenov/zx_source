#ifndef __ONLINEGAME_GS_REINFORCE_ITEM_H__
#define __ONLINEGAME_GS_REINFORCE_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../clstab.h"
#include "item_addon.h"
#include <crc.h>
#include "item_equip.h"
#include "../topic_site.h"

struct reinforce_param_t
{
	int need_level;
	float prop[5];		//成功 降级 清除属性 爆装备 不降级
};

const int reinforce_failed_type[5] = 
{
	item::REINFORCE_SUCCESS,
	item::REINFORCE_FAILED_LEVEL_0, 	//保留
	item::REINFORCE_FAILED_LEVEL_1,         //归0
	item::REINFORCE_FAILED_LEVEL_2,         //消失
	item::REINFORCE_FAILED_LEVEL_00,        //保留不降级
};

template <typename ESSENCE, int classid>
class item_reinforce_template : public item_body
{
private:
	static ClassInfo m_class_item_reinforce_template;
	static item_reinforce_template<ESSENCE,classid> *  CreateObject()
	{
		return  new item_reinforce_template<ESSENCE,classid>();
	}
	static ClassInfo *  GetClass() 
	{ 
		return m_class_item_reinforce_template;
	}
	virtual ClassInfo * GetRunTimeClass()
	{
		return &m_class_item_reinforce_template;
	}

	ESSENCE * _ess_list[item::EQUIP_INVENTORY_COUNT];
	int       _equip_mask;		//可以和哪个位置的物品进行增强
public:
	item_reinforce_template()
	{
		memset(_ess_list,0,sizeof(_ess_list));
		_equip_mask = 0;
		//检查一下物品概率是否正确

		for(size_t i = 0; i < ESSENCE::MAX_LEVEL; i ++)
		{
			const float *p = ESSENCE::reinforce_table[i].prop;
			float tmp = p[0] + p[1] + p[2] + p[3];
			ASSERT( tmp >= 1.0f && tmp <= 1.0f + 1e-2);
		}
	}

	int InsertEssence(int equip_mask, int addon_type, const int *arglist, size_t argnum)
	{
		if(argnum != ESSENCE::INIT_ARG_NUM) return -1;
		ESSENCE * ess = new ESSENCE(equip_mask, addon_type,arglist,argnum);
		if(ess->handler == NULL)
		{
			delete ess;
			return -12;
		}
		int counter = 0;
		for(size_t i = 0; i < item::EQUIP_INVENTORY_COUNT; i ++)
		{
			if(equip_mask &  ( 1 << i))
			{
				if(_ess_list[i] != NULL)
				{
					return -2;
				}
				_ess_list[i] = ess;
				counter ++;
			}
		}
		if(!counter) 
		{
			delete ess;
			return -3;
		}
		_equip_mask |= equip_mask & ((1 << item::EQUIP_INVENTORY_COUNT) - 1);
		return 0;
	}

	int ReinforceActivate(size_t equip_index, const equip_item * item, const equip_essence_new & value, gactive_imp * pImp, float adjust_ratio, int adjust_point, const item* parent)
	{
		if(equip_index >=item::EQUIP_INVENTORY_COUNT) return -1;
		size_t level = ESSENCE::GetLevel(value);
		if(level > ESSENCE::MAX_LEVEL)  return -2;
		ESSENCE * ess = _ess_list[equip_index];
		if(!ess) return -3;
		ess->Activate(pImp, value, item, adjust_ratio, adjust_point, parent);
		return 0;
	}

	int ReinforceDeactivate(size_t equip_index,const equip_item * item, const equip_essence_new & value, gactive_imp * pImp,float adjust_ratio, int adjust_point, const item* parent)
	{
		if(equip_index >=item::EQUIP_INVENTORY_COUNT) return -1;
		size_t level = ESSENCE::GetLevel(value);
		if(level > ESSENCE::MAX_LEVEL)  return -2;
		ESSENCE * ess = _ess_list[equip_index];
		if(!ess) return -3;
		ess->Deactivate(pImp, value, item,adjust_ratio, adjust_point, parent);
		return 0;
	}

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_REINFORCE;}

private:

	static int reinforce_item(const reinforce_param_t * table, size_t max_level, int cur_type, int& value, short & level, const float aoffset[4], bool newmode, const float adjust2[20])
	{
		if(cur_type == 0) return item::REINFORCE_UNAVAILABLE;			//增强目标不能为空
		if(value != 0 && cur_type != (unsigned short)value) return item::REINFORCE_UNAVAILABLE; //继续的增强目标必须和已经有的一致
		if((size_t)level >= max_level) return item::REINFORCE_UNAVAILABLE;	//增强的级别不能超过最大级别
		if(level >= 20) return item::REINFORCE_UNAVAILABLE;	//增强的级别不能超过最大级别

		
		//考虑概率
		float prop[5];
		memcpy(prop, table[level].prop,sizeof(prop));
		ASSERT(sizeof(prop) == sizeof(table[level].prop));
		if(newmode)
		{
			prop[0] = adjust2[level];
			prop[1] = 0;
			prop[2] = 0;
			prop[3] = 0;
			prop[4] = 1.0f - adjust2[level] + 1e-3;
		}
		else
		{
			//对prop进行修正 
			prop[0] += aoffset[0]; prop[1] += aoffset[1];
			prop[2] += aoffset[2]; prop[3] += aoffset[3];
		}
		int rst = abase::RandSelect(prop, 5);
		int failed_type = reinforce_failed_type[rst];
		if(failed_type != item::REINFORCE_SUCCESS)
		{
			if(failed_type == item::REINFORCE_FAILED_LEVEL_1)
			{
				//清除现在有的级别和状态
				value = 0;
				level = 0;
			}
			else if(failed_type == item::REINFORCE_FAILED_LEVEL_0)
			{
				//失败 级别降低一级
				level = level -1;
				if(level <= 0) 
				{
					level = 0;
					value = 0;
				}
			}
			else if(failed_type == item::REINFORCE_FAILED_LEVEL_00)
			{
				//保留不降级，不改变
			}
			
			return failed_type;
		}

		//精炼成功
		value = cur_type;
		level = level + 1;
		return item::REINFORCE_SUCCESS;
	}
public:

	virtual int DoReinforce(const equip_item * item, equip_essence_new & value, gactive_imp * pImp, const float aoffset[4], int & level_result, int &need_money,int & level_before,bool newmode, int require_level, const float adjust2[20]) const
	{
		//检测是否可以进行增强
		if((item->GetEquipMask() & _equip_mask) == 0)
		{
			return item::REINFORCE_UNAVAILABLE;
		}

		//检测是否满足增强条件
		if(!ESSENCE::CheckCondition(this, item, value, pImp))
		{
			return item::REINFORCE_UNAVAILABLE;
		}

		//检查新模式的特征
		if(newmode && ESSENCE::GetLevel(value) >require_level)
		{
			pImp->_runner->error_message(S2C::ERR_REQUIRE_REINFORCE_LEVEL);
			return item::REINFORCE_UNAVAILABLE;
		}

		//取得费用
		int fee = ESSENCE::GetReinforceFee(item->GetEssence().install_fee);
		if(fee <0) fee = 0;
		if(pImp->GetMoneyAmount() < (size_t)fee)
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return item::REINFORCE_UNAVAILABLE;
		}
		
		level_before = ESSENCE::GetLevel(value);
		int rst = reinforce_item(ESSENCE::reinforce_table, ESSENCE::MAX_LEVEL, _tid, ESSENCE::GetValue(value), ESSENCE::GetLevel(value), aoffset, newmode, adjust2);
		level_result = ESSENCE::GetLevel(value);
		need_money = fee;	//钱应当在外面扣除
		ESSENCE::PostReinforce(item, value,level_before);
		// Youshuang add
		if( rst == item::REINFORCE_SUCCESS )
		{
			ESSENCE::DeliverTopicSite( pImp, item->GetTID(), level_result );
		}
		// end
		return rst;
	}
	
	virtual void ModifyRequirement(int & level , int &require_level, int64_t &cls, int64_t &cls_require, int64_t &cls_require1, int value)
	{
		ESSENCE::ModifyRequirement(level,require_level,cls,cls_require, cls_require1,value);
	}

};
template <typename ESSENCE, int classid>
ClassInfo
item_reinforce_template<ESSENCE,classid>::m_class_item_reinforce_template("item_reinforce_template",classid,item_body::GetClass(),(substance *(*)())item_reinforce_template<ESSENCE,classid>::CreateObject, sizeof(item_reinforce_template<ESSENCE,classid>));

struct reinforce_stone_essence
{
	enum
	{
		MAX_LEVEL = MAX_REINFORCE_STONE_LEVEL,
		INIT_ARG_NUM = 0,
	};
	static reinforce_param_t reinforce_table[MAX_LEVEL];
	int equip_mask;
	int addon_type;
	addon_handler * handler;
	reinforce_stone_essence(int mask, int addon, const int *arg,size_t argnum)
	{
		ASSERT(argnum == 0);
		equip_mask = mask;
		addon_type = addon;
		handler = addon_manager::QueryHandler(addon);
		ASSERT(handler);
	}
	inline void MakeAddonData(addon_data &data, const  equip_essence_new & value, const equip_item * item)
	{
		data.id = addon_type;
		data.arg[0] = item->GetEStoneValue(value.rf_stone_level);
		data.arg[1] = 0;
		data.arg[2] = 0;
	}

	inline void Activate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float adjust_ratio,int adjust_point, const item* parent)
	{
		addon_data data;
		MakeAddonData(data, value,item);
		data.arg[0] = data.arg[0] + (int)(data.arg[0] * adjust_ratio + 0.5f) + adjust_point;
		handler->Activate(data,item,pImp,parent);
	}

	inline void Deactivate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float adjust_ratio, int adjust_point, const item* parent)
	{
		addon_data data;
		MakeAddonData(data, value,item);
		data.arg[0] = data.arg[0] + (int)(data.arg[0] * adjust_ratio + 0.5f) + adjust_point;
		handler->Deactivate(data,item,pImp,parent);
	}

	inline static bool CheckCondition(const item_body * __this, const equip_item * item, equip_essence_new & value,gactive_imp *pImp)
	{
		return item->GetEStoneID() == __this->GetTID();
	}

	inline static int & GetValue(equip_essence_new & value)
	{
		return value.rf_stone;
	}

	inline static short & GetLevel(equip_essence_new & value)
	{
		return value.rf_stone_level;
	}

	inline static unsigned short GetLevel(const equip_essence_new & value)
	{
		return value.rf_stone_level;
	}
	inline static int GetReinforceFee(const equip_install_fee & list)
	{
		return list.fee_install_estone;
	}
	inline static int GetRemoveFee(const equip_install_fee & list)
	{
		return 0;
	}
	inline static void ModifyRequirement(int & level , int &require_level, int64_t &cls, int64_t &cls_require, int64_t &cls_require1, int value)
	{
	}

	inline static void PostReinforce(const equip_item * item, equip_essence_new & value, int level_before)
	{
		item->ActivateReinforce(value, level_before);
	}
	// Youshuang add
	inline static void DeliverTopicSite( gactive_imp* pImp, unsigned int itemid, int level )
	{
		pImp->DeliverTopicSite( new TOPIC_SITE::reinforce( itemid, level ) );
	}
	// end
};

struct property_stone_essence
{
	enum 
	{
		MAX_LEVEL = MAX_PROPERTY_STONE_LEVEL,
		INIT_ARG_NUM = 3,
	};
	static reinforce_param_t reinforce_table[MAX_LEVEL];
	int equip_mask;
	int addon_type;
	addon_handler * handler;
	int arglist[MAX_LEVEL];
	addon_data  addon_arg;
	property_stone_essence(int mask, int addon, const int * alist, size_t argnum)
	{
		ASSERT(argnum == INIT_ARG_NUM);
		equip_mask = mask;
		addon_type = addon;
		handler = addon_manager::QueryHandler(addon);
		ASSERT(handler);
		addon_arg.id = addon;
		addon_arg.arg[0] = alist[0];
		addon_arg.arg[1] = alist[1];
		addon_arg.arg[2] = alist[2];
	}
	
	inline void Activate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float f, int i, const item* parent)
	{
		size_t level = value.pr_stone_level;
		handler->ScaleActivate(addon_arg,item,pImp,level,parent);
	}

	inline void Deactivate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float f, int i, const item* parent)
	{
		size_t level = value.pr_stone_level;
		handler->ScaleDeactivate(addon_arg,item,pImp,level,parent);
	}

	inline static int & GetValue(equip_essence_new & value)
	{
		return value.pr_stone;
	}

	inline static short & GetLevel(equip_essence_new & value)
	{
		return value.pr_stone_level;
	}

	inline static unsigned short  GetLevel(const equip_essence_new & value)
	{
		return value.pr_stone_level;
	}
	inline static void ModifyRequirement(int & level , int &require_level, int64_t &cls, int64_t &cls_require, int64_t &cls_require1, int value)
	{
	}
	inline static int GetReinforceFee(const equip_install_fee & list)
	{
		return list.fee_install_pstone;
	}
	inline static int GetRemoveFee(const equip_install_fee & list)
	{
		return list.fee_uninstall_pstone;
	}
	inline static bool CheckCondition(const item_body * __this, const equip_item * item, equip_essence_new & value,gactive_imp *pImp)
	{
		return true;
	}
	inline static void PostReinforce(const equip_item * item, equip_essence_new & value,int level_before)
	{
	}
	// Youshuang add
	inline static void DeliverTopicSite( gactive_imp* pImp, unsigned int itemid, int level )
	{
		pImp->DeliverTopicSite( new TOPIC_SITE::soul( itemid, level ) );
	}
	// end
};

struct skill_stone_essence
{
	enum 
	{
		MAX_LEVEL = MAX_SKILL_STONE_LEVEL,
		INIT_ARG_NUM = 2,
	};
	static reinforce_param_t reinforce_table[MAX_LEVEL];
	int equip_mask;
	int addon_type;
	addon_handler * handler;
	int skill;
	int level;
	skill_stone_essence(int mask, int addon, const int * alist, size_t argnum)
	{
		ASSERT(argnum == INIT_ARG_NUM);
		equip_mask = mask;
		addon_type = addon;
		handler = addon_manager::QueryHandler(addon);
		ASSERT(handler);
		skill = alist[0];
		level = alist[1];
	}

	inline void MakeAddonData(addon_data &data, const  equip_essence_new & value)
	{
		data.id = addon_type;
		data.arg[0] = skill;
		data.arg[1] = level;
		data.arg[2] = 0;
	}

	inline void Activate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float,int, const item* parent)
	{
		addon_data data;
		MakeAddonData(data, value);
		handler->Activate(data,item,pImp,parent);
	}

	inline void Deactivate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float,int, const item* parent)
	{
		addon_data data;
		MakeAddonData(data, value);
		handler->Deactivate(data,item,pImp,parent);
	}

	inline static int & GetValue(equip_essence_new & value)
	{
		return value.sk_stone;
	}

	inline static unsigned int & GetLevel(equip_essence_new & value)
	{
		return value.sk_stone_level;
	}

	inline static int GetLevel(const equip_essence_new & value)
	{
		return value.sk_stone_level;
	}

	inline static void ModifyRequirement(int & level , int &require_level, int64_t &cls, int64_t &cls_require, int64_t &cls_require1, int value)
	{
	}
	inline static int GetReinforceFee(const equip_install_fee & list)
	{
		return list.fee_install_sstone;
	}
	inline static int GetRemoveFee(const equip_install_fee & list)
	{
		return list.fee_uninstall_sstone;
	}
	inline static bool CheckCondition(const item_body * __this, const equip_item * item, equip_essence_new & value,gactive_imp *pImp)
	{
		return true;
	}
	inline static void PostReinforce(const equip_item * item, equip_essence_new & value,int level_before)
	{
	}
	inline static void DeliverTopicSite( gactive_imp* pImp, unsigned int itemid, int level )
	{
	}

};

struct spec_property_stone_essence : public property_stone_essence
{
	spec_property_stone_essence(int mask, int addon, const int * alist, size_t argnum):
		property_stone_essence(mask,addon,alist,argnum)
	{
	}

	inline void Activate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float,int, const item* parent)
	{
		//do nothing
	}

	inline void Deactivate(gactive_imp * pImp, const equip_essence_new & value, const equip_item * item, float,int, const item* parent)
	{
		//do nothing
	}

	inline static void ModifyRequirement(int & level , int &require_level, int64_t &cls, int64_t &cls_require, int64_t &cls_require1, int value)
	{
		require_level -= value;
		if(require_level < 0) require_level = 0;
	}
};
typedef item_reinforce_template<reinforce_stone_essence,CLS_ITEM_RF_STONE> item_reinforce_stone;
typedef item_reinforce_template<property_stone_essence,CLS_ITEM_PR_STONE> item_property_stone;
typedef item_reinforce_template<skill_stone_essence,CLS_ITEM_SK_STONE> item_skill_stone;
typedef item_reinforce_template<spec_property_stone_essence,CLS_ITEM_SPEC_PR_STONE> item_spec_property_stone;
#endif

