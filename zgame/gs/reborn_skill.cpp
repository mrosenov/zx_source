#include "gmatrix.h"
#include "reborn_skill.h"


bool reborn_bonus_man::InitFromDataMan(itemdataman & dataman) 
{ 
	DATA_TYPE dt;
	int id = dataman.get_first_data_id(ID_SPACE_CONFIG,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_CONFIG,dt))
	{
		if(dt == DT_RENASCENCE_SKILL_CONFIG)
		{
			const  RENASCENCE_SKILL_CONFIG &config = *(const RENASCENCE_SKILL_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_RENASCENCE_SKILL_CONFIG && &config);

			reborn_skill_bonus bonus;
			bonus.require[0] = config.occup_lev_0;
			bonus.require[1] = config.occup_lev_1;
			bonus.require[2] = config.occup_lev_2;
			bonus.require[3] = config.occup_lev_3;

			bonus.require1[0] = config.occup_lev_0_2;
			bonus.require1[1] = config.occup_lev_1_2;
			bonus.require1[2] = config.occup_lev_2_2;
			bonus.require1[3] = config.occup_lev_3_2;

			for(size_t i =0; i < 12; i ++)
			{
				bonus.bonus_skill[i].skill	= config.skills[i].id;
				bonus.bonus_skill[i].lvl	= config.skills[i].lev;
			}
			_bonus_list.push_back(bonus);
		}
		else if(dt == DT_RENASCENCE_PROP_CONFIG)
		{
			const  RENASCENCE_PROP_CONFIG &config = *(const RENASCENCE_PROP_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_RENASCENCE_PROP_CONFIG && &config);
			reborn_prop_bonus bonus;
			bonus.require[0] = config.occup_lev_0;
			bonus.require[1] = config.occup_lev_1;
			bonus.require[2] = config.occup_lev_2;
			bonus.require[3] = config.occup_lev_3;

			bonus.require1[0] = config.occup_lev_0_2;
			bonus.require1[1] = config.occup_lev_1_2;
			bonus.require1[2] = config.occup_lev_2_2;
			bonus.require1[3] = config.occup_lev_3_2;

			memcpy(bonus.hp, 		config.hp, 		sizeof(bonus.hp));
			memcpy(bonus.mp, 		config.mp, 		sizeof(bonus.mp));
			memcpy(bonus.dmg, 		config.dmg, 		sizeof(bonus.dmg));
			memcpy(bonus.def, 		config.def, 		sizeof(bonus.def));
			memcpy(bonus.attack, 		config.attack, 		sizeof(bonus.attack));
			memcpy(bonus.armor, 		config.armor, 		sizeof(bonus.armor));
			memcpy(bonus.crit_rate, 	config.crit_rate, 	sizeof(bonus.crit_rate));
			memcpy(bonus.crit_damage, 	config.crit_damage, 	sizeof(bonus.crit_damage));
			memcpy(bonus.anti, 		config.anti, 		sizeof(bonus.anti));
			_prop_list.push_back(bonus);
		}
	}
	
	return true;
}

void reborn_bonus_man::ActivatePropBonus(int reborn_count, const int r_level[4], const int r_prof[4], q_extend_prop & prop) const
{
	const reborn_prop_bonus * pBonus = GetPropBonus(r_prof[0],r_prof[1],r_prof[2],r_prof[3]);
	if(pBonus == NULL) return ;

	//开始进行各种数据的处理
	prop.max_hp	+= (int) CalcValue(r_level, pBonus->hp, reborn_count);
	prop.max_mp	+= (int) CalcValue(r_level, pBonus->mp, reborn_count);
	int dmg 	=  (int) CalcValue(r_level, pBonus->dmg, reborn_count);
	prop.damage_low +=dmg;
	prop.damage_high +=dmg;

	prop.defense 	+= (int) CalcValue(r_level, pBonus->def, reborn_count);
	prop.attack 	+= (int) CalcValue(r_level, pBonus->attack, reborn_count);
	prop.armor	+= (int) CalcValue(r_level, pBonus->armor, reborn_count);
	prop.crit_rate	+= (int) (CalcValue(r_level, pBonus->crit_rate, reborn_count) * 10);
	prop.crit_damage+= CalcValue(r_level, pBonus->crit_damage, reborn_count)/100.f;
	prop.resistance[0]+= (int) CalcValue(r_level, pBonus->anti[0], reborn_count);
	prop.resistance[1]+= (int) CalcValue(r_level, pBonus->anti[1], reborn_count);
	prop.resistance[2]+= (int) CalcValue(r_level, pBonus->anti[2], reborn_count);
	prop.resistance[3]+= (int) CalcValue(r_level, pBonus->anti[3], reborn_count);
	prop.resistance[4]+= (int) CalcValue(r_level, pBonus->anti[4], reborn_count);
	prop.resistance[5]+= (int) CalcValue(r_level, pBonus->anti[5], reborn_count);
}

