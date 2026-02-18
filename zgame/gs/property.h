#ifndef __ONLINEGAME_GS_OBJ_PROPERTY_H__
#define __ONLINEGAME_GS_OBJ_PROPERTY_H__ 
#include "config.h"
#include <stdint.h>

//对象的基本属性(很少受外界影响的属性）
struct basic_prop
{
	short level;		//级别
	short sec_level;	//修真级别
	short dt_level;		//修真等级
	int64_t exp;		//经验值
	int64_t dt_exp;		//修真经验
	int hp;			//当前hp
	int mp;			//当前mp
	int dp;			//当前dp
	int ink;		//萝莉职业墨水(暂时废掉)
	int status_point;	//剩余的属性点
	int rage;		//怒气值
};


template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, basic_prop &rhs)
{
	return wrapper.push_back(&rhs, sizeof(rhs));
}


template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, basic_prop &rhs)
{
	return wrapper.pop_back(&rhs, sizeof(rhs));
}

enum { CULT_DEF_NUMBER = 6};
struct q_extend_prop
{
	int 	max_hp;
	int 	max_mp;
	int	max_dp;
	int 	hp_gen[4];
	int 	mp_gen[4];
	int 	dp_gen[4];

	float	walk_speed;
	float 	run_speed;

	int 	crit_rate;		//暴击率, 千分数
	float 	crit_damage;		//暴击伤害

	int 	damage_low;		//最低攻击
	int	damage_high;		//最高攻击

	int 	attack;			//普攻命中
	int 	defense;		//防御
	int	armor;			//普攻闪躲

	int resistance[6];		//6个特殊魔法抗性 (眩晕 虚弱 定身 魅惑 昏睡 减速)
	int resistance_tenaciy[6];	//抗性韧性
	int resistance_proficiency[6];	//抗性精通
	float attack_range;		//攻击距离

	int     anti_crit;              //暴击抵抗 千分数
	float   anti_crit_damage;       //暴击伤害抵抗 小数
	int     skill_attack_rate;      //技能命中概率修正  千分数
	int     skill_armor_rate;       //技能命中闪躲概率修正 千分数

	int cult_defense[CULT_DEF_NUMBER];	//造化防御 仙 魔 佛 备选1 2 3
	int cult_attack[CULT_DEF_NUMBER];	//造化阵营相克 仙 魔 佛 备选1 2 3

	int	anti_transform;		//反变身
	int 	invisible_rate;		//隐身能力
	int	anti_invisible_rate;	//反隐身能力

	int	deity_power;		//神圣力
	int	anti_diet;		//禁食抗性
};

template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, const q_extend_prop &rhs)
{
	wrapper.push_back(&rhs,sizeof(rhs));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, q_extend_prop &rhs)
{
	wrapper.pop_back(&rhs,sizeof(rhs));
	return wrapper;
}

//装备产生的本体增强
struct q_item_prop
{
	int weapon_class;		//武器的id
	int damage_low;			//武器最低攻击力
	int damage_high;		//武器最高攻击力
	int deity_power;		//神圣力
	float attack_range;		//武器攻击范围
	int max_hp;
	int max_mp;
	int max_dp;
	int armor;
	int defense;
	int attack;
	int attack_cycle;		//攻击循环的时间
	int attack_point;		//攻击点的时间
};

struct q_enhanced_param
{
	int 	max_hp;
	int 	max_mp;
	int	max_dp;
	int 	defense;
	int 	attack;
	int	armor;
	int	damage;
	int 	resistance[6];	//6个特殊魔法抗性
	int 	resistance_tenaciy[6];	//抗性韧性
	int 	resistance_proficiency[6];	//抗性精通
	int 	crit_rate;
	float 	crit_damage;
	float 	run_speed;
	int 	dmg_reduce;
	int 	spec_damage;
	float 	override_speed;		//覆盖速度，只对陆地有效
	float 	mount_speed;		//骑乘速度修正值
	float 	flight_speed;		//飞行速度修正
	float	over_speed_square;	//超速调整值的平方

	int     anti_crit;              //暴击抵抗 千分数
	float   anti_crit_damage;       //暴击伤害抵抗 小数
	int     skill_attack_rate;      //技能命中概率修正
	int     skill_armor_rate;       //技能命中闪躲概率修正

	int cult_defense[CULT_DEF_NUMBER];	//造化防御 仙 魔 佛 备选1 2 3
	int cult_attack[CULT_DEF_NUMBER];	//造化防御 仙 魔 佛 备选1 2 3
	int	pet_attack_adjust;
	int	pet_attack_defense;
	
	int	anti_transform;		//反变身
	float 	attack_range;		//攻击距离
	int	invisible_rate;		//隐身能力
	int	anti_invisible_rate;	//反隐身能力
	int	deity_power;		//神圣力
	int	anti_diet;		//禁食抗性

	inline const q_enhanced_param & operator +=(const q_enhanced_param & en); 
	inline const q_enhanced_param & operator -=(const q_enhanced_param & en); 
};

struct q_scale_enhanced_param
{
	int 	max_hp;
	int 	max_mp;
	int	max_dp;
	int 	hp_gen;
	int 	mp_gen;
	int	dp_gen;
	int 	defense;
	int 	attack;
	int	armor;
	int	damage;
	int	dmg_reduce;
	int	ign_dmg_reduce;
	int	dmg_change;	//减少(增加)对人物伤害的百分比
	int	speed;

	int 	resistance[6];	//6个特殊魔法抗性
	int 	resistance_tenaciy[6];	//抗性韧性
	int 	resistance_proficiency[6];	//抗性精通
	int	anti_transform;		//反变身
	
	int	deity_power;		//神圣力
	
	inline const q_scale_enhanced_param & operator +=(const q_scale_enhanced_param & en); 
	inline const q_scale_enhanced_param & operator -=(const q_scale_enhanced_param & en); 
};




//扩展属性，随时会收到外界影响的属性
struct extend_prop
{
	int vitality;		//命
	int energy;		//神
	int strength;		//力
	int agility;		//敏
	int max_hp;		//最大hp
	int max_mp;		//最大mp
	int hp_gen;		//hp恢复速度
	int mp_gen;		//mp恢复速度 

	float walk_speed;	//行走速度 单位  m/s
	float run_speed;	//奔跑速度 单位  m/s
	float swim_speed;	//游泳速度 单位  m/s
	float flight_speed;	//飞行速度 单位  m/s

	int attack;		//攻击率 attack rate
	int damage_low;		//最低damage
	int damage_high;	//最大物理damage
	int attack_speed;	//攻击时间间隔 以tick为单位
	float attack_range;	//攻击范围
	struct 			//物理攻击附加魔法伤害
	{
		int damage_low;
		int damage_high;
	} addon_damage[MAGIC_CLASS];		
	int damage_magic_low;		//魔法最低攻击力
	int damage_magic_high;		//魔法最高攻击力

	int resistance[MAGIC_CLASS];	//魔法抗性
	int defense;		//防御力
	int armor;		//闪躲率（装甲等级）
	int max_ap;		//最大怒气值

	void BuildFrom(const q_extend_prop &  rhs)
	{
		max_hp 		= rhs.max_hp;
		max_mp 		= rhs.max_hp;
		hp_gen		= rhs.hp_gen[0];
		mp_gen		= rhs.mp_gen[0];
		

		walk_speed 	= rhs.walk_speed;
		run_speed 	= rhs.run_speed;
		swim_speed 	= rhs.run_speed;
		flight_speed 	= rhs.run_speed;

		attack		= rhs.attack;
		damage_low 	= rhs.damage_low;
		damage_high 	= rhs.damage_high;
		attack_speed	= 10;
		attack_range	= 2.5f;
		damage_magic_low= 0;
		damage_magic_high =0;
		defense 	= rhs.defense;
		armor 		= rhs.armor;
	}
};

	template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, const extend_prop &rhs)
{
	wrapper.push_back(&rhs,sizeof(rhs));
	return wrapper;
}

	template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, extend_prop &rhs)
{
	wrapper.pop_back(&rhs,sizeof(rhs));
	return wrapper;
}


//装备产生的本体增强
struct item_prop
{
	short weapon_type;		//武器攻击类别（melee,range,missile）
	short weapon_delay;		//武器攻击点的延迟时间 单位就是一个tick
	int attack_delay;		//计算后的延迟时间，这个数据不是直接从武器上来的
	int weapon_class;		//武器的攻击子类
	int weapon_level;		//武器的级别
	int damage_low;			//武器最低攻击力
	int damage_high;		//武器最高攻击力
	int damage_magic_low;		//魔法武器最低攻击力
	int damage_magic_high;		//魔法武器最高攻击力
	int attack_speed;		//武器攻击速度修正
	float attack_range;		//武器攻击范围
	float short_range;		//武器的最小攻击距离（低于这个距离将受到攻击惩罚）
	/*
	   int attack;			//武器产生的攻击率修正
	 */
	//	int defense;			//装备产生的防御
	//	int armor;			//装备产生闪躲
};

//各种操作产生的非本体增强，可以为负值
struct enhanced_param
{
	int max_mp;			//最大mana加值
	int max_hp;			//最大生命加值
	int hp_gen;
	int mp_gen;
	int vit;
	int eng;
	int agi;
	int str;

	//int weapon_damage;		//武器攻击加值	参与比例加成
	int damage_low;
	int damage_high;		//物理攻击加值
	int attack;			//攻击率
	int attack_speed;
	float attack_range;
	float walk_speed;
	float run_speed;
	float flight_speed;		//飞行
	
	int addon_damage[MAGIC_CLASS];	//物理攻击附加魔法伤害
	int resistance[MAGIC_CLASS];	//魔法抗性
	int magic_dmg_low;			//魔法攻击加值
	int magic_dmg_high;			//魔法攻击加值
	//	int magic_weapon_dmg;		//魔法武器攻击加值 参与比例加成

	int defense;			//防御加值(装备)
	int armor;			//闪躲加值(装备)

	float override_speed;		//覆盖速度，只对陆地有效

};

//各种操作产生的非本体增强(都是百分比)，可以为负值
struct scale_enhanced_param
{
	int max_mp;			//最大mana加值
	int max_hp;			//最大生命加值

	int hp_gen;			//hp恢复速度增加比率
	int mp_gen;			//mp恢复速度增加比率

	int walk_speed;			//走路
	int run_speed;			//跑步
	int swim_speed;			//游泳	
	int flight_speed;		//飞行

	int attack_speed;		//攻击速度

	int damage;			//物理攻击加值
	int attack;			//攻击率

	int base_damage;		//由本体数据产生的百分比加成
	int base_magic;			//由本体数据产生的魔法攻击百分比加成

	int magic_dmg;			//魔法攻击加值
	int resistance[MAGIC_CLASS];	//魔法抗性

	int defense;			//防御加值(装备)
	int armor;			//闪躲加值(装备)
	
};

#pragma pack(1)
struct buff_t                           //buff 操作
{       
	short buff_id;
	short buff_level;
	int buff_endtime;
	char  overlay_cnt;
	buff_t(short id, short level, int endtime, char cnt):buff_id(id),buff_level(level),buff_endtime(endtime),overlay_cnt(cnt)
	{}
};      
struct buff_t_s                         //buff simple版
{
	short buff_id;
	short buff_level;
	char  overlay_cnt;
	buff_t_s(short id, short level, char cnt):buff_id(id),buff_level(level), overlay_cnt(cnt)
	{}
};      
#pragma pack()

class world_manager;
struct team_mutable_prop
{
	short level;
	short dt_level;
	char  combat_state;
	unsigned char cls_type;
	int hp;
	int mp;
	int dp;
	int max_hp;
	int max_mp;
	int max_dp;
	int world_tag;
	int wallow_level;
	int pk_level;
	int family_id;
	int sect_master_id;
	int collision_score;
	bool is_zombie;
	char reborn_cnt;
	int rage;
	team_mutable_prop(){}
	template <typename ACTIVE_IMP>
		explicit team_mutable_prop(ACTIVE_IMP * pImp)
		{
			Init(pImp);
		}

/*	team_mutable_prop(const basic_prop & bp, const extend_prop &ep,char cs, int tag)
		:level(bp.level),combat_state(cs),sec_level(bp.sec_level),hp(bp.hp),mp(bp.mp),max_hp(ep.max_hp),max_mp(ep.max_mp)
		{}*/

	template <typename ACTIVE_IMP>
		void Init(ACTIVE_IMP * pImp)
		{
			const basic_prop & bp = pImp->_basic;
			const q_extend_prop &ep = pImp->_cur_prop;
			level = bp.level;
			dt_level = bp.dt_level;
			combat_state = pImp->IsCombatState()?1:0;
			cls_type = pImp->GetObjectClass() & 0xFF;
			hp = bp.hp;
			mp = bp.mp;
			dp = bp.dp;
			max_hp = ep.max_hp;
			max_mp = ep.max_mp;
			max_dp = ep.max_dp;
			world_tag = pImp->GetWorldTag();
			wallow_level = pImp->GetWallowLevel();
			family_id = pImp->OI_GetFamilyID();
			pk_level = pImp->GetPKLevel();
			sect_master_id = pImp->GetSectID();
			collision_score = pImp->GetCollisionScore();
			is_zombie = pImp->GetParent()->b_zombie;
			reborn_cnt = pImp->GetRebornCount(); 
			rage = bp.rage;
		}

	bool operator == (const team_mutable_prop & rhs)
	{
		return memcmp(this,&rhs,sizeof(rhs)) == 0;
	}

	bool operator != (const team_mutable_prop & rhs)
	{
		return memcmp(this,&rhs,sizeof(rhs));
	}
};

template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, const team_mutable_prop & rhs)
{
	wrapper.push_back(&rhs,sizeof(rhs));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, team_mutable_prop &rhs)
{
	wrapper.pop_back(&rhs,sizeof(rhs));
	return wrapper;
}

/*
struct  team_member_data
{
	int id;
	team_mutable_prop data;
	team_member_data()
	{}

	team_member_data(int member,const team_mutable_prop &prop):id(member),data(prop)
	{}
};
*/

struct leader_prop
{
	bool pvp_flag_enable;
	bool free_pvp_mode;	
	bool is_flying;
	char pvp_mask;
	char invader_counter;	//红粉名状态
	int pvp_combat_timer;
	int mafia_id;
	int family_id;
	int team_count;
	int level;
	int team_efflevel;
	int wallow_level;
	XID teamlist[TEAM_MEMBER_CAPACITY];
	int team_id;
	int team_seq;
	int cs_index;		//leader 的 cs_index
	int cs_sid;		//leader 的 cs_sid
	int duel_target;	//决斗的目标对象
	int pk_level;
	int mduel_mafia_id;
	bool sanctuary_mode;
	int safe_lock;
	int pet_attack_adjust;		//宠物伤害增加百分比 ，只用在InitFromMaster时取得主人数值，这样只有在招出时有效
	int cultivation;
	int master_id;
	int zone_id;
};

const q_enhanced_param & q_enhanced_param::operator +=(const q_enhanced_param & en) 
{
	max_hp += en.max_hp;
	max_mp += en.max_mp;
	max_dp += en.max_dp;
	defense += en.defense;
	attack += en.attack;
	armor += en.armor;
	damage += en.damage;
	for(size_t i = 0; i < 6; ++i)
	{
		resistance[i] += en.resistance[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_tenaciy[i] += en.resistance_tenaciy[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_proficiency[i] += en.resistance_proficiency[i];
	}
	crit_rate += en.crit_rate;
	crit_damage += en.crit_damage;
	run_speed += en.run_speed;
	dmg_reduce += en.dmg_reduce;
	spec_damage += en.spec_damage;
	override_speed += en.override_speed;
	mount_speed += en.mount_speed;
	flight_speed += en.flight_speed;
	over_speed_square += en.over_speed_square;
	anti_crit += en.anti_crit;
	anti_crit_damage += en.anti_crit_damage;
	skill_attack_rate += en.skill_attack_rate;
	skill_armor_rate += en.skill_armor_rate;
	for(size_t j = 0; j < CULT_DEF_NUMBER; ++j)
	{
		cult_defense[j] += en.cult_defense[j];
		cult_attack[j] += en.cult_attack[j];
	}
	pet_attack_adjust += en.pet_attack_adjust;
	pet_attack_defense += en.pet_attack_defense;
	anti_transform += en.anti_transform;
	attack_range += en.attack_range;
	invisible_rate += en.invisible_rate;
	anti_invisible_rate += en.anti_invisible_rate;
	deity_power += en.deity_power;
	anti_diet += en.anti_diet;
	return *this;
}
	
const q_enhanced_param & q_enhanced_param::operator -=(const q_enhanced_param & en) 
{
	max_hp -= en.max_hp;
	max_mp -= en.max_mp;
	max_dp -= en.max_dp;
	defense -= en.defense;
	attack -= en.attack;
	armor -= en.armor;
	damage -= en.damage;
	for(size_t i = 0; i < 6; ++i)
	{
		resistance[i] -= en.resistance[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_tenaciy[i] -= en.resistance_tenaciy[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_proficiency[i] -= en.resistance_proficiency[i];
	}
	crit_rate -= en.crit_rate;
	crit_damage -= en.crit_damage;
	run_speed -= en.run_speed;
	dmg_reduce -= en.dmg_reduce;
	spec_damage -= en.spec_damage;
	override_speed -= en.override_speed;
	mount_speed -= en.mount_speed;
	flight_speed -= en.flight_speed;
	over_speed_square -= en.over_speed_square;
	anti_crit -= en.anti_crit;
	anti_crit_damage -= en.anti_crit_damage;
	skill_attack_rate -= en.skill_attack_rate;
	skill_armor_rate -= en.skill_armor_rate;
	for(size_t j = 0; j < CULT_DEF_NUMBER; ++j)
	{
		cult_defense[j] -= en.cult_defense[j];
		cult_attack[j] -= en.cult_attack[j];
	}
	pet_attack_adjust -= en.pet_attack_adjust;
	pet_attack_defense -= en.pet_attack_defense;
	anti_transform -= en.anti_transform;
	attack_range -= en.attack_range;
	invisible_rate -= en.invisible_rate;
	anti_invisible_rate -= en.anti_invisible_rate;
	deity_power -= en.deity_power;
	anti_diet -= en.anti_diet;
	return *this;
}

const q_scale_enhanced_param & q_scale_enhanced_param::operator +=(const q_scale_enhanced_param & en)
{
	max_hp += en.max_hp;
	max_mp += en.max_mp;
	max_dp += en.max_dp;
	hp_gen += en.hp_gen;
	mp_gen += en.mp_gen;
	dp_gen += en.dp_gen;
	defense += en.defense;
	attack += en.attack;
	armor += en.armor;
	damage += en.damage;
	dmg_reduce += en.dmg_reduce;
	ign_dmg_reduce += en.ign_dmg_reduce;
	dmg_change += en.dmg_change;
	speed += en.speed;
	for(size_t i = 0; i < 6; ++i)
	{
		resistance[i] += en.resistance[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_tenaciy[i] += en.resistance_tenaciy[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_proficiency[i] += en.resistance_proficiency[i];
	}
	anti_transform += en.anti_transform;
	deity_power += en.deity_power;
	return *this;

}

const q_scale_enhanced_param & q_scale_enhanced_param::operator -=(const q_scale_enhanced_param & en)
{
	max_hp -= en.max_hp;
	max_mp -= en.max_mp;
	max_dp -= en.max_dp;
	hp_gen -= en.hp_gen;
	mp_gen -= en.mp_gen;
	dp_gen -= en.dp_gen;
	defense -= en.defense;
	attack -= en.attack;
	armor -= en.armor;
	damage -= en.damage;
	dmg_reduce -= en.dmg_reduce;
	ign_dmg_reduce -= en.ign_dmg_reduce;
	dmg_change -= en.dmg_change;
	speed -= en.speed;

	for(size_t i = 0; i < 6; ++i)
	{
		resistance[i] -= en.resistance[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_tenaciy[i] -= en.resistance_tenaciy[i];
	}
	for(size_t i = 0; i < 6; ++i)
	{
		resistance_proficiency[i] -= en.resistance_proficiency[i];
	}
	anti_transform -= en.anti_transform;
	deity_power -= en.deity_power;
	return *this;
}

#endif
