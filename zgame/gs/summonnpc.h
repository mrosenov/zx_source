#ifndef __ONLINE_GAME_GS_SUMMON_NPC_H__
#define __ONLINE_GAME_GS_SUMMON_NPC_H__

#include "npc.h"
#include "aipolicy.h"

class gplayer_imp;
class gsummon_dispatcher: public gnpc_dispatcher
{
public:
	DECLARE_SUBSTANCE(gsummon_dispatcher);
};


class gsummon_imp: public  gnpc_imp
{
private:
	friend class gsummon_policy;

	typedef abase::static_multimap<int, int, abase::fast_alloc<> >  ENEMY_LIST;
	ENEMY_LIST _enemy_list;
	leader_prop  _leader_data;

	attack_judge _attack_judge;
	enchant_judge _enchant_judge;
	attack_fill   _attack_fill;
	enchant_fill  _enchant_fill;
	
	char 	_summon_type;		//两种召唤类型: 动物和植物
	char	_aggro_state;		//两种仇恨模式: 被动和主动
	char	_move_state;		//两种跟随方式: 0 跟随，1　停留
	char 	_is_clone;		//0 - 不是  1 是
	bool	_is_invisible;		//是否隐身
	int	_summon_stamp;
	int	_master_attack_target;
	int	_heartbeat_counter;

	int 	_teleport_id;
	A3DVECTOR _teleport_pos;
	int	_teleport_max_count;
	int	_teleport_cur_count;
	int	_teleport_prob;

public:
	enum AGGRO_STATE
	{
		AGGRO_STATE_NONE = 0,
		AGGRO_STATE_DEFENSE = 1,
		AGGRO_STATE_OFFENSE = 2,
		AGGRO_STATE_PEACE = 3,
	};

	enum MOVE_TYPE
	{
		MOVE_STATE_NONE = 0,
		MOVE_STATE_FOLLOW = 1,
		MOVE_STATE_STAY = 2,
		MOVE_STATE_FIX = 3,	//烈山的传送机关
	};
	


public:
	DECLARE_SUBSTANCE(gsummon_imp);
	
	gsummon_imp();
	
	void SetAggroState (char state);
	void SetMoveState(char state);
	void SetCloneState(char is_clone);
	
	void SetSummonType (char type)
	{
		_summon_type = type;
	}
	
	void SetSummonStamp(int stamp)
	{
		_summon_stamp = stamp;
	}

	void SetSummonInvisible()
	{
		_is_invisible = true;
	}

	void SetTelePortInfo(int teleport_count, int teleport_prob)
	{
		_teleport_max_count = teleport_count;
		_teleport_prob = teleport_prob;
	}

	virtual void Init(world * pPlane,gobject*parent);
	virtual void Die( const XID& attacker, bool is_pariah, char attacker_mode );
	virtual void OnDeath( const XID & attacker, bool is_invader, bool bduel );
	virtual int MessageHandler(const MSG & msg);
	virtual void OnHeartbeat(size_t tick);
	virtual void FillAttackMsg(const XID & target, attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target,enchant_msg & enchant);
	virtual void RelocatePos(bool is_disappear); 
	virtual bool OI_IsPVPEnable();
	virtual bool OI_IsInPVPCombatState();
	virtual bool OI_IsInTeam();
	virtual int  OI_GetMafiaID();
	virtual int  OI_GetMasterID();
	virtual int OI_GetZoneID();
	virtual bool OI_IsMember(const XID & id);
	virtual int OI_GetPkLevel();
	virtual int OI_GetInvaderCounter();
	virtual int OI_GetMDuelMafiaID();
	virtual int OI_GetDuelTarget();
	virtual int OI_GetCultivation() { return _leader_data.cultivation;}
	virtual int GetObjectType() { return TYPE_SUMMON;}
	virtual bool IsInvisibleNPC(){ return _is_invisible;}
	virtual void SpyTrap();
	
	void InitFromMaster(gplayer_imp * pImp);
	void SetAttackHook(attack_judge judge1,enchant_judge judge2, attack_fill fill1, enchant_fill fill2);
	void AddAggroToEnemy(const XID & who,int rage);
	void SetLifeTime(int lifetime); 

};

#define SUMMON_FOLLOW_RANGE_LIMIT 80.0f

class gsummon_policy : public ai_policy
{
	bool 		_is_clone;		//是否是分身
	int 		_pathfind_result;	//寻路失败的记数
	int		_aggro_state;		
	int 		_move_state;	
	A3DVECTOR 	_stay_pos;
	CChaseInfo 	_chase_info;
	std::vector<int> _auto_skills;


private:
	void RelocateSummonPos(bool disappear = false);	//要求主人重新定位怪物的位置 
	bool GatherTarget();

public:
	DECLARE_SUBSTANCE(gsummon_policy);

	gsummon_policy();

	virtual void ChangeAggroState(int);
	virtual void ChangeStayMode(int);
	virtual void SetCloneState(bool); 
	virtual void UpdateChaseInfo(const CChaseInfo * pInfo);
	virtual void FollowMasterResult(int reason);
	virtual void OnHeartbeat();
	virtual void RollBack();
	virtual int AddSummonSkillTask(int skill_id, int skill_level, const XID& target);
};

#endif
