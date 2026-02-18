#include "item_fuwen.h"
#include "../player_imp.h"

int
item_fuwen::GetFuwenLevel(item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	int exp = 0;
	if(len == sizeof(fuwen_essence)) 
	{
		exp = *(int*)buf;
	}

	return player_template::GetFuwenLevel(exp, _quality); 
}


void 
item_fuwen::OnActivate(size_t index,gactive_imp* obj, item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	int exp = 0;
	if(len == sizeof(fuwen_essence)) 
	{
		exp = *(int*)buf;
	}

	int level = player_template::GetFuwenLevel(exp, _quality); 

	int add_value = 0; 
	int add_value2 = 0;	//千分数 (暴击率)
	float add_value3 = 0.f;	//浮点数 (暴击伤害)
	add_value = int(_value * level  + 0.1);
	add_value2 = int(_value * level * 10 + 0.1); //百分数转换成千分数
	add_value3 = _value * level;
	object_interface oif(obj);
	switch(_prop_type)
	{
		case PROP_TYPE_DAMAGE:
			oif.EnhanceDamage(add_value);
		break;

		case PROP_TYPE_DEF:
			oif.EnhanceDefense(add_value);
		break;

		case PROP_TYPE_HP:
			oif.EnhanceMaxHP(add_value);
		break;

		case PROP_TYPE_MP:
			oif.EnhanceMaxMP(add_value);
		break;

		case PROP_TYPE_ATTACK:
			oif.EnhanceAttack(add_value);
		break;

		case PROP_TYPE_ARMOR:
			oif.EnhanceArmor(add_value);
		break;

		case PROP_TYPE_CRIT_RATE:
			oif.EnhanceCrit(add_value2);
		break;

		case PROP_TYPE_CRIT_ANTI:
			oif.EnhanceAntiCritRate(add_value2);
		break;

		case PROP_TYPE_CRIT_DAMAGE:
			oif.EnhanceCritDmg(add_value3);
		break;

		case PROP_TYPE_CRIT_DAMAGE_ANTI:
			oif.EnhanceAntiCritDamage(add_value3);
		break;

		case PROP_TYPE_SKILL_ATTACK_RATE:
			oif.EnhanceSkillAttack(add_value2);
		break;

		case PROP_TYPE_SKILL_ARMOR_RATE:
			oif.EnhanceSkillArmor(add_value2);
		break;

		case PROP_TYPE_RESISTANCE_1:
			oif.EnhanceResistance(0, add_value);
		break;

		case PROP_TYPE_RESISTANCE_2:
			oif.EnhanceResistance(3, add_value);
		break;

		case PROP_TYPE_RESISTANCE_3:
			oif.EnhanceResistance(4, add_value);
		break;

		case PROP_TYPE_RESISTANCE_4:
			oif.EnhanceResistance(1, add_value);
		break;

		case PROP_TYPE_RESISTANCE_5:
			oif.EnhanceResistance(2, add_value);
		break;

		case PROP_TYPE_CULT_DEFANCE_1:
			oif.EnhanceCultDefense(0, add_value);
		break;

		case PROP_TYPE_CULT_DEFANCE_2:
			oif.EnhanceCultDefense(1, add_value);
		break;

		case PROP_TYPE_CULT_DEFANCE_3:
			oif.EnhanceCultDefense(2, add_value);
		break;

		case PROP_TYPE_CULT_ATTACK_1:
			oif.EnhanceCultAttack(0, add_value);
		break;

		case PROP_TYPE_CULT_ATTACK_2:
			oif.EnhanceCultAttack(1, add_value);
		break;

		case PROP_TYPE_CULT_ATTACK_3:
			oif.EnhanceCultAttack(2, add_value);
		break;

		case PROP_TYPE_SKILLED_1:
			oif.EnhanceResistanceProficiency(0, add_value);
		break;

		case PROP_TYPE_SKILLED_2:
			oif.EnhanceResistanceProficiency(3, add_value);
		break;

		case PROP_TYPE_SKILLED_3:
			oif.EnhanceResistanceProficiency(4, add_value);
		break;

		case PROP_TYPE_SKILLED_4:
			oif.EnhanceResistanceProficiency(1, add_value);
		break;

		case PROP_TYPE_SKILLED_5:
			oif.EnhanceResistanceProficiency(2, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_1:
			oif.EnhanceResistanceTenaciy(0, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_2:
			oif.EnhanceResistanceTenaciy(3, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_3:
			oif.EnhanceResistanceTenaciy(4, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_4:
			oif.EnhanceResistanceTenaciy(1, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_5:
			oif.EnhanceResistanceTenaciy(2, add_value);
		break;

		case PROP_TYPE_MOVE_SPEED:
			oif.EnhanceSpeed(add_value3);
		break;

		case PROP_TYPE_FLY_SPEED:
			oif.EnhanceMountSpeed(add_value3);
		break;
	}

}

void
item_fuwen::OnDeactivate(size_t index,gactive_imp* obj, item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	int exp = 0;
	if(len == sizeof(fuwen_essence)) 
	{
		exp = *(int*)buf;
	}

	int level = player_template::GetFuwenLevel(exp, _quality); 

	int add_value = 0; 
	int add_value2 = 0;	//千分数 (暴击率)
	float add_value3 = 0.f;	//浮点数 (暴击伤害)
	add_value = int(_value * level  + 0.1);
	add_value2 = int(_value * level * 10 + 0.1); //百分数转换成千分数
	add_value3 = _value * level;
	object_interface oif(obj);
	switch(_prop_type)
	{
		case PROP_TYPE_DAMAGE:
			oif.ImpairDamage(add_value);
		break;

		case PROP_TYPE_DEF:
			oif.ImpairDefense(add_value);
		break;

		case PROP_TYPE_HP:
			oif.ImpairMaxHP(add_value);
		break;

		case PROP_TYPE_MP:
			oif.ImpairMaxMP(add_value);
		break;

		case PROP_TYPE_ATTACK:
			oif.ImpairAttack(add_value);
		break;

		case PROP_TYPE_ARMOR:
			oif.ImpairArmor(add_value);
		break;

		case PROP_TYPE_CRIT_RATE:
			oif.ImpairCrit(add_value2);
		break;

		case PROP_TYPE_CRIT_ANTI:
			oif.ImpairAntiCritRate(add_value2);
		break;

		case PROP_TYPE_CRIT_DAMAGE:
			oif.ImpairCritDmg(add_value3);
		break;

		case PROP_TYPE_CRIT_DAMAGE_ANTI:
			oif.ImpairAntiCritDamage(add_value3);
		break;

		case PROP_TYPE_SKILL_ATTACK_RATE:
			oif.ImpairSkillAttack(add_value2);
		break;

		case PROP_TYPE_SKILL_ARMOR_RATE:
			oif.ImpairSkillArmor(add_value2);
		break;

		case PROP_TYPE_RESISTANCE_1:
			oif.ImpairResistance(0, add_value);
		break;

		case PROP_TYPE_RESISTANCE_2:
			oif.ImpairResistance(3, add_value);
		break;

		case PROP_TYPE_RESISTANCE_3:
			oif.ImpairResistance(4, add_value);
		break;

		case PROP_TYPE_RESISTANCE_4:
			oif.ImpairResistance(1, add_value);
		break;

		case PROP_TYPE_RESISTANCE_5:
			oif.ImpairResistance(2, add_value);
		break;

		case PROP_TYPE_CULT_DEFANCE_1:
			oif.ImpairCultDefense(0, add_value);
		break;

		case PROP_TYPE_CULT_DEFANCE_2:
			oif.ImpairCultDefense(1, add_value);
		break;

		case PROP_TYPE_CULT_DEFANCE_3:
			oif.ImpairCultDefense(2, add_value);
		break;

		case PROP_TYPE_CULT_ATTACK_1:
			oif.ImpairCultAttack(0, add_value);
		break;

		case PROP_TYPE_CULT_ATTACK_2:
			oif.ImpairCultAttack(1, add_value);
		break;

		case PROP_TYPE_CULT_ATTACK_3:
			oif.ImpairCultAttack(2, add_value);
		break;

		case PROP_TYPE_SKILLED_1:
			oif.ImpairResistanceProficiency(0, add_value);
		break;

		case PROP_TYPE_SKILLED_2:
			oif.ImpairResistanceProficiency(3, add_value);
		break;

		case PROP_TYPE_SKILLED_3:
			oif.ImpairResistanceProficiency(4, add_value);
		break;

		case PROP_TYPE_SKILLED_4:
			oif.ImpairResistanceProficiency(1, add_value);
		break;

		case PROP_TYPE_SKILLED_5:
			oif.ImpairResistanceProficiency(2, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_1:
			oif.ImpairResistanceTenaciy(0, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_2:
			oif.ImpairResistanceTenaciy(3, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_3:
			oif.ImpairResistanceTenaciy(4, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_4:
			oif.ImpairResistanceTenaciy(1, add_value);
		break;

		case PROP_TYPE_TOUGHNESS_5:
			oif.ImpairResistanceTenaciy(2, add_value);
		break;

		case PROP_TYPE_MOVE_SPEED:
			oif.ImpairSpeed(add_value3);
		break;

		case PROP_TYPE_FLY_SPEED:
			oif.ImpairMountSpeed(add_value3);
		break;
	}
}



void
item_fuwen::UpgradeFuwen(item *parent, gactive_imp *imp, size_t main_fuwen_index, int main_fuwen_where, size_t assist_count, int assist_fuwen_index[])
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	if(main_fuwen_where == gplayer_imp::IL_FUWEN) OnDeactivate(main_fuwen_index, imp, parent);

	int add_exp = 0;
	for(size_t i = 0; i < assist_count; ++i)
	{
		int index2 = assist_fuwen_index[i];
		item & it2 = pImp->GetInventory()[index2];
		
		if(it2.type == g_config.fuwen_fragment_id[0] || it2.type == g_config.fuwen_fragment_id[1])
		{
			add_exp += it2.count;
		}
		else if(it2.GetItemType() == item_body::ITEM_TYPE_FUWEN)
		{
			size_t len2;
			void * buf2 = it2.GetContent(len2);
			int init_exp = ((item_fuwen*)it2.body)->GetFuwenSelfExp();
			add_exp += init_exp;

			if(len2 == sizeof(fuwen_essence)) 
			{
				int fuwen_exp = *(int*)buf2;
				add_exp += fuwen_exp;
			}
		}
	}

	size_t len1;
	void * buf1 = parent->GetContent(len1);

	if(len1 == 0) 
	{
		fuwen_essence ess;
		memset(&ess, 0, sizeof(ess));
		ess.exp = add_exp;
		if(ess.exp > player_template::GetFuwenMaxExp(_quality)) ess.exp = player_template::GetFuwenMaxExp(_quality);
		parent->SetContent(&ess, sizeof(ess));
	}
	else
	{
		*(int*)buf1 += add_exp;
		int cur_exp = *(int*)buf1;

		if(cur_exp > player_template::GetFuwenMaxExp(_quality)) 
		{
			cur_exp = player_template::GetFuwenMaxExp(_quality);
			*(int*)buf1 = cur_exp;
		}
	}

	if(main_fuwen_where == gplayer_imp::IL_FUWEN) 
	{
		OnActivate(main_fuwen_index, imp, parent);
		property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
		if(pImp->_basic.hp > pImp->GetMaxHP()) pImp->_basic.hp = pImp->GetMaxHP();
		if(pImp->_basic.mp > pImp->GetMaxMP()) pImp->_basic.mp = pImp->GetMaxMP();
	}

	pImp->PlayerGetItemInfo(main_fuwen_where,main_fuwen_index);
}


void
item_fuwen::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::BODY:
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};
}

void
item_fuwen::OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::FUWEN_INVENTORY:
			Deactivate(index,obj,parent);
			break;
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};
}

