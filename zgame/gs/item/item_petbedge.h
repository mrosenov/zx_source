#ifndef __ONLINEGAME_GS_PETBEDGE_ITEM_H__
#define __ONLINEGAME_GS_PETBEDGE_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../petman.h"
#include <crc.h>

#pragma pack(1)
struct pet_bedge_enhanced_essence
{
	enum
	{
		//宠物牌获得方式
		PET_BEDGE_GAIN_TYPE_NULL = 1,
		PET_BEDGE_GAIN_TYPE_QUEST,
		PET_BEDGE_GAIN_TYPE_DROP,
		PET_BEDGE_GAIN_TYPE_NPCSHOP,
		PET_BEDGE_GAIN_TYPE_GSHOP,
		PET_BEDGE_GAIN_TYPE_LOTTERY,
		PET_BEDGE_GAIN_TYPE_REFINE,
	};
	enum
	{
		//这些定好了就不能改了,存数据库的
		MAX_PET_SKILL_COUNT = 16,
		MAX_PET_TANLENT_COUNT = 16,
		MAX_OWNER_NAME_LENGTH = 20,
		MAX_PET_NAME_LENGTH = 20,
		MAX_REBORN_PROP = 14,
	};

	enum
	{
		PET_MAGIC_NUMBER = 0xFFFFFFFF,
		PET_VERSION_NUMBER = 1,
	};
	int magic_number;
	char version_number;
	int owner_id;			//主人id,驯养标志
	char owner_name[MAX_OWNER_NAME_LENGTH];
	char name[MAX_PET_NAME_LENGTH];
	unsigned char origin;		//来源
//后面是脚本调整的 上面是脚本不能调整的
	unsigned char level;		//级别
	int exp;			//经验
	int cur_hp;			//当前血量
	int cur_vigor;			//当前精力
	unsigned char star;		//星等
	unsigned char main_type;	//宠物主属性
	unsigned char element;		//宠物五行
	unsigned char identify;		//鉴定标志
	unsigned char race;		//种族
	unsigned char horoscope;	//星象
	enum
	{
		MAX_HP,		//生命
		MAX_VIGOR,	//精力
		MIN_ATTACK,	//最小攻击
		MAX_ATTACK,	//最大攻击
		DEFENCE,	//防御
		HIT,		//命中
		JOUK,		//闪避
		RESISTANCE1,	//抗性
		RESISTANCE2,
		RESISTANCE3,
		RESISTANCE4,
		RESISTANCE5,
		RESISTANCE6,
		MAX_COMBAT_ATTR,
	};
	struct combat_attr_t
	{
		unsigned int init;		//初始值
		unsigned int potential;	//潜力值
		unsigned int cur_value;	//当前值
	};
	combat_attr_t attr[MAX_COMBAT_ATTR];	//属性数组

	unsigned char extra_damage;		//附加伤害
	unsigned char extra_damage_reduce;	//附加伤害减免
	float crit_rate;		//暴击率
	float crit_damage;		//暴击伤害
	float anti_crit_rate;		//减免暴击率
	float anti_crit_damage;		//减免暴击伤害
	float skill_attack_rate;	//技能命中率
	float skill_armor_rate;		//技能躲闪概率
	int damage_reduce;		//伤害减免
	float damage_reduce_percent;	//伤害减免百分比

	unsigned short max_hunger_point;//最大饱食度
	unsigned short cur_hunger_point;//当前饱食度
	unsigned short max_honor_point;	//最大亲密度
	unsigned short cur_honor_point;	//当前亲密度
	unsigned short age;		//年龄
	unsigned short life;		//寿命
	struct skill_t
	{
		unsigned short id;
		unsigned char level;
	};
	skill_t skills[MAX_PET_SKILL_COUNT];	//技能
	unsigned char talents[MAX_PET_TANLENT_COUNT];	//天赋
	enum
	{
		ACUITY, //敏锐
		STAMINA, //耐力
		SAWY, //悟性
		DOWRY, //天资
		WISDOM, //学识
		CNR, //果敢 courageous and resolute
		MAX_MAKE_ATTR,
	};
	unsigned char make_attr[MAX_MAKE_ATTR];
	unsigned char face;
	unsigned char shape;
	enum
	{
		PET_MAIN_STATUS_FIGHT,
		PET_MAIN_STATUS_COLLECT,
		PET_MAIN_STATUS_MAKE,
		PET_MAIN_STATUS_REST,
	};
	unsigned char main_status;	//主状态
	enum
	{
		PET_SUB_STATUS_PLANTING,
		PET_SUB_STATUS_CUTTING,
		PET_SUB_STATUS_HUNTING,
		PET_SUB_STATUS_FISHING,
		PET_SUB_STATUS_MINING,
		PET_SUB_STATUS_ARCHAEOLOGY,
	};

	enum
	{
		PET_SUB_STATUS_FIGHT_TONGLING,
		PET_SUB_STATUS_FIGHT_YUBAO,

	};
	unsigned char sub_status;	//副状态
	enum
	{
		PET_RANK_BASE,
		PET_RANK_COMBAT1,
		PET_RANK_COMBAT2,
		PET_RANK_COMBAT3,
		PET_RANK_COMBAT4,
		PET_RANK_COMBAT5,
		PET_RANK_COMBAT6,
		PET_RANK_WORKSHOP1,
		PET_RANK_WORKSHOP2,
		PET_RANK_WORKSHOP3,
		PET_RANK_WORKSHOP4,
		PET_RANK_WORKSHOP5,
		PET_RANK_WORKSHOP6,
		MAX_PET_RANK,
	};
	unsigned char rank;
	int last_die_timestamp;		//上次死亡时间,用于死亡等待
	int last_feed_timestamp;	//上次喂养时间,用于喂养冷却
	int reborn_cnt;
	int reborn_star;
	float reborn_prop_add[MAX_REBORN_PROP];
	int reserved[8];
};

//老的宠物存储结构, 新的存储结构统一使用上面的结构
struct old_pet_bedge_enhanced_essence
{
	int owner_id;			//主人id,驯养标志
	char owner_name[20];
	char name[20];
	unsigned char origin;		//来源
//后面是脚本调整的 上面是脚本不能调整的
	unsigned char level;		//级别
	int exp;			//经验
	int cur_hp;			//当前血量
	int cur_vigor;			//当前精力
	unsigned char star;		//星等
	unsigned char main_type;	//宠物主属性
	unsigned char element;		//宠物五行
	unsigned char identify;		//鉴定标志
	unsigned char race;		//种族
	unsigned char horoscope;	//星象
	struct combat_attr_t
	{
		unsigned short init;		//初始值
		unsigned short potential;	//潜力值
		unsigned short cur_value;	//当前值
	};
	combat_attr_t attr[13];	//属性数组
	unsigned char extra_damage;		//附加伤害
	unsigned char extra_damage_reduce;	//附加伤害减免
	float crit_rate;		//暴击率
	float crit_damage;		//暴击伤害
	unsigned short max_hunger_point;//最大饱食度
	unsigned short cur_hunger_point;//当前饱食度
	unsigned short max_honor_point;	//最大亲密度
	unsigned short cur_honor_point;	//当前亲密度
	unsigned short age;		//年龄
	unsigned short life;		//寿命
	struct skill_t
	{
		unsigned short id;
		unsigned char level;
	};
	skill_t skills[12];	//技能
	unsigned char talents[13];	//天赋
	unsigned char make_attr[6];
	unsigned char face;
	unsigned char shape;
	unsigned char main_status;	//主状态
	unsigned char sub_status;	//副状态
	unsigned char rank;
	int last_die_timestamp;		//上次死亡时间,用于死亡等待
	int last_feed_timestamp;	//上次喂养时间,用于喂养冷却
	int reserved;
};
#pragma pack()

class pet_bedge_essence
{
	const void* _content;
	size_t _size;
	abase::vector<float,abase::fast_alloc<> > _inner_data;
	pet_bedge_enhanced_essence _enhanced_essence;
	bool _dirty_flag;
	bool _content_valid;

	bool TestOldContentValid()
	{
		size_t size = _size;
		if(size < sizeof(old_pet_bedge_enhanced_essence) + sizeof(int)) return false;
		size_t * pData = (size_t*)((char *)_content + sizeof(old_pet_bedge_enhanced_essence));
		size -= sizeof(old_pet_bedge_enhanced_essence);
		size_t tmpSize = pData[0];
		if(tmpSize > 0x7FFFFFFF) return false;
		if(size < sizeof(size_t) + tmpSize) return false;
		return true;
	}

	bool TestContentValid()
	{
		size_t size = _size;
		if(size < sizeof(pet_bedge_enhanced_essence) + sizeof(int)) return false;
		size_t magic = *(int*)_content;
		if(magic != pet_bedge_enhanced_essence::PET_MAGIC_NUMBER) return false;
		char version = *(char*)((char*)_content + sizeof(int));
		if(version != pet_bedge_enhanced_essence::PET_VERSION_NUMBER) return false; 
		
		size_t * pData = (size_t*)((char *)_content + sizeof(pet_bedge_enhanced_essence));
		size -= sizeof(pet_bedge_enhanced_essence);
		size_t tmpSize = pData[0];
		if(tmpSize > 0x7FFFFFFF) return false;
		if(size < sizeof(size_t) + tmpSize) return false;
		return true;
	}

	void DuplicateOldInnerData(old_pet_bedge_enhanced_essence & old_enhanced_essence)
	{
		if(!_content_valid) return;
		int * pData = (int*)((char *)_content + sizeof(old_pet_bedge_enhanced_essence));
		size_t count =  pData[0]/sizeof(float);
		float * list = (float*)&pData[1];
		_inner_data.reserve(count);
		for(size_t i = 0; i < count; i ++)
		{
			_inner_data.push_back(list[i]);
		}
		memcpy(&old_enhanced_essence,_content,sizeof(old_pet_bedge_enhanced_essence));
	}

	void DuplicateInnerData()
	{
		if(!_content_valid) return;
		int * pData = (int*)((char *)_content + sizeof(pet_bedge_enhanced_essence));
		size_t count =  pData[0]/sizeof(float);
		float * list = (float*)&pData[1];
		_inner_data.reserve(count);
		for(size_t i = 0; i < count; i ++)
		{
			_inner_data.push_back(list[i]);
		}
		memcpy(&_enhanced_essence,_content,sizeof(pet_bedge_enhanced_essence));
	}

public:
	pet_bedge_essence()
	{
	}

	pet_bedge_essence(const void* buf,size_t size):_content(buf),_size(size),_dirty_flag(false)
	{
		_content_valid = TestContentValid();
	}

	void UpdateContent(item* parent,bool force = false);
	//给程序用的获取函数
	inline pet_bedge_enhanced_essence& InnerEssence()
	{
		return _enhanced_essence;
	}

	pet_bedge_enhanced_essence* QueryEssence() const
	{
		if(!_content_valid) return NULL;
		return (pet_bedge_enhanced_essence*)_content;
	}

        inline bool IsValid() { return _content_valid; }  

	inline bool IsDirty() { return _dirty_flag; }

public:
	inline bool ConvertData(void * buf, size_t len)
	{
		_content = buf;
		_size = len;

		_content_valid = TestOldContentValid();
		if(!_content_valid) return false;

		old_pet_bedge_enhanced_essence old_enhanced_essence;
		memset(&old_enhanced_essence,0,sizeof(old_enhanced_essence));

		_inner_data.clear();
		DuplicateOldInnerData(old_enhanced_essence);
		ConvertEssenceData(old_enhanced_essence);
		_dirty_flag = true;
		return true;
	}

	inline void ConvertEssenceData(old_pet_bedge_enhanced_essence & old_enhanced_essence)
	{
		memset(&_enhanced_essence,0,sizeof(_enhanced_essence));
		_enhanced_essence.magic_number = pet_bedge_enhanced_essence::PET_MAGIC_NUMBER;
		_enhanced_essence.version_number = pet_bedge_enhanced_essence::PET_VERSION_NUMBER; 

		pet_bedge_enhanced_essence & e1 = _enhanced_essence;
		old_pet_bedge_enhanced_essence & e2 = old_enhanced_essence;

		e1.owner_id = e2.owner_id;
		memcpy(e1.owner_name, e2.owner_name, sizeof(e2.owner_name));
		memcpy(e1.name, e2.name, sizeof(e2.name));
		e1.origin = e2.origin;
		e1.level = e2.level;
		e1.exp = e2.exp;
		e1.cur_hp = e2.cur_hp;
		e1.cur_vigor = e2.cur_vigor;
		e1.star = e2.star;
		e1.main_type = e2.main_type;
		e1.element = e2.element;
		e1.identify = e2.identify;
		e1.race = e2.race;
		e1.horoscope = e2.horoscope;
		
		for(size_t i = 0; i < pet_bedge_enhanced_essence::MAX_COMBAT_ATTR; ++i)
		{
			e1.attr[i].init = e2.attr[i].init;
			e1.attr[i].potential = e2.attr[i].potential;
			e1.attr[i].cur_value = e2.attr[i].cur_value;
		}

		e1.extra_damage = e2.extra_damage;
		e1.extra_damage_reduce = e2.extra_damage_reduce;
		e1.crit_rate = e2.crit_rate;
		e1.crit_damage = e2.crit_damage;

		e1.max_hunger_point = e2.max_hunger_point;
		e1.cur_hunger_point = e2.cur_hunger_point;
		e1.max_honor_point = e2.max_honor_point;
		e1.cur_honor_point = e2.cur_honor_point;
		e1.age = e2.age;
		e1.life = e2.life;

		memcpy(e1.skills, e2.skills, sizeof(e2.skills));
		memcpy(e1.talents, e2.talents, sizeof(e2.talents));
		memcpy(e1.make_attr, e2.make_attr, sizeof(e2.make_attr));

		e1.face = e2.face;
		e1.shape = e2.shape;
		e1.main_status = e2.main_status;
		e1.sub_status = e2.sub_status;
		e1.rank = e2.rank;
		e1.last_die_timestamp = e2.last_die_timestamp;
		e1.last_feed_timestamp =e2.last_feed_timestamp;

	}

	inline void PrepareData()
	{       
		memset(&_enhanced_essence,0,sizeof(_enhanced_essence));
		_enhanced_essence.magic_number = pet_bedge_enhanced_essence::PET_MAGIC_NUMBER;
		_enhanced_essence.version_number = pet_bedge_enhanced_essence::PET_VERSION_NUMBER; 

		_inner_data.clear();
		DuplicateInnerData();
	}

	inline void MakeFakeData()
	{
		if(_enhanced_essence.identify) return;

		_enhanced_essence.star = 0;
		_enhanced_essence.main_type = 0;
		_enhanced_essence.element = 0;
		for(size_t i = 0;i < pet_bedge_enhanced_essence::MAX_COMBAT_ATTR;++i)
		{
			_enhanced_essence.attr[i].potential = 0;
		}
		for(size_t i = pet_bedge_enhanced_essence::RESISTANCE1;i <= pet_bedge_enhanced_essence::RESISTANCE6;++i)
		{
			_enhanced_essence.attr[i].init = 0;
			_enhanced_essence.attr[i].potential = 0;
			_enhanced_essence.attr[i].cur_value = 0;
		}
		_enhanced_essence.extra_damage = 0;
		_enhanced_essence.extra_damage_reduce = 0;
		_enhanced_essence.crit_rate = 0.0f;
		_enhanced_essence.crit_damage = 0.0f;
		for(size_t i = 0;i < pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT;++i)
		{
			_enhanced_essence.skills[i].id = 0;
			_enhanced_essence.skills[i].level = 0;
		}
		for(size_t i = 0;i < pet_bedge_enhanced_essence::MAX_PET_TANLENT_COUNT;++i)
		{
			_enhanced_essence.talents[i] = 0;
		}
		for(size_t i = 0;i < pet_bedge_enhanced_essence::MAX_MAKE_ATTR;++i)
		{
			_enhanced_essence.make_attr[i] = 0;
		}
		_enhanced_essence.face = 0;
		_enhanced_essence.shape = 0;
	}

	//给脚本用的获取函数
	float QueryInnerData(size_t index) const
	{
		if(index >= _inner_data.size()) return 0.f;
		return _inner_data[index];
	}

	size_t GetInnerDataCount() const
	{
		return _inner_data.size();
	}

	inline int GetOwnerID() const
	{
		return _enhanced_essence.owner_id;
	}

	inline void SetOwnerID(int id)
	{
		_enhanced_essence.owner_id = id;
		_dirty_flag = true;
	}

	inline const void* GetOwnerName(size_t& size) const
	{
		size = pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH;
		return _enhanced_essence.owner_name;
	}

	inline void SetOwnerName(const void* buf,size_t size)
	{
		memset(_enhanced_essence.owner_name,0,pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH);
		if(size > pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH) size = pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH;
		memcpy(_enhanced_essence.owner_name,buf,size);
		_dirty_flag = true;
	}

	inline const void* GetName(size_t& size) const
	{
		char temp_buf[pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH];
		memset(temp_buf,0,pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH);
		if(memcmp(_enhanced_essence.name,temp_buf,pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH) == 0)
		{
			size = 0;
		}
		else
		{
			size = pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH;
		}
		return _enhanced_essence.name;
	}

	inline void SetName(const void* buf,size_t size)
	{
		memset(_enhanced_essence.name,0,pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH);
		if(size > pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH) size = pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH;
		memcpy(_enhanced_essence.name,buf,size);
		_dirty_flag = true;
	}

	inline int GetOrigin() const
	{
		return _enhanced_essence.origin;
	}       

	inline void SetOrigin(int origin)
	{
		_enhanced_essence.origin = origin;
		_dirty_flag = true;
	}

	inline int GetLevel() const
	{
		return _enhanced_essence.level;
	}       

	inline void SetLevel(int level)
	{
		_enhanced_essence.level = level;
		_dirty_flag = true;
	}

	inline int GetExp() const
	{
		return _enhanced_essence.exp;
	}       

	inline void SetExp(int exp)
	{
		_enhanced_essence.exp = exp;
		_dirty_flag = true;
	}

	inline int GetCurHP() const
	{
		return _enhanced_essence.cur_hp;
	}       

	inline void SetCurHP(int value)
	{
		if(value < 0) return;
		_enhanced_essence.cur_hp = value;
		_dirty_flag = true;
	}
	
	inline int GetCurVigor() const
	{
		return _enhanced_essence.cur_vigor;
	}       

	inline void SetCurVigor(int value)
	{
		if(value < 0) return;
		_enhanced_essence.cur_vigor = value;
		_dirty_flag = true;
	}

	inline int GetStar() const
	{
		return _enhanced_essence.star;
	}       

	inline void SetStar(int value)
	{
		if(value < 0) return;
		_enhanced_essence.star = value;
		_dirty_flag = true;
	}

	inline int GetRebornStar() const
	{
		return _enhanced_essence.reborn_star;
	}       

	inline void SetRebornStar(int value)
	{
		if(value < 0) return;
		_enhanced_essence.reborn_star = value;
		_dirty_flag = true;
	}

	inline int GetMainType() const
	{
		return _enhanced_essence.main_type;
	}       

	inline void SetMainType(int value)
	{
		if(value < 0) return;
		_enhanced_essence.main_type = value;
		_dirty_flag = true;
	}

	inline int GetElement() const
	{
		return _enhanced_essence.element;
	}       

	inline void SetElement(int value)
	{
		if(value < 0) return;
		_enhanced_essence.element = value;
		_dirty_flag = true;
	}

	inline int GetIdentify() const
	{
		return _enhanced_essence.identify;
	}       

	inline void SetIdentify(int value)
	{
		if(value < 0) return;
		_enhanced_essence.identify = value;
		_dirty_flag = true;
	}

	inline int GetRace() const
	{
		return _enhanced_essence.race;
	}       

	inline void SetRace(int value)
	{
		if(value < 0) return;
		_enhanced_essence.race = value;
		_dirty_flag = true;
	}

	inline int GetHoroscope() const
	{
		return _enhanced_essence.horoscope;
	}       

	inline void SetHoroscope(int value)
	{
		if(value < 0) return;
		_enhanced_essence.horoscope = value;
		_dirty_flag = true;
	}

	inline int GetCombatAttrInit(int index) const
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_COMBAT_ATTR) return 0;
		return _enhanced_essence.attr[index].init;
	}       

	inline void SetCombatAttrInit(int index,int value)
	{
		if(value < 0) return;
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_COMBAT_ATTR) return;
		_enhanced_essence.attr[index].init = value;
		_dirty_flag = true;
	}
	
	inline float GetCombatAttrPotential(int index) const
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_COMBAT_ATTR) return 0;
		return _enhanced_essence.attr[index].potential;
	}       

	inline void SetCombatAttrPotential(int index,int value)
	{
		if(value < 0) return;
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_COMBAT_ATTR) return;
		_enhanced_essence.attr[index].potential = value;
		_dirty_flag = true;
	}

	inline int GetCombatAttrCur(int index) const
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_COMBAT_ATTR) return 0;
		return _enhanced_essence.attr[index].cur_value;
	}       

	inline void SetCombatAttrCur(int index,int value)
	{
		if(value < 0) return;
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_COMBAT_ATTR) return;
		_enhanced_essence.attr[index].cur_value = value;
		_dirty_flag = true;
	}

	inline float GetRebornPropAdd(int index) const
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_REBORN_PROP) return 0;
		return _enhanced_essence.reborn_prop_add[index];
	}       

	inline void SetRebornPropAdd(int index,float value)
	{
		if(value < 0) return;
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_REBORN_PROP) return;
		_enhanced_essence.reborn_prop_add[index] = value;
		_dirty_flag = true;
	}

	inline int GetExtraDamage() const
	{
		return _enhanced_essence.extra_damage;
	}       

	inline void SetExtraDamage(int value)
	{
		if(value < 0) return;
		_enhanced_essence.extra_damage = value;
		_dirty_flag = true;
	}

	inline int GetExtraDamageReduce() const
	{
		return _enhanced_essence.extra_damage_reduce;
	}       

	inline void SetExtraDamageReduce(int value)
	{
		if(value < 0) return;
		_enhanced_essence.extra_damage_reduce = value;
		_dirty_flag = true;
	}

	inline float GetCritRate() const
	{
		return _enhanced_essence.crit_rate;
	}       

	inline void SetCritRate(float value)
	{
		if(value < 0) return;
		_enhanced_essence.crit_rate = value;
		_dirty_flag = true;
	}

	inline float GetCritDamage() const
	{
		return _enhanced_essence.crit_damage;
	}       

	inline void SetCritDamage(float value)
	{
		if(value < 0) return;
		_enhanced_essence.crit_damage = value;
		_dirty_flag = true;
	}

	inline float GetAntiCritRate() const
	{
		return _enhanced_essence.anti_crit_rate;
	}       

	inline void SetAntiCritRate(float value)
	{
		if(value < 0) return;
		_enhanced_essence.anti_crit_rate = value;
		_dirty_flag = true;
	}

	inline float GetAntiCritDamage() const
	{
		return _enhanced_essence.anti_crit_damage;
	}       

	inline void SetAntiCritDamage(float value)
	{
		if(value < 0) return;
		_enhanced_essence.anti_crit_damage = value;
		_dirty_flag = true;
	}

	inline float GetSkillAttackRate() const
	{
		return _enhanced_essence.skill_attack_rate;
	}       

	inline void SetSkillAttackRate(float value)
	{
		if(value < 0) return;
		_enhanced_essence.skill_attack_rate= value;
		_dirty_flag = true;
	}

	inline float GetSkillArmorRate() const
	{
		return _enhanced_essence.skill_armor_rate;
	}       

	inline void SetSkillArmorRate(float value)
	{
		if(value < 0) return;
		_enhanced_essence.skill_armor_rate= value;
		_dirty_flag = true;
	}

	inline int GetDamageReduce() const 
	{
		return _enhanced_essence.damage_reduce;
	}       

	inline void SetDamageReduce(int value)
	{
		if(value < 0) return;
		_enhanced_essence.damage_reduce = value;
		_dirty_flag = true;
	}

	inline float GetDamageReducePercent() const 
	{
		return _enhanced_essence.damage_reduce_percent;
	}       

	inline void SetDamageReducePercent(float value)
	{
		if(value < 0) return;
		_enhanced_essence.damage_reduce_percent = value;
		_dirty_flag = true;
	}

	inline int GetMaxHungerPoint() const
	{
		return _enhanced_essence.max_hunger_point;
	}       

	inline void SetMaxHungerPoint(int value)
	{
		if(value < 0) value = 0;
		_enhanced_essence.max_hunger_point = value;
		_dirty_flag = true;
	}

	inline int GetCurHungerPoint() const
	{
		return _enhanced_essence.cur_hunger_point;
	}       

	inline void SetCurHungerPoint(int value)
	{
		if(value < 0) value = 0;
		_enhanced_essence.cur_hunger_point = value;
		_dirty_flag = true;
	}

	inline int GetMaxHonorPoint() const
	{
		return _enhanced_essence.max_honor_point;
	}       

	inline void SetMaxHonorPoint(int value)
	{
		if(value < 0) value = 0;
		_enhanced_essence.max_honor_point = value;
		_dirty_flag = true;
	}

	inline int GetCurHonorPoint() const
	{
		return _enhanced_essence.cur_honor_point;
	}       

	inline void SetCurHonorPoint(int value)
	{
		if(value < 0) value = 0;
		_enhanced_essence.cur_honor_point = value;
		_dirty_flag = true;
	}

	inline int GetAge() const
	{
		return _enhanced_essence.age;
	}       

	inline void SetAge(int value)
	{
		if(value < 0) value = 0;
		_enhanced_essence.age = value;
		_dirty_flag = true;
	}
	
	inline int GetLife() const
	{
		return _enhanced_essence.life;
	}       

	inline void SetLife(int value)
	{
		if(value < 0) value = 0;
		_enhanced_essence.life = value;
		_dirty_flag = true;
	}

	inline int GetSkillID(int index) const
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT) return 0;
		return _enhanced_essence.skills[index].id;
	}       

	inline void SetSkillID(int index,int value)
	{
		if(value < 0) return;
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT) return;
		_enhanced_essence.skills[index].id = value;
		_dirty_flag = true;
	}

	inline int GetSkillLevel(int index) const
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT) return 0;
		return _enhanced_essence.skills[index].level;
	}       

	inline void SetSkillLevel(int index,int value)
	{
		if(value < 0) return;
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT) return;
		_enhanced_essence.skills[index].level = value;
		_dirty_flag = true;
	}

	inline int GetTalent(int index) const
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_PET_TANLENT_COUNT) return 0;
		return _enhanced_essence.talents[index];
	}       

	inline void SetTalent(int index,int value)
	{
		if(value < 0) return;
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_PET_TANLENT_COUNT) return;
		_enhanced_essence.talents[index] = value;
		_dirty_flag = true;
	}

	inline int GetMakeAttr(int index)
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_MAKE_ATTR) return 0;
		return _enhanced_essence.make_attr[index];
	}

	inline void SetMakeAttr(int index)
	{
		if(index < 0 || index >= pet_bedge_enhanced_essence::MAX_MAKE_ATTR) return;
		_enhanced_essence.make_attr[index];
		_dirty_flag = true;
	}

	inline int GetAcuity() const
	{
		return _enhanced_essence.make_attr[pet_bedge_enhanced_essence::ACUITY];
	}       

	inline void SetAcuity(int value)
	{
		if(value < 0) return;
		_enhanced_essence.make_attr[pet_bedge_enhanced_essence::ACUITY] = value;
		_dirty_flag = true;
	}

	inline int GetStamina() const
	{
		return _enhanced_essence.make_attr[pet_bedge_enhanced_essence::STAMINA];
	}       

	inline void SetStamina(int value)
	{
		if(value < 0) return;
		_enhanced_essence.make_attr[pet_bedge_enhanced_essence::STAMINA] = value;
		_dirty_flag = true;
	}

	inline int GetSawy() const
	{
		return _enhanced_essence.make_attr[pet_bedge_enhanced_essence::SAWY];
	}       

	inline void SetSawy(int value)
	{
		if(value < 0) return;
		_enhanced_essence.make_attr[pet_bedge_enhanced_essence::SAWY] = value;
		_dirty_flag = true;
	}

	inline int GetDowry() const
	{
		return _enhanced_essence.make_attr[pet_bedge_enhanced_essence::DOWRY];
	}       

	inline void SetDowry(int value)
	{
		if(value < 0) return;
		_enhanced_essence.make_attr[pet_bedge_enhanced_essence::DOWRY] = value;
		_dirty_flag = true;
	}

	inline int GetWisdom() const
	{
		return _enhanced_essence.make_attr[pet_bedge_enhanced_essence::WISDOM];
	}       

	inline void SetWisdom(int value)
	{
		if(value < 0) return;
		_enhanced_essence.make_attr[pet_bedge_enhanced_essence::WISDOM] = value;
		_dirty_flag = true;
	}

	inline int GetCNR() const
	{
		return _enhanced_essence.make_attr[pet_bedge_enhanced_essence::CNR];
	}       

	inline void SetCNR(int value)
	{
		if(value < 0) return;
		_enhanced_essence.make_attr[pet_bedge_enhanced_essence::CNR] = value;
		_dirty_flag = true;
	}

	inline int GetFace() const
	{
		return _enhanced_essence.face;
	}       

	inline void SetFace(int value)
	{
		if(value < 0) return;
		_enhanced_essence.face = value;
		_dirty_flag = true;
	}

	inline int GetShape() const
	{
		return _enhanced_essence.shape;
	}       

	inline void SetShape(int value)
	{
		if(value < 0) return;
		_enhanced_essence.shape = value;
		_dirty_flag = true;
	}

	inline int GetMainStatus() const
	{
		return _enhanced_essence.main_status;
	}       

	inline void SetMainStatus(int value)
	{
		if(value < 0) return;
		_enhanced_essence.main_status = value;
		_dirty_flag = true;
	}

	inline int GetSubStatus() const
	{
		return _enhanced_essence.sub_status;
	}       

	inline void SetSubStatus(int value)
	{
		if(value < 0) return;
		_enhanced_essence.sub_status = value;
		_dirty_flag = true;
	}

	inline int GetRank() const
	{
		return _enhanced_essence.rank;
	}       

	inline void SetRank(int value)
	{
		if(value < 0) return;
		_enhanced_essence.rank = value;
		_dirty_flag = true;
	}

	inline int GetRebornCnt() const
	{
		return _enhanced_essence.reborn_cnt;
	}

	inline void SetRebornCnt(int value)
	{
		if(value < 0) return;
		_enhanced_essence.reborn_cnt = value;
	}

public:
	//给脚本用的添加函数
	void ClearInnerDara()
	{
		_dirty_flag = true;
		_inner_data.clear();
	}

	void SetInnerData(size_t index, float value)
	{
		if(index >= _inner_data.size())
		{
			_inner_data.reserve((index < 5?10:(int)((index + 1)*1.5f)));
			//_inner_data.reserve(index + 1);
			for(size_t i = _inner_data.size(); i <=index; i ++)
			{
				_inner_data.push_back(0.f);
			}
		}
		_inner_data[index] = value;
		_dirty_flag = true;
	}

public:
	static int script_QueryInnerData(lua_State* L);
	static int script_GetInnerDataCount(lua_State* L);
	static int script_SetInnerData(lua_State* L);
	static int script_ClearInnerData(lua_State* L);

	static int script_QueryLevel(lua_State* L);
	static int script_SetLevel(lua_State* L);
	static int script_QueryExp(lua_State* L);
	static int script_SetExp(lua_State* L);
	static int script_QueryCurHP(lua_State* L);
	static int script_SetCurHP(lua_State* L);
	static int script_QueryCurVigor(lua_State* L);
	static int script_SetCurVigor(lua_State* L);
	static int script_QueryStar(lua_State* L);
	static int script_SetStar(lua_State* L);
	static int script_QueryRebornStar(lua_State* L);
	static int script_SetRebornStar(lua_State* L);
	static int script_QueryMainType(lua_State* L);
	static int script_SetMainType(lua_State* L);
	static int script_QueryElement(lua_State* L);
	static int script_SetElement(lua_State* L);
	static int script_QueryIdentify(lua_State* L);
	static int script_SetIdentify(lua_State* L);
	static int script_QueryRace(lua_State* L);
	static int script_SetRace(lua_State* L);
	static int script_QueryHoroscope(lua_State* L);
	static int script_SetHoroscope(lua_State* L);
	static int script_QueryCombatAttrInit(lua_State* L);
	static int script_SetCombatAttrInit(lua_State* L);
	static int script_QueryCombatAttrPotential(lua_State* L);
	static int script_SetCombatAttrPotential(lua_State* L);
	static int script_QueryCombatAttrCur(lua_State* L);
	static int script_SetCombatAttrCur(lua_State* L);
	static int script_QueryRebornPropAdd(lua_State* L);
	static int script_SetRebornPropAdd(lua_State* L);
	static int script_QueryExtraDamage(lua_State* L);
	static int script_SetExtraDamage(lua_State* L);
	static int script_QueryExtraDamageReduce(lua_State* L);
	static int script_SetExtraDamageReduce(lua_State* L);
	static int script_QueryCritRate(lua_State* L);
	static int script_SetCritRate(lua_State* L);
	static int script_QueryCritDamage(lua_State* L);
	static int script_SetCritDamage(lua_State* L);
	static int script_QueryAntiCritRate(lua_State* L);
	static int script_SetAntiCritRate(lua_State* L);
	static int script_QueryAntiCritDamage(lua_State* L);
	static int script_SetAntiCritDamage(lua_State* L);
	static int script_QuerySkillAttackRate(lua_State* L);
	static int script_SetSkillAttackRate(lua_State* L);
	static int script_QuerySkillArmorRate(lua_State* L);
	static int script_SetSkillArmorRate(lua_State* L);
	static int script_QueryDamageReduce(lua_State* L);
	static int script_SetDamageReduce(lua_State* L);
	static int script_QueryDamageReducePercent(lua_State* L);
	static int script_SetDamageReducePercent(lua_State* L);
	static int script_QueryMaxHungerPoint(lua_State* L);
	static int script_SetMaxHungerPoint(lua_State* L);
	static int script_QueryCurHungerPoint(lua_State* L);
	static int script_SetCurHungerPoint(lua_State* L);
	static int script_QueryMaxHonorPoint(lua_State* L);
	static int script_SetMaxHonorPoint(lua_State* L);
	static int script_QueryCurHonorPoint(lua_State* L);
	static int script_SetCurHonorPoint(lua_State* L);
	static int script_QueryAge(lua_State* L);
	static int script_SetAge(lua_State* L);
	static int script_QueryLife(lua_State* L);
	static int script_SetLife(lua_State* L);
	static int script_QuerySkillID(lua_State* L);
	static int script_SetSkillID(lua_State* L);
	static int script_QuerySkillLevel(lua_State* L);
	static int script_SetSkillLevel(lua_State* L);
	static int script_QueryTalent(lua_State* L);
	static int script_SetTalent(lua_State* L);
	static int script_QueryAcuity(lua_State* L);
	static int script_SetAcuity(lua_State* L);
	static int script_QueryStamina(lua_State* L);
	static int script_SetStamina(lua_State* L);
	static int script_QuerySawy(lua_State* L);
	static int script_SetSawy(lua_State* L);
	static int script_QueryDowry(lua_State* L);
	static int script_SetDowry(lua_State* L);
	static int script_QueryWisdom(lua_State* L);
	static int script_SetWisdom(lua_State* L);
	static int script_QueryCNR(lua_State* L);
	static int script_SetCNR(lua_State* L);
	static int script_QueryFace(lua_State* L);
	static int script_SetFace(lua_State* L);
	static int script_QueryShape(lua_State* L);
	static int script_SetShape(lua_State* L);
	static int script_QueryRebornCnt(lua_State * L);
	static int script_SetRebornCnt(lua_State * L);
};

class item_pet_bedge: public item_body
{
public:
	item_pet_bedge()
	{}
	
public:
	virtual ITEM_TYPE GetItemType() const
	{
		return ITEM_TYPE_PET_BEDGE;
	}
	virtual void InitFromShop(gplayer_imp* pImp,item* parent,int value) const;
	//返回值约定 0:成功  其他:错误号
	int AdoptPet(gplayer_imp* imp,int inv_index);
	int FreePet(gplayer_imp* imp,int inv_index);
	int RefinePet(gplayer_imp* imp,int inv_index_pet,int inv_index_c,int inv_index_a);
	int RefinePet2(gplayer_imp* imp,int inv_index_pet,int type);
	int RenamePet(gplayer_imp* imp,int inv_index,const char* name_buf,int name_size);

	bool RebornPet(gplayer_imp * imp, size_t inv_index_pet);
	bool ChangeShape(gplayer_imp * imp, size_t inv_index_pet, int shape_id);
	bool RefineAttr(gplayer_imp * imp, size_t inv_index_pet, size_t attr_type, size_t assist_id, size_t assist_count);

	//virtual void GetItemDataForClient(const void** data,size_t& size,const void* buf,size_t len) const;
	virtual void GetItemDataForClient(item_data_client& data,const void* buf,size_t len) const;
	virtual int GetClientSize(const void * buf, size_t len) const { return sizeof(pet_bedge_enhanced_essence);}

	bool GetRefineInfoByType(int type, int & c_id, int & count);
	virtual void OnInit(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;

private:
	void TryConvertData(item * parent) const;
};
#endif

