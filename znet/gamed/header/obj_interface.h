#ifndef __ONLINEGAME_GS_OBJECT_INTERFACE_H__
#define __ONLINEGAME_GS_OBJECT_INTERFACE_H__

#include "property.h"
#include "attack.h"
#include <vector>

class gactive_imp;
class filter;
class world;
struct pet_data;
class pet_bedge_essence;
namespace GNET { class SkillWrapper; }
namespace GDB { struct itemdata; struct shoplog;struct pocket_item;}
class object_interface
{
	gactive_imp * _imp;
	bool _battleground_flag;
public: 
	enum
	{
		AT_STATE_DT_ATTACK_CRIT	= 0x01,		//神圣暴击
		AT_STATE_ATTACK_RUNE2  	= 0x02,		//magic attack rune
		AT_STATE_RESIST 	= 0x04,		//抵抗
		AT_STATE_SECT_SKILL	= 0x08,		//門派技能效果
		AT_STATE_ATTACK_CRIT  	= 0x10,		//crit
		AT_STATE_ATTACK_RETORT	= 0x20,		//返震攻击
		AT_STATE_EVADE		= 0x40,		//无效攻击
		AT_STATE_IMMUNE		= 0x80,		//免疫此次攻击

	};

	enum COD_FACTION
	{
		TEAM_CIRCLE_OF_DOOM = 1, //组队模式阵法
		MAFIA_CIRCLE_OF_DOOM,    //帮派模式阵法
		FAMILY_CIRCLE_OF_DOOM,   //家族模式阵法
	};

	object_interface():_imp(0), _battleground_flag(false)
	{}

	object_interface(gactive_imp * imp, bool flag = false):_imp(imp), _battleground_flag(flag)
	{}

	void Attach(gactive_imp * imp)
	{
		_imp = imp;
	}
	gactive_imp * GetImpl() { return _imp;}
	bool GetBattlegroundFlag() { return _battleground_flag; }
public:
	void BeHurt(const XID & who,const attacker_info_t & info, int damage,char attacker_mode = 0);
	void BeHurt(const XID & who,const attacker_info_t & info, int damage,bool invader, char attacker_mode = 0);
	void Heal(const XID & healer,size_t life);
	void Heal(size_t life, bool is_potion = false);
	void InjectMana(int mana, bool is_potion = false);
	bool DrainMana(int mana);
	void InjectDeity(int deity, bool is_potion = false);
	bool DecDeity(int deity); 
	void DecHP(int hp);
	void DecHP(const XID & who, int hp);
	bool SessionOnAttacked(int session_id);

	void SetATDefenseState(char state);

	//飞行相关
	bool IsPlayerClass();
	int GetObjectType();
public:
	//filter 相关
	void AddFilter(filter*);
	void RemoveFilter(int filter_id);
	void ClearSpecFilter(int mask,int count = 0);
	bool IsFilterExist(int filter_id);
	bool IsMultiFilterExist(int id);
	void RemoveMultiFilter(int id); 
	void OnFilterAdd(int filter_id, const XID& caster);
	int FilterCnt(int buff_type);
	void AdjustSkill(int skill_id, short& level);
	bool ModifyFilter(int filterid, int ctrlname, void * ctrlval, size_t ctrllen);	
	
public:
	class MsgAdjust
	{
	public:
		virtual ~MsgAdjust(){}
		virtual void AdjustAttack(attack_msg & attack) {}
	};

	//攻击
	bool UseProjectile(size_t count);
	bool CanAttack(const XID & target);	//是否可以进行物理攻击
	void SetRetortState();			//设置下次攻击为反震攻击，一次有效
	void SetNextAttackState(char state);
	bool GetRetortState();                  //测试下次攻击是否为反震，以避免错误的TranslateSendAttack
	void Attack(const XID & target, attack_msg & attack,int use_projectile);
	void FillAttackMsg(const XID & target, attack_msg & attack,int use_projectile);

	void SendRegionAttack1(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,int count, std::vector<exclude_target>& target_exclude);
	
	void RegionAttack1(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,int count, std::vector<exclude_target>& target_exclude, bool transAttack = true);		//球
	void RegionAttack2(const A3DVECTOR& pos, float radius,attack_msg & attack,int up,float range,int count, std::vector<exclude_target>& target_exclude);									//柱
	void RegionAttack3(const A3DVECTOR& pos, float cos_half_angle,attack_msg & attack,int up,float range, int count, std::vector<exclude_target>& target_exclude);						//椎

	void RegionAttackFaceLine(float radius,attack_msg & attack,int up,float range,int count, std::vector<exclude_target>& target_exclude);		//线
	void RegionAttackFaceSector(float cos_half_angle,attack_msg & attack,int up,float range,int count, std::vector<exclude_target>& target_exclude);	//锥
	void RegionAttackCross(float radius, float range, float angle_to_north, attack_msg& attack, int max_count, std::vector<exclude_target>& target_exclude);

	void MultiAttack(const XID * target,size_t size, attack_msg & attack,int use_projectile);
	void MultiAttack(const XID * target,size_t size, attack_msg & attack,MsgAdjust & adj,int use_projectile);

	unsigned char GetAttackStamp();
	void IncAttackStamp();
	
	//技能
	void SendEnchantMsg(const XID& target, enchant_msg & msg);
	void SendRegionEnchant1Msg(const A3DVECTOR& pos, float radius,enchant_msg & enchant, size_t max_count, std::vector<exclude_target>& target_exclude);
	void Enchant(const XID & target, enchant_msg & msg);
	void EnchantZombie(const XID & target, enchant_msg & msg);

	void RegionEnchant1(const A3DVECTOR& pos, float radius,enchant_msg & msg,size_t mc, std::vector<exclude_target>& target_exclude);			//球
	void RegionEnchant2(const A3DVECTOR& pos, float radius,enchant_msg & msg,size_t mc, std::vector<exclude_target>& target_exclude);			//柱
	void RegionEnchant3(const A3DVECTOR& pos, float cos_half_angle,enchant_msg & msg,size_t mc, std::vector<exclude_target>& target_exclude);	//椎
	void MultiEnchant(const XID * target,size_t size, enchant_msg & msg);
	void TeamEnchant(enchant_msg & msg,bool exclude_self, bool norangelimit);
	void TeamEnchantZombie(enchant_msg & msg,bool exclude_self, bool norangelimit, int max_count);

	void RegionEnchantFaceLine(float radius,enchant_msg & enchant,int max_count, std::vector<exclude_target>& target_exclude);				//线
	void RegionEnchantFaceSector(float cos_half_angle, enchant_msg & enchant, int max_count, std::vector<exclude_target>& target_exclude);	//锥
	void RegionEnchantCross(float radius, float angle_to_north, enchant_msg& enchant, int max_count, std::vector<exclude_target>& target_exclude);

	void EnterCombatState();

	void KnockBack(const XID & attacker, const A3DVECTOR &source,float distance);
	void Flee(const XID & attacker, float distance);

	//复活
	bool Resurrect(float exp_reduce, float hp_recover); //0.0 ~ 1.0
	bool Resurrect(float exp_reduce);
	
	void SendHealMsg(const XID & target, int hp);		//给某个对象加血
	void SendAddManaMsg(const XID & target, int mp);

	void Reclaim();					//死亡后消失

	//查询 
	bool IsFemale();
	bool IsSpouse(const XID & target);
	bool IsMarried();
	bool IsDead();
	bool IsMember(const XID & member);
	bool IsInTeam();
	bool IsTeamLeader();
	bool GetLeader(XID & leader);
	int  GetClass();
	int  GetFaction();
	int  GetEnemyFaction();
	bool IsMafiaMember();
	int  GetMafiaID();
	bool IsFamilyMember();
	int GetFamilyID();
	const A3DVECTOR & GetPos();
	const int & GetTag();
	const XID & GetSelfID();
	const XID & GetCurTargetID();
	float GetBodySize();
	int QueryObject(const XID & who, A3DVECTOR & pos, float & body_size); 	//0 不存在 1: 正常 2: 死亡
	bool CheckGMPrivilege();
	bool IsPVPEnable();	//是否开启了PK开关
	bool IsMount();
	bool IsFlying();
	bool IsGathering();
	bool IsTransformState();

	//给客户端操作
	void SendClientMsgSkillCasting(int target_cnt, const XID* targets, int skill, unsigned short time,unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index);
	void SendClientMsgSkillInterrupted(char reason, char spirit_index);
	void SendClientMsgSkillContinue(int skill_id, char spirit_index);
	void SendClientMsgSkillPerform(char spirit_index);
	void SendClientNotifyRoot(unsigned char type);	//广播行为
	void SendClientSelfNotifyRoot(unsigned char type);	//只发送给个人
	void SendClientDispelRoot(unsigned char type);	//只发送给个人
	void SendClientCurSpeed();		//发送速度给客户端
	void SendClientEnchantResult(const XID & caster, int skill, char level, bool invader,char at_state, char stamp,int value);
	void SendClientInstantSkill(int target_cnt, const XID* targets, int skill, unsigned char level, const A3DVECTOR& pos, char spirit_index);
	void SendClientSkillAbility(int id, int ability);
	void SendClientCastPosSkill(const A3DVECTOR & pos, int skill,unsigned short time, unsigned char level);
	void SendClientRushMode(unsigned char is_active);
	void SendClientAttackData();
	void SendClientDefenseData();
	void SendClientDuelStart(const XID & target);
	void SendClientDuelStop(const XID & target);

	void SendClientSkillAddon(int skill, int level);
	void SendClientSkillCommonAddon(int common);
	void SendClientExtraSkill(int skill, int level);
	void SendClientLearnSkill(int id, int level);
	void SendClientSkillProficiency(int id, int proficiency);
	
	void SendClientAchieveData(size_t size, const void * data);
	void SendClientAchieveFinish(unsigned short achieve_id, int achieve_point, int finish_time);
	void SendClientPremissData(unsigned short achieve_id, char premiss_id,  size_t size, const void * data);
	void SendClientPremissFinish(unsigned short achieve_id, char premiss_id);
	void SendAchievementMessage(unsigned short achieve_id, int broad_type, int param, int finish_time);

	void SendClientTriggerSkillTime(short skillid, short time);
	void SendClientAddonSkillPermanent(int id, int level);


	void UpdateFamilySkill(int skill ,int ability);
	void TalismanGainExp(int exp, bool is_aircraft = false);
	int SpendTalismanStamina(float cost);
	void ReceiveTaskExp(int exp);
	void MagicGainExp(int exp);
	

public:
	//表面状态操作
	void IncVisibleState(unsigned short state);
	void DecVisibleState(unsigned short state);
	void ClearVisibleState(unsigned short state);

	void IncDirVisibleState(unsigned short state, short dir);
	void DecDirVisibleState(unsigned short state, short dir);
	void ClearDirVisibleState(unsigned short state, short dir);

	void UpdateBuff(short buff_id, short buff_level, int end_time, int overlayCnt = 1);
	void RemoveBuff(short buff_id, short buff_level);

	//设置变身标志	
	void ChangeShape(int shape);
	int GetShape();

	void LockEquipment(bool is_lock);
	void BindToGound(bool is_bind);

	//策略状态操作
	bool IsSilentSeal();
	bool IsDietSeal();
	bool IsMeleeSeal();
	bool IsRootSeal();

	void SetSilentSeal(bool isSeal);
	void SetSelfSilentSeal(bool isSeal);
	void SetDietSeal(bool isSeal);
	void SetMeleeSeal(bool isSeal);
	void SetRootSeal(bool isSeal);

	void ForbidAttack();
	void AllowAttack();

	bool IsAggressive();
	void SetAggressive(bool isActive = true);

	void DuelStart(const XID & target);
	void DuelStop();
	
	//在自己的仇恨列表中添加一定仇恨
	void AddAggro(const XID & attacker , int rage);
	void AddAggroToEnemy(const XID & helper, int rage);
	
	void BeTaunted(const XID & who,int aggro);

	void AddPlayerEffect(short effect);
	void RemovePlayerEffect(short effect);


	//取得免疫哪部分内容
	int GetImmuneMask();
	void SetImmuneMask(int mask);		//为1的位是要设置免疫的属性
	void ClearImmuneMask(int mask);		//为1的位是要清除的免疫属性

	void ActiveMountState(int mount_id, int mount_lvl, bool no_notify, char mount_type = 0);
	void DeactiveMountState();

	bool IsEquipWing();

	void Die();
	void Disappear();

public:
	//增强属性
	void EnhanceScaleHPGen(int hpgen);
	void ImpairScaleHPGen(int hpgen);

	void EnhanceScaleMPGen(int mpgen);
	void ImpairScaleMPGen(int mpgen);
	
	void EnhanceScaleDPGen(int dpgen);
	void ImpairScaleDPGen(int dpgen);

	void EnhanceMaxHP(int hp);
	void ImpairMaxHP(int hp);

	void EnhanceMaxMP(int mp);
	void ImpairMaxMP(int mp);
	
	void EnhanceMaxDP(int mp);	
	void ImpairMaxDP(int mp);

	void EnhanceScaleMaxHP(int hp);	
	void ImpairScaleMaxHP(int hp);	

	void EnhanceScaleMaxMP(int mp);	
	void ImpairScaleMaxMP(int mp);	
	
	void EnhanceScaleMaxDP(int mp);	
	void ImpairScaleMaxDP(int mp);	

	void EnhanceDefense(int def);
	void ImpairDefense(int def);
	void EnhanceScaleDefense(int def);
	void ImpairScaleDefense(int def);

	void EnhanceArmor(int ac);
	void ImpairArmor(int ac);
	void EnhanceScaleArmor(int ac);
	void ImpairScaleArmor(int ac);

	void EnhanceResistance(size_t cls, int res);	 //cls = [0,5]
	void ImpairResistance(size_t cls, int res);

	void EnhanceScaleResistance(size_t cls, int res);	 //cls = [0,5] res 千分制
	void ImpairScaleResistance(size_t cls, int res);

	void EnhanceResistanceTenaciy(size_t cls, int res);	 //cls = [0,5]
	void ImpairResistanceTenaciy(size_t cls, int res);

	void EnhanceScaleResistanceTenaciy(size_t cls, int res);	 //cls = [0,5] res 千分制
	void ImpairScaleResistanceTenaciy(size_t cls, int res);

	void EnhanceResistanceProficiency(size_t cls, int res);	 //cls = [0,5]
	void ImpairResistanceProficiency(size_t cls, int res);

	void EnhanceScaleResistanceProficiency(size_t cls, int res);	 //cls = [0,5] res 千分制
	void ImpairScaleResistanceProficiency(size_t cls, int res);

	void EnhanceScaleDamage(int dmg);
	void ImpairScaleDamage(int dmg);

	void EnhanceDamage(int dmg);
	void ImpairDamage(int dmg);

	void EnhanceDmgReduce(int dmg_reduce);
	void ImpairDmgReduce(int dmg_reduce);

	void EnhanceScaleDmgReduce(int dmg_reduce);
	void ImpairScaleDmgReduce(int dmg_reduce);
	
	void EnhanceScaleIgnDmgReduce(int ign_dmg_reduce);
	void ImpairScaleIgnDmgReduce(int ign_dmg_reduce);
	
	void EnhanceScaleDmgChange(int dmg_change);
	void ImpairScaleDmgChange(int dmg_change);

	void EnhanceSkillAttack(int skill_attack);
	void ImpairSkillAttack(int skill_attack);

	void EnhanceSkillArmor(int skill_armor);
	void ImpairSkillArmor(int skill_armor);

	void EnhanceAntiCritRate(int crit_rate);
	void ImpairAntiCritRate(int crit_rate);
	
	void EnhanceAntiCritDamage(float crit_damage);
	void ImpairAntiCritDamage(float crit_damage);

	void EnhanceDeityPower(int power);
	void ImpairDeityPower(int power);

	void EnhanceScaleDeityPower(int power);

	void EnhanceCultDefense(int index, int cult_defense);
	void ImpairCultDefense(int index, int cult_defense);

	void EnhanceCultAttack(int index, int cult_attack);
	void ImpairCultAttack(int index, int cult_attack);

	void EnhancePetDamage(int adjust);
	void EnhancePetAttackDefense(int adjust);

	void EnhanceOverrideSpeed(float speedup);
	void ImpairOverrideSpeed(float speedup);

	void EnhanceMountSpeed(float inc);
	void ImpairMountSpeed(float dec);

	void EnhanceSpeed(float speedup);
	void ImpairSpeed(float speedup);

	void EnhanceScaleSpeed(int inc);
	void ImpairScaleSpeed(int inc);

	void EnhanceScaleAttack(int attack);
	void ImpairScaleAttack(int attack);

	void EnhanceAttack(int attack);
	void ImpairAttack(int attack);

	void EnhanceCrit(int val);
	void ImpairCrit(int val);

	void EnhanceCritDmg(float val);
	void ImpairCritDmg(float val);

	void SetOverSpeed(float sp);

	//变身抗性
	void EnhanceAntiTransform(int val);
	void ImpairAntiTransform(int val);
	void EnhanceScaleAntiTransform(int val);
	void ImpairScaleAntiTransform(int val);

	//禁食抗性	
	void EnhanceAntiDiet(int val);
	void ImpairAntiDiet(int val);


	//属性的重新计算
	void UpdateDefenseData();
	void UpdateAttackData();
	void UpdateMagicData();	//人物抗性
	void UpdateSpeedData();
	void UpdateHPMPGen();
	void UpdateHPMP();
	void UpdateDPGen();
	void UpdateDP();
	void UpdateAllProp();

	void SetInvincibleFilter(bool is_vin, int timeout,bool immune);
	void AdjustDropRate(float rate);

//冷却
	bool TestCoolDown(unsigned short id);
	void SetCoolDown(unsigned short id, int ms);
	void ClrCoolDown(unsigned short id);

	void ReturnToTown();
	bool CanReturnToTown();

	void ReturnWaypoint(int point);
	bool CheckWaypoint(int point_index, int & point_domain);

	void JumpToSpouse();
	void SkillMove(const A3DVECTOR & pos);
	void BreakCurAction();

	int CalcPhysicDamage(int raw_damage, int attacker_level);
	int CalcMagicDamage(int dmg_class, int raw_damage, int attacker_level);

	float CalcLevelDamagePunish(int atk_level , int def_level);

	bool ModifySkillPoint(int offset);
	bool ModifyTalentPoint(int offset);
	void EnterSanctuary();
	void LeaveSanctuary();
	int GetCultivation();

public:
	//物品
	int CreateItem(int item_id,int count,int period);//创建物品
	int TakeOutItem(int item_id);			//去掉一个物品
	int TakeOutItem(int item_id, int count);	//去掉多个物品
	int TakeOutItem(int inv_index, int item_id, int count); //去掉指定位置某种物品
	bool CheckItem(int item_id,size_t count);	//检查是否存在某种物品
	bool CheckItem(int inv_index, int item_id, size_t count); //检查制定位置是否存在某种物品
	size_t GetMoney();
	void DecMoney(size_t money);
	void AddMoney(size_t inc);
	size_t GetInventorySize();
	size_t QueryItemPrice(int inv_index,int item_id);
	size_t QueryItemPrice(int item_id);
	size_t GetMallOrdersCount();
	int GetMallOrders(GDB::shoplog * list, size_t size);
	int GetRegionReputation(int index);
	void ModifyRegionReputation(int index, int rep);

	int GetLinkIndex();
	int GetLinkSID();
	int GetInventoryDetail(GDB::itemdata * list, size_t size);
	int GetTrashBoxDetail(GDB::itemdata * list, size_t size);
	int GetMafiaTrashBoxDetail(GDB::itemdata * list, size_t size);
	int GetEquipmentDetail(GDB::itemdata * list, size_t size);
	bool GetMallInfo(int & cash_used,int & cash, int &cash_delta,  int &order_id);
	
	bool IsCashModified();
	size_t GetEquipmentSize();
	size_t GetTrashBoxCapacity();
	size_t GetMafiaTrashBoxCapacity();
	size_t GetTrashBoxMoney();
	bool IsTrashBoxModified();
	bool IsEquipmentModified();
	int TradeLockPlayer(int get_mask,int put_mask);
	int TradeUnLockPlayer();
	int GetDBTimeStamp();
	int InceaseDBTimeStamp();

	int GetCityOwner(int city_id);
	bool TestSafeLock();
	int RebornCount();
	void Teleport(int tag, const A3DVECTOR & pos);
	unsigned int GetDBMagicNumber();

	size_t GetPetBedgeInventorySize();
	int GetPetBedgeInventoryDetail(GDB::itemdata * list, size_t size);
	size_t GetPetEquipInventorySize();
	int GetPetEquipInventoryDetail(GDB::itemdata * list, size_t size);

	size_t GetPocketInventorySize();
	int GetPocketInventoryDetail(GDB::pocket_item* list, size_t size);
	
	size_t GetFashionInventorySize();
	int GetFashionInventoryDetail(GDB::itemdata* list, size_t size);

	size_t GetMountWingInventorySize();
	int GetMountWingInventoryDetail(GDB::itemdata* list, size_t size);

	size_t GetGiftInventorySize();
	int GetGiftInventoryDetail(GDB::itemdata* list, size_t size);

	size_t GetFuwenInventorySize();
	int GetFuwenInventoryDetail(GDB::itemdata* list, size_t size);
public:
	//造一个小弟
	struct minor_param
	{
		int mob_id;		//模板ID是多少
		int vis_id;		//可见id，如果为0此值无效
		int remain_time;	//0 表示永久 否则表示存留的秒数
		int policy_classid;	//不能随意填写， 很重要默认填写0
		int policy_aggro;	//不能随意填写， 很重要默认填写0
		float exp_factor;	//经验值因子
		float drop_rate;	//掉落率修正
		float money_scale;	//掉落金钱修正
		XID spec_leader_id;	//指定的leader是谁 
		bool parent_is_leader;	//调用者就是leader 此时 spec_leader_id 无效
		bool use_parent_faction;//使用调用者的阵营信息，否则使用默认数据
		bool die_with_leader;	//leader 死亡或消失则自己也消失
		unsigned char mob_name_size;	//非0则用名称
		char mob_name[18];
		const void * script_data;
		size_t script_size;
	};

	struct mine_param
	{
		int mine_id;		//模板ID是多少
		int remain_time;	//0 表示永久 否则表示存留的秒数
	};

	void GenOwnerIDByType(char type, int & owner_id1, int & owner_id2);

	void CreateMinors(const A3DVECTOR & pos ,const minor_param & p);//在指定位置创建小弟
	void CreateMinors(const minor_param & p,float radius = 6.0f);	//在附近随机的位置创建小弟
	void CreateMine(const A3DVECTOR & pos , const mine_param & p);	//在制定位置创建矿物
	void CreateMines(int mid, int count, int lifetime, float range);//在制定位置创建矿物

	//Add by Houjun 2011-09-22 任务用会跨线的矿物和怪物创建接口，副本不可用
	void CreateMines(int mid, int tag, A3DVECTOR & pos, int lifetime, int count, char owner_type, float radius = 6.0f);//任务用来在指定地图和位置刷出有归属的矿物
	void CreateMonster(int id, int tag, A3DVECTOR & pos, int lifetime, int count, char owner_type, float radius = 6.0f);//任务用来在指定地图和位置刷出有归属的怪物
	//end

	void CreateNPC(const  object_interface::minor_param & param,float radius = 6.0f); //在附近随机位置创建NPC
	void CreateNPC(const A3DVECTOR & pos, const  object_interface::minor_param & param); //在指定位置创建NPC
	void CreateProtectedNPC(int id, int lifetime, int task_id);

	bool CreatePet(const A3DVECTOR& pos,const pet_data* pData,const pet_bedge_essence* pEss,int tid,size_t pet_index,XID& who);

	static void CreateMob(world * pPlane,const A3DVECTOR & pos ,const minor_param & p);//创建一个怪物 非小弟

	bool SummonMonster(int id, int count, int lifetime, char type, int skill_level, bool is_clone = false, bool exchange = false);  
	void UnSummonMonster(char type,  bool is_clone);

	void CreateMinors(float dist, float angle, const minor_param& p);//在玩家面向顺时针转angle角度，dist距离的位置创建小弟
	bool CreateSkillObject(int id, int count, int lifetime, int skill_level, float dist, float angle, int move_state);
	bool CreateSkillObject(int id, int count, int lifetime, int skill_level, A3DVECTOR& pos, int move_state);

	long GetGlobalValue(long lKey);
	void PutGlobalValue(long lKey, long lValue);
	void ModifyGlobalValue(long lKey, long lValue);
	void AssignGlobalValue(long srcKey, long destKey);

	void PlayerCatchPet(const XID& catcher);
	void PlayerDiscover(int discover_type);

	//照妖镜
	bool CanTransform(int template_id);
	void TransformMonster(int template_id, int lifetime);

	//变身
	bool StartTransform(int template_id, int level, int exp_level, int timeout, char type);
	void StopTransform(int template_id, char type);
	int  GetTransformID();
	int GetTransformLevel();
	int GetTransformExpLevel();

	//怪物模板ID
	int GetTemplateID();
	void DropItem(unsigned int item_id, unsigned int item_num, unsigned int expire_date);

	//连续技
	void GetComboColor(int & c1, int & c2, int & c3, int & c4, int & c5);
	void ClearComboSkill();

	//影遁	
	bool CanSetInvisible();
	bool IsInvisible();
	void SetInvisible(int invisible_rate);
	void ClearInvisible(int invisible_rate);
	void EnhanceInvisible(int value);
	void ImpairInvisible(int value);
	void EnhanceAntiInvisible(int value);
	void ImpairAntiInvisible(int value);
	void SetDimState(bool is_dim);

	//分身
	const XID  GetCloneID();
	bool IsCloneExist();
	void ExchangePos(const XID & who);
	void ExchangeStatus(const XID & who);

	//瞬移
	A3DVECTOR GetPosBetween(const A3DVECTOR& start, const A3DVECTOR& end, float dist_from_start);
	bool CheckTargetPosReachable(const A3DVECTOR& targetPos);
	bool CollisionDetect(const A3DVECTOR& targetPos, A3DVECTOR& nearPos, float sampledist = 2.0f);
	bool CanCharge(const XID& target_id, const A3DVECTOR& destPos, char chargeType, float distance);
	void Charge(const XID& target_id, const A3DVECTOR& destPos, char chargeType);
	void ChargeToTarget(const XID& target_id, const A3DVECTOR& destPos);

	//分身反弹和技能镜
	void SetCloneMirror(bool is_mirror, int prop);
	void SetSkillMirror(bool is_mirror, int prop);
	void SetSkillReflect(bool is_reflect, int prop);

	//绝对领域
	void SetAbsoluteZone(int skill_id, int skill_level, float radis, int count, int mp_cost, char force, char is_helpful, int var1, int var2, int visible_state);

	//吃药效果增强和减弱
	void SetHealEffect(int value);

	void SetSkillTalent(int id, int skill_talent[8]);

	void SetBreakCasting();

	//封神技能相关状态
	bool IsIgniteState() ;
	bool IsFrozenState() ;
	bool IsColdInjureState();
	bool IsFuryState() ;

	void SetIgniteState(bool on); 
	void SetFrozenState(bool on);
	void SetColdInjureState(bool on);
	void SetFuryState(bool on);
	void SetDarkState(bool on);

	//获取特定范围内玩家数量
	int GetRegionPlayerCount(float radius, std::vector<exclude_target>& target_exclude);

	//消耗封神修为
	bool DecDeityExp(int exp);	

	//转移攻击
	void TransferAttack(XID & target, char force_attack,  int dmg_plus, int radius, int total); 

	unsigned char GetDirection();
	void SetTurnBuffState(bool on);
	void SetTurnDebuffState(bool on);
	void SetBloodThirstyState(bool on);
    bool IsBloodThirstyState();
	void ScalePlayer(char on, int scale_ratio);

	//囚牢相关接口
	void AddLimitCycleArea(int owner, const A3DVECTOR& center, float radius);
	void AddPermitCycleArea(int owner, const A3DVECTOR& center, float radius);
	void RemoveLimitCycleArea(int owner);
	void RemovePermitCycleArea(int owner);
	void ClearLimitCycleArea();
	void ClearPermitCycleArea();
	void GetPlayerInJail(std::vector<XID>& list, const A3DVECTOR& target, float radius, float height);
	void SendRemovePermitCycleArea(const XID& target);

	bool IsLimitCycleAreaExist(int owner);
	bool IsPermitCycleAreaExist(int owner);

	//拉扯相关接口
	bool PlayerFollowTarget(const XID& target, float speed, float stop_dist);
	bool StopPlayerFollowTarget();

	void PlayerPulling(bool on);
	void PlayerBePulled(const XID& pulling_id, bool on);
	void PlayerBeSpiritDraged(const XID& id_drag_me, bool on);
	void NotifyCancelPull(const XID& target);

	//碰撞战场专用技能
	void pz_maxspeed();		//加速
	void pz_halfspeed();		//减速
	void pz_ashill(bool on);	//不动如山
	void pz_nomove(bool on);	//定身
	void pz_chaos(bool on);		//混乱
	void pz_void(bool on);		//虚无	

	//轩辕光暗值
	void SetDarkLight(int);
	int GetDarkLight();
	void IncDarkLight(int);
	void DecDarkLight(int);

	void SetDarkLightForm(char);
	void ClearDarkLightForm();
	char GetDarkLightForm();
	bool IsXuanYuan();

	//飞行
	void PreFly();
	bool CheckCanFly();
	void ActiveFlyState(char type);
	void DeactiveFlyState(char type);

	//轩辕灵体
	void SetDarkLightSpirit(int idx, char type);
	char GetDarkLightSpirit(int idx);
	void AddDarkLightSpirit(char type);

	void SummonCastSkill(int summonId, const XID& target, int skillid, int skilllevel);
	void SendMirrorImageCnt(int cnt);

	bool QueryTalismanEffects(int& level, std::vector<short>& skills);

	void SetTalismanEmbedSkillCooltime(int skill_id, int cooltime);
	void SendTalismanSkillEffects(const XID& target, int level, float range, char force, int skill_var[16], const std::vector<short>& skills);

	void SetExtraEquipEffectState(bool on, char weapon_effect_level);

	bool IsMaster();

	int GetXPSkill();

public:
	//阵法相关
	/**
	 * @brief CircleOfDoomPrepare 
	 *
	 * @param radius: 半径
	 * @param faction: 组队、帮派、家族
	 * @param max_member_num: 成员的最大人数，不包含自己。
	 * @param skill_id: 所使用的阵法技能id
	 */
	void CircleOfDoomPrepare( float radius, int faction, int max_member_num, int skill_id);

	/**
	 * @brief CircleOfDoomStartup: 阵法结成时调用，主要用于设置阵内所有人的状态。
	 */
	void CircleOfDoomStartup();

	/**
	 * @brief CircleOfDoomStop: 阵法结束的时候调用，包括正常或非正常结束。
	 */
	void CircleOfDoomStop();

	/**
	 * @brief GetPlayerInCircleOfDoom 
	 *
	 * @param playerlist:阵法成员的list，不包含自己。 
	 *
	 * @return: 人数
	 */
	int  GetPlayerInCircleOfDoom( std::vector<XID> &playerlist );

	/**
	 * @brief IsInCircleOfDoom :判断施法者是否已经在别的阵法里，如果是不允许释放技能
	 *
	 * @return :true表示已经在阵法里，false表示没在阵法里。
	 */
	bool IsInCircleOfDoom();

	int GetCircleMemberCnt();

	void EnhanceProp(int prop_id, int value);
	void ImpairProp(int prop_id, int value);

	
public:
	//取得基本参数
	const basic_prop & 		GetBasicProp();
	const q_extend_prop & 		GetExtendProp();
	const q_enhanced_param & 	GetEnhancedParam();
	const q_scale_enhanced_param & 	GetScaleEnhanecdParam();
	const q_item_prop &		GetCurWeapon();
	GNET::SkillWrapper &		GetSkillWrapper();
	const int GetCurWeaponType(); 
};
#endif

