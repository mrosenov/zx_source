#ifndef _BASE_FILTER_NEW_34545_
#define _BASE_FILTER_NEW_34545_

#include "filter.h"
#include "buffcondition.h"
#include "sfilterdef.h"
#include "statedef.h"
#include "skillwrapper.h"   // for FEEDBACK_HIT / FEEDBACK_KILL enum

#pragma pack(1)

namespace GNET
{
enum {
	CTRL_MODIFY =   0, 		//叠加filter
	CTRL_BUFFLEVEL  =   1, 	//获取叠加次数
};

//下面两个enum要和actobject.h中一致
//enum {  
//	FEEDBACK_HIT        = 0x01, //反馈击中消息
//	FEEDBACK_KILL       = 0x02, //反馈杀人消息 
//};


enum
{
	BUFF_TYPE_AURA   = 0x01, // 光环
	BUFF_TYPE_SWITCH = 0x02, // 开关
	BUFF_TYPE_SETUPPARAM = 0x04,
	BUFF_TYPE_SETUPPARAM2 = 0x08,
	BUFF_TYPE_SETUPPARAM3 = 0x10, // 传送用
	BUFF_TYPE_SETUPPARAM4 = 0x20, // 生成物品用
	BUFF_TYPE_SETUPPARAM5 = 0x40, // 图腾
	BUFF_TYPE_SETUPPARAM6 = 0x80, // 根据技能设置buff, debuff mask
	BUFF_TYPE_SETUPPARAM7 = 0x100,

	BUFF_TYPE_PROP = 0x100,
	BUFF_TYPE_TRANS_TO_TARGET = 0x200,
};

enum
{
	FLAG_INSTANT	= 0x01,
	FLAG_BROADCAST  = 0x02, 
	FLAG_COMBAT_SENSITIVE = 0x04,
};

class Skill;
class base_filter : public filter
{
protected:
	BuffCondition* _hc;
	std::map<int, int> _maxcounts;
	std::map<int, int> _counts;
	std::map<int, int> _endparam;
	int _polycount;
	int _feedback_beginmask;
	int _feedback_endmask;
	int _timeout_begin_end;		//用于判断延时生效、延时结束的计时器
	int _flag;
	int _hstate;
	int _vstate;
	bool _is_actived;
	int _effect_times;			//生效次数,只在子类的TakeEffect中修改，注意只能生效一次的效果，该值不会大于1
	bool _do_attach;

	//技能中设置的buff参数
	float	_ratio;		// 状态攻击参数
	float	_amount;		// 状态攻击参数
	float	_value;		// 状态攻击参数
	int 	_time;		// 持续时间, 不会改变，等于0表示永久
	int     _buffid;
	unsigned int     _skillid;
	int    _bufftype;
	int _heartbeat_count_max;			//持续生效buff的生效次数 	
	int _heartbeat_count;
	XID	_target;		//附加技能的释放对象
	// 注意, 上述字段类型变更或者在此处增加存盘的字段, 要到gs/filter_man中提升filter的版本号, 并作不同版本读取差异处理

protected:

	bool IsInstant() const { return _flag & FLAG_INSTANT; }
	bool IsBroadcast() const { return _flag & FLAG_BROADCAST; }

	void SetAttach()
	{
		_do_attach = true;
	}

	void SetTime(int time)
	{
		_time = time;
	}

	int GetTime()
	{
		return _time;
	}

	void SetupMask(int condition, int& mask)
	{
		mask = 0;
		if(condition & EFFECT_HIT)  
			mask |= FEEDBACK_HIT;
		if(condition & EFFECT_KILL) 
			mask |= FEEDBACK_KILL;
		if(condition & EFFECT_VALID_KILL)   
			mask |= FEEDBACK_KILL;
		if(condition & EFFECT_INVALID_KILL) 
			mask |= FEEDBACK_KILL;
	}

	bool CheckBegin()
	{
		if (!_hc) return false;
		return (_hc->_begincondition & EFFECT_BEGIN) != 0;
	}

	void Activate()
	{
		TakeEffect();
		_is_actived = true;
	}

	void CheckActivate(int effect)
	{
		if (_maxcounts[effect] > 0)
		{
			_counts[effect] += 1;
			if (_counts[effect] >= _hc->_beginparam[effect])
			{
				Activate();
				_counts[effect] = 0;
				_maxcounts[effect] -= 1;
			}
		}
		else if(_maxcounts[effect] == -1) //无限生效
		{
			_counts[effect] += 1;
			if (_counts[effect] >= _hc->_beginparam[effect])
			{
				Activate();
				_counts[effect] = 0;
			}
		}

	}

	//bool HasFilter(int filter_id)
	//{
	//	if (filter_id < 0)		//如果策划填了负值，表示所有的10个buffid都要检查
	//	{
	//		filter_id = -filter_id;
	//		filter_id += FILTER_ID_BEGIN;
	//		return _parent.IsMultiFilterExist(filter_id, filter_id+9);
	//	}
	//	else
	//	{
	//		return _parent.IsFilterExist(filter_id+FILTER_ID_BEGIN);
	//	}
	//}

	//bool CheckBegin()
	//{
	//	if (_hc->_begincondition & EFFECT_BEGIN)
	//		return true;

	//	const basic_prop& bprop = _parent.GetBasicProp();
	//	const q_extend_prop& eprop = _parent.GetExtendProp();

	//	if (_hc->_begincondition & EFFECT_STATE)
	//	{
	//		int filter_id = _hc->_beginparam[EFFECT_STATE];
	//		return HasFilter(filter_id);
	//	}

	//	if ((_hc->_begincondition & EFFECT_THRESHOLD) == 0)
	//		return false;

	//	if (_hc->_begincondition & EFFECT_BELOW_HP)
	//	{
	//		if (bprop.hp < (int)(eprop.max_hp * 0.01f * _hc->_beginparam[EFFECT_BELOW_HP]))
	//			return true;
	//	}

	//	if (_hc->_begincondition & EFFECT_ABOVE_HP)
	//	{
	//		if (bprop.hp > (int)(eprop.max_hp * 0.01f * _hc->_beginparam[EFFECT_ABOVE_HP]))
	//			return true;
	//	}

	//	if (_hc->_begincondition & EFFECT_BELOW_MP)
	//	{
	//		if (bprop.mp < (int)(eprop.max_mp * 0.01f * _hc->_beginparam[EFFECT_BELOW_MP]))
	//			return true;
	//	}

	//	if (_hc->_begincondition & EFFECT_ABOVE_MP)
	//	{
	//		if (bprop.mp > (int)(eprop.max_mp * 0.01f * _hc->_beginparam[EFFECT_BELOW_MP]))
	//			return true;
	//	}

	//	return false;
	//}
	virtual bool CheckParentState() { return true; }
	bool CheckDeactivate(int effect)
	{
		_endparam[effect] -= 1;
		if (_endparam[effect] <= 0)
			return true;

		return false;
	}

	// 在子类中用SetBuffCondition(get_xxx(buffid))
	void SetBuffCondition(BuffCondition* phc, int mask)
	{
		// 强制不使用图标
		if (phc->_buffdesc == 0)
			_hstate = 0;
		// 使用策划配置图标
		else if (phc->_buffdesc > 0)
			_hstate = phc->_buffdesc;
		// else 其他情况就是使用程序中的指定的默认图标


		// 光效同上
		if (phc->_buffvisi == 0)
			_vstate = 0;
		else if (phc->_buffvisi > 0)
			_vstate = phc->_buffvisi;
		
			
		_hc = phc;
		_maxcounts = _hc->_maxcounts;
		_endparam = _hc->_endparam;
		_mask |= mask;
		_mask |= _hc->FILTER_MASK;
	}

public:
	DECLARE_SUBSTANCE(base_filter)
	base_filter():filter(object_interface(), 0),_hc(NULL),_polycount(0),_feedback_beginmask(0),_feedback_endmask(0),
	_timeout_begin_end(0),_flag(0),_hstate(0),_vstate(0),_is_actived(false),_effect_times(0),_do_attach(false),_ratio(0),_amount(0),_value(0),_time(0), _buffid(0), _skillid(0), _bufftype(0), _heartbeat_count(0)
	{ 
	}
	virtual ~base_filter() {}

	void Init(object_interface object, int time, float ratio, int buffid, float amount, float value, bool tiny_time=false);

	bool IsCombatStateSensitive() const { return _flag & FLAG_COMBAT_SENSITIVE; }

	void ClearBuffMask(int clearmask)
	{
		ClearMask(clearmask);
	}
	
	void ResetupBuffMask(bool bless)
	{
	    ClearMask(FILTER_MASK_BUFF | FILTER_MASK_DEBUFF);
	    
	    if (bless)
		MergeMask(FILTER_MASK_BUFF);
	    else
		MergeMask(FILTER_MASK_DEBUFF);
	}

	bool IsAura() const
	{
		return _bufftype & BUFF_TYPE_AURA;
	}

	void SetSkillId(int skillid)
	{
		_skillid = skillid;
	}

	void SetBuffType(int mask)
	{
		_bufftype |= mask;
	}
	
	void SetBuffId(int buffid)
	{
		_buffid = buffid;
	}

	int GetBuffId() const
	{
		return _buffid;
	}
	int GetBuffType()
	{
		return _bufftype;
	}
	void ReInit()
	{
		OnInit();
	}
	virtual int GetVersion()
	{
		return _hc->_version;
	}
	virtual int OnGetTimeOut()
	{
		return GetTime();
	}

	//bool CheckAddCondition()
	//{
	//	if (_hc->_addcondition & ADD_HAS_ANY_FILTER)
	//	{
	//		std::vector<int> &filter_ids = _hc->_addparams[ADD_HAS_ANY_FILTER];
	//		for (size_t i = 0; i < filter_ids.size(); ++i)
	//		{
	//			if (HasFilter(filter_ids[i]))
	//				return true;
	//		}
	//		return false;
	//	}
	//	if (_hc->_addcondition & ADD_HAS_ALL_FILTER)
	//	{
	//		std::vector<int> &filter_ids = _hc->_addparams[ADD_HAS_ALL_FILTER];
	//		for (size_t i = 0; i < filter_ids.size(); ++i)
	//		{
	//			if (!HasFilter(filter_ids[i]))
	//				return false;
	//		}
	//		return true;
	//	}
	//	if (_hc->_addcondition & ADD_NOT_ANY_FILTER)
	//	{
	//		std::vector<int> &filter_ids = _hc->_addparams[ADD_NOT_ANY_FILTER];
	//		for (size_t i = 0; i < filter_ids.size(); ++i)
	//		{
	//			if (HasFilter(filter_ids[i]))
	//				return false;
	//		}
	//		return true;
	//	}
	//	return true;
	//}

	int PreAttach()
	{
		if (_hc->_timetype==TIME_INSTANT)
		{
			if (IsInstant())
				TakeEffect();
			return 1;
		}
		SetupMask(_hc->_begincondition, _feedback_beginmask);
		SetupMask(_hc->_endcondition, _feedback_endmask);

		if (!PolyPolicy())
			return 2;

		return 0;
	}

	bool PolyPolicy()
	{
		if (!_hc->_polyrule)		//默认为0，不走叠加覆盖逻辑,因为老的buff添加规则都是0
			return true;
		else if (_hc->_polyrule == RULE_WEAK)
		{
			_mask |= FILTER_MASK_WEAK;
			return true;
		}
		else if (_hc->_polyrule == RULE_UNIQUE)
		{
			_mask |= FILTER_MASK_UNIQUE;
			return true;
		}
		else if (_hc->_polyrule == RULE_ISOLATE)
			return true;
		if (!_parent.IsFilterExist(_filter_id))
			return true;

		// 可叠加的状态包生效条件只能是立刻生效
		if (_hc->_begincondition != EFFECT_BEGIN && _hc->_polyrule == RULE_ATTACH)
			return false;

		_parent.ModifyFilter(_filter_id, CTRL_MODIFY, this, sizeof(this));
		return _do_attach;
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == CTRL_BUFFLEVEL)
		{
			*((int*)ctrlval) = _polycount + 1;
			return ;
		}

		base_filter* pfilter = (base_filter*)ctrlval;

		// 叠加
		if (_hc->_polyrule == RULE_ATTACH)
		{
			if (_hc->_polymax > _polycount+1)
			{
				++_polycount;
				_heartbeat_count = 0;
				TakeEffect();
			}
			// 先注掉
			//else
			//{
				//if (IsInstant())
				//	TakeEffect();
			//}
			_maxcounts = pfilter->_maxcounts;
			_counts = pfilter->_counts;
			SetTime(pfilter->GetTime());
			ResetTimeout();
			if(_hstate)
			{       
				_parent.UpdateBuff(_filter_id, _hstate, GetTimeout(), _hc->_polyrule==RULE_ISOLATE);
			}     
		}
		else if (_hc->_polyrule==RULE_RATIOPRIOR)
		{
			if (pfilter->_ratio >= _ratio)
			{
				_is_deleted = true;
				pfilter->SetAttach();
			}
		}
		else if (_hc->_polyrule==RULE_VALUEPRIOR)
		{
			if (pfilter->_value >= _value)
			{
				_is_deleted = true;
				pfilter->SetAttach();
			}
		}
		else if (_hc->_polyrule==RULE_AMOUNTPRIOR)
		{
			if (pfilter->_amount >= _amount)
			{
				_is_deleted = true;
				pfilter->SetAttach();
			}
		}
		// 重置时间
		else if (_hc->_polyrule == RULE_TIMERESET)
		{
			if (pfilter->OnGetTimeOut() > GetTimeLeft())
			{
				SetTime(pfilter->GetTime());
				ResetTimeout();
				if(_hstate)
				{       
					_parent.UpdateBuff(_filter_id, _hstate, GetTimeout(), _hc->_polyrule==RULE_ISOLATE);
				}     
			}
		}
		// 如果状态总时间更长, 就覆盖
		else if (_hc->_polyrule == RULE_SUM_TIMEPRIOR)
		{
			if (pfilter->OnGetTimeOut() >= OnGetTimeOut())
			{
				_is_deleted = true;
				pfilter->SetAttach();
			}
		}
		else if (_hc->_polyrule == RULE_TIMEPRIOR)
		{
			if (pfilter->OnGetTimeOut() > GetTimeLeft())
			{
				_is_deleted = true;
				pfilter->SetAttach();
			}
		}
	}

	// 击中敌人时的检查
	void AttackEnemy(const XID& target)
	{
		_target = target;
		if (_hc->_begincondition & EFFECT_HIT)
			CheckActivate(EFFECT_HIT);
		if (_hc->_endcondition & EFFECT_HIT)
		{
			if (CheckDeactivate(EFFECT_HIT))
			{
				_is_deleted = true;
				return ;
			}
		}
	}

	// 被打时的检查
	virtual void BeAttacked(const XID& who, const attacker_info_t& info, int damage, bool invader, char attacker_mode, int at_state)
	{
		_target = who;
		if((_hc->_begincondition & EFFECT_BEINJURE) && damage>0) 
			CheckActivate(EFFECT_BEINJURE);

		if((_hc->_begincondition & EFFECT_BEHIT) && !(at_state&SKILL_IMMUNEATTACK))
			CheckActivate(EFFECT_BEHIT);

		if((_hc->_endcondition & EFFECT_BEINJURE) && damage>0)   
		{       
			if(CheckDeactivate(EFFECT_BEINJURE))
			{       
				_is_deleted = true;  //提前结束 
				return; 
			}       
		}       
		if((_hc->_endcondition & EFFECT_BEHIT) && !(at_state&SKILL_IMMUNEATTACK))
		{       
			if(CheckDeactivate(EFFECT_BEHIT) && CheckParentState())
			{       
				_is_deleted = true;  //提前结束 
				return; 
			}       
		}       
	}
	void PostCastSkill(int flag)
	{
		if (flag == 1)
		{
			if (_hc->_endcondition & EFFECT_POST_PERFORM)
			{
				if(CheckDeactivate(EFFECT_POST_PERFORM))
				{
					_is_deleted = true; 
					return ;
				}
			}
		}
	}
	// 挂掉前调用
	void BeforeDeath()
	{
		if (_hc->_endcondition & EFFECT_BEFORE_DEATH)
		{
			_is_deleted = true;
			return ;
		}

		if (!_is_actived)
		{
			if (_hc->_begincondition & EFFECT_BEFORE_DEATH)
				Activate();
		}
		else
		{
		    // 已经激活, 就调用对应filter的覆写函数
		    OnBeforeDeath();
		}
	}

	// 杀人时调用
	void KillEnemy(const XID& who, int level)
	{
		if (_hc->_begincondition & EFFECT_KILL)
		{
			if (_maxcounts[EFFECT_KILL] > 0)
			{
				Activate();
				_maxcounts[EFFECT_KILL] -= 1;
			}
			else if (_maxcounts[EFFECT_KILL] == -1)
			{
				Activate();
			}
		}

		if (_hc->_begincondition & EFFECT_VALID_KILL)
		{
			if (_parent.GetBasicProp().level - level < _hc->_beginparam[EFFECT_VALID_KILL])
			{
				if (_maxcounts[EFFECT_VALID_KILL] > 0)
				{
					Activate();
					_maxcounts[EFFECT_VALID_KILL] -= 1;
				}
				else if (_maxcounts[EFFECT_VALID_KILL] == -1)
				{
					Activate();
				}
			}
		}

		if (_hc->_endcondition & EFFECT_KILL)
		{
			_is_deleted = true;
			return;
		}

		if (_hc->_endcondition & EFFECT_VALID_KILL)
		{
			if (_parent.GetBasicProp().level - level < _endparam[EFFECT_VALID_KILL])
			{
				_is_deleted = true;
				return ;
			}
		}
	}

	// 发出攻击消息之前
	void TranslateSendAttack(const XID & target,attack_msg & msg)
	{
		//SetFeedbackMask(target, msg);
		if (_is_actived)
		{
			OnTranslateSendAttack(target, msg);
		}
	}

	// 收到攻击消息后
	void TranslateRecvAttack(const XID & attacker,attack_msg & msg)
	{
		if (_is_actived)
		{
			OnTranslateRecvAttack(attacker, msg);
		}
	}

	void TranslateSendEnchant(const XID & target,enchant_msg & msg)
	{
		//SetFeedbackMask(target, msg);

		if (_is_actived)
		{
			OnTranslateSendEnchant(target, msg);
		}
	}

	void TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
	{
		if (_is_actived)
		{
			OnTranslateSendEnchant(attacker, msg);
		}
	}

	void Heartbeat(int tick)
	{
		CheckCondition(tick);

		if (_is_actived)
			OnHeartbeat(tick);
	}

	void AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg)
	{
		if (_is_actived)
		{
			OnAdjustDamage(dmg, attacker, msg);
		}
	}
	
	void AdjustDamageForFinalDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg)
	{ 
		if (_is_actived)
		{
			OnAdjustDamageForFinalDamage(dmg, attacker, msg);
		}
	}
	void Immune_Enchant(attack_msg & msg)
	{
		if (_is_actived)
		{
			OnImmune_Enchant(msg);
		}
	}

	void PostEnchant(const XID& attacker, int flag)
	{
	    if (_is_actived)
	    {
		OnPostEnchant(attacker, flag);
	    }
	}

private:
	//template<typename ATTACK_MSG>
	//void SetFeedbackMask(const XID& target, ATTACK_MSG& msg)
	//{
	//	if (_is_actived)
	//	{
	//		msg.feedback_mask |= _feedback_endmask;
	//	}
	//	msg.feedback_mask |= _feedback_beginmask;
	//}

	void CheckCondition(const int& tick)
	{
		_timeout_begin_end += tick;
		if (!_is_actived)
		{
			if (_hc->_begincondition & EFFECT_DELAY)
			{
				if (_timeout_begin_end * 1000 >= _hc->_beginparam[EFFECT_DELAY])
				{
					Activate();
					return ;
				}
			}

			if (_hc->_begincondition & EFFECT_THRESHOLD)
			{
				const basic_prop& bprop = _parent.GetBasicProp();
				const q_extend_prop& eprop = _parent.GetExtendProp();

				if (_hc->_begincondition & EFFECT_BELOW_HP)
				{
					if (bprop.hp < (int)(eprop.max_hp * 0.01f * _hc->_beginparam[EFFECT_BELOW_HP]))
					{
						Activate();
						return ;
					}
				}

				if (_hc->_begincondition & EFFECT_ABOVE_HP)
				{
					if (bprop.hp > (int)(eprop.max_hp * 0.01f * _hc->_beginparam[EFFECT_ABOVE_HP]))
					{
						Activate();
						return ;
					}
				}

				if (_hc->_begincondition & EFFECT_BELOW_MP)
				{
					if (bprop.mp < (int)(eprop.max_mp * 0.01f * _hc->_beginparam[EFFECT_BELOW_MP]))
					{
						Activate();
						return ;
					}
				}

				if (_hc->_begincondition & EFFECT_ABOVE_MP)
				{
					if (bprop.mp > (int)(eprop.max_mp * 0.01f * _hc->_beginparam[EFFECT_ABOVE_MP]))
					{
						Activate();
						return ;
					}
				}
			}
		}

		if (_hc->_endcondition & EFFECT_DELAY)
		{
			if (_timeout_begin_end * 1000 >= _endparam[EFFECT_DELAY])
			{
				_is_deleted = true;
				return ;
			}
		}

		if (_hc->_endcondition & EFFECT_THRESHOLD)
		{
			const basic_prop& bprop = _parent.GetBasicProp();
			const q_extend_prop& eprop = _parent.GetExtendProp();

			if (_hc->_endcondition & EFFECT_BELOW_HP)
			{
				if (bprop.hp < (int)(eprop.max_hp * 0.01f * _endparam[EFFECT_BELOW_HP]))
				{
					_is_deleted = true;
					return ;
				}
			}

			if (_hc->_endcondition & EFFECT_ABOVE_HP)
			{
				if (bprop.hp > (int)(eprop.max_hp * 0.01f * _endparam[EFFECT_ABOVE_HP]))
				{
					_is_deleted = true;
					return ;
				}
			}

			if (_hc->_endcondition & EFFECT_BELOW_MP)
			{
				if (bprop.mp < (int)(eprop.max_mp * 0.01f * _endparam[EFFECT_BELOW_MP]))
				{
					_is_deleted = true;
					return ;
				}
			}

			if (_hc->_endcondition & EFFECT_ABOVE_MP)
			{
				if (bprop.mp > (int)(eprop.max_mp * 0.01f * _endparam[EFFECT_ABOVE_MP]))
				{
					_is_deleted = true;
					return ;
				}
			}
		}
	}

protected:
	// 击中效果加上的时候就调用这个
	void OnAttach();
	void OnSavedAttach();
	// 结束时调用
	void OnDetach();

public:
	// 子类中实现的方法
	virtual void PreSave(archive &ar)
	{
		ar << _buffid;
	}

	virtual void PreLoad(archive &ar)
	{
		 ar >> _buffid;
		 OnInit();		//设置buffcondition
		_maxcounts.clear();		//清除从buffcondition拷贝，会从数据库中加载
		_endparam.clear();
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		size_t sz = _maxcounts.size();
		ar << sz;
		if (sz > 0)
		{       
			std::map<int, int>::iterator it = _maxcounts.begin();
			std::map<int, int>::iterator ie = _maxcounts.end();
			for (; it != ie; ++it)
			{       
				ar << it->first;
				ar << it->second;
			}       
		}       
		sz = _counts.size();
		ar << sz;
		if (sz > 0)
		{       
			std::map<int, int>::iterator it = _counts.begin();
			std::map<int, int>::iterator ie = _counts.end();
			for (; it != ie; ++it)
			{       
				ar << it->first;
				ar << it->second;
			}       
		}       
		sz = _endparam.size();
		ar << sz;
		if (sz > 0)
		{       
			std::map<int, int>::iterator it = _endparam.begin();
			std::map<int, int>::iterator ie = _endparam.end();
			for (; it != ie; ++it)
			{       
				ar << it->first;
				ar << it->second;
			}       
		}       
		ar << _polycount;
		ar << _feedback_beginmask;
		ar << _feedback_endmask;
		ar << _timeout_begin_end;
		ar << _is_actived;
		ar << _effect_times;
		ar << _ratio;
		ar << _amount;
		ar << _value;
		ar << _time;
		ar << _skillid;
		ar << _bufftype;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		size_t sz;
		ar >> sz;
		if (sz > 0)
		{
			int key;
			for (size_t i = 0; i < sz; ++i)
			{
				ar >> key;
				ar >> _maxcounts[key];
			}
		}
		ar >> sz;
		if (sz > 0)
		{
			int key;
			for (size_t i = 0; i < sz; ++i)
			{
				ar >> key;
				ar >> _counts[key];
			}
		}
		ar >> sz;
		if (sz > 0)
		{
			int key;
			for (size_t i = 0; i < sz; ++i)
			{
				ar >> key;
				ar >> _endparam[key];
			}
		}
		ar >> _polycount;
		ar >> _feedback_beginmask;
		ar >> _feedback_endmask;
		ar >> _timeout_begin_end;
		ar >> _is_actived;
		ar >> _effect_times;
		ar >> _ratio;
		ar >> _amount;
		ar >> _value;
		ar >> _time;
		ar >> _skillid;
		ar >> _bufftype;
		return true;
	}

	virtual void OnInit() = 0;
	virtual void TakeEffect() { }
	virtual void SavedEffect() 
	{ 
		int effect_times = _effect_times;
		_effect_times = 0;
		for (int i = 0; i < effect_times; ++i)
			TakeEffect();
	}
	virtual void UndoEffect(){}
	virtual void OnHeartbeat(int tick){}
	virtual void OnTranslateSendAttack(const XID & target,attack_msg & msg){}
	virtual void OnTranslateRecvAttack(const XID & attacker,attack_msg & msg){}
	virtual void OnTranslateSendEnchant(const XID & target,enchant_msg & msg){}
	virtual void OnTranslateRecvEnchant(const XID & attacker,enchant_msg & msg){}
	virtual void OnAdjustDamage(damage_entry& dmg, const XID& attack, const attack_msg& msg){}
	virtual void OnBeforeDeath(){}
	virtual void OnPostEnchant(const XID& attacker, int flag){}
	virtual void OnAdjustDamageForFinalDamage(damage_entry& dmg, const XID& attack, const attack_msg& msg){}
	virtual void OnImmune_Enchant(attack_msg& msg){}
	virtual int GetImmuneMask() { return 0; }
	
	virtual void SetupParam1(Skill* skill, int* list){}
	virtual void SetupParam2(const XID& player, const attacker_info_t& info, char mode, bool invader){}
	virtual void SetupParam3(int worldid, float x, float y, float z) {}
	virtual void SetupParam4(int itemid, int itemcount, int exp_time) {}
	virtual void SetupParam5(int skilllevel, const A3DVECTOR& pos) {}
	virtual void SetupParam7(const XID& player, int skill_id, int skill_level, Skill* skill){}
	virtual void SetupProp(float prop) {}
	virtual void SetupTransTarget(const XID& target, const A3DVECTOR& pos, int tag) {}
};

}
#pragma pack()

#endif
