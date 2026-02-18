#ifndef __ONLINEGAME_GS_FILTER_H__
#define __ONLINEGAME_GS_FILTER_H__

#include <ASSERT.h>
#include <common/base_wrapper.h>
#include "attack.h"
#include "property.h"
#include "substance.h"
#include "obj_interface.h"

class filter : public substance
{
protected:
	int _timeout_t;
	bool _active;
	int _mask;	//处理那种内容
	int _filter_id;	//filter 的ID
	bool _is_deleted;	//是否标记为删除
	object_interface _parent;
	bool _attach;
	int _buff_id;

	bool IsDeleted() { return _is_deleted;}

public:
DECLARE_SUBSTANCE(filter);
	enum
	{	
		FILTER_MASK_TRANSLATE_SEND_MSG  	= 0x00000001,
		FILTER_MASK_TRANSLATE_RECV_MSG		= 0x00000002,
		FILTER_MASK_HEARTBEAT			= 0x00000004,
		FILTER_MASK_ADJUST_DAMAGE		= 0x00000008,
		FILTER_MASK_BEFORE_DAMAGE		= 0x00000010,	
		FILTER_MASK_ADJUST_EXP			= 0x00000020,
		FILTER_MASK_ADJUST_MANA_COST		= 0x00000040,
		FILTER_MASK_BEFORE_DEATH		= 0x00000080,
		FILTER_MASK_TRANSLATE_ENCHANT 		= 0x00000100,
		FILTER_MASK_TRANSLATE_SEND_ENCHANT	= 0x00000200,
		FILTER_MASK_BE_ATTACKED			= 0x00000400,
		FILTER_MASK_BE_REFLECTED		= 0x00000800,		//技能反弹
		FILTER_MASK_ON_FILTER_ADD		= 0x00001000,           //玩家被加上某种状态
		FILTER_MASK_CRITENEMY			= 0x00002000,
		FILTER_MASK_DAMAGEENEMY			= 0x00004000,
		FILTER_MASK_ADJUST_SKILL		= 0x00008000,
		FILTER_MASK_AFTER_DEATH			= 0x00010000,
		FILTER_MASK_ON_MOVED			= 0x00020000,

		FILTER_MASK_REMOVE_ON_DEATH		= 0x00200000,		//死亡时会被自动删除
		FILTER_MASK_MERGE			= 0x00400000,		//如果发现了一致的filter_id  会试图融合
		FILTER_MASK_SKILL2			= 0x00800000,
		FILTER_MASK_SKILL3			= 0x01000000,
		FILTER_MASK_SAVE_DB_DATA		= 0x02000000,		//是否需要保存到数据库中
		FILTER_MASK_NOSAVE			= 0x04000000,		//转移服务器时不保存
		FILTER_MASK_BATTLE_BUFF			= 0x08000000,		//战场BUFF
		FILTER_MASK_DEBUFF			= 0x10000000,
		FILTER_MASK_BUFF			= 0x20000000,
		FILTER_MASK_UNIQUE			= 0x40000000,		//覆盖原来所有相同filter_id  filter
		FILTER_MASK_WEAK			= 0x80000000,		//如果原来有相同的filter_id  则不进行加入
	};
	int GetMask() { return _mask;}
	int GetFilterID() { return _filter_id;}
	virtual bool Save(archive & ar)
	{
		ar << _mask << _filter_id << _active << _is_deleted << _attach;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ar >> _mask >> _filter_id >> _active >> _is_deleted << _attach;
		return true;
	}
	void MergeMask(int mask) { _mask  |= mask; }

	inline bool IsActived() { return _active; }   

	virtual int OnGetTimeOut() {return 0;} //本filter是否timeout <=0的值表示不会消失

	inline int GetTimeout()
	{
		return OnGetTimeOut();
	}

	void Release() 
	{
		Detach();
		delete this;
	}

	virtual void Attach()
	{
		if(!_active)
		{
			OnAttach();
			if(_buff_id >= 0)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), 1);
			}
			_active = true;
		}
	}

	virtual void Detach()
	{       
		if(_active)
		{
			_active = false;                //在 OnDetach 之前赋值 这个顺序很重要
			if(_buff_id >= 0)
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
			OnDetach();
		}
	}
	virtual ~filter(){};

	void SetAttach() { _attach = true; }
	virtual char GetOverlayCnt() { return 0;} 
	virtual char GetMaxOverlayCnt() {return 0;}
	virtual bool CanOverlay() { return false; }
	virtual bool Overlay() { return true; }

protected:
	/*
	*	filter中将被调用的函数
	*	这些函数被调用后如果返回非0，则会在调用后被删除 释放
	*/
	virtual void TranslateSendAttack(const XID & target,attack_msg & msg, char type, float radius, int max_cnt){ASSERT(false);}	//在发出攻击消息之前处理一下消息内容, 0, Attack, 1, RegionAttack1, 2, RegionAttack2, 3, RegionAttack3, 4, MultiAttack
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg){ASSERT(false);}	//在接收到消息后首先处理一下
	virtual void TranslateSendEnchant(const XID & target,enchant_msg & msg){ASSERT(false);}//在发送enchant的消息之前
	virtual void TranslateRecvEnchant(const XID & attacker,enchant_msg & msg){ASSERT(false);}	//在接收到消息后首先处理一下
	virtual void Heartbeat(int tick){ASSERT(false);}			//在心跳时作处理,tick表示本次间隔几秒
	virtual void AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg){ASSERT(false);}		//在做伤害之前处理一下
	virtual void BeforeDamage(int & damage){ASSERT(false);}		//对最终的伤害产生的影响进行修正
	virtual void AdjustExp(int type, int & exp){ASSERT(false);}		//对经验值进行修正
	virtual void AdjustManaCost(int &mana){ASSERT(false);}			//在耗费mana前做修正
	virtual void BeforeDeath(bool is_duel){ASSERT(false);}				//在调用死亡的OnDeath前调用
	virtual void AfterDeath(bool is_duel){ASSERT(false);}				//在调用死亡的OnDeath后调用
	virtual void BeAttacked(const XID & target, int damage, bool crit){ASSERT(false);}
	virtual void BeReflected(const XID & target){ASSERT(false);}
	virtual void OnFilterAdd(int filter_id, const XID& caster) { ASSERT(false); }
	virtual void CritEnemy(const XID& target) { ASSERT(false); }
	virtual void DamageEnemy(const XID& target, int damage) { ASSERT(false); }
	virtual void AdjustSkill(int skill_id, short& level) { ASSERT(false); }
	virtual void OnMoved(){ASSERT(false);}				//在调用死亡的OnDeath后调用
	virtual void Merge(filter * f) { ASSERT(false);}
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;

	virtual void  OnModify(int ctrlname,void * ctrlval,size_t ctrllen) { }

	friend class filter_man;
	filter(object_interface parent,int mask):_timeout_t(0),_active(false),_mask(mask),_filter_id(0),_is_deleted(false),_parent(parent),_attach(false), _buff_id(-1)
	{}
	filter(){}
	inline void  Modify(int ctrlname,void * ctrlval,size_t ctrllen) 
	{
		OnModify(ctrlname,ctrlval,ctrllen);
	}
	inline void ClearMask(int xmask)
	{
		_mask &= ~xmask;
	}

protected:
	virtual bool ResetTimeout(); //这个超时只能延长，不能提前结束 提前结束用is_deleted
	int GetTimeLeft();
};

class timeout_filter : public filter
{
protected:
	int _time;
	int _timeout;
	bool _can_overlay;
	int  _max_overlay_cnt;
	int  _overlay_cnt;

	timeout_filter(object_interface object,int timeout,int mask)
		:filter(object,mask),_time(timeout),_timeout(timeout),_can_overlay(false),_max_overlay_cnt(1), _overlay_cnt(1)
	{
		ASSERT(mask & FILTER_MASK_HEARTBEAT);
	}

	virtual ~timeout_filter(){}

	virtual bool Save(archive & ar)
	{
		filter::Save(ar);
		ar << _timeout;
		ar << _can_overlay;
		ar << _max_overlay_cnt;
		ar << _overlay_cnt;
		ar << _attach;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		filter::Load(ar);
		ar >> _timeout;
		ar >> _timeout;
		ar >> _can_overlay;
		ar >> _max_overlay_cnt;
		ar >> _overlay_cnt;
		ar >> _attach;
		return true;
	}

	timeout_filter(){}

	virtual int OnGetTimeOut()
	{
		return _timeout;
	}

	virtual bool ResetTimeout();
	virtual void OnModify(int ctrlname,void * ctrlval,size_t ctrllen);

	virtual void Attach()
	{
		if(!_active)
		{
			OnAttach();
			_active = true;
			if(_buff_id >= 0 && _overlay_cnt > 0)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			}
		}
	}

	virtual void Detach()
	{       
		if(_active)
		{
			_active = false;                //在 OnDetach 之前赋值 这个顺序很重要
			OnDetach();
			if(_buff_id >= 0)
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
		}
	}

public:

	virtual bool CanOverlay()
	{
		return _can_overlay;
	}
	
	virtual char GetOverlayCnt()
	{
		return _overlay_cnt;
	}

	virtual char GetMaxOverlayCnt()
	{
		return _max_overlay_cnt;
	}
 
	virtual bool Overlay();

protected:
	virtual void Heartbeat(int tick)
	{
		_timeout -= tick;
		if(_timeout <=0) _is_deleted = true;
	}
};

//for filter_man
enum
{
	FILTER_IDX_TRANSLATE_SEND_MSG 	,
	FILTER_IDX_TRANSLATE_RECV_MSG	,
	FILTER_IDX_HEARTBEAT		,
	FILTER_IDX_ADJUST_DAMAGE	,
	FILTER_IDX_BEFORE_DAMAGE	,
	FILTER_IDX_ADJUST_EXP		,
	FILTER_IDX_ADJUST_MANA_COST	,
	FILTER_IDX_BEFORE_DEATH		,
	FILTER_IDX_TRANSLATE_ENCHANT	,
	FILTER_IDX_TRANSLATE_SEND_ENCHANT,
	FILTER_IDX_BE_ATTACKED		,
	FILTER_IDX_BE_REFLECTED		,
	FILTER_IDX_ON_FILTER_ADD	,
	FILTER_IDX_CRIT_ENEMY		,
	FILTER_IDX_DAMAGE_ENEMY		,
	FILTER_IDX_ADJUST_SKILL		,
	FILTER_IDX_AFTER_DEATH		,
	FILTER_IDX_ON_MOVED		,

	FILTER_IDX_MAX	
};

#endif

