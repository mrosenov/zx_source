#ifndef __ONLINEGAME_GS_PROPERTY_FACADE_H__
#define __ONLINEGAME_GS_PROPERTY_FACADE_H__

#include <arandomgen.h>
#include "property.h"
namespace netgame
{
	template <typename EXTEND_PROP>
	inline int GetMaxHP(const EXTEND_PROP  & prop)
	{
		return prop.max_hp;
	}
	
	template <typename EXTEND_PROP>
	inline int GetMaxMP(const EXTEND_PROP  & prop)
	{
		return prop.max_mp;
	}
	
	template <typename EXTEND_PROP>
	inline int GetMaxDP(const EXTEND_PROP  & prop)
	{
		return prop.max_dp;
	}

	template <typename EXTEND_PROP, typename ITEM_PROP>
	inline int GetAttackSpeed(const EXTEND_PROP  & prop, const ITEM_PROP &)
	{
		return prop.attack_speed;
	}
	
	template <>
	inline int GetAttackSpeed<q_extend_prop, q_item_prop>(const q_extend_prop  & prop, const q_item_prop & item)
	{
		return item.attack_cycle;
	}

	template <typename EXTEND_PROP>
	inline int GetHPGen(const EXTEND_PROP  & prop,bool sitdown)
	{
		return prop.hp_gen * 4;
	}

	template <typename EXTEND_PROP>
	inline int GetCombatHPGen(const EXTEND_PROP  & prop, bool sitdown)
	{
		return prop.hp_gen;
	}

	template <>
	inline int GetCombatHPGen<q_extend_prop>(const q_extend_prop  & prop,bool sitdown)
	{
		return sitdown?prop.hp_gen[3]:prop.hp_gen[2];
	}

	template <>
	inline int GetHPGen<q_extend_prop>(const q_extend_prop  & prop, bool sitdown)
	{
		return sitdown?prop.hp_gen[1]:prop.hp_gen[0];
	}
	
	template <typename EXTEND_PROP>
	inline int GetMPGen(const EXTEND_PROP  & prop,bool sitdown)
	{
		return prop.mp_gen * 4;
	}

	template <typename EXTEND_PROP>
	inline int GetCombatMPGen(const EXTEND_PROP  & prop, bool sitdown)
	{
		return prop.mp_gen;
	}

	template <>
	inline int GetCombatMPGen<q_extend_prop>(const q_extend_prop  & prop,bool sitdown)
	{
		return sitdown?prop.mp_gen[3]:prop.mp_gen[2];
	}

	template <>
	inline int GetMPGen<q_extend_prop>(const q_extend_prop  & prop, bool sitdown)
	{
		return sitdown?prop.mp_gen[1]:prop.mp_gen[0];
	}
	
	template <typename EXTEND_PROP>
	inline int GetDPGen(const EXTEND_PROP  & prop,bool sitdown)
	{
		return prop.dp_gen * 4;
	}

	template <typename EXTEND_PROP>
	inline int GetCombatDPGen(const EXTEND_PROP  & prop, bool sitdown)
	{
		return prop.dp_gen;
	}

	template <>
	inline int GetCombatDPGen<q_extend_prop>(const q_extend_prop  & prop,bool sitdown)
	{
		return sitdown?prop.dp_gen[3]:prop.dp_gen[2];
	}

	template <>
	inline int GetDPGen<q_extend_prop>(const q_extend_prop  & prop, bool sitdown)
	{
		return sitdown?prop.dp_gen[1]:prop.dp_gen[0];
	}
	

	template <typename EXTEND_PROP, typename ENCHANCE_PRARM, typename ITEM_PROP, typename SCALE_ENHANCED_PARAM>
	inline int GeneratePhysicDamage(const EXTEND_PROP & base_prop, const ENCHANCE_PRARM & en_point, const ITEM_PROP & cur_item, const SCALE_ENHANCED_PARAM & en_percent, int scale_damage, int point_damage)
	{
		//取得基础攻击力
		int low = base_prop.damage_low  + en_point.damage_low  + cur_item.damage_low;
		int high= base_prop.damage_high + en_point.damage_high + cur_item.damage_high;
		low = abase::Rand(low,high);

		//计算比例加成
		float scale = 0.01f*(float)(100 + en_percent.damage + en_percent.base_damage + scale_damage);
		low = (int)(low * scale);

		//返回攻击
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	template <typename EXTEND_PROP,typename ENCHANCE_PRARM, typename ITEM_PROP, typename SCALE_ENHANCED_PARAM>
	inline int GenerateMagicDamage(const EXTEND_PROP & base_prop, const ENCHANCE_PRARM & en_point, const ITEM_PROP & cur_item, const SCALE_ENHANCED_PARAM & en_percent, int scale_damage, int point_damage)
	{
		//取得基础攻击力
		int low = base_prop.damage_magic_low  + en_point.magic_dmg_low  + cur_item.damage_magic_low;
		int high= base_prop.damage_magic_high + en_point.magic_dmg_high + cur_item.damage_magic_high;
		low = abase::Rand(low,high);
		
		//计算比例加成
		float scale = 0.01f*(float)(100 + en_percent.magic_dmg + en_percent.base_magic + scale_damage);
		low = (int)(low * scale);

		//返回攻击
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	template <>
	inline int GeneratePhysicDamage<q_extend_prop,q_enhanced_param,q_item_prop,q_scale_enhanced_param>
	(const q_extend_prop & base_prop, const q_enhanced_param & en_point, const q_item_prop & cur_item, const q_scale_enhanced_param & en_percent, int scale_damage, int point_damage)
	{
		//取得基础攻击力
		int low = base_prop.damage_low   + cur_item.damage_low;
		int high= base_prop.damage_high  + cur_item.damage_high;
		low = abase::Rand(low,high);

		//计算比例加成
		float scale = 0.01f*(float)(100 + en_percent.damage + scale_damage);
		low = (int)(low * scale);

		//返回攻击
		low = low + point_damage + en_point.damage;
		if(low < 0) low = 0;
		return low;
	}

	template <>
	inline int GenerateMagicDamage<q_extend_prop,q_enhanced_param,q_item_prop,q_scale_enhanced_param>
	(const q_extend_prop & base_prop, const q_enhanced_param & en_point, const q_item_prop & cur_item, const q_scale_enhanced_param & en_percent, int scale_damage, int point_damage)
	{
		return GeneratePhysicDamage(base_prop,en_point,cur_item,en_percent,scale_damage, point_damage);
	}

	template <typename EXTEND_PROP>
	inline int GeneratePhysicDamage(const EXTEND_PROP & prop)
	{
		return abase::Rand(prop.damage_low,prop.damage_high);
	}

	template <typename EXTEND_PROP>
	inline void GeneratePhysicDamage(const EXTEND_PROP & prop, int & low, int &high)
	{
		low = prop.damage_low;
		high = prop.damage_high;
	}

	template <typename EXTEND_PROP>
	inline int GenerateMagicDamage(const EXTEND_PROP & prop)
	{
		return abase::Rand(prop.damage_magic_low,prop.damage_magic_high);
	}

	template <>
	inline int GenerateMagicDamage<q_extend_prop>(const q_extend_prop & prop)
	{
		return GeneratePhysicDamage(prop);
	}


	template <typename EXTEND_PROP>
	inline float GetSpeedByMode(const EXTEND_PROP & prop, int mode)
	{
		float speed[]={prop.run_speed,prop.flight_speed,prop.swim_speed,prop.run_speed};
		int index = (mode & (C2S::MOVE_MASK_SKY|C2S::MOVE_MASK_WATER)) >> 6; //0 ground 1, sky,2 water, 3 other
		return speed[index];
	}

	template <>
	inline float GetSpeedByMode<q_extend_prop>(const q_extend_prop & prop, int mode)
	{
		return prop.run_speed;
	}


	template <typename EXTEND_PROP, typename ITEM_PROP>
	inline float GetAttackRange(const EXTEND_PROP & prop, const ITEM_PROP & cur_item)
	{
		return prop.attack_range;
	}

	template <>
	inline float GetAttackRange<q_extend_prop>(const q_extend_prop & prop, const q_item_prop & cur_item)
	{
		return prop.attack_range;
	}

	template <typename EXTEND_PROP>
	inline float GetAttackProbability(int attack_rate , const EXTEND_PROP & prop)
	{
		int rate =  (attack_rate + 180)/3 - (prop.armor - 55)/2;
		if(rate < 40) rate = 40;
		if(rate > 99) rate = 99;
		return rate * 0.01f;
	}

	template <typename EXTEND_PROP>
	inline int GetCritRate(const EXTEND_PROP & prop, int crit, int base_crit)
	{
		int tmp = crit + base_crit;
		if(tmp > 95) tmp = 95;
		return tmp;
	}

	template <>
	inline int GetCritRate<q_extend_prop>(const q_extend_prop & prop, int crit, int base_crit)
	{
		//crit base_crit 在player_template的计算里加入了，这里不再加入
		return prop.crit_rate;
	}

	template <typename EXTEND_PROP>
	inline float GetCritFactor(const EXTEND_PROP & prop)
	{
		return 1.0;
	}

	template <>
	inline float GetCritFactor<q_extend_prop>(const q_extend_prop & prop)
	{
		return prop.crit_damage;
	}
	
	template <typename EXTEND_PROP>
	inline int GetIgnDmgReduce(const EXTEND_PROP & prop)
	{
		return prop.ign_dmg_reduce;
	}
	
	template <typename EXTEND_PROP>
	inline int GetDmgChange(const EXTEND_PROP & prop)
	{
		return prop.dmg_change;
	}
	
	template <typename ENHANCE_PROP>
	inline int GetSpecDamage(const ENHANCE_PROP & prop)
	{
		return prop.spec_damage;
	}
	
	template <typename EXTEND_PROP>
	inline int GetDeityPower(const EXTEND_PROP & prop)
	{
		return prop.deity_power;
	}

	
}


#endif

