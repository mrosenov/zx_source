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
	int wallow_level;
};

struct attack_msg
{
	attacker_info_t ainfo;		//攻击者的信息
	
	int damage_low;			//最低攻击力
	int damage_high;		//最大攻击力
	int spec_damage;		//额外附加的忽视攻击的damage
	float damage_factor;		//攻击系数 最后计算完毕的伤害要乘的值
	int attack_rate;		//物理攻击的命中
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

	int skill_id;			//是否技能攻击， 及其对应的技能号
	char skill_modify[4];	
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
		PVP_MAFIA_DUEL	= 0x10,		//此次是帮派决斗
		PVP_SANCTUARY	= 0x20,		//安全区
		PVP_HOSTILE_DUEL = 0x40,	// 此次攻击是敌对帮派攻击
	};
};

struct damage_entry
{
	float damage;
	float adjust;
	damage_entry():damage(0),adjust(1.0f){}
	damage_entry(float damage, float adjust):damage(damage),adjust(adjust)
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
	char skill_level;
	char attacker_layer;
	char helpful;			//是否善意法术
	char attacker_mode;		//攻击者的状态(0 非PK,1:PK)
	char is_invader;		//是否卑劣攻击,现在提前判定了
	char attack_state;		//0x01 重击  0x02 攻击优化符  0x80 免疫此次公鸡
	unsigned char attack_stamp;	//攻击时戳
	char skill_modify[4];	
};

struct old_damage_entry
{
	float physic_damage;
	float magic_damage[MAGIC_CLASS];
};

#endif

