#ifndef __SKILL_SKILLWRAPPER_H
#define __SKILL_SKILLWRAPPER_H

#include <map>
#include <set>

#include "common/types.h"
#include "common/base_wrapper.h"
#include "obj_interface.h"

namespace SKILL
{
	enum
	{
		SKILL_STATE_INIT = 0,
		SKILL_STATE_PRAY = 1,
		SKILL_STATE_PERFORM = 2,
	};
	
	struct Data
	{
		unsigned int	id;     // 传入参数, 技能ID
		char	forceattack;    // 传入参数，是否强制攻击

		bool	skippable;      // 传出参数，当前状态是否可提前结束
		int	stateindex;     // 当前状态ID
		int	nextindex;      // 下个状态ID
		int	warmuptime;     // 蓄力时间
		short	level;          // 级别
		int 	cooltime;       // 冷却时间
		int 	item_id;	// 物品ID
		short 	item_index;	// 物品位置
		bool    consumable;	// 物品是否消耗	
		char 	spirit_index;// 代替轩辕释放技能的灵体Index, 不是灵释放的默认为-1
		A3DVECTOR pos;
		char 	skillstate;	
		std::vector<short> talismanRefineSkillList;
		Data(unsigned int i) : id(i),forceattack(0),skippable(false),stateindex(-1),nextindex(-1),warmuptime(-1), item_id(-1), item_index(-1), consumable(false)
		{ 
			level = 0;
			cooltime = 0;
			spirit_index = -1;
			skillstate = SKILL_STATE_INIT;
		}
	};
}

namespace GNET
{

enum{
	MASK_DEFAULT_SKILL = 0x01,
	MASK_ADDON_SKILL   = 0x02,
	MASK_FAMILY_SKILL  = 0x04,
	MASK_SECT_SKILL    = 0x08,
	MASK_NOLEARNTRIGGER_SKILL = 0x10,
	MASK_ADDON_SKILL_PERMANENT = 0x20,
	MASK_SYSTEM_DELIVERED_SKILL = 0x40,
};

enum
{
	SERIAL_SKILL_NONE = 0,
	SERIAL_SKILL_START,
	SERIAL_SKILL_SUCCEED,
	SERIAL_SKILL_TERMINAL,
};


enum { 
	FEEDBACK_HIT        = 0x01, //反馈击中消息
	FEEDBACK_DAMAGE     = 0x02, //反馈被伤害消息
	FEEDBACK_KILL       = 0x04, //反馈杀人消息
	FEEDBACK_MISS       = 0x08, //反馈未击中消息
	FEEDBACK_CRIT    	= 0x10, //反馈被暴击消息
};

#define MAX_SKILL_ELEM_NUM 10

class Skill;

class SkillWrapper
{
public:
	typedef unsigned int	ID;

protected:
	struct PersistentData
	{
		char	baselevel;  // 基础级别
		char	reallevel;  // 实际级别
		char	actilevel;  // 激活级别
		char    mask;
		int     cooltime;
		PersistentData(char _base = 0):baselevel(_base), reallevel(0),actilevel(0),mask(0),cooltime(0){ }
	};

	struct WeaponAddonTalent
	{
		int value1;
		int value2;
		WeaponAddonTalent(int v1=0, int v2=0):value1(v1), value2(v2){}
	};

	struct SkillElems
	{
		int	curr_num; //已填写技能元素的数量	
		unsigned short ids[MAX_SKILL_ELEM_NUM];
		SkillElems() : curr_num(0) { memset(ids, 0, sizeof(ids)); }
		SkillElems(const SkillElems& elems)
		{
			curr_num = elems.curr_num;
			memcpy(ids, elems.ids, sizeof(ids));
		}

		SkillElems& operator=(const SkillElems& elems)
		{
			curr_num = elems.curr_num;
			memcpy(ids, elems.ids, sizeof(ids));
			return *this;
		}
	};
	
	typedef std::map<ID,PersistentData>	StorageMap;
	typedef std::map<ID, bool>	SkillActiveMap;
	typedef std::map<ID,WeaponAddonTalent>	WeaponAddonMap;
	typedef std::map<ID, int>	ProficiencyMap;
	typedef std::map<ID, SkillElems>	SkillElemsMap;

	StorageMap	   map;
	WeaponAddonMap	   wamap;
	ProficiencyMap	prfmap;
	SkillElemsMap 	semap;
	SkillActiveMap trigger_active_map;

	short prayspeed;  
	short commonlevel;
	int   rejectrate;    	  // 反制庇佑反弹成功概率
	
	int dec_skill_level; 	  // 状态效果降低技能等级
	int curr_cast_speed_rate; // 施法时间比例,百分数

	int dark_skill_cd_adjust;
	int light_skill_cd_adjust;

	int record_map_id;
	A3DVECTOR record_pos;
	
public:
	SkillWrapper();

	// 技能类别 1.主动攻击，2.主动祝福，3.主动诅咒，4.物品技能，5.被动，6.武器附加, 7.生产，8.瞬移
	static char GetType(ID id); 	
	// 技能攻击范围：0点 1线 2自身球 3目标球 4圆锥形 5自身, 6, 地面无目标
	static char RangeType(ID id);

	int Learn( ID id, object_interface player );	// 返回新的级别，错误返回-1
	int Forget(bool all, object_interface player);   // 洗点 all: 1,洗去所有技能; 0,洗去当前职业技能
	int ForgetTalent(object_interface player);   // 洗去所有天赋
	void ForgetCulSkills(object_interface player); //洗去所有造化技能
	void ForgetDeitySkills(object_interface player);	//洗去所有封神技能

	void OnAddSkill(ID id, object_interface player);
	void OnRemoveSkill(ID id, object_interface player);

	// 武器附加属性,某技能级别+n,id为0则所有技能级别+n
	int Upgrade(ID id, unsigned int level, object_interface player); 
	int Degrade(ID id, unsigned int level, object_interface player); 

	// 技能石,装备武器后获得特定技能
	int InsertSkill(ID id, unsigned int level, object_interface player); 
	int RemoveSkill(ID id, unsigned int level, object_interface player); 

	int InsertSkillPermament(ID id, unsigned int level, object_interface player);

	//触发技能, 有状态效果激活的技能
	int InsertTriggerSkill(ID id, unsigned int level, object_interface player);
	int ClearTriggerSkill(ID id, object_interface player); 

	//系统发放的技能，自安喜爱副本使用
	int InsertSysDeliveredSkill(ID id, unsigned int level, object_interface player, bool sendClient);
	int ClearSysDeliveredSkill(ID id, object_interface player, bool sendClient);

	// 清除“情侣,夫妻”技;
	int ClearSpouseSkill(object_interface player);

	int Condition( ID id, object_interface player, const XID * target, int size ); // 返回 error_code

	// 返回值为下次调用时间间隔(毫秒),-1表示结束
	int StartSkill( SKILL::Data & skilldata, object_interface player, const XID * target, int size, int & next_interval);
	int Run( SKILL::Data & skilldata, object_interface player, const XID * target, int size, int & next_interval );
	int StartSkill( SKILL::Data & skilldata, object_interface player, const A3DVECTOR& target,int & next_interval);
	int Run( SKILL::Data & skilldata, object_interface player, const A3DVECTOR& target, int & next_interval );
	// 打断，受攻击时调用
	bool Interrupt( SKILL::Data & skilldata, object_interface player );
	// 玩家主动取消正在执行的技能
	bool Cancel( SKILL::Data & skilldata, object_interface player );
	// 提前结束当前状态，例如蓄力
	int Continue( SKILL::Data& skilldata, object_interface player, const XID* target,int size, int& next,int elapse);

	// 使用瞬发技能, 0 成功，-1 失败
	int InstantSkill( SKILL::Data & skilldata, object_interface player, const XID * target, int size, const A3DVECTOR& chargeDestPos, const XID& chargeTarget);

	// 使用物品附加技能, 0 成功, -1 失败
	int CastRune(SKILL::Data & skilldata, object_interface player, int level, int coolid);

	int CastExtraSkillToSelf( SKILL::Data& skilldata, object_interface player, int level, const attack_msg* original_attack ); // Youshuang add

	// 读取和保存PersistentData
	void LoadDatabase(object_interface player, archive & ar );
	void StoreDatabase( archive & ar );
	void StorePartial( archive & ar );
	void StorePartialAddonPermanent(archive& ar);
	
	int GetBaseLevel(ID id);
	int GetLevelSum(int cls);

	// 主动攻击效果计算
	bool Attack(object_interface target, const XID&, const A3DVECTOR&,attack_msg& msg, bool invader, int damage, int is_crit);
	bool Attack(object_interface target, const XID&, const A3DVECTOR&,enchant_msg& msg, bool invader );

	// 被动技能，人物初始化
	bool EventReset(object_interface player);
	// 被动技能，变身
	bool EventChange(object_interface player, int form);
	// 被动技能，是否有分身存在, Add by Houjun 2010-03-11
	bool EventCloneExist(object_interface player, bool isCloneExist);
	// 被动技能，进入世界玩家属性计算完成后生效
	bool EventAfterEnterWorld(object_interface player);
	//玩家改变仙魔佛阵营
	void CultivationChange(object_interface player, int culold, int culnew);

	// 返回值为下次调用时间间隔(毫秒),-1表示结束
	int NpcStart(ID id, object_interface npc, const XID * target, int size, int& next_interval);
	void NpcEnd(ID id, object_interface npc, int level, const XID * target, int size );
	// 打断，受攻击时调用
	bool NpcInterrupt(ID id, object_interface npc, int level);
	float NpcCastRange(ID id, object_interface npc, int level);

	// 修改吟唱间隔
	int IncPrayTime(int inc);
	int DecPrayTime(int dec);
	
	int GetCurrCastSpeedRate() { return prayspeed; } 

	// 修改某技能冷却时间
	int IncCoolTime(ID id, int inc);
	int DecCoolTime(ID id, int dec);

	int GetDarkSkillCDAdjust() const;
	int GetLightSkillCDAdjust() const;
	int IncDarkSkillCoolTime(int inc);
	int DecDarkSkillCoolTime(int dec);
	int IncLightSkillCoolTime(int inc);
	int DecLightSkillCoolTime(int dec);
	
	void RemoveAura(object_interface player);
	void UndoPassive(ID id, int level, object_interface player);
	int GetTalentSum();
	int GetSpSum();
	void SetSkillTalent(Skill* skill);
	void SetSkillTalent(Skill* skill, const int* list);

	void SetSkillTalent(Skill* skill, object_interface player, bool adjustLevel = false);
	
	//Add by Houjun 2010-06-08, 获取天书技能，将其放到list中
	void SetSkillTalent(ID id, int list[8], object_interface player);

	//Add by Houjun 2010-03-15, 添加和删除组合技技能元素
	void AddSkillElement(ID id, int elemID);
	void SetSkillElement(ID id, int idx, int elemID);
	void DelSkillElement(ID id, int idx);	
	void SetSkillElemsNum(Skill* skill, const SkillElems& elem);
	void SetSkillElemsNum(Skill* skill, unsigned short elems[10]);
	//Add end.

	//Add by Houjun 2010-03-05, 保存和读取SkillElems数据
	void SaveSkillElems(archive& ar);
	void LoadSkillElems(archive& ar);
	void SaveSkillElemsClient(archive& ar);
	//Add end.

	//Add by Houjun 2010-06-29, 被动天书技能重置相关函数	
	bool OnTalentChange(object_interface player, int talent_skill_id, int old_talent_skill_lvl, int new_talent_skill_level);
	//Add end.

	bool CheckConsistency();
	static int GetCooldownId(ID id);
	static int GetMpCost(ID id, int level);
	
	int GetSerialSkillType(ID id);
	void GetSucceedSkillColor(ID id, int& color, int& num);
	void SetSucceedSkillColor(Skill* skill, object_interface player);
	void SetSucceedSkillColor(Skill* skill, char colors[5]);

	void SetDecSkillLevel(int level);

	void RecordPos(object_interface player);
	void ClearRecordPos();
	int GetRecordMapId() const { return record_map_id; }
	const A3DVECTOR& GetRecordPos() const { return record_pos; }

	bool SetXPSkill(int id);
	bool ClearXPSkill(object_interface player);
	bool ClearXPSkillCoolTime(object_interface player);
	
public:
	void Swap(SkillWrapper&);
	int GetSkillLevel(ID id);
	short GetSkillData(ID id, short& level, int& cooltime, object_interface player);
	virtual ~SkillWrapper(){}

	void Store( archive & ar ){}
	void Load( archive & ar ){}
	void SetFamilySkill(int *pairs, int size,object_interface player);

	void SetSectSkill(int *pairs, int size,object_interface player);
	void GetSectSkill(int *pairs, int& size);
	int  Forget(object_interface player, int mask);   
	void WeaponAddon(ID id, int value1, int value2);   
	void SetRejectRate(int rate);
	int  GetRejectRate();

	void AddProficiency(object_interface player, ID skillid, int add);			//增加技能熟练度
	bool DecProficiency(object_interface player, ID skillid, int dec);			//减少技能熟练度，返回成功或失败

	static unsigned int GetSkillLimit(ID id);			//获得技能使用限制
 	static bool IsMonsterLimitSkill(ID id);
	static bool IsPetLimitSkill(ID id);
	static bool IsSummonLimitSkill(ID id);
	static bool IsPlayerLimitSkill(ID id);
	static bool IsSelfLimitSkill(ID id);
	static bool IsBattleLimitSkill(ID id);
	static bool IsCombatLimitSkill(ID id);
	static bool IsNonCombatLimitSkill(ID id);
	static bool IsNoSummonPetLimitSkill(ID id);
	static bool IsMountLimitSkill(ID id);
	static bool IsFlyLimitSkill(ID id);
	static bool IsNoSummonLimitSkill(ID id);
	static int 	LimitSkillSummonID(ID id);
	static bool IsBossLimitSkill(ID id);

	static bool IsWarmup(ID id);
	static bool IsCycle(ID id);
	static bool IsTalisman(ID id);

	static int GetMaxSkillLevel(ID id);

	//0, None; 1, Dark; 2, Light;
	static char GetDarkLightType(ID id);
	static bool IsCastInPreSkill(ID id);
	static int  GetPreCastSkillId(ID id);

	static bool IsXPSkill(ID id);

};

};

#endif

