#ifndef __ONLINEGAME_GS_AI_AGGRO_H__
#define __ONLINEGAME_GS_AI_AGGRO_H__

#include "config.h"
#include "aggrolist.h"
#include <ASSERT.h> 

enum
{
	AGGRO_VALUE_WATCH	= 1,
	AGGRO_VALUE_OTHER	= 2,
	AGGRO_VALUE_NORMAL	= 3,

};

enum
{
	AGGRO_POLICY_NULL 	= 0,
	AGGRO_POLICY_BOSS	= 1,
	AGGRO_POLICY_BOSS_MINOR	= 2,
	AGGRO_POLICY_PET	= 3,
	AGGRO_POLICY_TURRET	= 4,
	AGGRO_POLICY_MOBS	= 5, 
	AGGRO_POLICY_SUMMON	= 6, 

};

struct  aggro_param
{	
	int   aggro_policy;
	float aggro_range;
	float sight_range;
	int   aggro_time;
	int   enemy_faction;
	int   faction_ask_help;
	int   faction_accept_help;
	int   faction;
};

/*
	aggro_policy 负责控制仇恨策略，它提供了一个仇恨控制的接口
*/
class aggro_policy : public substance 
{
protected:
	aggro_list _alist;
	float _aggro_range;
	int _aggro_time;	//以秒为单位
	int _cur_time;
	int _enemy_faction;
	int _faction;
	int _aggro_state;
	bool _aggro_watch;
	friend class gnpc_ai;
public:
	enum 
	{
		STATE_NORMAL,
		STATE_FREEZE,
	};
public:
	bool Save(archive & ar)
	{
		ar << _aggro_range << _aggro_time << _cur_time << _enemy_faction << _faction << _aggro_state << _aggro_watch;
		size_t size = _alist.Size();
		ar << size;
		for(size_t i=0; i < size; i ++)
		{
			XID target;
			int rage = _alist.GetEntry(i,target);
			ar << target.type << target.id << rage;
		}
		return true;
	}
	
	bool Load(archive & ar)
	{
		ar >> _aggro_range >> _aggro_time >> _cur_time >> _enemy_faction >> _faction >> _aggro_state >> _aggro_watch;
		size_t size;
		ASSERT(_alist.Size() == 0);
		_alist.Clear();
		ar >> size;
		for(size_t i=0; i < size; i ++)
		{
			XID target;
			int rage;
			ar >> target.type >> target.id >> rage;
			_alist.AddRage(target,rage);
		}
		return true;
	}

DECLARE_SUBSTANCE(aggro_policy);
	aggro_policy():_alist(MAX_AGGRO_ENTRY),_aggro_range(30.f),_aggro_time(20),_cur_time(0),_enemy_faction(0),_faction(0),_aggro_state(0),_aggro_watch(true){}
	virtual ~aggro_policy(){}
	void Init(const aggro_param & aggp)
	{
		_aggro_range = aggp.aggro_range;
		_aggro_time = aggp.aggro_time;
		_enemy_faction = aggp.enemy_faction;
		_faction = aggp.faction;
	}

	void SetAggroState(int state)
	{
		_aggro_state = state;
	}

	void SetAggroWatch(bool bVal)
	{
		_aggro_watch = bVal;
	}

	float GetAggroRange() { return _aggro_range;}

	virtual bool GetFirst(XID & target)
	{
		return _alist.GetFirst(target);
	}
	
	virtual int GetEntry(int index, XID & target)
	{
		return _alist.GetEntry(index,target);
	}

	void GetAll(abase::vector<XID> & list)
	{
		return _alist.GetAll(list);
	}

	size_t Size()
	{
		return _alist.Size();
	}

	void Clear()
	{
		_cur_time = 0;
		return _alist.Clear();
	}

	
	bool IsEmpty(){ return _alist.IsEmpty();}

	void RefreshTimer(const XID & target)
	{
		if(_alist.IsFirst(target))
		{
			_cur_time = _aggro_time;
		}
	}

	void RegroupAggro()
	{
		_alist.RegroupAggro();
	}

	void SwapAggro(size_t index1,size_t index2)
	{
		_alist.SwapAggro(index1,index2);
	}

	void BeTaunted(const XID & target)
	{
		BeTaunted(target,4);
	}

	void FadeTarget(const XID & target)
	{
		_alist.AddToLast(target);
	}

	void AggroFade()
	{
		_alist.Fade();
	}

public:
	virtual void Remove(const XID & id)
	{
		if(_alist.Remove(id) == 0)
		{
			_cur_time = _aggro_time;
		}
	}

	virtual int AddAggro(const XID & id, int rage)
	{
		return _alist.AddRage(id,rage);
	}

	virtual int AddAggro(const XID & id, int rage, int max_rage)
	{
		return _alist.AddRage(id,rage,max_rage);
	}
	
	virtual void OnDeath(const XID & attacker)
	{
		Clear();
	}

	virtual int AggroGen(const XID &who , int rage)
	{
		if(_alist.AddRage(who,rage) == 0)
		{
			_cur_time = _aggro_time;
		}
		return rage;
	}
	
	virtual int BeTaunted(const XID & who , int second) = 0;
	
	virtual int AggroGen(const MSG& msg)
	{
		ASSERT(msg.content_length == sizeof(msg_aggro_info_t));
		ASSERT(msg.source.type != -1);
		if(msg.content_length != sizeof(msg_aggro_info_t)) return 0;
		const msg_aggro_info_t * pInfo = (const msg_aggro_info_t *)msg.content;
		ASSERT(msg.source == pInfo->source);
		if(_alist.AddRage(pInfo->source,pInfo->aggro) == 0)
		{
			_cur_time = _aggro_time;
		}
		return pInfo->aggro;
	}

	virtual void OnHeartbeat()
	{
		if(_cur_time)
		{
			if(--_cur_time <= 0)
			{
				//.....
				_alist.RemoveFirst();
				//这是是否应该处理一下？？？
				XID target;
				if(_alist.GetFirst(target))
					_cur_time = _aggro_time;
				else
				{
					_cur_time = 0;
					Clear();
				}
			}
		}
	}
	
	virtual bool AggroWatch(const MSG & msg)
	{
		if(!_aggro_watch) return false;
		ASSERT(msg.content_length == sizeof(msg_watching_t));
		if(msg.content_length != sizeof(msg_watching_t)) return false;
		const msg_watching_t * pInfo = (const msg_watching_t *)msg.content;
		if(_alist.IsEmpty() && (pInfo->faction & _enemy_faction))
		{
			_alist.AddRage(msg.source,1,1);
			_cur_time = _aggro_time;
			return true;
		}
		return false;
	}


	virtual void AggroTransfer(const MSG & msg)
	{
		//do nothing
	}
	
	virtual void AggroAlarm(const MSG & msg)
	{
		
		//do nothing
	}
	virtual void AggroWakeUp(const MSG & msg)
	{
		//do nothing
	}
	
	virtual void AggroTest(const MSG & msg)
	{
		//do nothing
	}
};

class pet_aggro_policy : public aggro_policy 
{
public:

DECLARE_SUBSTANCE(pet_aggro_policy);

	virtual int AddAggro(const XID & id, int rage)
	{
		switch(_aggro_state)
		{
			case STATE_NORMAL:
				return aggro_policy::AddAggro(id,rage);
			case STATE_FREEZE:
				return 0;
		}
		return 0;
	}

	virtual int AddAggro(const XID & id, int rage, int max_rage)
	{
		switch(_aggro_state)
		{
			case STATE_NORMAL:
				return aggro_policy::AddAggro(id,rage,max_rage);
			case STATE_FREEZE:
				return 0;
		}
		return 0;
	}

	virtual int AggroGen(const XID &who , int rage)
	{
		switch(_aggro_state)
		{
			case STATE_NORMAL:
				return aggro_policy::AggroGen(who,rage);
			case STATE_FREEZE:
				return 0;
		}
		return 0;
	}
	
	virtual int BeTaunted(const XID & who , int second)
	{
		switch(_aggro_state)
		{
			case STATE_NORMAL:
				//return aggro_policy::BeTaunted(who,rage);
			case STATE_FREEZE:
				return 0;
		}
		return 0;
	}
	
	virtual int AggroGen(const MSG& msg)
	{
		switch(_aggro_state)
		{
			case STATE_NORMAL:
				return aggro_policy::AggroGen(msg);
			case STATE_FREEZE:
				return 0;
		}
		return 0;
	}

	virtual void OnHeartbeat()
	{
		//仇恨永远不衰减....
		//不过距离参数会生效的
	}
	
	virtual bool AggroWatch(const MSG & msg)
	{
		switch(_aggro_state)
		{
			case STATE_NORMAL:
				return aggro_policy::AggroWatch(msg);
			case STATE_FREEZE:
				return 0;
		}
		return false;
	}

};

class turret_aggro_policy : public aggro_policy 
{
public:

DECLARE_SUBSTANCE(turret_aggro_policy);
	turret_aggro_policy(){}

	virtual ~turret_aggro_policy(){}

public:
	virtual int AddAggro(const XID & id, int rage)
	{
		if(id.IsPlayerClass()) return 0;
		return aggro_policy::AddAggro(id,rage);
	}

	virtual int AddAggro(const XID & id, int rage, int max_rage)
	{
		if(id.IsPlayerClass()) return 0;
		return aggro_policy::AddAggro(id,rage,max_rage);
	}

	virtual int AggroGen(const XID &who , int rage)
	{
		if(who.IsPlayerClass()) return 0;
		return aggro_policy::AggroGen(who,rage);
	}
	
	virtual int BeTaunted(const XID & who , int second)
	{
		return 0;
	}
	
	virtual int AggroGen(const MSG& msg)
	{
		if(msg.source.IsPlayerClass()) return 0;
		return aggro_policy::AggroGen(msg);
	}

	virtual bool AggroWatch(const MSG & msg)
	{
		return 0;
	}
};

class aggro_mobs_policy : public aggro_policy 
{
public:
DECLARE_SUBSTANCE(aggro_mobs_policy);

public:
	virtual int AddAggro(const XID & id, int rage)
	{
		if(_alist.IsEmpty())
			return _alist.AddRage(id,rage);
		else if(rage >= AGGRO_VALUE_NORMAL)
		{
			if(_alist.GetRage(0) < AGGRO_VALUE_NORMAL)
			{
				_alist.Clear();
				return _alist.AddRage(id,rage);
			}
			if(_alist.GetID(0) == id) return 0;
		}
		return -1;
	}

	virtual int AddAggro(const XID & id, int rage, int max_rage)
	{
		return AddAggro(id, rage);
	}
	
	virtual void OnDeath(const XID & attacker)
	{
		Clear();
	}

	virtual int AggroGen(const XID &who , int rage)
	{
		if(AddAggro(who, rage) >= 0)
		{
			_cur_time = _aggro_time;
		}
		return rage;
	}
	
	virtual int BeTaunted(const XID & who , int second)
	{
		if(_alist.IsEmpty()) 
		{
			_alist.AddRage(who,1024);
		}
		else
		{
			_alist.Clear();
			_alist.AddRage(who,1024);
		}
		_cur_time = _aggro_time;
		return 1024;
	}
	
	virtual int AggroGen(const MSG& msg)
	{
		ASSERT(msg.content_length == sizeof(msg_aggro_info_t));
		ASSERT(msg.source.type != -1);
		if(msg.content_length != sizeof(msg_aggro_info_t)) return 0;
		const msg_aggro_info_t * pInfo = (const msg_aggro_info_t *)msg.content;
		ASSERT(msg.source == pInfo->source);
		if(AddAggro(pInfo->source,pInfo->aggro) >= 0)
		{
			_cur_time = _aggro_time;
		}
		return pInfo->aggro;
	}
	
	virtual bool AggroWatch(const MSG & msg)
	{
		if(!_aggro_watch) return false;
		ASSERT(msg.content_length == sizeof(msg_watching_t));
		if(msg.content_length != sizeof(msg_watching_t)) return false;
		const msg_watching_t * pInfo = (const msg_watching_t *)msg.content;
		if(_alist.IsEmpty() && (pInfo->faction & _enemy_faction))
		{
			AddAggro(msg.source,1);
			_cur_time = _aggro_time;
			return true;
		}
		return false;
	}
};
	
class aggro_boss_policy : public aggro_policy 
{
	XID _taunt_target; 
	int _taunt_timeout;
public:
DECLARE_SUBSTANCE(aggro_boss_policy);
	aggro_boss_policy():aggro_policy(),_taunt_target(-1,-1),_taunt_timeout(-1){}

	virtual bool GetFirst(XID & target)
	{
		if(_taunt_timeout >0)
		{
			target = _taunt_target; 
			return true;
		}
		return _alist.GetFirst(target);
	}
	
	virtual int GetEntry(int index, XID & target)
	{
		if(_taunt_timeout >0)
		{
			target = _taunt_target; 
			return 1024;
		}
		return _alist.GetEntry(index,target);
	}

	virtual int BeTaunted(const XID & who , int t)
	{
		AddAggro(who, AGGRO_VALUE_NORMAL);
		_taunt_timeout = t;
		_taunt_target = who;
		_cur_time = _aggro_time;
		return AGGRO_VALUE_NORMAL;
	}

	virtual void Remove(const XID & id)
	{
		if(_taunt_timeout >0)
		{
			if(_taunt_target == id)
			{
				_taunt_timeout = -1;
			}
		}
		if(_alist.Remove(id) == 0)
		{
			_cur_time = _aggro_time;
		}
	}
	virtual void OnHeartbeat()
	{
		if(_taunt_timeout > 0)
		{
			_taunt_timeout --;
		}
		aggro_policy::OnHeartbeat();
	}

	virtual void Clear()
	{
		_taunt_timeout = -1;
		aggro_policy::Clear();
	}
};

class aggro_minor_policy : public aggro_mobs_policy  
{
public:
	DECLARE_SUBSTANCE(aggro_minor_policy);

	virtual void AggroTransfer(const MSG & msg)
	{
		//do nothing
		ASSERT(msg.content_length == sizeof(XID));
		XID & id = *(XID*)msg.content;
		int rage = msg.param;
		if(id.type == -1 || id.id == -1)
		{
			_alist.Clear();
			_cur_time = 0;
		}
		else
		{
			_alist.Clear();
			_alist.AddRage(id,rage);
			_cur_time = _aggro_time;
		}
	}
};

class summon_aggro_policy : public aggro_policy 
{
public:

DECLARE_SUBSTANCE(summon_aggro_policy);

	virtual int BeTaunted(const XID & who , int second)
	{
		return 0;
	}

	virtual void OnHeartbeat()
	{
	}
};

#endif

