#ifndef __ONLINE_GAME_GS_PET_NPC_H__
#define __ONLINE_GAME_GS_PET_NPC_H__

//这里整块都要从完美世界的宠物代码里进行移植操作
#include "npc.h"
#include "aipolicy.h"
#include "petman.h"
#include "cooldown.h"
#include "item/item_petbedge.h"

class gplayer_imp;
class pet_bedge_enhanced_essence;

class gpet_dispatcher: public gnpc_dispatcher
{
public: 
	virtual void sync_pet_prop(int cs_index, int uid, int sid, int pet_index, q_extend_prop & prop);
	DECLARE_SUBSTANCE(gpet_dispatcher);
};

class gpet_imp : public  gnpc_imp
{
	friend class gpet_policy;
private:
	typedef abase::static_multimap<int, int, abase::fast_alloc<> >  ENEMY_LIST;
	int _hp_notified;
	int _vp_notified;
	int _notify_master_counter;
	size_t _pet_index;
	int _pet_stamp;
	int _pet_tid;
	int _honor_level;
	int _hunger_level;
	int _damage_reduce;
	int _reborn_cnt;
	float _damage_reduce_percent;
	attack_judge _attack_judge;
	enchant_judge _enchant_judge;
	attack_fill   _attack_fill;
	enchant_fill  _enchant_fill;
	ENEMY_LIST _enemy_list;
	
	leader_prop  _leader_data;
	cd_manager _cooldown;

	char	_leader_force_attack;
	char	_aggro_state;		//三种仇恨状态  0 被动 1 主动 2 发呆
	char	_stay_state;		//两种跟随方式: 0 跟随，1　停留
	char    _old_combat_state;      //原来的combat state
	int	_master_attack_target;
	int 	_peep_counter;

	struct
	{
		int skill;
		int level;
		bool auto_cast;
	} _skills[pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT];
public:
	inline void SetPetStamp(int stamp)
	{
		_pet_stamp = stamp;
	}

	inline void SetPetIndex(size_t pet_index)
	{
		_pet_index = pet_index;
	}

	inline void SetTID(int tid)
	{
		_pet_tid = tid;
	}

	inline void AddSkill(int skillid,int level)
	{
		for(size_t i = 0;i < pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT;++i)
		{
			if(_skills[i].skill == 0)
			{       
				_skills[i].skill = skillid;
				_skills[i].level = level;
				_skills[i].auto_cast = false;
				break;
			}
		}
	}

	inline int GetSkillLevel(int skillid)
	{
		int i = 0;
		for( i = 0; i <pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT; ++i )
		{
			if(_skills[i].skill == skillid) return _skills[i].level;
		}
		return -1;
	}

	inline void ClearSkill()
	{
		memset(_skills, 0, sizeof(_skills));
	}

	enum
	{
		PET_AGGRO_DEFENSE = 0,
		PET_AGGRO_AUTO = 1,
		PET_AGGRO_PASSIVE = 2,
	};

	enum
	{
		PET_MOVE_FOLLOW = 0,
		PET_STAY_STAY = 1,
	};

	DECLARE_SUBSTANCE(gpet_imp);
public:
	gpet_imp();
	virtual void Init(world * pPlane,gobject*parent);
	bool CheckCoolDown( int cooldown_index );
	void SetCoolDown( int cooldown_index, int cooldown_time );
	void ClrCoolDown(int idx);
	void NotifySkillStillCoolDown();
	virtual void Die( const XID& attacker, bool is_pariah, char attacker_mode );
	virtual void OnDeath( const XID & attacker, bool is_invader, bool bduel );
	virtual int MessageHandler(const MSG & msg);
	virtual void OnHeartbeat(size_t tick);
	virtual void DispatchPlayerCommand(int target, const void * buf, size_t size);
	virtual void RelocatePos(bool is_disappear); 
	void AddAggroToEnemy(const XID & who,int rage);
	void AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg);
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
	virtual bool OI_IsFlying() { return _reborn_cnt > 0 && _leader_data.is_flying; }
	virtual void FillAttackMsg(const XID & target, attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target,enchant_msg & enchant);
	virtual void NPCEndSkill(unsigned int skill_id,int level,const XID & target);

	void SetAttackHook(attack_judge judge1,enchant_judge judge2, attack_fill fill1, enchant_fill fill2);
	void InitFromMaster(gplayer_imp * pImp);
	void SetHonorLevel(int honor_level);
	void SetHungerLevel(int hunger_level);
	void SetAggroState(int aggro_state);
	void SetStayState(int stay_state);
	void SetRebornCnt(int reborn_cnt){_reborn_cnt = reborn_cnt;}
	void LoadCoolDown(void * cd_buf, size_t cd_len);

	void NotifySelfDataToMaster();
	void OI_UpdateAllProp();
	void OI_UpdataAttackData();
	virtual int OI_GetCultivation() { return _leader_data.cultivation;}
	virtual int GetObjectType() { return TYPE_PET;}
	virtual int OI_GetTemplateID(){return 0;}
};

class gpet_policy : public ai_policy
{
	CChaseInfo 	_chase_info;
	int 		_pathfind_result;	//寻路失败的记数
	int		_aggro_state;		//1代表主动寻敌
	int 		_stay_state;		//0 跟随 1 停留
	std::vector<int> _auto_skills;
	A3DVECTOR 	_stay_pos;

private:
	void RelocatePetPos(bool disappear = false);	//要求宠物主人重新定位宠物的位置 
	bool GatherTarget();
public:
	DECLARE_SUBSTANCE(gpet_policy);

	gpet_policy();

	void AddPetPrimaryTask(const XID & target, int strategy);
	virtual void UpdateChaseInfo(const CChaseInfo * pInfo);
	virtual void FollowMasterResult(int reason);
	virtual void ChangeAggroState(int);
	virtual void ChangeStayMode(int);
	virtual int GetInvincibleTimeout();
public:
	
	virtual void OnHeartbeat();
	virtual void DeterminePolicy(const XID & target);
	virtual void RollBack();
	void SetPetAutoSkill(int skill_id,int set_flag);
	bool CheckCoolDown( int cooldown_index );
	void SetCoolDown( int cooldown_index, int cooldown_time );
	int AddPetSkillTask(int skill_id,int& target);
	int AddAutoCastSkill( XID target );
};

#endif

