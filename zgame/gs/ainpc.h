#ifndef __ONLINEGAME_NPC_AI_H__
#define __ONLINEGAME_NPC_AI_H__

#include "amemobj.h"
#include <common/message.h>
#include "config.h"
#include "aipolicy.h"
#include "aiaggro.h"

class gnpc_controller;


/**
	这个接口是npc对象实现的一些封装,是一个proxy类
	主要供ai策略进行数据取用和操作
*/
class  ai_npcobject : public ai_actobject
{
	gnpc_controller *_ctrl;
	aggro_policy *_aggro;
	int _hate_count;
public:
	ai_npcobject(gactive_imp * imp,gnpc_controller * ctrl,aggro_policy * aggro)
			:ai_actobject(imp),_ctrl(ctrl),_aggro(aggro),_hate_count(0)
	{}

	//destructor
	virtual ai_object * Clone() const
	{
		return new ai_npcobject(*this);
	}
	virtual ~ai_npcobject()
	{
	}

public:
	//ai control
	virtual float GetIgnoreRange()
	{
		return _aggro->GetAggroRange();
	}

	virtual const XID & GetLeaderID();
	virtual void  SetLeaderID(const XID & leader_id);
	
public:
	//property
	virtual int GetState();

	//aggro operation
	virtual size_t GetAggroCount()
	{
		return _aggro->Size();
	}
	
	virtual void ClearAggro()
	{
		return _aggro->Clear();
	}

	virtual void SetAggroWatch(bool bVal)
	{
		_aggro->SetAggroWatch(bVal);
	}

	virtual int GetAggroEntry(size_t index, XID & id)
	{
		return _aggro->GetEntry(index, id);
	}
	
	virtual int GetFirstAggro(XID & id) 
	{
		return _aggro->GetFirst(id);
	}
	
	virtual void RemoveAggroEntry(const XID & id)
	{
		return _aggro->Remove(id);
	}

	virtual float GetSightRange();
	
	virtual void ChangeAggroEntry(const XID & id, int rage)
	{
		_aggro->AddAggro(id,rage);
	}

	virtual void RegroupAggro()
	{
		_aggro->RegroupAggro();
	}

	virtual void SwapAggro(int index1,int index2)
	{
		_aggro->SwapAggro(index1,index2);
	}

	virtual void BeTaunted(const XID & target)
	{
		_aggro->BeTaunted(target);
	}

	virtual void FadeTarget(const XID & target)
	{
		_aggro->FadeTarget(target);
	}
	
	virtual void AggroFade()
	{
		_aggro->AggroFade();
	}

	virtual void ForwardFirstAggro(const XID & id,int rage);
	virtual void HateTarget(const XID & target)
	{
		if(!_hate_count) SendMessage(target,GM_MSG_HATE_YOU);
		_hate_count = (_hate_count + 1) & 0x01;
	}

	virtual void ClearDamageList() 
	{
		((gnpc_imp*)_imp)->ClearDamageList();
	}
	
	virtual const A3DVECTOR & GetBirthPlace()
	{
		return ((gnpc_imp*)_imp)->_birth_place;
	}

	virtual int GetInitPathID()
	{
		return ((gnpc_imp*)_imp)->GetInitPathID();
	}
	
	virtual int GetInitPathType()
	{
		return ((gnpc_imp*)_imp)->GetInitPathType();
	}

	virtual bool IsReturnHome(A3DVECTOR & pos, float offset_range);
	virtual void BeHurt(int hp);
	virtual void ReturnHome(const A3DVECTOR & pos,float range);
	virtual void AddInvincibleFilter(const XID & who);
	virtual bool CanRest();
	virtual bool IsInIdleHeartbeat();
	virtual void GetPatrolPos(A3DVECTOR & pos);
	virtual void GetPatrolPos(A3DVECTOR & pos,float range);
	virtual bool CheckWorld();
	virtual void Say(const char * msg);
	virtual void Say(const XID & target, const void * msg, size_t size, int level);
	virtual void BattleFactionSay(const void * msg, size_t size);
	virtual void BattleSay(const void * msg, size_t size);
	virtual void BroadcastSay(const void * msg, size_t size, char channel);
	virtual void SendClientTurretNotify(int id);
	virtual void RelocatePos(bool is_disappear);
	virtual int GetSealMode();
	virtual bool IsDisappearLifeExpire();

};


class gnpc_ai : public abase::ASmallObject 
{
protected:
	gnpc_controller * _commander;
	aggro_policy	* _aggro;
	ai_policy	* _core;
	float _squared_sight_range;
	float _sight_range;
	int   _faction_ask_help;
	int   _faction_accept_help;
public:
	gnpc_ai():_commander(0),_aggro(NULL),_core(NULL),_squared_sight_range(0.f),_faction_ask_help(0){}

	bool Init(gnpc_controller * pControl,const aggro_param & aggp, const ai_param & aip);
	//int policy_class,int primary_strategy);
	virtual ~gnpc_ai()
	{
		if(_core) delete _core;
		if(_aggro) delete _aggro;
	}

	float GetSightRange() {return _sight_range;}

	void Heartbeat();
	void OnBorn()
	{
		_core->OnBorn();
	}
	void OnDeath(const XID & attacker)
	{
		_aggro->OnDeath(attacker);
		_core->OnDeath(attacker);
	}
	
	void OnSkillAttack(int id)
	{
		_core->OnSkillAttack(id);
	}
	
	inline aggro_policy * GetAggroCtrl()
	{	
		return _aggro;
	}

	inline ai_policy * GetAICtrl()
	{
		return _core;
	}
	inline gnpc_controller * GetCommander()
	{
		return _commander;
	}
	void SetIdleMode(bool isIdle)
	{
		_core->SetIdleMode(isIdle);
	}

	void SetSealMode(int seal_flag)
	{
		_core->SetSealMode(seal_flag);
	}


	inline int GetFactionAskHelp()
	{
		return _faction_ask_help;
	}

	inline void SetLife(int life)
	{
		_core->SetLife(life);
	}

	inline void SetDieWithLeader(bool val)
	{
		_core->SetDieWithLeader(val);
	}
public:
	bool Save(archive & ar);
	bool Load(archive & ar);
	void ReInit(gnpc_controller * pControl)
	{
		_commander = pControl;
		if(_core)
		{
			_core->ReInit(ai_npcobject((gactive_imp*)_commander->_imp,_commander,_aggro));
		}
	}
	
	void SessionStart(int task_id, int session_id)
	{
		_core->SessionStart(task_id,session_id);
	}

	void SessionEnd(int task_id, int session_id, int retcode)
	{
		_core->SessionEnd(task_id,session_id,retcode);
	}
	bool GetNextWaypoint(A3DVECTOR & pos)
	{
		return _core->GetNextWaypoint(pos);
	}
	void SessionUpdateChaseInfo(int task_id,const void * buf ,size_t size)
	{
		_core->SessionUpdateChaseInfo(task_id, buf, size);
	}
	
public:
	void SessionTerminate()
	{
	}

	void RefreshAggroTimer(const XID & target)
	{
		_aggro->RefreshTimer(target);
	}

	void Reborn()
	{
		_aggro->Clear();
		_core->Reborn();
	}
	
	
//仇恨度函数转发
	void AddAggro(const XID & id, int rage)
	{
		if(_aggro->AddAggro(id,rage) == 0) //第一位的才调用OnAggro
		{
			_core->OnAggro();
		}
	}
	
	void AggroGen(const MSG& msg)
	{
		int rst;
		if((rst =_aggro->AggroGen(msg)) > 2 && _faction_ask_help)
		{
			//需要进行求救，考虑是否进行
			_commander->CryForHelp(msg.source,_faction_ask_help,_sight_range);
		}
		_core->OnAggro();
	}

	inline void TryCryForHelp(const XID & attacker)
	{
		if(_faction_ask_help)
		{
			_commander->CryForHelp(attacker,_faction_ask_help,_sight_range);
		}
	}

	
	void AggroGen(const XID & who, int rage)
	{
		int rst;
		if((rst =_aggro->AggroGen(who,rage)) > 2 && _faction_ask_help)
		{
			//需要进行求救，考虑是否进行
			_commander->CryForHelp(who,_faction_ask_help,_sight_range);
		}
		_core->OnAggro();
	}

	void BeTaunted(const XID & who,int second)
	{
		/*
		int rst;
		if((rst =_aggro->BeTaunted(who,rage)) > 2 && _faction_ask_help)
		{
			//需要进行求救，考虑是否进行
			_commander->CryForHelp(who,_faction_ask_help,_sight_range);
		}
		_core->OnAggro();
		*/
		_aggro->BeTaunted(who,second);
		_core->OnAggro();
	}
	
	void AggroWatch(const MSG & msg)
	{
		if( !_aggro->Size() && 
				_commander->_imp->_parent->pos.squared_distance(msg.pos) < _squared_sight_range)
		{
			if(_aggro->AggroWatch(msg)) 
			{
				if(_faction_ask_help)
				{
					_commander->CryForHelp(msg.source,_faction_ask_help,_sight_range);
				}
				_core->OnAggro();
			}
		}
	}

	void AggroHelp(const XID & attacker, int lamb_faction)
	{
		if((_faction_accept_help & lamb_faction) &&
				_aggro->Size() <= 1)
		{
			//最大只加两点仇恨
			if(_aggro->AddAggro(attacker,2,2) == 0)
			{
				_core->OnAggro();
			}
		}
	}
	
	void AggroTransfer(const MSG & msg)
	{
		_aggro->AggroTransfer(msg);
		_core->OnAggro();
	}
	
	void AggroAlarm(const MSG & msg)
	{
		ASSERT(msg.content_length == sizeof(msg_aggro_alarm_t));
		msg_aggro_alarm_t * agg = (msg_aggro_alarm_t*)msg.content;
		if((_faction_accept_help & agg->target_faction) && (agg->faction & _aggro->_enemy_faction))
		{
			_aggro->AggroGen(agg->attacker, agg->rage);
			_core->OnAggro();
		}
	}

	void AggroWakeUp(const MSG & msg)
	{
		_aggro->AggroWakeUp(msg);
	}
	
	void AggroTest(const MSG & msg)
	{
		_aggro->AggroTest(msg);
	}

	void AggroClear(const XID & id)
	{
		_aggro->Remove(id);
		_core->OnAggro();
	}

	void KillTarget(const XID & target)
	{
		_core->KillTarget(target);
	}

	bool ChangeTurretMaster(const XID & master)
	{
		return _core->ChangeTurretMaster(master);
	}

	void SetFastRegen(bool b)
	{
		return _core->SetFastRegen(b);
	}
};

#endif
