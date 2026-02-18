#ifndef __ONLINEGAME_GS_ATTACK_H__
#define __ONLINEGAME_GS_ATTACK_H__


#include "config.h"
#include <common/types.h>


//考虑在这里面加入宠物的内容 ， 以让宠物攻击算成玩家的攻击
struct attacker_info_t
{
	XID attacker;			//攻击者的ID 对于宠物， 此ID与消息ID不一致
	short level;			//攻击者级别
	short eff_level;		//有效级别 为组队做准备的
	int team_id;			//攻击者组id
	int team_seq;			//攻击者组的序号
	int cs_index;			//如果是player，表示了player的cs_index
	int sid;			//如果是player，表示了player的cs_index 对应的sid 
	int mafia_id;			//组团序号，非0则为组团攻击，此攻击不会影响团内成员
	int family_id;			//家族ID,用于攻击判定
	int master_id;			//师傅ID 
	int wallow_level;
	int invisible_rate;		//隐身能力
	int zone_id;			//服务器id
};

//技能使用限制
enum
{
	SKILL_LIMIT_MONSTER 	= 0x00000001,	//对怪物无效
	SKILL_LIMIT_PET			= 0x00000002,	//对宠物无效
	SKILL_LIMIT_SUMMON		= 0x00000004,	//对召唤无效
	SKILL_LIMIT_PLAYER		= 0x00000008,	//对人物无效
	SKILL_LIMIT_SELF		= 0x00000010,	//对自身无效
	SKILL_LIMIT_BATTLEFIELD	= 0x00000020,	//仅战场下可用

	//Add by Houjun 2010-02-08
	SKILL_LIMIT_COMBAT		= 0x00000040,	//战斗时不能使用
	SKILL_LIMIT_NONCOMBAT	= 0x00000080, 	//战斗外不能使用
	SKILL_LIMIT_NOSUMMONPET = 0x00000100,	//无召唤兽不能使用
	//Add end.

	//Add by Houjun 2010-03-10
	SKILL_LIMIT_MOUNT 		= 0x00000200,	//骑乘时不能使用
	SKILL_LIMIT_FLY			= 0x00000400,	//飞行时不能使用
	//Add end.
	
	SKILL_LIMIT_NOSUMMON	= 0x00000800,	//无召唤兽不可用，其中召唤兽ID单独指定
	SKILL_LIMIT_BOSS		= 0x00001000,	//对BOSS无效
	SKILL_LIMIT_INVISIBLE_NPC	= 0x00002000,	//仅对隐身NPC生效
};
	
struct attack_msg
{
	attacker_info_t ainfo;		//攻击者的信息
	
	int damage_low;			//最低攻击力
	int damage_high;		//最大攻击力
	int damage_no_crit;		//无暴击伤害
	int spec_damage;		//额外附加的忽视攻击的damage
	float damage_factor;		//攻击系数 最后计算完毕的伤害要乘的值
	int attack_rate;		//物理攻击的命中
	int skill_attack_rate;		//技能攻击命中率
	int attacker_faction;		//攻击者阵营
	int target_faction;		//攻击者的敌人阵营(自己的阵营只有符合这个阵营才能被非强制攻击伤害)
	int crit_rate;			//致命攻击的概率
	float crit_factor;		//致命攻击如果发生，应该乘的因子
	char normal_attack;		//是否普通攻击 普通攻击才会进算命中
	char force_attack;		//是否强制攻击
	char attack_state;		//0x01 重击  0x02 攻击优化符
	char attacker_mode;		//攻击者的状态(0x01 PK , 0x02 FREE, 0x04 已进入PK模式)
	char is_invader;		//是否卑劣攻击,现在提前判定了
	unsigned char attack_stamp;	//攻击时戳
	char cultivation;		//仙魔佛
	char is_pet;
	char is_flying;			//攻击者是否在飞行	
	char is_mirror;			//是否是反弹的技能消息
	char combo_color[5];
	int pet_attack_adjust;		//宠物攻击增强

	int skill_id;			//是否技能攻击， 及其对应的技能号
	int skill_modify[8];	
	unsigned int skill_limit;	//技能使用限制
	int feedback_mask;
	
	struct
	{
		int skill;
		int level;
	} attached_skill;

	int fashion_weapon_addon_id;	// Youshuang add
	
	int skill_var[16];
	unsigned short skill_element[10];
	int ign_dmg_reduce;		//无视伤害减免的百分比
	int dmg_change;			//减少(增加)对人物伤害百分比
	int cult_attack[3];		//造化克仙 克佛 克魔
	int deity_power;		//神圣力
	int fixed_damage;		//一定生效的固定伤害, 无视任何防御和减免

	short talismanSkills[8];//如果是法宝的需要包含飞升法宝技能
	int resistance_proficiency[6];	//抗性精通

	enum 
	{
		PVP_ENABLE	= 0x01,
		PVP_FREE   	= 0x02,		//自由PK
		PVP_DURATION 	= 0x04,		//已经在和玩家PK
		PVP_DUEL	= 0x08,		//此次是决斗攻击
		PVP_MAFIA_DUEL	= 0x10,		//此次是帮派决斗
		PVP_SANCTUARY	= 0x20,		//安全区
		PVP_HOSTILE_DUEL = 0x40,	// 此次攻击是敌对帮派攻击
	};
};

struct damage_entry
{
	float damage;
	float adjust;
	bool  ignore_attack;
	float damage_no_crit;
	damage_entry():damage(0),adjust(1.0f),ignore_attack(0),damage_no_crit(0) {}
	damage_entry(float damage, float adjust,float damage_no_crit = 0):damage(damage),adjust(adjust),ignore_attack(0),damage_no_crit(damage_no_crit)
	{
	}
};

struct old_attack_msg
{
	attacker_info_t ainfo;		//攻击者的信息

	float attack_range;		//此次攻击的范围（攻击点在消息里面）
	float short_range;		//此次攻击的最小范围  超出这个攻击范围则攻击力减半 魔法攻击不算
	int physic_damage;		//物理攻击的伤害力
	int attack_rate;		//物理攻击的命中
	int magic_damage[MAGIC_CLASS];	//魔法伤害力
	int attacker_faction;		//攻击者阵营
	int target_faction;		//攻击者的敌人阵营(自己的阵营只有符合这个阵营才能被非强制攻击伤害)
	int crit_rate;
	char physic_attack;		//是否物理攻击
	char force_attack;		//是否强制攻击
	char attacker_layer;		//攻击者处于什么位置 0 地上 1 天上 2 水上 
	char attack_state;		//0x01 重击  0x02 攻击优化符
	short speed;
	char attacker_mode;		//攻击者的状态(0x01 PK , 0x02 FREE, 0x04 已进入PK模式)
	char is_invader;		//是否卑劣攻击,现在提前判定了
	unsigned char attack_stamp;	//攻击时戳
	int skill_id;			//是否技能攻击， 及其对应的技能号
	struct
	{
		int skill;
		int level;
	} attached_skill;

	enum 
	{
		PVP_ENABLE	= 0x01,
		PVP_FREE   	= 0x02,		//自由PK
		PVP_DURATION 	= 0x04,		//已经在和玩家PK
		PVP_DUEL	= 0x08,		//此次是决斗攻击
	};
};

struct enchant_msg
{
	attacker_info_t ainfo;		//攻击者的信息

	int attacker_faction;		//攻击者阵营
	int target_faction;		//攻击者的敌人阵营(自己的阵营只有符合这个阵营才能被非强制攻击伤害)
	float attack_range;
	int skill;
	int skill_reserved1;		//技能内部使用
	char force_attack;		//是否强制攻击
	short skill_level;
	char attacker_layer;
	char helpful;			//是否善意法术
	char attacker_mode;		//攻击者的状态(0 非PK,1:PK)
	char is_invader;		//是否卑劣攻击,现在提前判定了
	char attack_state;		//0x01 重击  0x02 攻击优化符  0x80 免疫此次公鸡
	unsigned char attack_stamp;	//攻击时戳
	char is_flying;			//攻击者是否在飞行	
	char is_mirror;			//是否是反弹的技能消息
	char combo_color[5];
	int skill_modify[8];	
	unsigned int skill_limit;	//技能使用限制
	int skill_var[16];
	int resistance_proficiency[6];	//抗性精通
	unsigned short skill_element[10];
	short talismanSkills[8];//如果是法宝的需要包含飞升法宝技能
};

struct old_damage_entry
{
	float physic_damage;
	float magic_damage[MAGIC_CLASS];
};

#endif

