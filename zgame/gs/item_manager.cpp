#include "item_manager.h"
#include "template/itemdataman.h"
#include "template/globaldataman.h"
#include "cooldowncfg.h"
#include "item/item_addon.h"
#include "item/item_equip.h"
#include "item/item_reinforce.h"
#include "item/item_dummy.h"
#include "item/item_recipe.h"
#include "item/item_poppet.h"
#include "item/item_teleport.h"
#include "item/item_potion.h"
#include "item/item_taskdice.h"
#include "item/item_dbexp.h"
#include "item/item_pkreset.h"
#include "item/item_skill.h"
#include "item/item_mount.h"
#include "item/item_couplejump.h"
#include "item/item_lottery.h"
#include "item/item_lottery2.h"
#include "item/item_lottery3.h"
#include "item/item_fireworks.h"
#include "item/item_recorder.h"
#include "item/item_talisman.h"
#include "item/item_bugle.h"
#include "item/item_townscroll.h"
#include "item/item_control.h"
#include "item/item_petbedge.h"
#include "item/item_petequip.h"
#include "item/item_petfood.h"
#include "item/item_petautofood.h"
#include "item/item_aircraft.h"
#include "item/item_off_agent.h"
#include "item/item_destroy_item.h"
#include "item/item_specname.h"
#include "item/item_giftbox.h"
#include "item/item_onlineaward_giftbox.h"
#include "item/item_vipcard.h"
#include "item/item_magic.h"
#include "item/item_assistcard.h"
#include "item/item_telestation.h"
#include "item/item_treasure_digcount.h"
#include "item/item_rune.h"
#include "item/item_astrology.h"
#include "item/item_battleflag.h"
#include "item/item_bottle.h"
#include "item/item_sale_promotion.h"
#include "item/item_lottery_ty.h"
#include "item/item_giftbox_set.h"
#include "item/item_propadd.h"
#include "item/item_fuwen.h"
#include "item/item_colorant.h"  // Youshuang add

#include "gmatrix.h"
#include <glog.h>
#include <vector>
#include <map>

item_manager item_manager::__instance;
char item_manager::_cash_item_flag[MAX_ITEM_INDEX];

#define IM_CHECK_ITEM(NAME)	\
	do {\
	if(dt == DT_##NAME)     \
	{                       \
		DATA_TYPE dt2;		\
		const NAME & data= *(const NAME*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt2); \
		ASSERT(dt2 == DT_##NAME && &data); \
		addon_data * first = NULL, *end = NULL; \
		if(setaddon_map.find(id) != setaddon_map.end()) {\
			first = &*setaddon_map[id].begin();\
			end = &*setaddon_map[id].end();\
		}\
		item_body * pItem = MakeItem(dataman,(int)id, data, first, end); \
		if(pItem) {	\
		_map[(int)id] = pItem;    \
		pItem->SetTID(id);}	\
		else printf("生成物品%u时发生错误,物品类型%s\n",id,"DT_"#NAME); \
	} \
	} while(0)


void item_manager::UseItemLog(int rid, int id, int guid1,int guid2,int count)
{
	if(!IsCashItem(id)) return;
	GLog::cash("item_disappear:roleid=%d:item=%d:count=%d:guid1=%d:guid2=%d", rid, id, count, guid1, guid2);
}

static item_body * MakeItem(itemdataman & dataman,int id, const ESTONE_ESSENCE & ess, addon_data * first, addon_data * end)
{
	if(id & 0xFFFF0000) return NULL;
	item_reinforce_stone * pItem = new item_reinforce_stone();

	int count = 0;
	for(size_t i = 0; i < 4; i ++)
	{
		int addon = ess.effects[i].effect_addon_type;
		if(addon == 0) continue;
		int rst = pItem->InsertEssence(ess.effects[i].equip_mask,addon,0,0);
		if(rst) 
		{
			printf("加入增强石本体返回错误%d\n",rst);
			delete pItem;
			return NULL;
		}
		count ++;
	}
	if(count == 0)
	{
		delete pItem;
		return NULL;
	}
	return pItem;
}

static item_body * MakeItem(itemdataman & dataman,int id, const PSTONE_ESSENCE & ess, addon_data * first, addon_data * end)
{
	if(id & 0xFFFF0000) return NULL;

	int addon_id = ess.effect_addon_id;
	DATA_TYPE dt3;
	const EQUIPMENT_ADDON & addon= *(const EQUIPMENT_ADDON*)dataman.get_data_ptr(addon_id,ID_SPACE_ADDON,dt3);
	if(&addon == NULL  || dt3 != DT_EQUIPMENT_ADDON) return NULL;

	int addon_type = addon.type;
	addon_data data = {addon_type, {addon.param1,addon.param2, addon.param3}};
	if(!addon_manager::TransformData(data)) return false;

	int rst = 0;
	item_body * pItem = NULL;
	switch(ess.type)
	{
		case 1:
			{
				item_spec_property_stone * it = new item_spec_property_stone();
				rst = it->InsertEssence(ess.equip_mask,addon_type,data.arg,3);
				pItem = it;
			}
			break;
		case 0:
			{
				item_property_stone * it = new item_property_stone();
				rst = it->InsertEssence(ess.equip_mask,addon_type,data.arg,3);
				pItem = it;
			}
			break;
		default:
			return NULL;
	}

	if(rst) 
	{
		printf("加入属性石本体返回错误%d\n",rst);
		delete pItem;
		return NULL;
	}

	return pItem;
}

static item_body * MakeItem(itemdataman & dataman,int id, const REFINE_TICKET_ESSENCE & ess, addon_data * first, addon_data * end)
{
	if(ess.ext_reserved_prob > 2.0) return NULL;
	if(ess.ext_succeed_prob  > 1.0) return NULL;

	return new item_dummy();

}

static item_body * MakeItem(itemdataman & dataman,int id, const RECIPEROLL_ESSENCE & ess, addon_data * first, addon_data * end)
{
	int id2 = ess.id_recipe;
	if(id2 <= 0) return NULL;

	DATA_TYPE dt2;
	const void * pTmp = dataman.get_data_ptr(id2, ID_SPACE_RECIPE,dt2);
	if(pTmp == NULL || dt2 != DT_RECIPE_ESSENCE) return NULL;

	return new item_recipe(id2, ((RECIPE_ESSENCE*)pTmp)->level);

}

static item_body * MakeItem(itemdataman & dataman,int id, const SSTONE_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return NULL;	//主线肯定没有技能石头，技能石被用作强化随机属性了
	if(id & 0xFFFF0000) return NULL;
	item_skill_stone * pItem = new item_skill_stone();

	int arg[] = {ess.skill_id, ess.skill_level};
	int rst = pItem->InsertEssence(ess.equip_mask,ITEM_ADDON_SKILL_STONE,arg, 2);
	if(rst) 
	{
		printf("加入技能石本体返回错误%d\n",rst);
		delete pItem;
		return NULL;
	}

	return pItem;
}

template <typename T>
static void testRange(T & var, T value_low, T value_high,T value_default, bool & adjust)
{
	if(var < value_low || var > value_high)
	{
		var = value_default;
		adjust = true;
	}
	
}
static item_body * MakeItem(itemdataman & dataman ,int id, const TALISMAN_MAINPART_ESSENCE & ess, addon_data * first, addon_data * end)
{

	static bool script_flag = true;
	if(script_flag) 
	{
		gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
		lua_State * L = keeper.GetState();
		if(L == NULL) return NULL;
		lua_register(L, "__ABASE_TALISMAN_QueryLevel", talisman_essence::script_QueryLevel);
		lua_register(L, "__ABASE_TALISMAN_QueryExp", talisman_essence::script_QueryExp);
		lua_register(L, "__ABASE_TALISMAN_QueryInnerData", talisman_essence::script_QueryInnerData);
		lua_register(L, "__ABASE_TALISMAN_SetLevel", talisman_essence::script_SetLevel);
		lua_register(L, "__ABASE_TALISMAN_SetExp", talisman_essence::script_SetExp);
		lua_register(L, "__ABASE_TALISMAN_GetInnerDataCount", talisman_essence::script_GetInnerDataCount);
		lua_register(L, "__ABASE_TALISMAN_SetInnerData", talisman_essence::script_SetInnerData);
		lua_register(L, "__ABASE_TALISMAN_ClearInnerData", talisman_essence::script_ClearInnerData);
		lua_register(L, "__ABASE_TALISMAN_AddAddon", talisman_essence::script_AddAddon);
		lua_register(L, "__ABASE_TALISMAN_SetQuality", talisman_essence::script_SetQuality);
		lua_register(L, "__ABASE_TALISMAN_SetStamina", talisman_essence::script_SetStamina);
		lua_register(L, "__ABASE_TALISMAN_SetHP", talisman_essence::script_SetHP);
		lua_register(L, "__ABASE_TALISMAN_SetMP", talisman_essence::script_SetMP);
		lua_register(L, "__ABASE_TALISMAN_SetAttackEnhance", talisman_essence::script_SetAttackEnhance);
		lua_register(L, "__ABASE_TALISMAN_SetDamage", talisman_essence::script_SetDamage);
		lua_register(L, "__ABASE_TALISMAN_SetMPEnhance", talisman_essence::script_SetMPEnhance);
		lua_register(L, "__ABASE_TALISMAN_SetHPEnhance", talisman_essence::script_SetHPEnhance);
		lua_register(L, "__ABASE_TALISMAN_SetResistance", talisman_essence::script_SetResistance);

		script_flag = false;
	}
	int require_gender = 0;
	switch(ess.require_gender)
	{
		case 0:
			require_gender = 0x01;
			break;
		case 1:
			require_gender = 0x02;
			break;
		case 2:
			require_gender = 0x03;
			break;
	}
	
	//Todo, Add by Houjun 2011-07-14, 模板修改后增加角色位
	talisman_item *pItem = new talisman_item(ess.character_combo_id,ess.character_combo_id2,ess.require_level,require_gender);
	
	pItem->Init( ess.max_level, ess.max_level2, ess.energy_recover_speed, ess.energy_recover_factor, ess.energy_drop_speed, ess.init_level);
	pItem->SetAircraft(ess.is_aircraft, ess.fly_speed, (int)ess.fly_energy_drop_speed, ess.fly_exp_added, ess.color_change);

	//加入额外的附加属性
	if(first && end)
	{
		while(first != end)
		{
			pItem->InsertAddon(*first);
			first ++;
		}
	}
	return pItem;
}

// Youshuang add
static item_body * MakeItem( itemdataman & dataman, int id, const COLORANT_ITEM_ESSENCE& ess, addon_data * first, addon_data * end )
{
	colorant_essence_data data;
	memset( &data,0,sizeof(data) );
	
	data.type = ess.id;
	data.color = ess.color;
	data.quality = ess.quality;
	data.index = ess.index;
	data.shop_price = ess.shop_price;

	colorant_item* pItem = new colorant_item(data);
	return pItem;
}
// end

static item_body * MakeItem(itemdataman & dataman,int id, const EQUIPMENT_ESSENCE & ess, addon_data * first, addon_data * end)
{
	equip_data data;
	memset(&data,0,sizeof(data));

	data.equip_type = ess.equip_type;	
	data.class_required = ess.character_combo_id;
	data.class_required1 = ess.character_combo_id2;
	data.level_required = ess.require_level;
	data.require_reborn_count = ess.renascence_count;
	data.require_reborn_prof[0]  = ess.char_lev_1;
	data.require_reborn_prof[1]  = ess.char_lev_2;
	data.require_reborn_prof[2]  = ess.char_lev_3;
	data.require_reborn_prof1[0]  = ess.char_lev_1_2;
	data.require_reborn_prof1[1]  = ess.char_lev_2_2;
	data.require_reborn_prof1[2]  = ess.char_lev_3_2;
	data.require_title = ess.title_require;
	data.require_cultivation = ess.god_devil_mask;
	data.require_territory = ess.require_territory;
	switch(ess.require_gender)
	{
		case 0:
			data.require_gender = 0x01;
			break;
		case 1:
			data.require_gender = 0x02;
			break;
		case 2:
			data.require_gender = 0x03;
			break;
	}
	data.require_rep_idx1 = ess.require_repu[0].index;
	data.require_rep_val1 = ess.require_repu[0].num;
	data.require_rep_idx2 = ess.require_repu[1].index;
	data.require_rep_val2 = ess.require_repu[1].num;
	data.hp = ess.hp;
	data.mp = ess.mp;
	data.dp = ess.dp;
	data.attack = ess.attack;
	data.damage_low = ess.min_dmg;
	data.damage_high = ess.max_dmg;
	data.deity_power = ess.deity_power;
	data.armor = ess.armor;
	data.defense = ess.defence;
	data.attack_range = ess.attack_range;
	data.level = ess.level;
	data.equip_mask = ess.equip_mask;
	data.can_be_enchanted = ess.blood_sacrifice;
	data.equip_enchant_level = ess.soul_level;
	data.can_gem_slot_identify = ess.gem_slot_identify;
	data.can_open_astrology_slot = ess.can_punching;
	data.require_kingdom_title = ess.nation_position_mask;
	// Youshuang add
	data.fashion_can_add_color = ess.color;
	//data.fixed_init_color = ess.fixed_init_color;
	//data.init_color_value = ess.init_color_value;
	// end
	
	if(data.level < 1 || data.level > 10) 
	{
		printf("error level %d\n", data.level);
		return NULL;
	}
	if((data.install_fee.fee_install_estone = ess.fee_estone) <= 0)
	{
		printf("error install_estone %d\n", ess.fee_estone);
		return NULL;
	}
	if((data.install_fee.fee_install_pstone   = ess.fee_install_pstone) <= 0)
	{
		printf("error install_pstone %d\n", ess.fee_install_pstone);
		return NULL;
	}
	if((data.install_fee.fee_install_sstone   = ess.fee_install_sstone) <= 0) 
	{
		printf("error install_sstone %d\n", ess.fee_install_sstone);
		return NULL;
	}
	if((data.install_fee.fee_uninstall_pstone = ess.fee_uninstall_pstone) <= 0)
	{
		printf("error uninstall_pstone %d\n", ess.fee_uninstall_pstone);
		return NULL;
	}

	if((data.install_fee.fee_uninstall_sstone = ess.fee_uninstall_sstone) <= 0)
	{
		printf("error uninstall_sstone %d\n", ess.fee_uninstall_sstone);
		return NULL;
	}


	//进行最大数字区间校正
	bool adjust = false;
	testRange(data.damage_low, -10000,50000,1, adjust);
	testRange(data.damage_high, -10000,50000,1, adjust);
	testRange(data.hp, -20000,30000,0, adjust);
	testRange(data.mp, -20000,30000,0, adjust);
	testRange(data.armor, -10000,10000,0, adjust);
	testRange(data.defense, -10000,50000,0, adjust);
	testRange(data.attack_range, 0.f,16.f,2.5f, adjust);
	if(adjust)
	{
		printf("装备%d由于数值范围过大发生了数据调整\n",id);
	}

	//检查是否武器
	if( (ess.equip_type == 0 ) && !( ess.equip_mask & item::EQUIP_MASK_FASHION_WEAPON ) )  // Youshuang add
	{
		data.weapon_type = ess.action_type;
		data.attack_cycle = SECOND_TO_TICK(1.0f);
		data.attack_point = SECOND_TO_TICK(0.5f);
		if(ess.equip_mask & ~(item::EQUIP_MASK_WEAPON) || ess.attack_range <= 0.1f)
		{
			return NULL;
		}
	}
	
	equip_item * pItem = new equip_item(data);

	bool bErr = false;
	//加入强化石
	if(ess.id_estone)
	{
		for(size_t i = 0; i < MAX_REINFORCE_STONE_LEVEL; i ++)
		{
			int val = ess.ehanced_value[i];
			if(val < 0 || (size_t)val > (i*i * i +127)* ess.require_level*(1+3 *ess.renascence_count))
			{
				__PRINTF("装备%d中发现了不正确的强化参数%d\n",id, val);
				bErr = true;
			}
		}
		pItem->SetReinforce(ess.id_estone, ess.ehanced_value);
	}
	if(bErr) 
	{
		delete pItem;
		return NULL;
	}

	//加入强化随机属性
	pItem->SetReinforceAddon(0,ess.hidden_prop[0].level, ess.hidden_prop[0].prop);
	pItem->SetReinforceAddon(1,ess.hidden_prop[1].level, ess.hidden_prop[1].prop);

	//检查addon
	int m[]= { ess.id_addon1,ess.id_addon2,ess.id_addon3,ess.id_addon4,ess.id_addon5};
	for(size_t i = 0; i < sizeof(m)/ sizeof(int); i ++)
	{
		if (m[i] <= 0) continue;
		int addon_id = m[i];
		const addon_data_spec * pSpec = addon_data_man::Instance().GetAddon(addon_id);
		if(!pSpec)
		{
			__PRINTF("装备%d中发现了不正确的addon %d\n",id, addon_id);
			continue;
		}
		/*
			根据 addon ID 生成addon data供装备使用 
		 */
		
		//这里不直接应用pSpec了,因为后面也使用了这些东西
		pItem->InsertAddon(pSpec->data);
		// Youshuang add
		if( pSpec->data.id == ITEM_ADDON_FASION_WEAPON_SKILL )
		{
			pItem->SetFashionWeaponAddonID( addon_id );
		}
		// end
	}

	//加入额外的附加属性
	if(first && end)
	{
		while(first != end)
		{
			pItem->InsertAddon(*first);
			first ++;
		}
	}


	//加入隐藏addon
	pItem->SetHiddenAddon(ess.hidden_addon);
	int n[] = {ess.hidden_addon[0].addon, ess.hidden_addon[1].addon, ess.hidden_addon[2].addon};
	for(size_t i = 0; i < sizeof(n)/ sizeof(int); i ++)
	{
		if (n[i] <= 0) continue;
		int addon_id = n[i];
		const addon_data_spec * pSpec = addon_data_man::Instance().GetAddon(addon_id);
		if(!pSpec)
		{
			__PRINTF("装备%d中发现了不正确的隐藏属性addon %d\n",id, addon_id);
			continue;
		}
	}
	return pItem;
}

static item_body * MakeItem(itemdataman & dataman,int id, const MEDICINE_ESSENCE & ess, addon_data * first, addon_data * end)
{
	switch(ess.type)
	{
		case 0:
			//瞬回HP
			if(ess.pile_num_max != 1) return NULL;
			if(ess.hp[0] <=0) return NULL;
			return  (new item_healing_potion(ess.cool_time, ess.require_level, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);
		case 1:
			//瞬回MP
			if(ess.pile_num_max != 1) return NULL;
			if(ess.mp[0] <= 0) return NULL;
			return  (new item_mana_potion(ess.cool_time, ess.require_level, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);
		case 2:
			//瞬回MP HP 
			if(ess.pile_num_max != 1) return NULL;
			if(ess.hp[0] <=0) return NULL;
			if(ess.mp[0] <=0) return NULL;
			return  (new item_rejuvenation_potion(ess.cool_time, ess.require_level, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);
		case 3:
			//持续回HP
			return  (new item_step_healing_potion(ess.cool_time, ess.require_level,ess.hp, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);
		case 4:
			//持续回MP
			return  (new item_step_mana_potion(ess.cool_time, ess.require_level,ess.mp, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);
		case 5:
			//两个都回
			return  (new item_step_rejuvenation_potion(ess.cool_time, ess.require_level,ess.hp, ess.mp, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);

		case 6:
			//瞬回dp
			return (new item_deity_potion(ess.cool_time, ess.require_level, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);
		case 7:
			//持续回dp
			return (new item_step_deity_potion(ess.cool_time, ess.require_level, ess.dp, ess.renascence_count, ess.map_restrict))->SetBattleGroundFlag(ess.only_in_war);
		default:
			return NULL;
	}
}

static item_body * MakeItem(itemdataman & dataman,int id, const TRANSMITROLL_ESSENCE & ess, addon_data * first, addon_data * end)
{
	if(ess.tag <= 0) return NULL;
	if(ess.x <= -512 || ess.x>= 512) return NULL;
	if(ess.y <= 0 || ess.y>= 800) return NULL;
	if(ess.z <= -512 || ess.z>= 512) return NULL;

	return new item_teleport(ess.tag,ess.x,ess.y,ess.z,ess.renascence_count, ess.require_level, 3.f);
}

static item_body * MakeItem(itemdataman & dataman,int id, const LUCKYROLL_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_poppet();
}


static item_body * MakeItem(itemdataman & dataman,int id, const TASKDICE_ESSENCE & ess, addon_data * first, addon_data * end)
{
	item_taskdice * pItem = new item_taskdice();
	if(!pItem->SetTaskList(ess.task_lists) || !pItem->SetTriggerPointList(ess.type, ess.trigger_point, ess.map_trigger, ess.pos_trigger[0], ess.pos_trigger[1], ess.pos_trigger[2], ess.radius_trigger))
	{
		delete pItem;
		return NULL;
	}
	return pItem;
}

static item_body * MakeItem(itemdataman & dataman,int id, const DOUBLE_EXP_ESSENCE & ess, addon_data * first, addon_data * end)
{
	if(ess.double_exp_time <= 0) return NULL;
	if(ess.double_exp_time > 3600*10) return NULL;
	if(ess.exp_multiple < 2 || ess.exp_multiple >= 10) return NULL;
	return new item_dbl_exp(ess.double_exp_time, ess.exp_multiple);

}

/*
static item_body * MakeItem(itemdataman & dataman,int id, const DOUBLE_EXP_ESSENCE & ess)
{
	return new item_pk_reset();
}
*/

static item_body * MakeItem(itemdataman & dataman,int id, const SKILLMATTER_ESSENCE & ess, addon_data * first, addon_data * end)
{
	int ctype = COOLDOWN_INDEX_SKILL_ITEM_1;
	switch(ess.cool_type)
	{
		case 0:
			ctype = COOLDOWN_INDEX_SKILL_ITEM_1;
			break;
		case 1:
			ctype = COOLDOWN_INDEX_SKILL_ITEM_2;
			break;
		case 2:
			ctype = COOLDOWN_INDEX_SKILL_ITEM_3;
			break;
		case 3:
			ctype = COOLDOWN_INDEX_SKILL_ITEM_4;
			break;
		case 4:
			ctype = COOLDOWN_INDEX_SKILL_ITEM_5;
			break;
		default:
			ctype = COOLDOWN_INDEX_SKILL_ITEM_1;
			break;
	}
	return new item_skill(ess.id_skill, ess.level_skill, ctype, ess.level_required, ess.only_in_war, ess.permenent, ess.god_devil_mask);
}

static item_body * MakeItem(itemdataman & dataman,int id, const VEHICLE_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_mount(id, ess.speed, ess.level_required, ess.only_in_war, ess.is_multi_ride, ess.renascence_count, ess.nation_position_mask, ess.only_qianji);
}

static item_body * MakeItem(itemdataman & dataman,int id, const COUPLE_JUMPTO_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_couple_jump();
}

static item_body * MakeItem(itemdataman & dataman,int id, const LOTTERY_ESSENCE & ess, addon_data * first, addon_data * end)
{
	int rst = item_lottery::CheckScript(id, ess.dice_count);
	if(rst < 0) 
	{
		printf("脚本内部错误，不能继续初始化 物品ID%d\n",id);
		//_exit(-1);
		return NULL;
	}
	if(rst ) return NULL;
	if(ess.pile_num_max != 1) return NULL;
	return new item_lottery(ess.dice_count);
}

static item_body * MakeItem(itemdataman & dataman,int id, const LOTTERY2_ESSENCE & ess, addon_data * first, addon_data * end)
{
	ASSERT(ess.dice_count == 1);
	int rst = item_lottery::CheckScript(id, ess.dice_count);
	if(rst < 0) 
	{
		printf("脚本内部错误，不能继续初始化 物品ID%d\n",id);
		//_exit(-1);
		return NULL;
	}
	if(rst ) return NULL;
	return new item_lottery2(ess.dice_count);
}

static item_body * MakeItem(itemdataman & dataman,int id, const LOTTERY3_ESSENCE & ess, addon_data * first, addon_data * end)
{
	ASSERT(ess.dice_count == 1);
	int rst = item_lottery::CheckScript(id, ess.dice_count);
	if(rst < 0) 
	{
		printf("脚本内部错误，不能继续初始化 物品ID%d\n",id);
		//_exit(-1);
		return NULL;
	}
	if(rst ) return NULL;
	return new item_lottery3(ess.dice_count, ess.award_item);
}

static item_body * MakeItem(itemdataman & dataman,int id, const LOTTERY_TANGYUAN_ITEM_ESSENCE & ess, addon_data * first, addon_data * end)
{
	int rst = item_lottery_ty::CheckScript(id);
	if(rst < 0) 
	{
		printf("脚本内部错误，不能继续初始化 物品ID%d\n",id);
		return NULL;
	}
	if(rst ) return NULL;
	if(ess.pile_num_max != 1) return NULL;
	return new item_lottery_ty(ess.exp_get, ess.exp_level, ess.open_item, ess.open_item_num);
}

static item_body * MakeItem(itemdataman & dataman,int id, const GIFT_PACK_ITEM_ESSENCE & ess, addon_data * first, addon_data * end)
{
	giftbox_item list[4];
	for(int i = 0; i < 4; i ++)
	{
		list[i].item_id = ess.item_gifts[i].id;
		list[i].apoint = ess.item_gifts[i].score;
	}
	return new item_giftbox_set(ess.open_time > 0 ? ess.open_time : 0, list);
}


static item_body * MakeItem(itemdataman & dataman,int id, const FIREWORKS_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_fireworks();
}

static item_body * MakeItem(itemdataman & dataman,int id, const CAMRECORDER_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_recorder();
}

static item_body * MakeItem(itemdataman & dataman,int id, const TEXT_FIREWORKS_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_fireworks_text(ess.char_can_use, 256, ess.max_char);
}

static item_body * MakeItem(itemdataman & dataman,int id, const TALISMAN_ENERGYFOOD_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_talisman_stamina_potion(ess.energy_used_minimum, ess.energy_total);
}

static item_body * MakeItem(itemdataman & dataman,int id, const SPEAKER_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_bugle(ess.id_icon_set);
}

static item_body * MakeItem(itemdataman & dataman,int id, const TOWNSCROLL_ESSENCE & ess, addon_data * first, addon_data * end)
{
	if(ess.blank)
	{
		return new item_townscroll_paper();
	}
	else
	{
		g_config.item_townscroll_id = id;
		return new item_townscroll();
	}
}

static item_body * MakeItem(itemdataman & dataman,int id, const SIEGE_ARTILLERY_SCROLL_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_control_mob();
}

static item_body * MakeItem(itemdataman& dataman,int id,const PET_BEDGE_ESSENCE& ess,addon_data * first,addon_data * end)
{
	static bool script_flag = true;
	if(script_flag) 
	{
		gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
		lua_State* L = keeper.GetState();
		if(L == NULL) return NULL;

		lua_register(L,"__ABASE_PETBEDGE_QueryInnerData", pet_bedge_essence::script_QueryInnerData);
		lua_register(L,"__ABASE_PETBEDGE_GetInnerDataCount", pet_bedge_essence::script_GetInnerDataCount);
		lua_register(L,"__ABASE_PETBEDGE_SetInnerData", pet_bedge_essence::script_SetInnerData);
		lua_register(L,"__ABASE_PETBEDGE_ClearInnerData", pet_bedge_essence::script_ClearInnerData);

		lua_register(L,"__ABASE_PETBEDGE_QueryLevel",pet_bedge_essence::script_QueryLevel);
		lua_register(L,"__ABASE_PETBEDGE_SetLevel",pet_bedge_essence::script_SetLevel);
		lua_register(L,"__ABASE_PETBEDGE_QueryExp", pet_bedge_essence::script_QueryExp);
		lua_register(L,"__ABASE_PETBEDGE_SetExp", pet_bedge_essence::script_SetExp);
		lua_register(L,"__ABASE_PETBEDGE_QueryCurHP",pet_bedge_essence::script_QueryCurHP);
		lua_register(L,"__ABASE_PETBEDGE_SetCurHP",pet_bedge_essence::script_SetCurHP);
		lua_register(L,"__ABASE_PETBEDGE_QueryCurVigor",pet_bedge_essence::script_QueryCurVigor);
		lua_register(L,"__ABASE_PETBEDGE_SetCurVigor",pet_bedge_essence::script_SetCurVigor);
		lua_register(L,"__ABASE_PETBEDGE_QueryStar",pet_bedge_essence::script_QueryStar);
		lua_register(L,"__ABASE_PETBEDGE_SetStar",pet_bedge_essence::script_SetStar);
		lua_register(L,"__ABASE_PETBEDGE_QueryRebornStar",pet_bedge_essence::script_QueryRebornStar);
		lua_register(L,"__ABASE_PETBEDGE_SetRebornStar",pet_bedge_essence::script_SetRebornStar);
		lua_register(L,"__ABASE_PETBEDGE_QueryMainType",pet_bedge_essence::script_QueryMainType);
		lua_register(L,"__ABASE_PETBEDGE_SetMainType",pet_bedge_essence::script_SetMainType);
		lua_register(L,"__ABASE_PETBEDGE_QueryElement",pet_bedge_essence::script_QueryElement);
		lua_register(L,"__ABASE_PETBEDGE_SetElement",pet_bedge_essence::script_SetElement);
		lua_register(L,"__ABASE_PETBEDGE_QueryIdentify",pet_bedge_essence::script_QueryIdentify);
		lua_register(L,"__ABASE_PETBEDGE_SetIdentify",pet_bedge_essence::script_SetIdentify);
		lua_register(L,"__ABASE_PETBEDGE_QueryRace",pet_bedge_essence::script_QueryRace);
		lua_register(L,"__ABASE_PETBEDGE_SetRace",pet_bedge_essence::script_SetRace);
		lua_register(L,"__ABASE_PETBEDGE_QueryHoroscope",pet_bedge_essence::script_QueryHoroscope);
		lua_register(L,"__ABASE_PETBEDGE_SetHoroscope",pet_bedge_essence::script_SetHoroscope);
		lua_register(L,"__ABASE_PETBEDGE_QueryCombatAttrInit",pet_bedge_essence::script_QueryCombatAttrInit);
		lua_register(L,"__ABASE_PETBEDGE_SetCombatAttrInit",pet_bedge_essence::script_SetCombatAttrInit);
		lua_register(L,"__ABASE_PETBEDGE_QueryCombatAttrPotential",pet_bedge_essence::script_QueryCombatAttrPotential);
		lua_register(L,"__ABASE_PETBEDGE_SetCombatAttrPotential",pet_bedge_essence::script_SetCombatAttrPotential);
		lua_register(L,"__ABASE_PETBEDGE_QueryCombatAttrCur",pet_bedge_essence::script_QueryCombatAttrCur);
		lua_register(L,"__ABASE_PETBEDGE_SetCombatAttrCur",pet_bedge_essence::script_SetCombatAttrCur);
		lua_register(L,"__ABASE_PETBEDGE_QueryRebornPropAdd",pet_bedge_essence::script_QueryRebornPropAdd);
		lua_register(L,"__ABASE_PETBEDGE_SetRebornPropAdd",pet_bedge_essence::script_SetRebornPropAdd);
		lua_register(L,"__ABASE_PETBEDGE_QueryExtraDamage",pet_bedge_essence::script_QueryExtraDamage);
		lua_register(L,"__ABASE_PETBEDGE_SetExtraDamage",pet_bedge_essence::script_SetExtraDamage);
		lua_register(L,"__ABASE_PETBEDGE_QueryExtraDamageReduce",pet_bedge_essence::script_QueryExtraDamageReduce);
		lua_register(L,"__ABASE_PETBEDGE_SetExtraDamageReduce",pet_bedge_essence::script_SetExtraDamageReduce);
		lua_register(L,"__ABASE_PETBEDGE_QueryCritRate",pet_bedge_essence::script_QueryCritRate);
		lua_register(L,"__ABASE_PETBEDGE_SetCritRate",pet_bedge_essence::script_SetCritRate);
		lua_register(L,"__ABASE_PETBEDGE_QueryCritDamage",pet_bedge_essence::script_QueryCritDamage);
		lua_register(L,"__ABASE_PETBEDGE_SetCritDamage",pet_bedge_essence::script_SetCritDamage);
		lua_register(L,"__ABASE_PETBEDGE_QueryAntiCritRate",pet_bedge_essence::script_QueryAntiCritRate);
		lua_register(L,"__ABASE_PETBEDGE_SetAntiCritRate",pet_bedge_essence::script_SetAntiCritRate);
		lua_register(L,"__ABASE_PETBEDGE_QueryAntiCritDamage",pet_bedge_essence::script_QueryAntiCritDamage);
		lua_register(L,"__ABASE_PETBEDGE_SetAntiCritDamage",pet_bedge_essence::script_SetAntiCritDamage);
		lua_register(L,"__ABASE_PETBEDGE_QuerySkillAttackRate",pet_bedge_essence::script_QuerySkillAttackRate);
		lua_register(L,"__ABASE_PETBEDGE_SetSkillAttackRate",pet_bedge_essence::script_SetSkillAttackRate);
		lua_register(L,"__ABASE_PETBEDGE_QuerySkillArmorRate",pet_bedge_essence::script_QuerySkillArmorRate);
		lua_register(L,"__ABASE_PETBEDGE_SetSkillArmorRate",pet_bedge_essence::script_SetSkillArmorRate);
		lua_register(L,"__ABASE_PETBEDGE_QueryDamageReduce",pet_bedge_essence::script_QueryDamageReduce);
		lua_register(L,"__ABASE_PETBEDGE_SetDamageReduce",pet_bedge_essence::script_SetDamageReduce);
		lua_register(L,"__ABASE_PETBEDGE_QueryDamageReducePercent",pet_bedge_essence::script_QueryDamageReducePercent);
		lua_register(L,"__ABASE_PETBEDGE_SetDamageReducePercent",pet_bedge_essence::script_SetDamageReducePercent);
		lua_register(L,"__ABASE_PETBEDGE_QueryMaxHungerPoint",pet_bedge_essence::script_QueryMaxHungerPoint);
		lua_register(L,"__ABASE_PETBEDGE_SetMaxHungerPoint",pet_bedge_essence::script_SetMaxHungerPoint);
		lua_register(L,"__ABASE_PETBEDGE_QueryCurHungerPoint",pet_bedge_essence::script_QueryCurHungerPoint);
		lua_register(L,"__ABASE_PETBEDGE_SetCurHungerPoint",pet_bedge_essence::script_SetCurHungerPoint);
		lua_register(L,"__ABASE_PETBEDGE_QueryMaxHonorPoint",pet_bedge_essence::script_QueryMaxHonorPoint);
		lua_register(L,"__ABASE_PETBEDGE_SetMaxHonorPoint",pet_bedge_essence::script_SetMaxHonorPoint);
		lua_register(L,"__ABASE_PETBEDGE_QueryCurHonorPoint",pet_bedge_essence::script_QueryCurHonorPoint);
		lua_register(L,"__ABASE_PETBEDGE_SetCurHonorPoint",pet_bedge_essence::script_SetCurHonorPoint);
		lua_register(L,"__ABASE_PETBEDGE_QueryAge",pet_bedge_essence::script_QueryAge);
		lua_register(L,"__ABASE_PETBEDGE_SetAge",pet_bedge_essence::script_SetAge);
		lua_register(L,"__ABASE_PETBEDGE_QueryLife",pet_bedge_essence::script_QueryLife);
		lua_register(L,"__ABASE_PETBEDGE_SetLife",pet_bedge_essence::script_SetLife);
		lua_register(L,"__ABASE_PETBEDGE_QuerySkillID",pet_bedge_essence::script_QuerySkillID);
		lua_register(L,"__ABASE_PETBEDGE_SetSkillID",pet_bedge_essence::script_SetSkillID);
		lua_register(L,"__ABASE_PETBEDGE_QuerySkillLevel",pet_bedge_essence::script_QuerySkillLevel);
		lua_register(L,"__ABASE_PETBEDGE_SetSkillLevel",pet_bedge_essence::script_SetSkillLevel);
		lua_register(L,"__ABASE_PETBEDGE_QueryTalent",pet_bedge_essence::script_QueryTalent);
		lua_register(L,"__ABASE_PETBEDGE_SetTalent",pet_bedge_essence::script_SetTalent);
		lua_register(L,"__ABASE_PETBEDGE_QueryAcuity",pet_bedge_essence::script_QueryAcuity);
		lua_register(L,"__ABASE_PETBEDGE_SetAcuity",pet_bedge_essence::script_SetAcuity);
		lua_register(L,"__ABASE_PETBEDGE_QueryStamina",pet_bedge_essence::script_QueryStamina);
		lua_register(L,"__ABASE_PETBEDGE_SetStamina",pet_bedge_essence::script_SetStamina);
		lua_register(L,"__ABASE_PETBEDGE_QuerySawy",pet_bedge_essence::script_QuerySawy);
		lua_register(L,"__ABASE_PETBEDGE_SetSawy",pet_bedge_essence::script_SetSawy);
		lua_register(L,"__ABASE_PETBEDGE_QueryDowry",pet_bedge_essence::script_QueryDowry);
		lua_register(L,"__ABASE_PETBEDGE_SetDowry",pet_bedge_essence::script_SetDowry);
		lua_register(L,"__ABASE_PETBEDGE_QueryWisdom",pet_bedge_essence::script_QueryWisdom);
		lua_register(L,"__ABASE_PETBEDGE_SetWisdom",pet_bedge_essence::script_SetWisdom);
		lua_register(L,"__ABASE_PETBEDGE_QueryCNR",pet_bedge_essence::script_QueryCNR);
		lua_register(L,"__ABASE_PETBEDGE_SetCNR",pet_bedge_essence::script_SetCNR);
		lua_register(L,"__ABASE_PETBEDGE_QueryFace",pet_bedge_essence::script_QueryFace);
		lua_register(L,"__ABASE_PETBEDGE_SetFace",pet_bedge_essence::script_SetFace);
		lua_register(L,"__ABASE_PETBEDGE_QueryShape",pet_bedge_essence::script_QueryShape);
		lua_register(L,"__ABASE_PETBEDGE_SetShape",pet_bedge_essence::script_SetShape);
		lua_register(L,"__ABASE_PETBEDGE_QueryRebornCnt",pet_bedge_essence::script_QueryRebornCnt);
		lua_register(L,"__ABASE_PETBEDGE_SetRebornCnt",pet_bedge_essence::script_SetRebornCnt);

		script_flag = false;
	}
	item_pet_bedge *pItem = new item_pet_bedge;
	return pItem;
}

static item_body * MakeItem(itemdataman& dataman,int id,const PET_ARMOR_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_pet_equip();
}

static item_body * MakeItem(itemdataman& dataman,int id,const PET_FOOD_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_pet_food(ess.pet_level_min,ess.pet_level_max,ess.pet_type_mask,ess.food_usage,ess.pet_food_type,ess.food_value);
}

static item_body * MakeItem(itemdataman& dataman,int id,const PET_AUTO_FOOD_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_pet_auto_food(ess.food_value,ess.hp_gen,ess.mp_gen,ess.hunger_gen);
}

static item_body * MakeItem(itemdataman& dataman,int id,const PET_REFINE_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_dummy();
}

static item_body * MakeItem(itemdataman& dataman,int id,const PET_ASSIST_REFINE_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_dummy();
}

static item_body * MakeItem(itemdataman& dataman,int id,const AIRCRAFT_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return NULL;
//	return  new item_aircraft(ess.speed, ess.level_required, ess.character_combo_id);
}

static item_body * MakeItem(itemdataman& dataman,int id,const OFFLINE_TRUSTEE_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_offline_agent(ess.max_trustee_time/60);
}

static item_body * MakeItem(itemdataman& dataman,int id,const DESTROYING_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_destroying_item();
}

static item_body * MakeItem(itemdataman& dataman,int id,const SPECIAL_NAME_ITEM_ESSENCE& ess,addon_data * first,addon_data * end)
{
	if(ess.pile_num_max != 1) return NULL;
	return new item_special_name();
}

static item_body * MakeItem(itemdataman& dataman,int id,const GIFT_BAG_ESSENCE& ess,addon_data * first,addon_data * end)
{
	//判断是否有掩码位重复
	if((ess.normalize_group[0] + ess.normalize_group[1] + ess.normalize_group[2] + ess.normalize_group[3]) !=
	   (ess.normalize_group[0] | ess.normalize_group[1] | ess.normalize_group[2] | ess.normalize_group[3]))      	
	{
		printf("礼包组里面有重复的掩码, 礼包id=%d\n", id);
		return NULL;	
	}

	abase::vector<gift_item> gift_group_list[4];
	abase::vector<gift_item> gift_random_list;

	gift_item gift;
	bool found = false;
	for(size_t i = 0; i < 16; ++i)
	{
		found = false;
		memcpy(&gift, &ess.gifts[i], sizeof(gift_item));
		for(size_t j = 0; j < 4; ++j)
		{
			if(ess.normalize_group[j] & (1 << i))
			{
				gift_group_list[j].push_back(gift);
				found = true;
				continue;
			}
		}

		if(!found)
		{
			gift_random_list.push_back(gift);

		}
	}

	for(size_t i = 0; i < 4; ++i)
	{
		if(!gift_group_list[i].empty())
		{
			float prob = 0.0f;
			for(size_t j = 0; j < gift_group_list[i].size(); ++j)
			{
				prob += gift_group_list[i][j].prob;
			}

			if(fabs(1.0f - prob) > 1e-5)
			{
				printf("礼包里面组物品的概率和不为1, id=%d, group index=%d\n", id, i);
				return NULL;
			}
		}
	}
	
	return new item_giftbox(ess.open_time > 0 ? ess.open_time : 0, gift_group_list, gift_random_list);
}

static item_body * MakeItem(itemdataman& dataman,int id,const SMALL_ONLINE_GIFT_BAG_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_onlineaward_giftbox(ess.open_time > 0 ? ess.open_time : 0);
}

static item_body * MakeItem(itemdataman& dataman,int id,const VIP_CARD_ESSENCE& ess,addon_data * first,addon_data * end)
{
	if(ess.multiple_exp < 0.1 || ess.multiple_exp > 3) return NULL;
	return new item_vipcard(ess.expires);
}

static item_body * MakeItem(itemdataman& dataman,int id,const MERCENARY_CREDENTIAL_ESSENCE& ess,addon_data * first,addon_data * end)
{
	if(ess.pile_num_max != 1) return NULL;
	return new item_assist_card();
}

static item_body * MakeItem(itemdataman& dataman,int id,const TELEPORTATION_ESSENCE& ess,addon_data * first,addon_data * end)
{
	if(ess.pile_num_max != 1) return NULL;
	return new item_telestation(ess.max_tele_point, ess.default_day);
}

static item_body * MakeItem(itemdataman& dataman,int id,const SCROLL_DIG_COUNT_INC_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_treasure_digcount(ess.dig_count);
}

static item_body * MakeItem(itemdataman& dataman,int id,const XINGZUO_ITEM_ESSENCE & ess,addon_data * first,addon_data * end)
{
	static bool script_flag = true;
	if(script_flag) 
	{
		gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_ASTROLOGY);
		lua_State * L = keeper.GetState();
		if(L == NULL) return NULL;

		lua_register(L, "__ABASE_ASTROLOGY_GetLevel", astrology_essence::script_GetLevel);
		lua_register(L, "__ABASE_ASTROLOGY_SetLevel", astrology_essence::script_SetLevel);
		lua_register(L, "__ABASE_ASTROLOGY_GetScore", astrology_essence::script_GetScore);
		lua_register(L, "__ABASE_ASTROLOGY_SetScore", astrology_essence::script_SetScore);
		lua_register(L, "__ABASE_ASTROLOGY_GetEnergyConsume", astrology_essence::script_GetEnergyConsume);
		lua_register(L, "__ABASE_ASTROLOGY_SetEnergyConsume", astrology_essence::script_SetEnergyConsume);
		lua_register(L, "__ABASE_ASTROLOGY_GetProp", astrology_essence::script_GetProp);
		lua_register(L, "__ABASE_ASTROLOGY_SetProp", astrology_essence::script_SetProp);
		script_flag = false;
	}

	
	return new item_astrology(ess.equip_mask, ess.reborn_count);
}

static item_body * MakeItem(itemdataman& dataman,int id,const RUNE_EQUIP_ESSENCE & ess,addon_data * first,addon_data * end)
{
	static bool script_flag = true;
	if(script_flag) 
	{
		gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_RUNE);
		lua_State * L = keeper.GetState();
		if(L == NULL) return NULL;
		lua_register(L, "__ABASE_RUNE_QueryLevel", rune_essence::script_QueryLevel);
		lua_register(L, "__ABASE_RUNE_QueryExp", rune_essence::script_QueryExp);
		lua_register(L, "__ABASE_RUNE_QueryHole", rune_essence::script_GetHole);
		lua_register(L, "__ABASE_RUNE_QueryQuality", rune_essence::script_QueryQuality);
		lua_register(L, "__ABASE_RUNE_SetQuality", rune_essence::script_SetQuality);
		lua_register(L, "__ABASE_RUNE_SetRefineQuality", rune_essence::script_SetRefineQuality);
		lua_register(L, "__ABASE_RUNE_GetAttCnt", rune_essence::script_GetAttCnt);
		lua_register(L, "__ABASE_RUNE_GetAtt", rune_essence::script_GetAtt);
		lua_register(L, "__ABASE_RUNE_SetAtt", rune_essence::script_SetAtt);
		lua_register(L, "__ABASE_RUNE_SetRefineAtt", rune_essence::script_SetRefineAtt);
		lua_register(L, "__ABASE_RUNE_GetRefineCnt", rune_essence::script_GetRefineCnt);
		lua_register(L, "__ABASE_RUNE_GetResetCnt", rune_essence::script_GetResetCnt);
		lua_register(L, "__ABASE_RUNE_GetAvgGrade", rune_essence::script_GetAvgGrade);
		lua_register(L, "__ABASE_RUNE_SetAvgGrade", rune_essence::script_SetAvgGrade);

		script_flag = false;
	}

	return new rune_item(ess.quality, ess.hole_max, ess.require_level, ess.renascence_count); 
}


static item_body * MakeItem(itemdataman & dataman ,int id, const CHANGE_SHAPE_CARD_ESSENCE & ess, addon_data * first, addon_data * end)
{

	static bool script_flag = true;
	if(script_flag) 
	{
		gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_MAGIC);
		lua_State * L = keeper.GetState();
		if(L == NULL) return NULL;
		lua_register(L, "__ABASE_MAGIC_QueryLevel", magic_essence::script_QueryLevel);
		lua_register(L, "__ABASE_MAGIC_SetLevel", magic_essence::script_SetLevel);
		lua_register(L, "__ABASE_MAGIC_QueryExp", magic_essence::script_QueryExp);
		lua_register(L, "__ABASE_MAGIC_SetExp", magic_essence::script_SetExp);
		lua_register(L, "__ABASE_MAGIC_QueryMaxExp", magic_essence::script_QueryMaxExp);
		lua_register(L, "__ABASE_MAGIC_SetMaxExp", magic_essence::script_SetMaxExp);
		lua_register(L, "__ABASE_MAGIC_QueryExpLevel", magic_essence::script_QueryExpLevel);
		lua_register(L, "__ABASE_MAGIC_SetExpLevel", magic_essence::script_SetExpLevel);
		lua_register(L, "__ABASE_MAGIC_SetExpSpeed", magic_essence::script_SetExpSpeed);
		lua_register(L, "__ABASE_MAGIC_QueryInnerData", magic_essence::script_QueryInnerData);
		lua_register(L, "__ABASE_MAGIC_GetInnerDataCount", magic_essence::script_GetInnerDataCount);
		lua_register(L, "__ABASE_MAGIC_SetInnerData", magic_essence::script_SetInnerData);
		lua_register(L, "__ABASE_MAGIC_ClearInnerData", magic_essence::script_ClearInnerData);
		lua_register(L, "__ABASE_MAGIC_AddAddon", magic_essence::script_AddAddon);
		lua_register(L, "__ABASE_MAGIC_SetDuration", magic_essence::script_SetDuration);
		lua_register(L, "__ABASE_MAGIC_SetMaxDuration", magic_essence::script_SetMaxDuration);
		lua_register(L, "__ABASE_MAGIC_SetQuality", magic_essence::script_SetQuality);
		lua_register(L, "__ABASE_MAGIC_SetHP", magic_essence::script_SetHP);
		lua_register(L, "__ABASE_MAGIC_SetMP", magic_essence::script_SetMP);
		lua_register(L, "__ABASE_MAGIC_SetDamage", magic_essence::script_SetDamage);
		lua_register(L, "__ABASE_MAGIC_SetAttack", magic_essence::script_SetAttack);
		lua_register(L, "__ABASE_MAGIC_SetDefense", magic_essence::script_SetDefense);
		lua_register(L, "__ABASE_MAGIC_SetArmor", magic_essence::script_SetArmor);
		lua_register(L, "__ABASE_MAGIC_SetResistance", magic_essence::script_SetResistance);

		script_flag = false;
	}
	int require_gender = 0;
	switch(ess.require_gender)
	{
		case 0:
			require_gender = 0x01;
			break;
		case 1:
			require_gender = 0x02;
			break;
		case 2:
			require_gender = 0x03;
			break;
	}
	
	//Todo, Add by Houjun 2011-07-14, 模板修改后增加角色位
	magic_item *pItem = new magic_item(ess.character_combo_id,ess.character_combo_id2,ess.require_level,require_gender, 
			ess.energy_decrease_speed, ess.fee_recover_wakan, ess.renascence_count,
			ess.repu_index, ess.require_repu, ess.ref_temp_id);	
	
	//加入额外的附加属性
	if(first && end)
	{
		while(first != end)
		{
			pItem->InsertAddon(*first);
			first ++;
		}
	}
	return pItem;
}

static item_body * MakeItem(itemdataman& dataman,int id,const FLAG_BUFF_ITEM_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_battleflag(ess.cool_time);
}

static item_body * MakeItem(itemdataman& dataman,int id,const XINGZUO_ENERGY_ITEM_ESSENCE & ess,addon_data * first,addon_data * end)
{
	return new item_astrology_energy(ess.energy);
}

static item_body * MakeItem(itemdataman& dataman,int id,const CASH_MEDIC_MERGE_ITEM_ESSENCE& ess,addon_data * first,addon_data * end)
{
	return new item_bottle(ess.type, ess.item_ids, ess.require_level, ess.renascence_count, ess.cool_time);
}

static item_body * MakeItem(itemdataman& dataman, int id, const SALE_PROMOTION_ITEM_ESSENCE& ess, addon_data * first, addon_data * end)
{
	return new item_sale_promotion(ess);
}

static item_body * MakeItem(itemdataman& dataman,int id,const GIFT_BAG_LOTTERY_DELIVER_ESSENCE& ess,addon_data * first,addon_data * end)
{
	//判断是否有掩码位重复
	if((ess.normalize_group[0] + ess.normalize_group[1] + ess.normalize_group[2] + ess.normalize_group[3]) !=
	   (ess.normalize_group[0] | ess.normalize_group[1] | ess.normalize_group[2] | ess.normalize_group[3]))      	
	{
		printf("礼包组里面有重复的掩码, 礼包id=%d\n", id);
		return NULL;	
	}

	abase::vector<gift_item> gift_group_list[4];
	abase::vector<gift_item> gift_random_list;

	gift_item gift;
	bool found = false;
	for(size_t i = 0; i < 16; ++i)
	{
		found = false;
		memcpy(&gift, &ess.gifts[i], sizeof(gift_item));
		for(size_t j = 0; j < 4; ++j)
		{
			if(ess.normalize_group[j] & (1 << i))
			{
				gift_group_list[j].push_back(gift);
				found = true;
				continue;
			}
		}

		if(!found)
		{
			gift_random_list.push_back(gift);

		}
	}

	for(size_t i = 0; i < 4; ++i)
	{
		if(!gift_group_list[i].empty())
		{
			float prob = 0.0f;
			for(size_t j = 0; j < gift_group_list[i].size(); ++j)
			{
				prob += gift_group_list[i][j].prob;
			}

			if(fabs(1.0f - prob) > 1e-5)
			{
				printf("礼包里面组物品的概率和不为1, id=%d, group index=%d\n", id, i);
				return NULL;
			}
		}
	}
	
	return new item_giftbox(ess.open_time > 0 ? ess.open_time : 0, gift_group_list, gift_random_list);
}

static item_body * MakeItem(itemdataman& dataman, int id, const PROP_ADD_ITEM_ESSENCE& ess, addon_data * first, addon_data * end)
{
	return new item_propadd(ess.material_id, ess.require_level, ess.renascence_count, ess.prop_add);
}

static item_body * MakeItem(itemdataman& dataman, int id, const RUNE2013_ITEM_ESSENCE & ess, addon_data * first, addon_data * end)
{
	return new item_fuwen(ess.quality, ess.prop_type, ess.add_value, ess.self_exp);
}

int item_manager::__GetSpiritAddonSet(int crc, int soul_eid, int * pData, int dataCount) const
{
	if(dataCount <=0) return 0;
	/*
	   if(_sp_set_chk_table.find(crc) == _sp_set_chk_table.end()) return 0;
	   std::sort(begin, end);
	   spirit_set node(begin);
	   abase::hash_map<spirit_set, std::vector<int>, spirit_set>::const_iterator it = _spirit_set_table.find(node);
	   if(it == _spirit_set_table.end()) return 0;

	   const std::vector<int> & list = it->second;
	   int index = 0;
	   for(size_t i =0; i < list.size() && index < dataCount; i ++, index ++)
	   {
	   pData[index] = list[i];
	   index ++;
	   }
	 */
	abase::hash_map<unsigned int, std::vector<int> >::const_iterator it = _sp_eid_table.find(soul_eid);
	if(it == _sp_eid_table.end()) return 0;

	const std::vector<int> & list = it->second;
	int index = 0;
	for(size_t i =0; i < list.size() && index < dataCount; i ++, index ++)
	{
		pData[index] = list[i];
		//index ++;
	}
	return index;
}

bool 
item_manager:: __InitFromDataMan(itemdataman & dataman)
{
	//扫描附加属性数据
	DATA_TYPE  dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_ADDON,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ADDON,dt))
	{
		//__PRINTF("the addon id is: %d\n", id);
		DATA_TYPE dt;
		const EQUIPMENT_ADDON & addon= *(const EQUIPMENT_ADDON*)dataman.get_data_ptr(id,ID_SPACE_ADDON,dt);
		if(&addon == NULL  || dt != DT_EQUIPMENT_ADDON) 
		{
			__PRINTF("发现了不正确的addon %d %d\n",id, __LINE__);
			continue;
		}

		addon_data data = {addon.type, {addon.param1, addon.param2, addon.param3}};
		if(!addon_data_man::Instance().InsertAddon(id, data))
		{
			__PRINTF("发现了不正确的addon %d %d\n",id, __LINE__);
			continue;
		}
	}
	
	//扫描套装列表，组织套装临时结构
	std::map<int, std::vector<addon_data> > setaddon_map;
	id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	int set_addon_id = ITEM_ADDON_SET_BEGIN;
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_SUITE_ESSENCE)
		{
			DATA_TYPE dt2;
			const SUITE_ESSENCE & data= *(const SUITE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt2);
			ASSERT(dt2 == dt && &data);
			if(!data.equip_soul_suite)
			{
				std::vector<addon_data> alist;
				for(size_t i = 0; i < 13; i ++)
				{
					if(data.addons[i].id == 0) continue;
					//组织套装所属的附加属性
					int addon_id = data.addons[i].id;
					const addon_data_spec *pSpec = addon_data_man::Instance().GetAddon(addon_id);
					if(!pSpec)
					{
						__PRINTF("套装%d中发现了不正确的addon %d\n",id, addon_id);
						continue;
					}

					addon_data ad = pSpec->data;

					//加入一个套装所属的专用附加属性
					InsertSetAddon(set_addon_id,i + 2, ad.id);
					ad.id = set_addon_id;
					set_addon_id ++;

					//记录下这个属性
					alist.push_back(ad);
				}

				for(size_t i = 0; i < 14; i ++)
				{
					int sid = data.equipments[i].id;
					if(sid)
					{
						//组织套装内容
						if(setaddon_map.find(sid) != setaddon_map.end()) 
						{
							__PRINTF("套装%d中发现了重复的物品%d\n",id, sid);
						}
						setaddon_map[sid] = alist;
					}
				}
			}
			else
			{
				std::vector<int> alist;
				for(size_t i = 0; i < 13; i ++)
				{
					if(data.addons[i].id == 0) continue;
					//组织套装所属的附加属性
					int addon_id = data.addons[i].id;
					const addon_data_spec *pSpec = addon_data_man::Instance().GetAddon(addon_id);
					if(!pSpec)
					{
						__PRINTF("器魄组合%d中发现了不正确的addon %d\n",id, addon_id);
						continue;
					}

					//记录下这个属性
					alist.push_back(addon_id);
				}
				
				std::vector<int> elist;
				for(size_t i = 0; i < 14; i ++)
				{
					int sid = data.equipments[i].id;
					if(sid > 0) elist.push_back(sid);
				}
				ASSERT((int)SPIRIT_SET_SLOT_COUNT == (int)MAX_ENCHANT_SLOT);
				if(elist.size() != MAX_ENCHANT_SLOT) 
				{
					__PRINTF("器魄组合%d中器魄数目不正确\n",id);
					continue;
				}
				//std::sort(elist.begin(), elist.end());
				//spirit_set node(&elist[0]);
				//_spirit_set_table[node].swap(alist);
				//_sp_set_chk_table[node(node)] += 1;
				for (std::vector<int>::iterator it = elist.begin(); it != elist.end(); ++it)
				{
					ASSERT(_sp_eid_table.find(*it) == _sp_eid_table.end());
					std::copy(alist.begin(), alist.end(), std::back_inserter(_sp_eid_table[*it]));
					//_sp_eid_table[*it] = alist;
				}
			}
		}
	}


	
	//根据物品类型，生成所有可以生成的物品实体
	id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		IM_CHECK_ITEM(EQUIPMENT_ESSENCE);
		IM_CHECK_ITEM(ESTONE_ESSENCE);
		IM_CHECK_ITEM(PSTONE_ESSENCE);
		IM_CHECK_ITEM(SSTONE_ESSENCE);
		IM_CHECK_ITEM(REFINE_TICKET_ESSENCE);
		IM_CHECK_ITEM(RECIPEROLL_ESSENCE);
		IM_CHECK_ITEM(MEDICINE_ESSENCE);
		IM_CHECK_ITEM(TRANSMITROLL_ESSENCE);
		IM_CHECK_ITEM(LUCKYROLL_ESSENCE);
		IM_CHECK_ITEM(TASKDICE_ESSENCE);
		IM_CHECK_ITEM(DOUBLE_EXP_ESSENCE);
		IM_CHECK_ITEM(SKILLMATTER_ESSENCE);
		IM_CHECK_ITEM(VEHICLE_ESSENCE);
		IM_CHECK_ITEM(COUPLE_JUMPTO_ESSENCE);
		IM_CHECK_ITEM(LOTTERY_ESSENCE);
		IM_CHECK_ITEM(LOTTERY2_ESSENCE);
		IM_CHECK_ITEM(LOTTERY3_ESSENCE);	
		IM_CHECK_ITEM(FIREWORKS_ESSENCE);
		IM_CHECK_ITEM(CAMRECORDER_ESSENCE);
		IM_CHECK_ITEM(TEXT_FIREWORKS_ESSENCE);
		IM_CHECK_ITEM(TALISMAN_MAINPART_ESSENCE);
		IM_CHECK_ITEM(TALISMAN_ENERGYFOOD_ESSENCE);
		IM_CHECK_ITEM(SPEAKER_ESSENCE);
		IM_CHECK_ITEM(TOWNSCROLL_ESSENCE);
		IM_CHECK_ITEM(SIEGE_ARTILLERY_SCROLL_ESSENCE);
		IM_CHECK_ITEM(PET_BEDGE_ESSENCE);
		IM_CHECK_ITEM(PET_FOOD_ESSENCE);
		IM_CHECK_ITEM(PET_ARMOR_ESSENCE);
		IM_CHECK_ITEM(PET_AUTO_FOOD_ESSENCE);
		IM_CHECK_ITEM(PET_REFINE_ESSENCE);
		IM_CHECK_ITEM(PET_ASSIST_REFINE_ESSENCE);
		IM_CHECK_ITEM(AIRCRAFT_ESSENCE);
		IM_CHECK_ITEM(OFFLINE_TRUSTEE_ESSENCE);
		IM_CHECK_ITEM(DESTROYING_ESSENCE);
		IM_CHECK_ITEM(SPECIAL_NAME_ITEM_ESSENCE);
		IM_CHECK_ITEM(GIFT_BAG_ESSENCE);
		IM_CHECK_ITEM(SMALL_ONLINE_GIFT_BAG_ESSENCE);
		IM_CHECK_ITEM(VIP_CARD_ESSENCE);
		IM_CHECK_ITEM(CHANGE_SHAPE_CARD_ESSENCE);
		IM_CHECK_ITEM(MERCENARY_CREDENTIAL_ESSENCE);
		IM_CHECK_ITEM(TELEPORTATION_ESSENCE);
		IM_CHECK_ITEM(SCROLL_DIG_COUNT_INC_ESSENCE);
		IM_CHECK_ITEM(RUNE_EQUIP_ESSENCE);
		IM_CHECK_ITEM(XINGZUO_ITEM_ESSENCE);
		IM_CHECK_ITEM(FLAG_BUFF_ITEM_ESSENCE);
		IM_CHECK_ITEM(XINGZUO_ENERGY_ITEM_ESSENCE);
		IM_CHECK_ITEM(CASH_MEDIC_MERGE_ITEM_ESSENCE);
		IM_CHECK_ITEM(SALE_PROMOTION_ITEM_ESSENCE);
		IM_CHECK_ITEM(GIFT_BAG_LOTTERY_DELIVER_ESSENCE);
		IM_CHECK_ITEM(LOTTERY_TANGYUAN_ITEM_ESSENCE);
		IM_CHECK_ITEM(GIFT_PACK_ITEM_ESSENCE);
		IM_CHECK_ITEM(PROP_ADD_ITEM_ESSENCE);
		IM_CHECK_ITEM(RUNE2013_ITEM_ESSENCE);
		IM_CHECK_ITEM(COLORANT_ITEM_ESSENCE);  // Youshuang add

		const item_data * pData = (const item_data *)dataman.get_item_for_sell(id);
		if(pData && pData->proc_type & item::ITEM_PROC_TYPE_CASHITEM)
		{
			SetCashItem(id);
		}
	}


	if(!title_manager::InitFromDataMan(dataman))
	{
		printf("初始化称谓模板错误\n");
		return false;
	}

	if(!__InitVipAward(dataman))
	{
		printf("初始化VIP奖励数据错误\n");
		return false;
	}

	return __InitMall(dataman) && __InitBonusMall(dataman) && __InitZoneMall(dataman); 
}

bool 
item_manager::__InitVipAward(itemdataman & dataman)
{
	const abase::vector<VIP_AWARD_ITEM_SERV> & list = globaldata_getvipawardservice();

	for(size_t i = 0; i < list.size(); i++)
	{
		vipgame::node_t node;
		node.award_id			= list[i].award_id;
		node.award_type			= list[i].award_type;
		node.award_item_id		= list[i].award_item_id;
		node.award_item_count	= list[i].award_item_count;
		node.obtain_level		= list[i].obtain_level;
		node.obtain_type		= list[i].obtain_type;
		node.expire_time		= list[i].expire_time;

		//ASSERT(_vip_award.AddAward(node) && "VIP奖励数据错误");
		if(!_vip_award.AddAward(node))
		{
			return false;
		}
	}

	if(!_vip_award.CheckLevelAwardCnt())
	{
		return false;
	}

	__PRINTF("\nVIP奖励共有%d件物品！\n", list.size());
	_vipaward_timestamp = globaldata_getvipawardtimestamp();

	return true;
}

bool 
item_manager::__InitMall(itemdataman & dataman)
{
	const abase::vector<MALL_ITEM_SERV> & list = globaldata_getmallitemservice();
	bool bRst = true;
	for(size_t i = 0; i < list.size(); i ++)
	{
		int id = list[i].goods_id;
		int count = list[i].goods_count;
		int bonus = list[i].bonus;
		int props = list[i].props;
		int discount = list[i].discount;
		bool has_present = list[i].has_present;
		bool present_bind = list[i].present_bind;
		unsigned int present_id = list[i].present_id;
		unsigned int present_count = list[i].present_count;
		unsigned int present_time = list[i].present_time;

		if(id <= 0 || count <= 0) 
		{
			printf("百宝阁中物品id为%d及其数量为%d\n",id, count);
			bRst = false;
			continue;
		}

		int pile_limit = dataman.get_item_pile_limit(id);
		if(pile_limit <= 0 || count > pile_limit) 
		{
			printf("百宝阁中物品%d数量为%d，堆叠上限为%d\n",id, count, pile_limit);
			bRst = false;
			continue;
		}
		if(discount > 100)
		{
			printf("百宝阁中物品%d的折扣率为%d, 上限为100", id, discount);
			bRst = false;
			continue;
		}

		unsigned int present_pile_limit = dataman.get_item_pile_limit(present_id);
		if(present_time && present_pile_limit != 1)
		{
			printf("百宝阁中物品%d的赠品%u堆叠上限不为1,但是有过期时间\n", id, present_id); 
			bRst = false;
			continue;
		}
		if(has_present && present_count == 0)
		{
			printf("百宝阁中物品%d的赠品%u数量为0\n", id, present_id);
			bRst = false;
			continue;
		}

		//商城限时销售物品
		bool sale_time_active = false;
		time_t t1 = time(NULL);
		struct tm tm1;
		localtime_r(&t1, &tm1);
		int tz_adjust = -tm1.tm_gmtoff;    //gshop编辑器保存的时间是UTC,sale_time根据gs运行的时区进行调整
		int st_type  = list[i].type;
		int st_start = list[i].start_time;
		int st_end   = list[i].end_time;
		int st_param = list[i].param;
		//时区调整
		if(st_type == qgame::mall::sale_time::TYPE_INTERZONE)
		{
			if(qgame::mall::sale_time::HAS_STARTTIME & st_param)   st_start += tz_adjust;
			if(qgame::mall::sale_time::HAS_ENDTIME & st_param)   st_end += tz_adjust;
		}
		if(st_type != qgame::mall::sale_time::TYPE_NOLIMIT)
		{
			sale_time_active = true;
		}

		qgame::mall::node_t node;
		memset(&node, 0, sizeof(node));

		node.goods_id = id;
		node.goods_count = count;
		node.bonus_ratio = bonus; 
		node.props = props; 
		node.discount = discount; 

		node.has_present = has_present;
		node.present_bind = present_bind;
		node.present_id = present_id;
		node.present_count = present_count;
		node.present_time = present_time;

		node.sale_time_active = sale_time_active;
		if(sale_time_active)
		{
			ASSERT(node._sale_time.TrySetParam(st_type, st_start, st_end, st_param));
		}
			
		ASSERT(qgame::mall::MAX_ENTRY == 4);
		bool bExpire = false;
		size_t slot_count = 0;
		for(size_t j = 0; j < 4;  j ++)
		{
			if(list[i].list[j].cash_need <= 0) break;
			int expire_time = list[i].list[j].expire_time;
			node.entry[slot_count].expire_time = expire_time;
			node.entry[slot_count].cash_need = list[i].list[j].cash_need;
			if(expire_time) bExpire = true;
			slot_count ++;
		}

		if(slot_count == 0)
		{
			printf("百宝阁中物品%d不存在可用的条目索引%d\n", id, i);
			bRst = false;
			continue;
		}
		
		if(bExpire && pile_limit != 1)
		{
			printf("百宝阁中物品存在可堆叠但存在有效期的物品%d，无法进行初始化操作 索引%d\n", id, i);
			bRst = false;
			continue;
		}

		_mall.AddGoods(node);
		if(sale_time_active)
		{
			_mall.AddLimitGoods(node, i);
		}
		//printf("加入%d个%d到白宝阁中\n",count,id);
	}
	printf("百宝阁内共有%d样货物\n",list.size());
	_mall_timestamp = globaldata_getmalltimestamp();
	return bRst;
}

bool 
item_manager::__InitBonusMall(itemdataman & dataman)
{
	const abase::vector<MALL_ITEM_SERV> & list = globaldata_getbonusmallitemservice();
	bool bRst = true;
	for(size_t i = 0; i < list.size(); i ++)
	{
		int id = list[i].goods_id;
		int count = list[i].goods_count;
		int bonus = list[i].bonus;
		int props = list[i].props;
		int discount = list[i].discount;
		bool has_present = list[i].has_present;
		bool present_bind = list[i].present_bind;
		unsigned int present_id = list[i].present_id;
		unsigned int present_count = list[i].present_count;
		unsigned int present_time = list[i].present_time;
		if(id <= 0 || count <= 0) 
		{
			printf("鸿利商城中物品id为%d及其数量为%d\n",id, count);
			bRst = false;
			continue;
		}

		int pile_limit = dataman.get_item_pile_limit(id);
		if(pile_limit <= 0 || count > pile_limit) 
		{
			printf("鸿利商城中物品%d数量为%d，堆叠上限为%d\n",id, count, pile_limit);
			bRst = false;
			continue;
		}

		if(discount > 100)
		{
			printf("鸿利商城种物品%d的折扣率为%d, 上限为100", id, discount);
			bRst = false;
			continue;
		}

		unsigned int present_pile_limit = dataman.get_item_pile_limit(present_id);
		if(present_time && present_pile_limit != 1)
		{
			printf("鸿利商城中物品%d的赠品%u堆叠上限不为1,但是有过期时间\n", id, present_id); 
			bRst = false;
			continue;
		}
		if(has_present && present_count == 0)
		{
			printf("鸿利商城中物品%d的赠品%u数量为0\n", id, present_id);
			bRst = false;
			continue;
		}
		if(has_present && present_pile_limit == 1 && present_count != 1)
		{
			printf("鸿利商城中物品%d的赠品堆叠数量为1但是数量不等于1\n", id);
			bRst = false;
			continue;
		}

		qgame::mall::node_t node;
		memset(&node, 0, sizeof(node));

		node.goods_id = id;
		node.goods_count = count;
		node.bonus_ratio = bonus;
		node.props = props;
		node.discount = discount;
		
		node.has_present = has_present;
		node.present_bind = present_bind;
		node.present_id = present_id;
		node.present_count = present_count;
		node.present_time = present_time;

		ASSERT(qgame::mall::MAX_ENTRY == 4);
		bool bExpire = false;
		size_t slot_count = 0;
		for(size_t j = 0; j < 4;  j ++)
		{
			if(list[i].list[j].cash_need <= 0) break;
			int expire_time = list[i].list[j].expire_time;
			node.entry[slot_count].expire_time = expire_time;
			node.entry[slot_count].cash_need = list[i].list[j].cash_need;
			if(expire_time) bExpire = true;
			slot_count ++;
		}

		if(slot_count == 0)
		{
			printf("鸿利商城中物品%d不存在可用的条目索引%d\n", id, i);
			bRst = false;
			continue;
		}
		
		if(bExpire && pile_limit != 1)
		{
			printf("鸿利商城中物品存在可堆叠但存在有效期的物品%d，无法进行初始化操作 索引%d\n", id, i);
			bRst = false;
			continue;
		}

		_bonusmall.AddGoods(node);
	}
	printf("鸿利商城内共有%d样货物\n",list.size());
	_bonusmall_timestamp = globaldata_getbonusmalltimestamp();
	return bRst;
}

bool 
item_manager::__InitZoneMall(itemdataman & dataman)
{
	const abase::vector<MALL_ITEM_SERV> & list = globaldata_getzonemallitemservice();
	bool bRst = true;
	for(size_t i = 0; i < list.size(); i ++)
	{
		int id = list[i].goods_id;
		int count = list[i].goods_count;
		int bonus = list[i].bonus;
		int props = list[i].props;
		int discount = list[i].discount;
		bool has_present = list[i].has_present;
		bool present_bind = list[i].present_bind;
		unsigned int present_id = list[i].present_id;
		unsigned int present_count = list[i].present_count;
		unsigned int present_time = list[i].present_time;
		if(id <= 0 || count <= 0) 
		{
			printf("跨服商城中物品id为%d及其数量为%d\n",id, count);
			bRst = false;
			continue;
		}

		int pile_limit = dataman.get_item_pile_limit(id);
		if(pile_limit <= 0 || count > pile_limit) 
		{
			printf("跨服商城中物品%d数量为%d，堆叠上限为%d\n",id, count, pile_limit);
			bRst = false;
			continue;
		}

		if(discount > 100)
		{
			printf("跨服商城种物品%d的折扣率为%d, 上限为100", id, discount);
			bRst = false;
			continue;
		}

		unsigned int present_pile_limit = dataman.get_item_pile_limit(present_id);
		if(present_time && present_pile_limit != 1)
		{
			printf("跨服商城中物品%d的赠品%u堆叠上限不为1,但是有过期时间\n", id, present_id); 
			bRst = false;
			continue;
		}
		if(has_present && present_count == 0)
		{
			printf("跨服商城中物品%d的赠品%u数量为0\n", id, present_id);
			bRst = false;
			continue;
		}
		if(has_present && present_pile_limit == 1 && present_count != 1)
		{
			printf("跨服商城中物品%d的赠品堆叠数量为1但是数量不等于1\n", id);
			bRst = false;
			continue;
		}

		qgame::mall::node_t node;
		memset(&node, 0, sizeof(node));

		node.goods_id = id;
		node.goods_count = count;
		node.bonus_ratio = bonus;
		node.props = props;
		node.discount = discount;
		
		node.has_present = has_present;
		node.present_bind = present_bind;
		node.present_id = present_id;
		node.present_count = present_count;
		node.present_time = present_time;

		ASSERT(qgame::mall::MAX_ENTRY == 4);
		bool bExpire = false;
		size_t slot_count = 0;
		for(size_t j = 0; j < 4;  j ++)
		{
			if(list[i].list[j].cash_need <= 0) break;
			int expire_time = list[i].list[j].expire_time;
			node.entry[slot_count].expire_time = expire_time;
			node.entry[slot_count].cash_need = list[i].list[j].cash_need;
			if(expire_time) bExpire = true;
			slot_count ++;
		}

		if(slot_count == 0)
		{
			printf("跨服商城中物品%d不存在可用的条目索引%d\n", id, i);
			bRst = false;
			continue;
		}
		
		if(bExpire && pile_limit != 1)
		{
			printf("跨服商城中物品存在可堆叠但存在有效期的物品%d，无法进行初始化操作 索引%d\n", id, i);
			bRst = false;
			continue;
		}

		_zonemall.AddGoods(node);
	}
	printf("跨服商城内共有%d样货物\n",list.size());
	_zonemall_timestamp = globaldata_getzonemalltimestamp();
	return bRst;
}



bool title_manager::__InitFromDataMan(itemdataman & dataman)
{
	DATA_TYPE dt;
	int id = dataman.get_first_data_id(ID_SPACE_CONFIG,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_CONFIG,dt))
	{
		if(dt == DT_TITLE_PROP_CONFIG)
		{
			const  TITLE_PROP_CONFIG &config = *(const TITLE_PROP_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_TITLE_PROP_CONFIG && &config);
			if(config.id_title <= 0) 
			{
				printf("属性称谓%d是非法称谓\n",config.id_title);
				return false;
			}

			int a[3] = {config.id_addons[0], config.id_addons[1], config.id_addons[2]};
			if(!__AddTitleHandler(dataman, config.id_title, a))
			{
				printf("属性称谓%d加入失败，此称谓被忽略\n",config.id_title);
				continue;
			}
		}
	}
	printf("共加入有属性称谓%d个\n",_map.size());

	return true;
}

bool
title_manager::__AddAddon(itemdataman & dataman, node_t & node, int addon_id)
{
	DATA_TYPE dt3;
	const EQUIPMENT_ADDON & addon= *(const EQUIPMENT_ADDON*)dataman.get_data_ptr(addon_id,ID_SPACE_ADDON,dt3);
	if(&addon == NULL  || dt3 != DT_EQUIPMENT_ADDON) 
	{
		return false;
	}

	addon_data data = {addon.type, {addon.param1, addon.param2, addon.param3}};
	if(!addon_manager::TransformData(data)) 
	{
		return false;
	}

	addon_handler * handler = addon_manager::QueryHandler(addon.type);
	if(!handler) return false;
	
	node.AddAddon(handler, data);
	return true;
}

bool
title_manager::__AddTitleHandler(itemdataman & dataman, int id,  int addon_id[3])
{
	node_t node;
	for(size_t i = 0; i < 3; i ++)
	{
		if(addon_id[i])
		{
			if(!__AddAddon(dataman, node, addon_id[i])) return false;
		}
	}
	if(!node.count) return false;

	_map[id] = node;
	return true;
}

bool
title_manager::__ActiveTitle(gactive_imp * imp, int title)
{
	__TITLE_MAP::iterator it = _map.find(title);
	if(it == _map.end()) return false;
	const node_t & node = it->second;
	const node_t::entry_t * pEntry = node.addon;
	for(int i = 0; i < node.count; i ++, pEntry ++)
	{
		pEntry->_handler->Activate(pEntry->_data, NULL, imp, NULL);
	}
	return true;
}

bool
title_manager::__DeactiveTitle(gactive_imp * imp, int title)
{
	__TITLE_MAP::iterator it = _map.find(title);
	if(it == _map.end()) return false;
	const node_t & node = it->second;
	const node_t::entry_t * pEntry = node.addon;
	for(int i = 0; i < node.count; i ++, pEntry ++)
	{
		pEntry->_handler->Deactivate(pEntry->_data, NULL, imp, NULL);
	}
	return true;
}

title_manager title_manager::__instance;

