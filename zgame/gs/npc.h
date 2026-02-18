#ifndef __ONLINEGAME_GS_NPC_H__
#define __ONLINEGAME_GS_NPC_H__

#include "aggrolist.h"
#include "actobject.h"
#include <common/types.h>
#include <hashmap.h>

struct aggro_param;
struct ai_param;
class world;
class service_npc;

class gnpc_dispatcher : public  dispatcher
{
public:
DECLARE_SUBSTANCE(gnpc_dispatcher);

public:
	virtual ~gnpc_dispatcher(){}
	virtual void begin_transfer(){}
	virtual void end_transfer(){}
	virtual void enter_slice(slice * ,const A3DVECTOR &pos);
	virtual void leave_slice(slice * ,const A3DVECTOR &pos);
	virtual void get_base_info() { }	//NPC不会自己取得自己的信息，所以无效
	virtual void enter_world();
	virtual void leave_world() { }		//要告诉其他人自己离开了
	virtual void move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode);
	virtual void stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mode);
	virtual void start_attack(const XID &);
	virtual void on_death(const XID & ,bool reserver, int time = 0);
	virtual void disappear(char at_once =0);
	virtual void notify_root(unsigned char type);
	virtual void dodge_attack(const XID &attacker, int skill_id, const attacker_info_t & ainfo, char at_state,char speed,bool orange);
	virtual void be_damaged(const XID & id, int skill_id, const attacker_info_t & ainfo,int damage,int dt_damage,int dura_index,char,char stamp, bool orange);
	virtual void be_hurt(const XID & id, const attacker_info_t &,int damage,bool invader);
	virtual void query_info00(const XID & target, int cs_index,int sid);
	virtual void cast_skill(int target_cnt, const XID* target, int skill,unsigned short time, unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index);
	virtual void skill_interrupt(char reason);
	virtual void takeoff();
	virtual void landing();
	virtual void query_info_1(int uid,int cs_index, int cs_sid);
	virtual void send_turrent_leader(int id);
	virtual void level_up();
	virtual void enter_dim_state(bool state);
	virtual void dir_visible_state(int id, int dir, bool on);
	virtual void npc_invisible(bool on);
	virtual void player_mobactive_state_cancel(const XID & playerid, const XID & mobactive, int pos,const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos);
	virtual void teleport_skill_info(int leader_id, int npc_tid, int skill_id, int teleport_count);
public:
	virtual bool Save(archive & ar) { return true;}
	virtual bool Load(archive & ar) { return true;}
protected:
};


class gnpc_ai;
class gnpc_imp;
class gnpc_controller : public controller
{
protected:
	friend class gnpc_imp;
	int 		_svr_belong;			//NPC属于哪个服务器，对于自由转移的NPC，这个值是-1
	int 		_extern_svr;			//当前NPC处于哪个外部服务器，在本机的话，值为-1
	gnpc_ai * 	_ai_core;
	inline gnpc * GetParent()
	{
		return (gnpc*)_imp->_parent;
	}
public:
	int		_cry_for_help_timestamp;	//用于验证是否发送求救消息的时间戳
	float		_ignore_range;			//超过这个距离，npc就将目标的仇恨消除
	
public:
DECLARE_SUBSTANCE(gnpc_controller);
	gnpc_controller();
	virtual ~gnpc_controller();
	virtual void Init(gobject_imp * imp);
public:
//	virtual int DispatchControllerMessage(world * pPlane, const MSG & msg);
	virtual int MessageHandler(const MSG & msg);
	virtual int CommandHandler(int cmd_type,const void * buf, size_t size);
	virtual int MoveBetweenSlice(gobject *obj,slice * src, slice * dest);
	virtual void Release(bool free_parent = true);
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	virtual void OnHeartbeat(size_t tick);
	virtual void OnBorn();
	virtual void OnDeath(const XID & attacker);
	virtual void OnSkillAttack(int skill_id);
	virtual void Reborn();
	virtual void NPCSessionStart(int task_id, int session_id);
	virtual void NPCSessionEnd(int task_id,int session_id, int retcode);
	virtual void NPCSessionUpdateChaseInfo(int task_id,const void * buf ,size_t size);
	virtual bool NPCGetNextWaypoint(A3DVECTOR & target);

	void CryForHelp(const XID & attacker,int faction_ask_help,float sight_range);
	void TryCryForHelp(const XID & attacker);
	void AddAggro(const XID & who, int rage);
	void BeTaunted(const XID & who, int rage);
	int GetFactionAskHelp();
	void SetLifeTime(int life);
	void SetDieWithLeader(bool val);
	
public:
	bool CreateAI(const aggro_param & aggp, const ai_param & aip);
	gnpc_ai * GetAI() { return _ai_core;}
	void ReleaseAI();
	gactive_imp * GetImpl() { return (gactive_imp *) _imp;}
	
	void RefreshAggroTimer(const XID & target);
	void SetIdleMode(bool isIdle);
	void SetSealMode(int seal_flag);
	void SetFastRegen(bool b);
	
	
protected:
	int user_move(const void * buf, size_t size);
	inline void ActiveCombatState(bool state)
	{
		((gactive_imp*)_imp)->ActiveCombatState(state);
	}

};

class gnpc_notify : public abase::ASmallObject 
{
public:
	virtual ~gnpc_notify (){}
	virtual void OnDeath(gnpc_imp * imp){}
	virtual void OnMove(gnpc_imp * imp){}
	virtual void OnHeartbeat(gnpc_imp * impl,size_t tick){}
	virtual void OnDisappear(gnpc_imp * imp){}
	virtual void ForwardFirstAggro(gnpc_imp * imp, const XID & id, int rage) {}
	virtual int MoveOutHeartbeat(gnpc_imp * impl,int tick){ return 0;}
	virtual void OnUpdate(gnpc_imp * impl) {} 
};

class npc_spawner;
class gnpc_imp : public gactive_imp
{
protected:
	struct hurt_entry
	{
		int team_id;
		int team_seq;
		int damage;
		int level;
		int wallow_level;
		hurt_entry():team_id(-1),team_seq(0),damage(0),level(1),wallow_level(0){}
	};

	friend class gnpc_controller;
	friend class gnpc_dispatcher;
	gnpc_notify *_native_notify;		//通知原生对象的结构
	int _npc_state;				//NPC的状态
	typedef abase::hash_map<XID,hurt_entry,XID_HashFunc, abase::fast_alloc<> > DAMAGE_MAP;
	DAMAGE_MAP _dmg_list;
	XID 	   _first_attacker;		//第一个攻击自己的人
	//abase::hash_map<XID,hurt_entry,XID_HashFunc, abase::fast_alloc<> > _dmg_list;
//	aggro_list 	_dmg_list;		//伤害值列表
public:
	inline gnpc * GetParent()
	{
		return (gnpc*)_parent;
	}
	A3DVECTOR _birth_place;
	npc_spawner * 	_spawner;
	float _money_scale;
	float _drop_rate;
	XID _leader_id;				//leader的id
	char _inhabit_type;			//栖息地
	char _inhabit_mode;			//栖息地模式 
	char _after_death;			//死亡后续
	char _birth_dir;			//出生时的面朝方向
	bool	   _fix_drop_rate;		//模板中设计掉落概率不随是否群攻而变
	float	   _skill_drop_adjust;		//随技能而变的掉落率
	int  _corpse_delay;			//死亡时延迟时间
	int  _dead_time_count;			//死亡时间用于进行死亡延迟
	char _knocked_back;			//被击退的标志
	char _fast_regen;
	char _regen_spawn;			//用重新生成的方式来进行 reborn
	char _drop_no_protected;                //掉落不保护
	char _is_boss;				//是否BOSS
	char _player_cannot_attack;		//玩家攻击是否无效
	bool _battle_npc;			//是否为战场npc
	bool _collision_actived;		//是否激活了自己的凸包
	bool _no_exp_punish;			//怪物经验是否有经验惩罚
	bool _disappear_life_expire;		//寿命到了是否消失还是死亡
	int  _init_path_id;			//初始的路径ID
	int  _init_path_type;			//初始的路径类型
	bool _share_npc_kill;		//只要在伤害列表中就通知任务杀死NPC
	bool _share_drop;			//只要在伤害列表中就能拾取物品
	bool _is_fly;                   // Youshuang add 
	bool _show_damage;		// Youshuang add
	int  _buffarea_tid;			//buff区域的模板ID，大于0为有效
		
	template <int>
	inline char GetMoveModeByInhabitType()
	{
		//临时版
		static char mode[] = { 0,C2S::MOVE_MASK_WATER, C2S::MOVE_MASK_SKY,C2S::MOVE_MODE_RUN, 0,0,0,};
			return mode[(size_t)_inhabit_mode];
	}

	inline bool TestKnockBackFlag()
	{
		return _knocked_back;
	}

	void SetFastRegen(bool b);
	void SetBattleNPC(bool b);

	void SetDisappearLifeExpire(bool b);
	bool IsDisappearLifeExpire();

DECLARE_SUBSTANCE(gnpc_imp);
public:
	enum
	{
		NPC_STATE_NORMAL,
		NPC_STATE_WAITING_SWITCH,
		NPC_STATE_MOVEOUT,		//原生NPC已经转移到外部
		NPC_STATE_SPAWNING,		//在生成器中
	};
	int GetState() { return _npc_state;}
	void ClearDamageList()
	{
		_dmg_list.clear();
		_dmg_list.resize(20);
		_first_attacker.type = -1;
		_first_attacker.id = -1;
		_skill_drop_adjust = 0.f;
	}


public:
	gnpc_imp();
	~gnpc_imp();
	virtual void Init(world * pPlane,gobject*parent);
	virtual int MessageHandler(const MSG & msg);
	virtual int ZombieMessageHandler(const MSG & msg); 
	virtual int DispatchMessage(const MSG & msg);
	virtual int DoAttack(const XID & target,char force_attack);
	

	void ForwardFirstAggro(const XID & id,int rage);
	int GetNPCID() { return ((gnpc*)_parent)->tid; }
	int GetInitPathID() { return _init_path_id; }
	int GetInitPathType() { return _init_path_type;	}

//	void SendInfo00(const XID & target);
	void SetDieWithLeader(bool val) { ((gnpc_controller*)_commander)->SetDieWithLeader(val);}
	int GetComboSealMode();

	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	virtual void OnHeartbeat(size_t tick);
	virtual void SendInfo00(link_sid *begin, link_sid *end);	//发送自身数据给玩家
	virtual void SendBuffData(link_sid *begin, link_sid *end);	//发送Buff数据给玩家
	virtual void KnockBack(const XID & target, const A3DVECTOR & source, float distance);
	virtual void Flee(const XID & target,  float distance); 
	virtual void BounceTo(const A3DVECTOR & target_pos, float distance);
	virtual void Throwback(const A3DVECTOR & target_pos, float distance);
	virtual void DrawTo(const XID & target, const A3DVECTOR & pos);
	virtual void Repel2(const A3DVECTOR & target_pos, float distance);
	virtual void Reborn();
	virtual bool StepMove(const A3DVECTOR &offset);
	virtual bool CanMove();
	virtual void OnSetSealMode();
	virtual void SetCombatState();
	virtual void AddNPCAggro(const XID & who, int rage);
	virtual void BeTaunted(const XID & who, int rage);
	virtual void BeTaunted2(const XID & who, int rage);
	virtual int GetMonsterFaction();
	virtual int GetFactionAskHelp();
	virtual void SetLifeTime(int life);
	virtual void RelocatePos(bool is_disappear);
	virtual void OI_UpdateAllProp();
	virtual bool OI_GetLeader(XID & leader);
	virtual void OI_SetDropRate(float rate) { if(rate > 1.f) rate = 1.f; _skill_drop_adjust = rate; }
	virtual void PlayerCatchPet(const XID catcher);
	virtual int GetObjectType() { return TYPE_NPC;}
	virtual bool IsMonsterBoss() { return _is_boss > 0; }
	virtual bool CanTransform(int template_id);
	virtual void TransformMonster(int template_id, int lifetime);
	virtual int OI_GetTemplateID();
	virtual void OnCreate();
	virtual void SetDimState(bool is_dim);
	virtual void SetFrozenState(bool on);
	virtual void FillAttackMsg(const XID & target, attack_msg & attack, int dec_arrow = 0);  // Youshuang add
	virtual bool OI_IsFlying() { return _is_fly;} 
	
protected:
	virtual void DispatchExp(XID & owner, int &team,int &team_seq, int &own_level, XID & task_owner,int & wallow_level );
	void DropItem(const XID & owner, int owner_level, int team_id,int team_seq,int wallow_level);
	void DropItemFromData(const XID & owner, int owner_level, int team_id,int team_seq, int wallow_level);
	bool DropItemFromGlobal(const XID & owner, int owner_level, int team_id, int team_seq, int wallow_level);//返回是否继续掉落
	void DropMoney(const XID & owner, int owner_level, int team_id, int team_seq, int wallow_level);
	void  AddHurtEntry(const XID & attacker, int damage, int team,int seq,int level, int wallow_level);
	void  LifeExhaust();
	void AddAggroEntry(const XID & who , int faction, int level, int rage);

	gnpc_imp * TransformMob(int target_id, int lifetime);
	service_npc * TransformNPC(int target_id, int lifetime);
	void TransformMine(int target_id, int lifetime); 

protected:
	virtual void OnDamage(const XID & attacker,int skill_id, const attacker_info_t&info,int damage,int dt_damage,char at_state,char speed,bool orange);
	virtual void OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader);
	
	virtual void OnBorn();
	virtual void OnDeath(const XID & lastattack,bool is_invader, bool no_drop, bool is_hostile_duel, int time);
	virtual void OnAttacked(const MSG & msg, attack_msg * attack, damage_entry & dmg,bool is_hit);

	virtual void AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry &dmg);
	virtual bool CheckInvader(world * pPlane, const XID & source);
	
	virtual bool CatchedByPlayer();
	virtual bool IsNPCOwner(int roleid, int teamid, int master_id);
};




#endif

/*
	AI policy
	Aggro Policy

**/

