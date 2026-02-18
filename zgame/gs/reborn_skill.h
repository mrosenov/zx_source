#ifndef __ONLINE_GAME_GS_REBORN_SKILL_H__
#define __ONLINE_GAME_GS_REBORN_SKILL_H__

#include <stdlib.h>
#include <algorithm>
#include "property.h"

struct reborn_skill_bonus
{
	int64_t require[4];		//0 转 一转 二转 3转 的需求
	int64_t require1[4];	//0 转 一转 二转 3转 的需求1
	struct
	{
		short skill;
		short lvl;
	} bonus_skill[12];

public:
	reborn_skill_bonus()
	{
		memset(require, 0, sizeof(require));
		memset(require1, 0, sizeof(require1));
		memset(bonus_skill, 0, sizeof(bonus_skill));
	}

	inline static bool CheckCls(int64_t c, int64_t mask1, int64_t mask2)
	{
		int cc = c & 0x7F;
		return ((cc <= 0x3F) ? (((int64_t)1 << cc) & mask1) : (((int64_t)1 << (cc - 0x40)) & mask2)) 
			|| (cc + mask1 + mask2 == 0);
	}

	inline bool IsMatch(int64_t prof0, int64_t prof1, int64_t prof2, int64_t prof3) const
	{
		return CheckCls(prof0, require[0], require1[0])
			&& CheckCls(prof1, require[1], require1[1])
			&& CheckCls(prof2, require[2], require1[2])
			&& CheckCls(prof3, require[3], require1[3]);
	}
};

struct reborn_prop_bonus
{
	int64_t require[4];		//0 转 一转 二转 3转 的需求
	int64_t require1[4];	//0 转 一转 二转 3转 的需求1

	float                   hp[15];
	float                   mp[15];
	float                   dmg[15];
	float                   def[15];
	float                   attack[15];
	float                   armor[15];
	float                   crit_rate[15];
	float                   crit_damage[15];
	float                   anti[6][15];

public:
	reborn_prop_bonus()
	{
		memset(require, 0, sizeof(require));
		memset(require1, 0, sizeof(require1));
		memset(hp, 0, sizeof(hp));
		memset(mp, 0, sizeof(mp));
		memset(dmg, 0, sizeof(dmg));
		memset(def, 0, sizeof(def));
		memset(attack, 0, sizeof(attack));
		memset(armor, 0, sizeof(armor));
		memset(crit_rate, 0, sizeof(crit_rate));
		memset(crit_damage, 0, sizeof(crit_damage));
		memset(anti, 0, sizeof(anti));
	}

	inline static bool CheckCls(int64_t c, int64_t mask1, int64_t mask2)
	{
		int cc = c & 0x7F;
		return ((cc <= 0x3F) ? (((int64_t)1 << cc) & mask1) : (((int64_t)1 << (cc - 0x40)) & mask2))
			|| (cc + mask1 + mask2 == 0);
	}

	inline bool IsMatch(int64_t prof0, int64_t prof1, int64_t prof2, int64_t prof3) const
	{
		return CheckCls(prof0, require[0], require1[0])
			&& CheckCls(prof1, require[1], require1[1])
			&& CheckCls(prof2, require[2], require1[2])
			&& CheckCls(prof3, require[3], require1[3]);
	}

	
};

class itemdataman;
class reborn_bonus_man
{
	abase::vector<reborn_skill_bonus> _bonus_list;
	abase::vector<reborn_prop_bonus> _prop_list;

	const reborn_prop_bonus * GetPropBonus(int64_t prof0,int64_t prof1,int64_t prof2,int64_t prof3) const
	{
		for(size_t i = 0; i < _prop_list.size(); i ++)
		{
			const reborn_prop_bonus *  pBonus = &_prop_list[i];
			if(pBonus->IsMatch(prof0, prof1, prof2, prof3)) return pBonus;
		}
		return NULL;
	}

	static float CalcValue(const int level[4], const float arg_o[15], int n) 
	{
		const float * arg = &arg_o[-1];
		float f1[] = { 0, 1, arg[13], 	arg[14]};
		float f2[] = { 0, 0, 1, 	arg[15]};
		float f3[] = { 0, 0, 0, 	1};
		int lvl  = level[0];
		
		float part1 = (arg[1]*(lvl*lvl*lvl) + arg[2] *(lvl*lvl) + arg[3] * lvl + arg[4]);
		float part2 = 1.0f + (f1[n] *(level[1] - arg[7])*arg[8] + f2[n]*(level[2] - arg[9])*arg[10] + f3[n]*(level[3] - arg[11]) * arg[12])/100.f;

		return part1 *	std::min(arg[5], std::max(arg[6], part2));
	}
public:
	bool InitFromDataMan(itemdataman & dataman);

	const reborn_skill_bonus * GetBonus(int64_t prof0,int64_t prof1,int64_t prof2,int64_t prof3) const
	{
		for(size_t i = 0; i < _bonus_list.size(); i ++)
		{
			const reborn_skill_bonus *  pBonus = &_bonus_list[i];
			if(pBonus->IsMatch(prof0, prof1, prof2, prof3)) return pBonus;
		}
		return NULL;
	}

	void ActivatePropBonus(int reborn_count, const int r_level[4], const int r_prof[4], q_extend_prop & prop) const;
};


#endif

