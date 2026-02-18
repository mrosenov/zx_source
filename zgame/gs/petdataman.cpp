#include "world.h"
#include "player_imp.h"
#include "petdataman.h"
#include "template/itemdataman.h"

bool pet_bedge_dataman::LoadTemplate(itemdataman& dataman)
{
	//转换模板
	DATA_TYPE  dt; 
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{       
		if(dt == DT_PET_BEDGE_ESSENCE)
		{
			const PET_BEDGE_ESSENCE& ess = *(const PET_BEDGE_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);                 
			ASSERT(&ess && dt == DT_PET_BEDGE_ESSENCE);
			pet_bedge_data_temp pt;
			memset(&pt,0,sizeof(pt));
			pt.tid = id;
			pt.body_size = ess.size;
			pt.pet_type = ess.pet_type;
			pt.immune_type = ess.immune_type;
			pt.init_level = ess.level;
			pt.sight_range = ess.sight_range;
			pt.attack_range = ess.attack_range;
			pt.honor_point_inc_speed = ess.intimacy_inc_speed;
			pt.hunger_point_dec_speed[0] = ess.hunger_speed1;
			pt.hunger_point_dec_speed[1] = ess.hunger_speed2;
			pt.hp_gen[0] = ess.hp_gen1;
			pt.hp_gen[1] = ess.hp_gen2;
			pt.vp_gen[0] = ess.mp_gen1;
			pt.vp_gen[1] = ess.mp_gen2;
			pt.walk_speed = ess.walk_speed;
			pt.run_speed = ess.run_speed;
			Insert(pt);
		}
	}
	return true;
}

bool pet_bedge_dataman::GenerateBaseProp(int tid,int level,q_extend_prop& prop)
{
	const pet_bedge_data_temp * pTmp = Get(tid);
	if(!pTmp) return false;
	/*
	memset(&prop,0,sizeof(prop));

	prop.max_hp = (int)(pTmp->hp_factor * (level + 2));
	prop.max_mp =  0;
	int hp_gen = (int)(pTmp->hp_gen_factor * (level + 2));
	prop.hp_gen[0] = hp_gen;
	prop.hp_gen[1] = hp_gen;
	prop.hp_gen[2] = hp_gen;
	prop.hp_gen[3] = hp_gen;

	float speed = pTmp->speed_a + pTmp->speed_b * (level - 1);
	prop.run_speed = speed;

	int damage = (int)((pTmp->damage_a * level + pTmp->damage_b*level*level) * pTmp->damage_c);
	prop.attack = (int)(pTmp->attack_factor * level);
	prop.damage_low = damage;
	prop.damage_high = damage;

	int res = (int)(pTmp->resistance_factor*level - 15);
	prop.resistance[0] = res;
	prop.resistance[1] = res;
	prop.resistance[2] = res;
	prop.resistance[3] = res;
	prop.resistance[4] = res;
	prop.resistance[5] = res;

	prop.defense = (int)(pTmp->defense_factor * level - 15);
	prop.armor = (int)(pTmp->armor_factor * level);
	*/
	return true;
}

bool pet_equip_dataman::LoadTemplate(itemdataman& dataman)
{
	//转换模板
	DATA_TYPE  dt; 
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{       
		if(dt == DT_PET_ARMOR_ESSENCE)
		{
			const PET_ARMOR_ESSENCE& ess = *(const PET_ARMOR_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);                 
			ASSERT(&ess && dt == DT_PET_ARMOR_ESSENCE);
			pet_equip_data_temp pt;
			memset(&pt,0,sizeof(pt));
			pt.tid = id;
			pt.pet_type_mask = ess.pet_type_mask;
			pt.level_require = ess.lev_required;
			pt.astro_type = ess.astro_type;
			pt.astro_level = ess.astro_level;
			pt.equip_location = ess.equip_location;
			ASSERT(sizeof(pt.int_props) == sizeof(ess.int_props));
			memcpy(pt.int_props,ess.int_props,sizeof(int) * 2 * 12);
			ASSERT(sizeof(pt.float_props) == sizeof(ess.float_props));
			memcpy(pt.float_props,ess.float_props,sizeof(float) * 2 * 2);
			Insert(pt);
		}
	}
	return true;
}

bool pet_equip_dataman::GenerateBaseProp(int tid,int level,q_extend_prop& prop)
{
	const pet_equip_data_temp* pTmp = Get(tid);
	if(!pTmp) return false;
	return true;
}

