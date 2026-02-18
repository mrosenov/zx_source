#ifndef __ONLINEGAME_GS_ACTIVEOBJ_H__
#define __ONLINEGAME_GS_ACTIVEOBJ_H__

#include "config.h"
#include "gimp.h"
#include "object.h"
#include "attack.h"
#include "property.h"
#include "filter_man.h"
#include "aipolicy.h"
#include "skillwrapper.h"
#include <timer.h>
#include <arandomgen.h>
#include <common/protocol.h>
#include <glog.h>
#include "property_facade.h"
#include "playertemplate.h"
#include "gmatrix.h"
#include "active_buff.h"
#include "statedef.h"
#include "topic_site.h"
#include "sfilterdef.h"
#include "topic_site.h"

namespace GDB { struct itemdata;struct pocket_item;}
class gmatrix;
class property_policy;
/*
*	对象所处位置的逻辑判断结构（临时）
*/

enum
{
	//这里的数值和顺序其他地方已经使用了 
	LAYER_GROUND,
	LAYER_AIR,
	LAYER_WATER,
	LAYER_INVALID,
};

enum IMMUNE_MASK
{
	IMMUNE_PHYSIC = 0,
	IMMUNE_GOLD,
	IMMUNE_WOOD,
	IMMUNE_WATER,
	IMMUNE_FIRE,
	IMMUNE_EARTH,

	IMMUNE_MASK_PHYSIC	= 0x0001,
	IMMUNE_MASK_GOLD	= 0x0002,
	IMMUNE_MASK_WOOD	= 0x0004,
	IMMUNE_MASK_WATER	= 0x0008,
	IMMUNE_MASK_FIRE	= 0x0010,
	IMMUNE_MASK_EARTH	= 0x0020,

};

struct object_layer_ctrl
{
	enum 
	{
		MODE_GROUND,
		MODE_FLY,
		MODE_FALL,
		MODE_SWIM,
	};

	char layer;		//对象处于什么层面，0 地面，  1 天上  2 水中
	char move_mode;		//玩家的移动模式 

	char GetLayer() { return layer;}
	char GetMode() { return move_mode;}
	bool CanSitDown() { return move_mode == MODE_GROUND; }
	bool IsFalling() { return move_mode == MODE_FALL;}
	bool IsOnGround() { return layer == LAYER_GROUND;}
	bool IsOnAir()
	{
		return layer == LAYER_AIR || move_mode == MODE_FALL;
	}

	bool IsFlying()
	{
		return move_mode == MODE_FLY;
	}

	bool CheckAttack()
	{
		return move_mode != MODE_FALL;
	}

	//起飞
	void TakeOff()
	{
		move_mode = MODE_FLY;
		layer = LAYER_AIR;
	}

	//降落
	void Landing()
	{
		move_mode = MODE_FALL;
		layer = LAYER_AIR;
	}

	void Swiming()
	{
		move_mode = MODE_SWIM;
		layer = LAYER_WATER;
	}

	//降落完成
	void Ground()
	{
		move_mode = MODE_GROUND;
		layer = LAYER_GROUND;
	}

	void UpdateMoveMode(int mode)
	{
		layer = LAYER_GROUND;
		move_mode = MODE_GROUND;
		if(mode & C2S::MOVE_MASK_SKY) 
		{
			layer = LAYER_AIR;
			move_mode = MODE_FLY;
		}
		else if(mode & C2S::MOVE_MASK_WATER) 
		{
			layer = LAYER_WATER;
			move_mode = MODE_SWIM;
		}

		switch(mode & 0x3F)
		{
			case C2S::MOVE_MODE_FALL:
			case C2S::MOVE_MODE_SLIDE:
			case C2S::MOVE_MODE_FLY_FALL:
				move_mode = MODE_FALL;
			break;
		}
		
	}


	void UpdateStopMove(int mode)
	{
		//假设这里已经通过了验证
		layer = LAYER_GROUND;
		move_mode = MODE_GROUND;
		if(mode & C2S::MOVE_MASK_SKY) 
		{
			layer = LAYER_AIR;
			move_mode = MODE_FLY;
		}
		else if(mode & C2S::MOVE_MASK_WATER) 
		{
			layer = LAYER_WATER;
			move_mode = MODE_SWIM;
		}
	}
};

template <typename WRAPPER> WRAPPER &  operator >>(WRAPPER & ar, object_layer_ctrl & ctrl)
{
	ar.pop_back(&ctrl,sizeof(ctrl)); return ar;
}

template <typename WRAPPER> WRAPPER & operator <<(WRAPPER & ar, object_layer_ctrl & ctrl)
{
	ar.push_back(&ctrl,sizeof(ctrl)); return ar;
}


class act_session;
/**
 *	能够运动的对象的基础类实现
 */
class gactive_imp : public gobject_imp 
{
	int	_session_id;
	int _spirit_session_id;
protected:
	int _switch_dest;
	A3DVECTOR _switch_pos;

	active_buff	_buff;
	abase::static_multimap<int,int, abase::fast_alloc<> >  _set_addon_map;

	int  _cur_shape;		//当前变身状态 (1-魔魂姿态 2-蛊王姿态 3-烈山狐狸 4-五行遁 5-原灵术 6-傀儡姿态 7-麒麟姿态 8-人马人形态)
	char _silent_seal_flag;		//沉默 不能使用技能
	char _diet_seal_flag;		//禁食 不能使用药品
	char _melee_seal_flag;		//禁攻 不能普通攻击
	char _root_seal_flag; 		//定身 不能移动
	A3DVECTOR _direction;		//面朝的方向

public:
	GNET::SkillWrapper _skill;	//技能结构
	unsigned int _faction;		//自己的阵营
	unsigned int _enemy_faction;	//敌人的阵营
	filter_man _filters;		//filter管理器
	basic_prop _basic;		//基本属性值
	q_extend_prop _base_prop;	//基础属性值，刨除装备影响的基础值 同时这些数据也是保存在数据库中
	q_extend_prop _cur_prop;	//当前属性值
	q_item_prop  _cur_item;		//物品所产生的属性(本体)
	int _crit_rate;			//重击概率	 表示重击发生的百分比
	int _base_crit_rate;		//敏捷重击率加成
	int _exp_addon;			//经验加成，仅对player有效
	int _money_addon;		//金钱加成，仅对player有效
	int _immune_state;		//天生免疫的效果
	int _immune_state_adj;		//后来附加的免疫效果
	q_enhanced_param _en_point;	//按点数增强的属性
	q_scale_enhanced_param _en_percent;//按百分比增强的属性
	object_layer_ctrl _layer_ctrl;	//位置控制结构

	bool _combat_state; 		//是否对象处于战斗状态 
	bool _refresh_state;		//刷新状态，当对象的属性有变化时，这个值应该true
	bool _lock_equipment;		//装备锁定标志
	bool _lock_pet_bedge;		//宠物牌栏锁定标志
	bool _lock_pet_equip;		//宠物装备栏锁定标志
	bool _lock_inventory;		//包裹锁定标志
	bool _skill_mirror;		//技能镜, 反弹技能
	bool _clone_mirror;		//分身分担伤害
	bool _skill_reflect;		//技能反射
	int  _skill_mirror_rate;	//技能镜反弹概率
	int  _clone_mirror_rate;	//分身承受伤害概率
	int  _skill_reflect_rate;	//技能反射概率
	int  _heal_effect;		//吃药效果
	char _bind_to_ground;		//无法飞行 这个现在是计数了
	char _deny_all_session;		//禁止加入session操作 这个现在是个计数
	char _at_attack_state;		//临时保存一些攻击的状态  无需保存 
	char _at_defense_state;		//临时保存防御时的一些状态，这个状态与攻击状态重叠  无需保存
	char _at_feedback_state;	//攻击后反馈的状态
	char _sitdown_state;		//是否坐下状态
	char _pk_level;			//pk的级别
	unsigned char _attack_stamp;	//攻击的记数
	int  _pk_value;			//PK 状态
	int _session_state;		//当前对象的状态(这个state保存的是session state)
	act_session * _cur_session;	//当前的session
	int _hp_gen_counter;		//记录加血的计数值，用于少量加血
	int _mp_gen_counter;		//记录加血的计数值，用于少量加血
	int _dp_gen_counter;		//记录加神力的计数值，用于少量加神力
	abase::vector<act_session *, abase::fast_alloc<> > _session_list;	//考虑以后用队列实现
	int _expire_item_date;		//是否存在有期限的物品，这里面保存最近一次到期的到期时间
	XID _last_attack_target;	//最后一次攻击的目标，供反沉迷判断使用

	enum 
	{
		MAX_HP_DEC_DELAY = 4
	};

	enum
	{
		AT_STATE_DT_ATTACK_CRIT	= 0x01,		//神圣暴击
//		AT_STATE_ATTACK_RUNE2  	= 0x02,		//magic attack rune
		AT_STATE_IMMUNE_DAMAGE	= 0x02,		//免疫此次攻击伤害(buff效果不免疫)
		AT_STATE_RESIST 	= 0x04,		//抵抗
		AT_STATE_SECT_SKILL	= 0x08,		//門派技能效果
		AT_STATE_ATTACK_CRIT  	= 0x10,		//crit
		AT_STATE_ATTACK_RETORT	= 0x20,		//返震攻击
		AT_STATE_EVADE		= 0x40,		//无效攻击
		AT_STATE_IMMUNE		= 0x80,		//免疫此次攻击

	};

	enum
	{
		STATE_SESSION_IDLE,
		STATE_SESSION_MOVE,
		STATE_SESSION_ATTACK,
		STATE_SESSION_USE_ITEM,
		STATE_SESSION_GATHERING,
		STATE_SESSION_TRAHSBOX,
		STATE_SESSION_EMOTE,
		STATE_SESSION_OPERATE,
		STATE_SESSION_TRADE,
		STATE_SESSION_ZOMBIE,
		STATE_SESSION_SKILL,
		STATE_SESSION_GENERAL,
		STATE_SESSION_SITDOWN,
		STATE_SESSION_PRODUCE,
		STATE_SESSION_LOGON,
	};

	enum 
	{
		IDLE_MODE_NULL		= 0x00,
		IDLE_MODE_SLEEP		= 0x01,
		IDLE_MODE_STUN		= 0x02,
	};


	inline void UpdateDataToParent()		//将自己的基本数据更新的_parent中
	{
		gactive_object * pObj  = (gactive_object*)_parent;
		pObj->base_info.faction = GetFaction();	//阵营可能变化
		pObj->base_info.level = _basic.level;
		pObj->base_info.hp = _basic.hp;
		pObj->base_info.max_hp = GetMaxHP();
		pObj->base_info.mp = _basic.mp;
	}

	inline void ActiveCombatState(bool state) { _combat_state = state; } 
	inline bool IsCombatState() {return _combat_state;} 
	inline bool GetRefreshState() {return _refresh_state;}
	inline void SetRefreshState() {_refresh_state = true;} 
	inline void ClearRefreshState() {_refresh_state = false;} 
	inline void RecalcDirection() {_parent->dir = a3dvector_to_dir(_direction);} 
	inline void ClearDirection() { _direction = A3DVECTOR(0,0,0);}
	inline void SetDirection(unsigned char dir) {_parent->dir = dir;}
	inline int ActivateSetAddon(int addon_id)
	{
		return ++_set_addon_map[addon_id];
	}

	inline int DeactivateSetAddon(int addon_id)
	{
		return --_set_addon_map[addon_id];
	}

	inline unsigned char GetAttackStamp()
	{
		return _attack_stamp;
	}
	
	inline void IncAttackStamp()
	{
		_attack_stamp ++;
	}

	inline void UpdateExpireItem(int date)
	{
		if(_expire_item_date <= 0)
		{
			_expire_item_date = date;
		}                    
		else if(_expire_item_date > date)
		{                    
			_expire_item_date = date;
		}
	}


public:
//取得属性接口函数族
	inline int GetHP() const
	{
		return _basic.hp;
	}

	inline int GetMP() const
	{
		return _basic.mp;
	}

	inline int GetDP() const
	{
		return _basic.dp;
	}

	inline int GetInk() const
	{
		return _basic.ink;
	}

	inline int GetMaxHP() const
	{
		return netgame::GetMaxHP(_cur_prop);
	}

	inline int GetMaxMP() const
	{
		return netgame::GetMaxMP(_cur_prop);
	}
	
	inline int GetMaxDP() const
	{
		return netgame::GetMaxDP(_cur_prop);
	}

	inline int GetAttackSpeed() const
	{
		return netgame::GetAttackSpeed(_cur_prop, _cur_item);
	}

	inline void GetAttackCycle(int & cycle, int &point) const
	{
		cycle = _cur_item.attack_cycle;
		point = _cur_item.attack_point;
	}


	inline float GetAttackRange() const
	{
		return netgame::GetAttackRange(_cur_prop, _cur_item);
	}

	inline int GetAttackRate() const
	{
		return _cur_prop.attack;
	}

	inline int GetInvisibleRate() const
	{
		return _cur_prop.invisible_rate;
	}

	inline int GetAntiInvisibleRate() const
	{
		return _cur_prop.anti_invisible_rate;
	}

	inline int GetObjectLevel() const
	{
		return _basic.level;
	}

	inline int GetDeityLevel() const
	{
		return _basic.dt_level;
	}
	
	inline int GetRawPKValue() const
	{
		return _pk_value;
	}
	

	inline int GetPKLevel() const
	{
		return _pk_level;
	}

	inline active_buff & GetActiveBuff() 
	{
		return _buff;
	}	      

	
public:
	typedef bool (*attack_judge)(gactive_imp * __this , const MSG & msg, attack_msg & amsg);
	typedef bool (*enchant_judge)(gactive_imp * __this, const MSG & msg, enchant_msg & emsg);
	typedef void (*attack_fill)(gactive_imp * __this, attack_msg & attack);
	typedef void (*enchant_fill)(gactive_imp * __this, enchant_msg & enchant);
public:

	gactive_imp();
	~gactive_imp();
	virtual void Init(world * pPlane,gobject*parent);
	virtual void ReInit();
public:
	bool StartSession();
	bool EndCurSession();
	void TryStopCurSession();	//试图停止当前的session，不一定成功 ,同时会试图开始
	void TerminateCurSession(); 	//停止当前的session, 保证一定成功
	bool AddSession(act_session * ses);
	void AddStartSession(act_session * ses);
	act_session * GetCurSession() { return _cur_session;}
	act_session * GetNextSession() { if(HasNextSession()) return _session_list[0]; else return NULL;}
	bool HasNextSession() { return _session_list.size();}
	bool HasSession() { return _cur_session || _session_list.size();}
	void ClearSession();
	void ClearMoveSession();
	void ClearAttackSession();
	void ClearSpecSession(int exclusive_mask);
	void ResetSession();	//cur_session不以endsession的方式结束
	void ClearNextSession();
	bool CurSessionValid(int id);
	int GetCurSessionID() { return _session_id;}
	inline int GetNextSessionID() 
	{
		_session_id++;
		_session_id &= 0x7FFFFFFF;
		return _session_id;
	}
	inline int GetNextSpiritSessionID()
	{
		_spirit_session_id++;
		_spirit_session_id &= 0x7FFFFFFF;
		return _spirit_session_id;
	}
	int GetPKValue() const;


//虚函数
private:
	virtual int DoAttack(const XID & target,char force_attack);
public:
	virtual int MessageHandler(const MSG & msg);
	virtual bool CanAttack(const XID & target) { return true;}
	virtual void OnAttack() {};
	virtual bool StandOnGround() { return true;}
	virtual bool InFlying() { return false;}
	virtual void OnHeal(const XID & healer, int life){}
	virtual void OnHeal(int life, bool is_potion){}
	virtual void OnInjectMana(int mana, bool is_potion){}
	virtual int TakeOutItem(int item_id,int count) { return -1; }
	virtual int TakeOutItem(int inv_index, int item_id, int count){return -1;}
	virtual bool CheckItemExist(int item_id, size_t count) {return false;}
	virtual bool CheckItemExist(int inv_index, int item_id, size_t count) {return false;}
	virtual int CheckItemPrice(int inv_index, int item_id) { return 0;}
	virtual size_t GetMoneyAmount() { return 0;}
	virtual void AddMoneyAmount(size_t money) {}
	virtual void DecMoneyAmount(size_t money) {}
	virtual bool OI_IsMember(const XID & member) { return false;}
	virtual bool OI_IsInTeam() { return false;}
	virtual bool OI_IsTeamLeader() { return false;}
	virtual int OI_GetTeamLeader() { return 0;}
	virtual bool Resurrect(float exp_reduce) {return false;}
	virtual void EnterResurrectReadyState(float exp_reduce, float hp_recover) {}
	virtual void KnockBack(const XID & target, const A3DVECTOR & source, float distance) {}
	virtual void Flee(const XID & target,  float distance) {}
	virtual bool DrainMana(int mana) { return true; }
	virtual bool DecDeity(int deity) { return true; }
	virtual bool DecInk(int ink){return true;} 
	virtual void SendInfo00(link_sid *begin, link_sid *end) = 0;	//发送自身数据给玩家
	virtual void SendBuffData(link_sid *begin, link_sid *end) = 0;	//发送Buff数据给玩家
	virtual void SendTeamDataToMembers(){} ;	//发送组队可见信息给队友
	virtual void OnSetSealMode() = 0;
	virtual void SendAttackMsg(const XID & target, attack_msg & attack);
	virtual int GetCSIndex() { return -1;}
	virtual int  GetCSSid() { return -1;}
	virtual void SendEnchantMsg(int message,const XID & target, enchant_msg & attack);
	virtual void SendMsgToTeam(const MSG & msg,float range,bool exclude_self, bool norangelimit, int max_count){}
	virtual void AddNPCAggro(const XID & who,int rage){}
	virtual void BeTaunted(const XID & who,int rage){}
	virtual void BeTaunted2(const XID & who,int rage){}
	virtual void AddAggroToEnemy(const XID & who,int rage){}
	virtual void SetCombatState() {}
	virtual void FillAttackMsg(const XID & target, attack_msg & attack, int dec_arrow = 0);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int GetObjectClass() { return -1;}			//取得对象的职业
	virtual bool IsObjectFemale() { return false;}
	virtual bool CheckCoolDown(int idx) { return true;}
	virtual void SetCoolDown(int idx, int msec) {}
	virtual void ClrCoolDown(int idx) {}
	virtual int GetMonsterFaction() { return 0;}
	virtual int GetFactionAskHelp() { return 0;}
	virtual void SetLifeTime(int life) {} //设置寿命，只有NPC有这个相应
	virtual void EnableFreePVP(bool bVal) {}
	virtual void ObjReturnToTown() {}
	virtual void AddEffectData(short effect) {}  		//只给 player 用
	virtual void RemoveEffectData(short effect) {}		//只给 player 用
	virtual void EnterCosmeticMode(unsigned short inv_index,int cos_id) {}		//只给 player 用
	virtual void LeaveCosmeticMode(unsigned short inv_index) {}			//只给 player 用
	virtual void SetPerHitAP(int ap_per_hit){}
	virtual bool IsPlayerClass() { return 0;}
	virtual int GetObjectType() { return -1;}
	virtual bool IsMonsterBoss() { return false; }
	virtual bool IsInvisibleNPC(){ return false;}
	virtual int GetLinkIndex() { return -1;}
	virtual int GetLinkSID() { return -1;}
	virtual size_t OI_GetInventorySize() { return 0;}
	virtual int OI_GetInventoryDetail(GDB::itemdata * list, size_t size) { return -1;}
	virtual size_t OI_GetPetBedgeInventorySize() { return 0;}
	virtual int OI_GetPetBedgeInventoryDetail(GDB::itemdata * list, size_t size) { return -1;}
	virtual size_t OI_GetPetEquipInventorySize() { return 0;}
	virtual int OI_GetPetEquipInventoryDetail(GDB::itemdata * list, size_t size) { return -1;}
	virtual size_t OI_GetMallOrdersCount() { return 0;}
	virtual int OI_GetMallOrders(GDB::shoplog * list, size_t size) { return 0;}
	virtual int TradeLockPlayer(int get_mask,int put_mask) { return -1;}
	virtual int TradeUnLockPlayer() { return -1;}
	virtual int TradeCashLockPlayer(int get_mask, int put_mask) { return -1; }
	virtual int TradeCashUnLockPlayer() { return -1; }
	virtual void OnDuelStart(const XID & target);
	virtual void OnDuelStop();
	virtual int GetDuelTargetID() { return 0; }
	virtual void Die(const XID & attacker, bool is_pariah, char attacker_mode );
	virtual void ActiveMountState(int mount_id, int mount_lvl, bool no_notify, char mount_type = 0) {};
	virtual void DeactiveMountState() {};
	virtual bool AddPetToSlot(void * data) { return false;}
	virtual bool FeedPet(int food_mask, int hornor) { return false;}
	virtual bool OI_IsMafiaMember() { return false;}
	virtual int OI_GetMafiaID() { return 0;}
	virtual int OI_GetFamilyID() { return 0;}
	virtual int OI_GetMasterID() {return 0;}
	virtual int OI_GetZoneID() {return 0;}
	virtual int OI_GetInvaderCounter() { return 0; }
	virtual int OI_GetPkLevel() { return 0; }
	virtual int OI_GetMDuelMafiaID() { return 0; }
	virtual int OI_GetDuelTarget() {return 0;}
	virtual bool CheckGMPrivilege() { return false;}
	virtual int GetFaction() { return _faction;}
	virtual int GetEnemyFaction() { return _enemy_faction;}
	virtual size_t OI_GetTrashBoxCapacity() { return 0;}
	virtual int OI_GetTrashBoxDetail(GDB::itemdata * list, size_t size) { return -1;}
	virtual size_t OI_GetMafiaTrashBoxCapacity() { return 0;}
	virtual int OI_GetMafiaTrashBoxDetail(GDB::itemdata * list, size_t size) { return -1;}
	virtual bool OI_IsTrashBoxModified() {return false;}
	virtual bool OI_IsEquipmentModified() {return false;}
	virtual size_t OI_GetTrashBoxMoney() {return 0;}
	virtual int OI_GetEquipmentDetail(GDB::itemdata * list, size_t size) { return -1;}
	virtual size_t OI_GetEquipmentSize() { return 0;}
	virtual size_t OI_GetPocketInventorySize() {return 0;}
	virtual int OI_GetPocketInventoryDetail(GDB::pocket_item *list, size_t size){return -1;}
	virtual size_t OI_GetFashionInventorySize(){return 0;}
	virtual int OI_GetFashionInventoryDetail(GDB::itemdata * list, size_t size){return -1;}
	virtual size_t OI_GetMountWingInventorySize() {return 0;}
	virtual int OI_GetMountWingInventoryDetail(GDB::itemdata * list, size_t size){return -1;}
	virtual size_t OI_GetGiftInventorySize() {return 0;}
	virtual int OI_GetGiftInventoryDetail(GDB::itemdata * list, size_t size){return -1;}
	virtual size_t OI_GetFuwenInventorySize() {return 0;}
	virtual int OI_GetFuwenInventoryDetail(GDB::itemdata * list, size_t size){return -1;}
	virtual int OI_GetDBTimeStamp() { return 0;}
	virtual int OI_InceaseDBTimeStamp() { return 0;}
	virtual bool CheckWaypoint(int point_index, int & point_domain) { return false;}
	virtual void ReturnWaypoint(int point) {}
	virtual attack_judge GetHookAttackJudge(){return NULL;}
	virtual enchant_judge GetHookEnchantJudge(){return NULL;}
	virtual attack_fill GetHookAttackFill(){return NULL;}
	virtual enchant_fill GetHookEnchantFill(){return NULL;}
	virtual bool OI_IsPVPEnable() { return false;}
	virtual bool OI_IsInPVPCombatState() { return  false;}
	virtual bool OI_IsMount() { return  false;}

	virtual bool OI_IsFlying() { return  false;}
	virtual bool OI_IsGathering() { return  false;}
	virtual void OI_DenyAttack() {}
	virtual void OI_AllowAttack() {} 
	virtual void ModifyItemPoppet(int offset) {}
	virtual void ModifyExpPoppet(int offset) {}
	virtual bool OI_GetMallInfo(int &cash_used, int & cash, int &cash_delta,  int &order_id) { return false;}
	virtual bool OI_IsCashModified() { return false;}
	virtual void OI_UpdateAllProp() = 0;
	virtual void OI_UpdataAttackData() { property_policy::UpdateAttack(GetObjectClass(),this); }
	virtual void OI_Die();
	virtual void OI_Disappear();
	virtual void OI_ToggleSanctuary(bool active) {}
	virtual void OI_TalismanGainExp(int exp, bool is_aircraft) {}
	virtual void OI_ReceiveTaskExp(int exp) {}
	virtual void OnAntiCheatAttack() {}
	virtual bool ModifyTalentPoint(int offset) { return false;}
	virtual void SetTalismanAutobotArg( const addon_data& data ) {}
	virtual void ClearTalismanAutobotArg( const addon_data& data ) {}
	virtual bool OI_TestSafeLock() { return false;}
	virtual void NotifyStartAttack(const XID& target,char force_attack ){}
	virtual int OI_RebornCount() { return 0;}
	virtual bool LongJump(const A3DVECTOR &pos,int target_tag) { return false;}
	virtual int QueryRebornInfo(int level[3], int prof[4]) { memset(level, 0, sizeof(int)*3); memset(prof, 0 , sizeof(int) * 4); return 0;}
	virtual bool OI_GetLeader(XID & leader) { return false;}
	virtual int OI_SpendTalismanStamina(float cost) { return 0;}
	virtual unsigned int GetDBMagicNumber() {return 0;}
	virtual int SkillCreateItem(int id, int count, int period) { return -1;}
	virtual int OI_GetRegionReputation(int index) { return 0;}
	virtual void OI_ModifyRegionReputation(int index, int rep) { return ;}
	virtual int OI_GetCultivation() { return 0;}
	virtual char OI_GetPVPMask(){ return 0;} 
	virtual void OI_SetDropRate(float rate) { }
	virtual bool OI_IsSpouse(const XID & target) { return false; }
	virtual bool OI_IsMarried() { return false; }
	virtual void OI_JumpToSpouse() {}
	virtual void OI_MagicGainExp(int exp) {}
	virtual void OI_AddProficiency(int id, int add){}
	virtual void OI_AddTransformSkill(int skill_id, int skill_level){}
	virtual void OI_OnSilentSeal(){}
	virtual int OI_GetTemplateID(){return 0;}
	virtual bool OI_StartTransform(int template_id, int level, int exp_level, int timeout, char type){return false;}
	virtual void OI_StopTransform(int template_id, char type){return;}
	virtual int OI_GetTransformID(){return 0;}
	virtual int OI_GetTransformLevel(){return 0;}
	virtual int OI_GetTransformExpLevel(){return 0;}
	virtual bool OI_IsCloneExist(){return false;}
	virtual void OI_UnSummonMonster(char type){}
	virtual bool OI_SummonExist(int tid, XID& id){ return false; }

	virtual long OI_GetGlobalValue(long lKey);
	virtual void OI_PutGlobalValue(long lKey, long lValue);
	virtual void OI_ModifyGlobalValue(long lKey, long lValue);
	virtual void OI_AssignGlobalValue(long srcKey, long destKey);
	

	virtual void OI_AddHP(int hp){} 
	virtual void OI_AddMP(int mp){} 
	virtual void OI_AddDefense(int defense){}
     	virtual void OI_AddAttack(int attack){}
	virtual void OI_AddArmor(int armor){}
	virtual void OI_AddDamage(int damage){}	
	virtual void OI_AddResistance(const int resistance[6]){}	
	virtual void OI_AddCrit(int crit_rate, float crit_damage){}	
	virtual void OI_AddSpeed(float speed){}	
	virtual void OI_AddAntiCrit(int anti_crit, float anti_crit_damage){}	
	virtual void OI_AddSkillRate(int skill_attack_rate, int skill_armor_rate){}	
	virtual void OI_AddAttackRange(float range){}

	virtual void OI_AddScaleHP(int hp){}
	virtual void OI_AddScaleMP(int mp){}
	virtual void OI_AddScaleDefense(int defense){}
	virtual void OI_AddScaleAttack(int attack){}
	virtual void OI_AddScaleArmor(int armor){}
	virtual void OI_AddScaleDamage(int damage){}
	virtual void OI_AddScaleSpeed(int speed){}
	virtual void OI_AddScaleResistance(const int resistance[6]){}
	
	virtual int OI_GetTerritoryID() const { return 0;}
	
	virtual void PlayerCatchPet(const XID catcher){}
	virtual void PlayerDiscover(int discover_type){} 
	virtual bool CanTransform(int template_id){return false;}
	virtual void TransformMonster(int template_id, int lifetime){}
	virtual bool IsTransformState(){return false;}
	virtual bool IsBindState(){return false;}
	virtual bool CanActivateEquipment(){return true;}

	virtual void ClearComboSkill(){}
	virtual void GetComboColor(int& c1, int& c2, int& c3, int& c4, int& c5){}

	virtual bool CanSetInvisible(){return false;}
	virtual void SetInvisible(int invisible_rate){}
	virtual void ClearInvisible(int invisible_rate){}
	virtual const XID & GetCurTarget(){ ASSERT(false); return *(new XID(-1,-1));}
	virtual XID GetCloneID(){return XID(-1, -1);}
	virtual XID GetSummonID(){return XID(-1, -1);}
	virtual void SetDimState(bool is_dim){}
	virtual void ExchangePos(const XID & who){}
	virtual void ExchangeStatus(const XID & who){}
	virtual void SetAbsoluteZone(int skill_id, int skill_level, float radis, int count, int mp_cost, char force, char is_helpful, int var1, int var2, int visible_state){}
	virtual void SetBreakCasting(); 

	virtual bool IsIgniteState() { return false;} 
	virtual bool IsFrozenState() { return false;}
	virtual bool IsColdInjureState() { return false;}
	virtual bool IsFuryState() { return false;}
	virtual bool IsTurnBuffState() { return false; }
	virtual bool IsTurnDebuffState(){ return false; }
	virtual bool IsBloodThirstyState() { return false; }

	virtual void SetIgniteState(bool on){}
	virtual void SetFrozenState(bool on){} 
	virtual void SetColdInjureState(bool on){}
	virtual void SetFuryState(bool on){}
	virtual void SetDarkState(bool on){}
	virtual void SetCycleState(bool on){}
	virtual void SetTurnBuffState(bool on){}
	virtual void SetTurnDebuffState(bool on){}
	virtual void SetBloodThirstyState(bool on) {}
	virtual void SetExtraEquipEffectState(bool on, char level) {}

	virtual void TransferAttack(XID & target, char force_attack, int dmg_plus, int radius, int total);
	virtual void HandleTransferAttack(const XID & source, const A3DVECTOR &pos, char force_attack, int dmg_plus, int radius, int total);

	virtual void SendAttackFeedBack(const XID& target, int mask, int param2);

	//阵法相关
	virtual bool SetCircleOfDoomPrepare(float radius, int faction, int max_player_num, int skill_id) {return false;}
	virtual void SetCircleOfDoomStop() {}
	virtual void SetCircleOfDoomStartup() {}
	virtual bool IsInCircleOfDoom() { return false; }
	virtual int  GetPlayerInCircleOfDoom( std::vector<XID> &playerlist ) { return 0; }
	virtual int  GetCircleMemberCnt() { return 0; }

	virtual void AddLimitCycleArea(int owner, const A3DVECTOR& center, float radius) {}
	virtual void AddPermitCycleArea(int owner, const A3DVECTOR& center, float radius) {}
	virtual void RemoveLimitCycleArea(int owner) {}
	virtual void RemovePermitCycleArea(int owner) {}
	virtual void ClearLimitCycleArea() {}
	virtual void ClearPermitCycleArea() {}
	virtual bool CheckMoveArea(const A3DVECTOR& pos) { return true; }
	virtual bool IsLimitCycleAreaExist(int owner) { return false; }
	virtual bool IsPermitCycleAreaExist(int owner) { return false; }

	virtual void FollowTarget(const XID& target, float speed, float stop_dist) {}
	virtual void StopFollowTarget() {}
	virtual void SetIdPullingMe(int target, bool on) {}
	virtual void SetIdSpiritDragMe(int target, bool on) {}

	virtual void SetCollisionMaxSpeed(){}
	virtual void SetCollisionHalfSpeed(){}
	virtual void SetCollisionAshill(bool on){}
	virtual void SetCollisionNomove(bool on){}
	virtual void SetCollisionChaos(bool on){}
	virtual void SetCollisionVoid(bool on){}
	
	//轩辕光暗值
	virtual void SetDarkLight(int) {}
	virtual int GetDarkLight() { return 0; }
	virtual void IncDarkLight(int) {}
	virtual void DecDarkLight(int) {}
	virtual void SetDarkLightForm(char s) {}
	virtual void ClearDarkLightForm() {}
	virtual char GetDarkLightForm() { return 0; }

	//轩辕灵体
	virtual void SetDarkLightSpirit(int idx, char type){}
	virtual char GetDarkLightSpirit(int idx) { return 0; }
	virtual void AddDarkLightSpirit(char type) {}

	virtual void PreFly() {}
	virtual bool CheckCanFly() { return false; }
	virtual void ActiveFlyState(char type) {}
	virtual void DeactiveFlyState(char type) {}

	virtual void SetMirrorImage(int cnt){}
	virtual void DecMirrorImage(int cnt){}
	virtual int GetMirrorImage() { return 0; }
	virtual void ClearMirrorImage(){}
	virtual void SummonCastSkill(int summonId, const XID& target, int skillid, int skilllevel) {}

	virtual bool GetTalismanRefineSkills(int& level, std::vector<short>& skills) { return false; }
	virtual void SetTalismanEmbedSkillCooltime(int skill_id, int cooltime) {}
	virtual void ActiveTalismanSkillEffects(const XID& target, int parent_skill_level, float range, char force, const int skill_var[16]){}

	virtual bool IsXuanYuan() { return false; }
	virtual bool IsRenMa() { return false; }
	virtual void OnUsePotion(char type, int value) {}
	virtual void OnGatherMatter(int matter_tid, int cnt) {}
	virtual void CurseTeleport(const XID & who){}
	virtual void SpyTrap(){}
	virtual void SetPuppetForm(bool on, int num){}
	virtual void SetPuppetSkill(int skill_id, int immune_prob){}
	virtual void BounceTo(const A3DVECTOR & target_pos, float distance){}
	virtual void Throwback(const A3DVECTOR & target_pos, float distance){}
	virtual void TryCharge(int type, const A3DVECTOR & dest_pos, int target_id){}
	virtual void DrawTo(const XID & target, const A3DVECTOR & pos){}
	virtual void Repel2(const A3DVECTOR & target_pos, float distance){}
	virtual void Blowoff();
	virtual void CastCycleSkill(int skill_id, int skill_level, char skill_type,  float radius, int coverage, 
			int mpcost, int hpcost,int dpcost, int inkcost, char force, float ratio, int plus, int period, int var[16], int crit_rate){}
	virtual void SetQilinForm(bool on){}
	virtual void SetRage(int v){}
	virtual void AddRage(int v){}
	virtual int GetPuppetid(){ return 0;}
	virtual void ReduceSkillCooldown(int id, int cooldown_msec){}
	virtual void DeliverTopicSite( TOPIC_SITE::topic_data* data ){}  // Youshuang add
	virtual void NotifyBloodPoolStatus(bool on, int cur_hp, int max_hp){}
	virtual void OnChangeShape(int shape){}
	virtual XID GetQilinLeaderID() { return XID(-1,-1);}
	virtual void BeHurtOnSeekAndHideRaid(const XID & attacker) {}
	virtual void SendClientHiderTaunted(bool isStart) {}
	virtual void GetSeekAndHiderTauntedScore() {}

public:
	inline void NormalAttack(const XID & target, char force_attack)
	{
		DoAttack(target,force_attack);
		IncAttackStamp();
	}
	
	inline void TranslateAttack(const XID & target , attack_msg & attack, char type, float radius = -1, int max_cnt = -1)
	{
		_filters.EF_TranslateSendAttack(target,attack, type, radius, max_cnt);
	}

	inline void TranslateEnchant(const XID & target , enchant_msg & enchant)
	{
		_filters.EF_TranslateSendEnchant(target,enchant);
	}
	
//inlnie 逻辑操作
	inline void UpdateStopMove(int move_mode) { _layer_ctrl.UpdateStopMove(move_mode); } 
	inline void UpdateMoveMode(int move_mode) { _layer_ctrl.UpdateMoveMode(move_mode); }
	inline void Heal(const XID & healer, int life, bool can_heal_bloodpool)
	{
		if(_parent->IsZombie()) return;
		int newhp = _basic.hp + life;
		if(newhp > GetMaxHP())
		{
			int remain_hp = newhp - GetMaxHP();
			if(remain_hp && can_heal_bloodpool)
			{
				_filters.ModifyFilter(FILTER_BLOODPOOL, FMID_FILL_BLOODPOOL, &remain_hp, sizeof(int));
			}

			newhp = GetMaxHP();
		}
		life = newhp - _basic.hp;
		_basic.hp = newhp;
		SetRefreshState();
		OnHeal(healer,life);
	}
	inline void Heal(int life, bool is_potion, bool can_heal_bloodpool)
	{
		if(_parent->IsZombie()) return;
		if(HasHealEffect() && is_potion)  life += (int)((float)GetHealEffect() / (float)100.0 * life);
		
		int newhp = _basic.hp + life;
		if(newhp > GetMaxHP())
		{
			int remain_hp = newhp - GetMaxHP();
			if(remain_hp && can_heal_bloodpool)
			{
				_filters.ModifyFilter(FILTER_BLOODPOOL, FMID_FILL_BLOODPOOL, &remain_hp, sizeof(int));
			}

			newhp = GetMaxHP();
		}
		life = newhp - _basic.hp;
		_basic.hp = newhp;
		SetRefreshState();
		OnHeal(life, is_potion);
	}
	inline void InjectMana(int mana, bool is_potion)
	{
		if(IsRenMa()) return;

		if(HasHealEffect() && is_potion)  mana += (int)((float)GetHealEffect() / (float)100.0 * mana);

		if(_basic.mp < GetMaxMP())
		{
			_basic.mp += mana;	
			if(_basic.mp > GetMaxMP())
			{
				_basic.mp = GetMaxMP();
			}
			SetRefreshState();
		}
		OnInjectMana(mana, is_potion);
	}
	
	inline void InjectDeity(int deity, bool is_potion)
	{
		if(HasHealEffect() && is_potion)  deity += (int)((float)GetHealEffect() / (float)100.0 * deity);

		if(_basic.dp < GetMaxDP())
		{
			_basic.dp += deity;	
			if(_basic.dp > GetMaxDP())
			{
				_basic.dp = GetMaxDP();
			}
			SetRefreshState();
		}
	}

	//检查是否能移动
	bool CheckMove(int usetime,int move_mode)
	{
		if(usetime  < 80 || usetime > 1000) return -1;
		if((move_mode & C2S::MOVE_MASK_SKY) && !_layer_ctrl.IsFlying())
		{
			return false;
		}

		//这里是不是太严格?
		if(!(move_mode & C2S::MOVE_MASK_SKY) && _layer_ctrl.IsFlying())
		{
			return false;
		}
		return true;
	}

	float GetSpeedByMode(int mode)
	{
		return netgame::GetSpeedByMode(_cur_prop, mode);
	}

	bool CheckStopMove(const A3DVECTOR & target,int usetime,int move_mode)
	{
		return true;
	}

	inline void ObjectSitDown()
	{
		_sitdown_state = 1;
		((gactive_object*)_parent)->object_state |= gactive_object::STATE_SITDOWN;
	}

	inline bool IsSitDownMode()
	{
		return _sitdown_state;
	}

	inline void ObjectStandUp()
	{
		_sitdown_state = 0;
		((gactive_object*)_parent)->object_state &= ~gactive_object::STATE_SITDOWN;
	}

	inline void ChangeShape(int shape) 
	{ 
		gactive_object * pObj = (gactive_object *)_parent;
		pObj->shape_form = shape & 0xFF;
		pObj->object_state &= ~gactive_object::STATE_SHAPE;
		if(shape) pObj->object_state |= gactive_object::STATE_SHAPE;

		_cur_shape = shape; 
		OnChangeShape(shape);
	}

	inline int GetShape() { return _cur_shape; }

	//修改长期的表情策略
	inline void SetEmoteState(unsigned char emote)
	{
		gactive_object * pObj = (gactive_object *)_parent;
		pObj->emote_form = emote;
		pObj->object_state |= gactive_object::STATE_EMOTE;
	}

	inline bool HasEmoteState()
	{
		gactive_object * pObj = (gactive_object *)_parent;
		return pObj->object_state & gactive_object::STATE_EMOTE;
	}
	
	inline void ClearEmoteState()
	{
		gactive_object * pObj = (gactive_object *)_parent;
		pObj->emote_form = 0;
		pObj->object_state &= ~gactive_object::STATE_EMOTE;
	}

	inline void LockEquipment(bool is_lock)
	{
		_lock_equipment = is_lock;
	}

	inline void LockPetBedge(bool is_lock)
	{
		_lock_pet_bedge = is_lock;
	}

	inline void LockPetEquip(bool is_lock)
	{
		_lock_pet_equip = is_lock;
	}

	inline void LockInventory(bool is_lock)
	{
		_lock_inventory = is_lock;
	}
	
	
	inline void BindToGound(bool is_bind)
	{
		_bind_to_ground += is_bind?1:-1;
	}

	inline bool IsBindGound()
	{
		return _bind_to_ground;
	}

	inline void SetSkillMirror(bool is_mirror, int rate)
	{
		_skill_mirror = is_mirror;
		_skill_mirror_rate = rate;
		if(!is_mirror) _skill_mirror_rate = 0;
	}

	inline void SetCloneMirror(bool is_mirror, int rate)
	{
		_clone_mirror = is_mirror;
		_clone_mirror_rate = rate;
		if(!is_mirror) _clone_mirror_rate = 0;
	}
	
	inline void SetSkillReflect(bool is_reflect, int rate)
	{
		_skill_reflect = is_reflect;
		_skill_reflect_rate = rate;
		if(!is_reflect) _skill_reflect_rate = 0;
	}

	inline void SetHealEffect(int value)
	{
		_heal_effect += value;
	}

	inline bool HasHealEffect()
	{
		return _heal_effect != 0;
	}

	inline int GetHealEffect() 
	{
		int heal_effect = _heal_effect;
		if(heal_effect > 100) heal_effect = 100;
		else if(heal_effect < -100) heal_effect = -100;
		return heal_effect;
	}

	inline bool IsZoneServer()
	{
		return gmatrix::IsZoneServer();
	}
	

public:
//装备影响的函数系列
	inline void ResistanceEnhance(const int resistance[6])
	{
		_en_point.resistance[0]		+= resistance[0];
		_en_point.resistance[1]		+= resistance[1];
		_en_point.resistance[2]		+= resistance[2];
		_en_point.resistance[3]		+= resistance[3];
		_en_point.resistance[4]		+= resistance[4];
		_en_point.resistance[5]		+= resistance[5];
	}

	inline void ResistanceImpair(const int resistance[6])
	{
		_en_point.resistance[0]		-= resistance[0];
		_en_point.resistance[1]		-= resistance[1];
		_en_point.resistance[2]		-= resistance[2];
		_en_point.resistance[3]		-= resistance[3];
		_en_point.resistance[4]		-= resistance[4];
		_en_point.resistance[5]		-= resistance[5];
	}

	template <typename EQUIPMENT_DATA>
		inline void EquipItemEnhance(const EQUIPMENT_DATA & data)
		{
			_cur_item.damage_low 		+= data.damage_low;
			_cur_item.damage_high 		+= data.damage_high;
			_cur_item.max_hp 		+= data.hp;
			_cur_item.max_mp 		+= data.mp;
			_cur_item.attack 		+= data.attack;
			_cur_item.armor 		+= data.armor;
			_cur_item.defense 		+= data.defense;
		}
	
	template <typename EQUIPMENT_DATA>
		inline void EquipItemImpair(const EQUIPMENT_DATA & data)
		{
			_cur_item.damage_low 		-= data.damage_low;
			_cur_item.damage_high 		-= data.damage_high;
			_cur_item.max_hp 		-= data.hp;
			_cur_item.max_mp 		-= data.mp;
			_cur_item.attack 		-= data.attack;
			_cur_item.armor 		-= data.armor;
			_cur_item.defense 		-= data.defense;
		}
	
	template <typename EQUIPMENT_DATA>
		inline void EquipItemEnhance2(const EQUIPMENT_DATA & data)
		{
			_cur_item.damage_low 		+= data.damage_low;
			_cur_item.damage_high 		+= data.damage_high;
			_cur_item.deity_power		+= data.deity_power;
			_cur_item.max_hp 		+= data.hp;
			_cur_item.max_mp 		+= data.mp;
			_cur_item.max_dp		+= data.dp;
			_cur_item.attack 		+= data.attack;
			_cur_item.armor 		+= data.armor;
			_cur_item.defense 		+= data.defense;
		}

	template <typename EQUIPMENT_DATA>
		inline void EquipItemImpair2(const EQUIPMENT_DATA & data)
		{
			_cur_item.damage_low 		-= data.damage_low;
			_cur_item.damage_high 		-= data.damage_high;
			_cur_item.deity_power		-= data.deity_power;
			_cur_item.max_hp 		-= data.hp;
			_cur_item.max_mp 		-= data.mp;
			_cur_item.max_dp		-= data.dp;
			_cur_item.attack 		-= data.attack;
			_cur_item.armor 		-= data.armor;
			_cur_item.defense 		-= data.defense;
		}


	template <typename EQUIPMENT_DATA>
		inline void WeaponItemEnhance(const EQUIPMENT_DATA & data)
		{
			_cur_item.weapon_class		= data.weapon_type;
			_cur_item.attack_range		= data.attack_range;
			_cur_item.attack_cycle		= data.attack_cycle;
			_cur_item.attack_point		= data.attack_point;
		}

	template <typename EQUIPMENT_DATA>
		inline void WeaponItemImpair(const EQUIPMENT_DATA & data)
		{
			_cur_item.weapon_class		= 0;
			_cur_item.attack_range		= 0;
			_cur_item.attack_cycle		= 0;
			_cur_item.attack_point		= 0;
		}
	
	template <typename EQUIPMENT_DATA>
		inline void TalismanItemEnhance(const EQUIPMENT_DATA & data)
		{
			_cur_item.damage_low 		+= data.damage_low;
			_cur_item.damage_high 		+= data.damage_high;
			_cur_item.max_hp 		+= data.hp;
			_cur_item.max_mp 		+= data.mp;
		}

	template <typename EQUIPMENT_DATA>
		inline void TalismanItemImpair(const EQUIPMENT_DATA & data)
		{
			_cur_item.damage_low 		-= data.damage_low;
			_cur_item.damage_high 		-= data.damage_high;
			_cur_item.max_hp 		-= data.hp;
			_cur_item.max_mp 		-= data.mp;
		}

public:
//攻击函数系列
	inline int GeneratePhysicDamage(int scale_damage, int point_damage)
	{
		return netgame::GeneratePhysicDamage(_base_prop,_en_point, _cur_item, _en_percent, scale_damage, point_damage);
	}

	inline int GenerateMagicDamage2(int scale_damage, int point_damage)
	{
		return netgame::GenerateMagicDamage(_base_prop,_en_point, _cur_item, _en_percent, scale_damage, point_damage);
	}

	inline void GenerateAttackDamage(int & low, int & high)
	{
		netgame::GeneratePhysicDamage(_cur_prop, low, high);
	}
	
	inline int GenerateAttackDamage()
	{
		return netgame::GeneratePhysicDamage(_cur_prop);
	}

	inline int GenerateMagicDamage()
	{
		return netgame::GenerateMagicDamage(_cur_prop);
	}

	inline int GeneratePhysicDamageWithoutRune(int scale_damage, int point_damage)
	{
		return GeneratePhysicDamage(scale_damage, point_damage);
	}

	inline int GenerateMaigicDamage2WithoutRune(int scale_damage, int point_damage)
	{
		return GenerateMagicDamage2(scale_damage, point_damage);
	}

	inline int GenerateAttackDamageWithoutRune()
	{
		return GenerateAttackDamage();
	}

	inline int GenerateMagicDamageWithoutRune()
	{
		return GenerateMagicDamage();
	}

	inline int GetCritRate()
	{
		return netgame::GetCritRate(_cur_prop, _crit_rate ,_base_crit_rate);
	}

	inline float GetCritDamageFactor()
	{
		return netgame::GetCritFactor(_cur_prop);
	}

	inline int GetIgnDmgReduce()
	{
		return netgame::GetIgnDmgReduce(_en_percent);
	}

	inline int GetDmgChange()
	{
		return netgame::GetDmgChange(_en_percent);
	}

	inline int GetDeityPower()
	{
		return netgame::GetDeityPower(_cur_prop);
	}

	//设置下次攻击的类别为反震攻击，一次有效
	inline void SetRetortState()
	{
		_at_attack_state = AT_STATE_ATTACK_RETORT;
	}

	inline bool GetRetortState()
	{
		return _at_attack_state & AT_STATE_ATTACK_RETORT;
	}

	inline void SetNextAttackState(char state)
	{
		_at_attack_state |= state;
	}

	int MakeAttackMsg(attack_msg & attack,char force_attack);
	bool CheckAttack(const XID & target,bool report_err=true);
	bool CheckAttack(const XID & target,int * flag,float * pDis ,A3DVECTOR & pos);
	bool CheckSendSkillAttack(int skill_id);
	bool CheckRecvSkillAttack(unsigned int skill_limit, int id);
	virtual bool SetSeekRaidRandomSkillAttack(unsigned int & skill_id) { return true; }

public:
//消息发送函数
	template<int>
		void SendTo(int message,const XID & target, int param) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param);
			gmatrix::SendMessage(msg);
		}

	template<int>
		void SendTo2(int message,const XID & target, int param, int param2) 
		{
			MSG msg;
			BuildMessage2(msg,message,target,_parent->ID,_parent->pos,param,param2);
			gmatrix::SendMessage(msg);
		}

	template<int>
		void LazySendTo(int message,const XID & target, int param, int latancy) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param);
			gmatrix::SendMessage(msg,latancy);
		}

	template<int>
		void LazySendTo(int message,const XID & target, int param, int latancy,const void * buf, size_t len) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param,buf,len);
			gmatrix::SendMessage(msg,latancy);
		}

	template<int>
		void SendTo(int message,const XID & target, int param,const void * buf, size_t len) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param,buf,len);
			gmatrix::SendMessage(msg);
		}

	template<int>
		void SendTo2(int message,const XID & target,int param,int param2,const void * buf, size_t len) 
		{
			MSG msg;
			BuildMessage2(msg,message,target,_parent->ID,_parent->pos,param,param2,buf,len);
			gmatrix::SendMessage(msg);
		}

	void SendTeamVisibleStateToOther(int user_id,int cs_index, int cs_sid);
	
protected:

	void Swap(gactive_imp * rhs);

	void UpdateBuff(short buff_id, short buff_level, int end_time, char overlay_cnt);

	void RemoveBuff(short buff_id, short buff_level = 1);

	friend class object_interface;
	friend class ai_actobject;
public:
	inline void DecHP(int hp)
	{
		DoDamage(hp);
		if(_basic.hp <=0)
		{
			_basic.hp = 0;
			Die(XID(-1,-1),false,0);
		}
	}
	
	inline void DecHP(const XID & who, int hp)
	{
		if(who.id == -1 || who.type == -1) return;
		if(hp <= 0) return;
		MSG msg;
		BuildMessage(msg, GM_MSG_DEC_HP, who, _parent->ID, _parent->pos, hp);
		gmatrix::SendMessage(msg);
	}
	
	//因为某种原因受到特定的伤害 这个函数主要由filter来调用
	template<typename ATTACK_INFO>
	void BeHurt(const XID & who,const ATTACK_INFO & info,int damage,bool invader, char attacker_mode)
	{
		if(_parent->IsZombie()) return;
		//这个调用必须被延迟，否则死亡时产生的任务或者其他操作可能会增加新的filter导致出错

		int param2 = (invader?0xFF00:0) | (attacker_mode & 0xFF);
		MSG msg;
		BuildMessage2(msg, GM_MSG_HURT, _parent->ID, who, _parent->pos, damage, param2, &info, sizeof(info));
		gmatrix::SendMessage(msg);
	}


	/*	inline void GetIdleMode(bool & sleep, bool & stun)
	{
		sleep = _idle_mode_flag & IDLE_MODE_SLEEP;
		stun  = _idle_mode_flag & IDLE_MODE_STUN ;
	}

	inline void GetSealMode(bool & silent,bool & root)
	{
		silent = _seal_mode_flag & SEAL_MODE_SILENT;
		root  = _seal_mode_flag & SEAL_MODE_ROOT;
	}
	
	inline int GetSealMode()
	{ 
		return _seal_mode_flag;
	}

	inline bool IsRootMode()
	{
		return _root_seal_flg
	}
	*/


	bool GetSilentSealMode() { return _silent_seal_flag; }
	bool GetDietSealMode() { return _diet_seal_flag; }
	bool GetMeleeSealMode() { return _melee_seal_flag; }
	bool GetRootMode() { return _root_seal_flag; }
	
	void SetSilentSealMode(bool seal);
	void SetDietSealMode(bool seal);
	void SetMeleeSealMode(bool seal);
	void SetRootMode(bool seal);
	void Say(const char * msg);

	bool IsCrit(){ return _at_feedback_state & GNET::FEEDBACK_CRIT; }
	

	
protected:
//受到攻击和技能的判定
	int HandleAttackMsg(const MSG & msg, attack_msg * );
	int HandleEnchantMsg(const MSG & msg, enchant_msg * );
	bool AttackJudgement(attack_msg * attack,damage_entry &dmg);
	inline int DoDamage(int damage)
	{
		if(_immune_state_adj & IMMUNEDAMAGE) return 0; 
		_filters.EF_BeforeDamage(damage);
		if(damage <= 0) return 0;

		if(damage > _basic.hp) damage = _basic.hp;
		_basic.hp -= damage;
		SetRefreshState();
		return damage;
	}

protected:
//杂项
	void MH_query_info00(const MSG & msg);
	void DoHeartbeat(size_t tick);
	inline void IncHP(int hp_gen)
	{
		int tmp = _basic.hp + hp_gen;
		if(tmp > GetMaxHP()) tmp = GetMaxHP();
		if(tmp != _basic.hp)
		{
			_basic.hp = tmp;
			SetRefreshState();
		}
	}
	
	inline void IncMP(int mp_gen)
	{
		int tmp = _basic.mp + mp_gen;
		if(tmp > GetMaxMP()) tmp = GetMaxMP();
		if(tmp != _basic.mp)
		{
			_basic.mp = tmp;
			SetRefreshState();
		}
	}

	struct func
	{
		//这个是每八秒增加传入的数值
		static inline int  Update(int & base, int & counter,int offset,int max)
		{    
			counter += offset;
			int xo = counter & 0x07;
			base += counter >> 3;
			counter = xo;
			if(base > max) 
				base = max;
			else if(base < 0)
				base = 0;
			return base;
		}
	}; 

	inline int GenHP(int hp_gen)
	{
		int tmp = _basic.hp;
		if( tmp != func::Update(_basic.hp,_hp_gen_counter,hp_gen,GetMaxHP()))
		{
			SetRefreshState();
		}
		return _basic.hp;
	}
	
	inline void GenHPandMP(int hp_gen,int mp_gen)
	{
		int tmp = _basic.hp;
		if( tmp != func::Update(_basic.hp,_hp_gen_counter,hp_gen,GetMaxHP()))
		{
			SetRefreshState();
		}

		tmp = _basic.mp;
		if( tmp != func::Update(_basic.mp,_mp_gen_counter,mp_gen,GetMaxMP()))
		{
			SetRefreshState();
		}
	}
	
	inline int GenDP(int dp_gen)
	{
		int tmp = _basic.dp;
		if( tmp != func::Update(_basic.dp,_dp_gen_counter,dp_gen,GetMaxDP()))
		{
			SetRefreshState();
		}
		return _basic.dp;
	}

private:
//私有虚函数组
	virtual void OnHeartbeat(size_t tick) = 0;
	virtual void OnDamage(const XID & attacker,int skill_id, const attacker_info_t&info,int damage,int dt_damage,char at_state,char stamp,bool orange)=0;
	virtual void OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader)=0;
	virtual void OnDeath(const XID & lastattack,bool is_invader,bool no_drop = false, bool is_hostile_duel = false, int time = 0) = 0;
	virtual void OnAttacked(const MSG & msg, attack_msg * attack,damage_entry & dmg,bool hit) = 0;
	virtual void OnEnchant(const MSG & msg, enchant_msg * enchant){}
	virtual void AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg) = 0;
	virtual bool CheckInvader(world * pPlane, const XID & source){ return false;} //检测粉名攻击是否有效
	virtual void OnPickupMoney(size_t money,int drop_id){}
	virtual void OnPickupItem(const A3DVECTOR &pos,const void * data, size_t size,bool isTeam, int drop_id) {}
	virtual bool OnAttackMirror(const MSG & msg, attack_msg * attack);
	virtual bool OnEnchantMirror(const MSG & msg, enchant_msg *  enchant);

public:
//技能的所有接口
	inline int  CheckSkillCondition(unsigned int skill,const XID * target, int size)
	{
		object_interface obj_if(this);
		return _skill.Condition(skill,obj_if,target,size);
	}
	
	inline int NPCStartSkill(unsigned int skill_id, const XID & target,int & next_interval)
	{
		return _skill.NpcStart(skill_id,this,&target,1,next_interval);
	}
	
	inline void NPCEndSkill(unsigned int skill_id,int level,const XID & target)
	{
		_skill.NpcEnd(skill_id,this,level,&target,1);
	}

	inline bool NPCSkillOnAttacked(unsigned int skill_id,int level)
	{
		return _skill.NpcInterrupt(skill_id, object_interface(this),level);
	}

	inline float NPCSkillRange(unsigned int skill_id,int level)
	{
		return _skill.NpcCastRange(skill_id,object_interface(this),level);
	}
	

	inline int StartSkill(SKILL::Data & data, const XID * target,int size,int & next_interval)
	{
		return _skill.StartSkill(data,object_interface(this),target,size,next_interval);
	}

	inline int RunSkill(SKILL::Data & data, const XID * target,int size,int & next_interval)
	{
		return _skill.Run(data, object_interface(this),target,size,next_interval);
	}

	inline int StartSkill(SKILL::Data & data, const A3DVECTOR &pos,int & next_interval)
	{
		return _skill.StartSkill( data, object_interface(this), pos, next_interval);
	}

	inline int RunSkill(SKILL::Data & data, const A3DVECTOR & pos,int & next_interval)
	{
		return _skill.Run( data, object_interface(this), pos, next_interval);
	}
	
	inline int ContinueSkill(SKILL::Data & data,const XID * target,int size,int & next_interval,int elapse_time) 
	{
		return _skill.Continue(data, object_interface(this),target,size,next_interval,elapse_time);
	}

	inline int CastInstantSkill(SKILL::Data & data, const XID * target,int size, const A3DVECTOR& chargeDestPos, const XID& chargeTarget )
	{
		return _skill.InstantSkill(data,object_interface(this),target,size, chargeDestPos, chargeTarget);
	}

	inline bool SkillOnAttacked(SKILL::Data & data)
	{
		return _skill.Interrupt(data, object_interface(this));
	}

	inline bool CancelSkill(SKILL::Data & data) 
	{
		return _skill.Cancel(data, object_interface(this));
	}

	inline int GetSkillLevel(int skill_id)
	{
		return _skill.GetSkillLevel(skill_id);
	}
	
	inline int GetSkillBaseLevel(int skill_id)
	{
		return _skill.GetBaseLevel(skill_id);
	}

	inline void IncSkillAbility(int skill_id, int inc = 1)
	{
	}

	inline int GetSkillAbility(int skill_id)
	{
		return 0;
	}

	inline int GetSkillAbilityRatio(int skill_id, int level)
	{
		return 0;
	}

	inline void SetSkillTalent(int skill_id, int skill_talent[8])
	{
		_skill.SetSkillTalent(skill_id, skill_talent, this);
	}	    

	inline int InsertSysSkill(int skill_id, int level)
	{
		return _skill.InsertSysDeliveredSkill(skill_id, level, this, false);
	}

	inline int ClearSysSkill(int skill_id)
	{
		return _skill.ClearSysDeliveredSkill(skill_id, this, false);
	}

};


/*	active object 的对AI的包装借口*/

class  ai_actobject : public ai_object
{
	protected:
		gactive_imp * _imp;
	public:
		ai_actobject(gactive_imp * imp):_imp(imp)
		{}

		//destructor
		virtual ~ai_actobject()
		{
		}

		virtual gactive_imp * GetImpl()
		{
			return _imp;
		}

		//property
		virtual void GetID(XID & id)
		{
			id = _imp->_parent->ID;
		}

		virtual void GetPos(A3DVECTOR & pos)
		{
			pos = _imp->_parent->pos;
		}

		//virtual int GetState() = 0;

		virtual int GetHP()
		{
			return _imp->_basic.hp;
		}

		virtual int GetMaxHP()
		{
			return _imp->GetMaxHP();
		}

		virtual int GenHP(int hp)
		{
			return _imp->GenHP(hp);
		}

		virtual float GetAttackRange()
		{
			return _imp->GetAttackRange();
		}

		virtual float GetMagicRange(unsigned int id,int level)
		{	
			return _imp->NPCSkillRange(id,level);
		}
		
		virtual float GetBodySize()
		{
			return _imp->_parent->body_size;
		}

		virtual int GetFaction()
		{
			return _imp->_faction;
		}

		virtual int GetEnemyFaction()
		{
			return _imp->_enemy_faction;
		}


		//operation
		virtual void AddSession(act_session * pSession)
		{
			_imp->AddSession(pSession);
			_imp->StartSession();
		}

		virtual bool HasSession()
		{
			return _imp->_cur_session || _imp->_session_list.size();
		}

		virtual void ClearSession()
		{
			_imp->ClearSession();
		}

		virtual void SendMessage(const XID & id, int msg);
		virtual void ActiveCombatState(bool state)
		{
			_imp->ActiveCombatState(state);
		}

		virtual bool GetCombatState()
		{
			return _imp->IsCombatState();
		}

		virtual int GetAttackSpeed()
		{
			return _imp->GetAttackSpeed();
		}

		virtual world_manager* GetWorldManager()
		{
			return _imp->GetWorldManager();
		}
	public:

		virtual int QueryTarget(const XID & id, target_info & info);
};


#endif

