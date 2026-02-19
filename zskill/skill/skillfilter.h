#ifndef __SKILL_FILTER_H__
#define __SKILL_FILTER_H__

#include <common/types.h>
#include <obj_interface.h>
#include <filter.h>
#include "sfilterdef.h"
#include "skillwrapper.h"
#include "statedef.h"
#include "base_filter.h"
//#include "filterclsid.h"
#include "playerwrapper.h"
#include "skill.h"

#include "substance.h"


//haokaixin add==============================start
struct cycle_area
{
	int owner;
	A3DVECTOR center;
	float radius;
	int time;

	cycle_area() : owner(0), center(), radius(0.0f), time(0) {}
};


struct trigger_skill 
{ 
	short skillid; short skilllevel; int time; 
};

//haokaixin add=============================end


using namespace GNET;

class filter_Disperseanti : public timeout_filter
{
protected:
	int _var[5];

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Disperseanti() {}
public:
	DECLARE_SUBSTANCE(filter_Disperseanti);

	filter_Disperseanti(
		object_interface object,
		float ratio,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_DISPERSEANTI;
		_buff_id = HSTATE_DISPERSEANTI;
		int sum = 0;
		for (int i = 0; i < 5; i++)
		{
			_var[i] = object.GetExtendProp().resistance[i];
			sum += _var[i];
		}
		sum = sum * ratio / 5.0;
		for (int i = 0; i < 5; i++)
		{
			_var[i] -= sum;
		}
	}

	~filter_Disperseanti()
	{
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		for (int i = 0; i < 5; i++)
		{
			ar << _var[i];
		}
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		for (int i = 0; i < 5; i++)
		{
			ar >> _var[i];
		}
		return true;
	}

	void OnAttach()
	{
		for (int i = 0; i < 5; i++)
		{
			_parent.ImpairResistance(i, _var[i]);;
		}
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		for (int i = 0; i < 5; i++)
		{
			_parent.EnhanceResistance(i, _var[i]);;
		}
		_parent.UpdateMagicData();
	}

};

class filter_Frenzied : public timeout_filter
{
protected:
	float _ratio;
	int _addend;
	int _maxcount;
	int _count;
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Frenzied() {}
public:
	DECLARE_SUBSTANCE(filter_Frenzied);

	filter_Frenzied(
		object_interface object,
		float ratio,
		int addend,
		int max,
		int prob,
		int period ) : timeout_filter( object, period, FILTER_MASK),
						_ratio(ratio), _addend(addend), _maxcount(max), _count(0), _prob(prob)
	{
		_filter_id = FILTER_FRENZIED;
		_buff_id = HSTATE_FRENZIED;
	}

	~filter_Frenzied(){}

	void AdjustDamage(
		damage_entry* const dmg,
		const XID* const attacker,
		const attack_msg* const msg)
	{
		if (_count < _maxcount && rand() % 100 <= _prob)
		{
			if (_ratio > _parent.GetBasicProp().hp / _parent.GetExtendProp().max_hp)
			{
				_parent.EnhanceDamage(_addend);
				_parent.UpdateAttackData();
				_count++;
			}
		}
	}


	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _addend << _count << _maxcount << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _addend >> _count >> _maxcount >> _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		if (_count)
		{
			_parent.ImpairDamage(_count * _addend);
			_parent.UpdateAttackData();
		}
	}


};

class filter_Qilinform : public filter
{
protected:
	float _speed;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Qilinform() {}
public:
	DECLARE_SUBSTANCE(filter_Qilinform);

	filter_Qilinform(object_interface object, float value) : filter(object, FILTER_MASK),_speed(value)
	{
		_filter_id = FILTER_QILINFORM;
		_buff_id = HSTATE_QILINFORM;
	}

	~filter_Qilinform() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _speed;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _speed;
		return true;
	}

	void OnAttach()
	{
		_parent.SetQilinForm(1);
		_parent.ChangeShape(7);
		_parent.EnhanceSpeed(_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.SetQilinForm(0);
		_parent.ChangeShape(0);
		_parent.ImpairSpeed(_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

};

class filter_Insanityform : public filter
{
protected:
	int _addhp;
	int _addmp;
	int _addattack;
	int _adddefence;
	int _addanti;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK 
	};

	filter_Insanityform() {}
public:
	DECLARE_SUBSTANCE(filter_Insanityform);

	filter_Insanityform(
		object_interface object,
		int addhp,
		int addmp,
		int addattack,
		int adddefence,
		int addanti) : filter(object, FILTER_MASK), _addhp(addhp), _addmp(addmp),
						_addattack(addattack), _adddefence(adddefence), _addanti(addanti)
	{
		_filter_id = FILTER_INSANITYFORM;
		_buff_id = HSTATE_INSANITYFORM;
	}

	~filter_Insanityform() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addhp << _addmp << _addattack << _adddefence << _addanti;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addhp >> _addmp >> _addattack >> _adddefence >> _addanti;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMaxHP(_addhp);
		_parent.EnhanceMaxMP(_addmp);
		_parent.EnhanceDamage(_addattack);
		_parent.EnhanceDefense(_adddefence);
		_parent.EnhanceResistance(0, _addanti);
		_parent.EnhanceResistance(1,_addanti);
		_parent.EnhanceResistance(2,_addanti);
		_parent.EnhanceResistance(3,_addanti);
		_parent.EnhanceResistance(4,_addanti);
		_parent.EnhanceResistance(5,_addanti);
		_parent.UpdateAllProp();
		_parent.ChangeShape(2);
	}

	void OnDetach()
	{
		_parent.ImpairMaxHP(_addhp);
		_parent.ImpairMaxMP(_addmp);
		_parent.ImpairDamage(_addattack);
		_parent.ImpairDefense(_adddefence);
		_parent.ImpairResistance(0,_addanti);
		_parent.ImpairResistance(1,_addanti);
		_parent.ImpairResistance(2,_addanti);
		_parent.ImpairResistance(3,_addanti);
		_parent.ImpairResistance(4,_addanti);
		_parent.ImpairResistance(5,_addanti);
		_parent.UpdateAllProp();
		_parent.ChangeShape(0);
	}

};

class filter_Dizzytimer : public timeout_filter
{

protected:
	int _cycle;
	int _duration;
	int _counter;
	int _active;
	int _buffcnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Dizzytimer() {}
public:
	DECLARE_SUBSTANCE(filter_Dizzytimer);

	filter_Dizzytimer(
		object_interface object,
		int duration,
		int cycle,
		int period) : timeout_filter(object, period, FILTER_MASK),
		_cycle(cycle), _duration(duration), _counter(0), _active(0), _buffcnt(0)
	{
		_filter_id = FILTER_DIZZYTIMER;
		_buff_id = HSTATE_DIZZYTIMER;
	}

	~filter_Dizzytimer() {}

	void Deactivate()
	{
		this->_active = 0;
		_parent.SetSilentSeal(0);
		_parent.SetMeleeSeal(0);
		_parent.SetRootSeal(0);
		_parent.SendClientDispelRoot(1);
		_parent.RemoveBuff(1, _filter_id);
		_parent.DecVisibleState(VSTATE_DIZZY);
	}

	void Heartbeat(int tick)
	{
		if (!_counter)
		{
			_parent.SetSilentSeal(true);
			_parent.SetMeleeSeal(true);
			_parent.SetRootSeal(true);
			_parent.SendClientNotifyRoot(1);
			_parent.UpdateBuff(1,_filter_id,GetTimeout(),1);
			_parent.IncVisibleState(VSTATE_DIZZY);
			this->_active = 1;
		}
		_counter += tick;
		if (_active)
			_buffcnt += tick;
		if (_buffcnt >= _cycle && _active)
		{
			Deactivate();
			_buffcnt = 0;
		}
		if (_counter >= _duration)
			_counter = 0;
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _cycle << _duration << _counter << _active << _buffcnt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _cycle >> _duration >> _counter >> _active >> _buffcnt;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		if (_active)
		{
			Deactivate();
		}
	}


};

//5

class filter_Drunk : public timeout_filter
{
protected:
	int _ratio;
	int _subdefence;
	int _subanti;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Drunk() {}
public:
	DECLARE_SUBSTANCE(filter_Drunk);

	filter_Drunk(
		object_interface object,
		float ratio,
		int sub1,
		int sub2,
		int period) : timeout_filter(object, period, FILTER_MASK),
					_ratio(ratio), _subdefence(sub1), _subanti(sub2)
	{
		_filter_id = FILTER_DRUNK;
		_buff_id = HSTATE_DRUNK;
	}

	~filter_Drunk() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _subdefence << _subanti;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _subdefence >> _subanti;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairDefense(_subdefence);
		_parent.UpdateDefenseData();
		_parent.ImpairResistance( 4, _subanti);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceDefense(_subdefence);
		_parent.UpdateDefenseData();
		_parent.EnhanceResistance(4,_subanti);
		_parent.UpdateMagicData();
	}
};

class filter_Damagemove : public timeout_filter
{
protected:
	float _value;
	A3DVECTOR pos;
	XID _performer;
	attacker_info_t _pinfo;  //0x54

	bool _invader;  //0x88
	char _mode;     //0x89

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Damagemove() {}
public:
	DECLARE_SUBSTANCE(filter_Damagemove);

	filter_Damagemove(
		object_interface object,
		int value,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK),_value(value)
	{
		_filter_id = buffid + FILTER_DAMAGEMOVE; 
		_buff_id = HSTATE_DAMAGEMOVE;
	}

	~filter_Damagemove() {}

	void SetUp(
		const XID& player,
		const attacker_info_t& info,
		char mode,
		bool invader)
	{
		_performer.type = player.type;
		_performer.id = player.id;
		memcpy(&_pinfo, &info, sizeof(_pinfo));
		_mode = mode;
		_invader = invader;
	}

	void Heartbeat(int tick)
	{
		const A3DVECTOR &curPos = _parent.GetPos();
		float dist = sqrtf(curPos.squared_distance(pos));
		int damage = dist * _value;
		if (dist <= 20.0 && damage > 0)
		{
			_parent.BeHurt(_performer, _pinfo, damage, _invader, _mode);
		}
		pos = curPos;
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value << pos << _performer;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value >> pos >> _performer;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		pos = _parent.GetPos();
		_parent.IncVisibleState(VSTATE_DAMAGEMOVE);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_DAMAGEMOVE);
	}
};

class filter_Tongxin : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Tongxin() {}
public:
	DECLARE_SUBSTANCE(filter_Tongxin);

	filter_Tongxin(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _addend(r), _ratio(ratio)
	{
		_filter_id = buffid + FILTER_TONGXIN;
		_buff_id = HSTATE_TONGXIN;
	}

	~filter_Tongxin() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.EnhanceDamage(_addend);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.ImpairDamage(_addend);
		_parent.UpdateAttackData();
	}
};

class filter_Subantiwrap2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantiwrap2() {}
public:
	DECLARE_SUBSTANCE(filter_Subantiwrap2);

	filter_Subantiwrap2(
		object_interface object,
		int v,
		int period,
		int overlay_cnt) : timeout_filter(object, period, FILTER_MASK), _value(v)
	{
		_filter_id = FILTER_SUBANTIWRAP2;
		_buff_id = HSTATE_SUBANTIWRAP2;

		_max_overlay_cnt = overlay_cnt;
		_can_overlay = true;

	}

	~filter_Subantiwrap2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance( 2,_value);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(2, _value);
		_parent.UpdateMagicData();
	}
};

class filter_Sharestatewithclone : public timeout_filter
{
protected:
	float _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Sharestatewithclone() {}
public:
	DECLARE_SUBSTANCE(filter_Sharestatewithclone);

	filter_Sharestatewithclone(
		object_interface object,
		int period,
		float prob) : timeout_filter(object, period, FILTER_MASK), _prob(prob)
	{
		_filter_id = FILTER_SHARESTATEWITHCLONE;
		_buff_id = HSTATE_SHARESTATEWITHCLONE;

	}

	~filter_Sharestatewithclone() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _prob;
		return true;
	}

	void OnAttach()
	{
		_parent.SetCloneMirror(true, _prob * 100.0);
	}

	void OnDetach()
	{
		_parent.SetCloneMirror(false, _prob);
	}
};

//10

class filter_Decskillaccu : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decskillaccu() {}
public:
	DECLARE_SUBSTANCE(filter_Decskillaccu);

	filter_Decskillaccu(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _ratio(r)
	{
		_filter_id = buffid + FILTER_DECSKILLACCU;
		_buff_id = HSTATE_DECSKILLACCU;

	}

	~filter_Decskillaccu() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceSkillAttack(-_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceSkillAttack(_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Yuanling : public filter
{
protected:
	float _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Yuanling() {}
public:
	DECLARE_SUBSTANCE(filter_Yuanling);

	filter_Yuanling(object_interface object, float r) : filter(object, FILTER_MASK), _addend(r)
	{
		_filter_id = FILTER_YUANLING;
		_buff_id = HSTATE_YUANLING;
	}

	~filter_Yuanling() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.ForbidAttack();
		_parent.EnhanceSpeed(_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.SetImmuneMask(IMMUNESLOW);
		_parent.SetImmuneMask(IMMUNEWRAP);
		_parent.ChangeShape(5);
	}

	void OnDetach()
	{
		_parent.AllowAttack();
		_parent.ImpairSpeed(_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.ClearImmuneMask(IMMUNESLOW); 
		_parent.ClearImmuneMask(IMMUNEWRAP);
		_parent.ChangeShape(0);
	}

};

class filter_Iceshield : public timeout_filter
{
protected:
	int _health;
	int _addon;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Iceshield() {}
public:
	DECLARE_SUBSTANCE(filter_Iceshield);

	filter_Iceshield(
		object_interface object,
		int amount,
		int addon,
		int period) : timeout_filter(object, period, FILTER_MASK), _addon(addon), _counter(0)
	{
		_filter_id = FILTER_ICESHIELD;
		_buff_id = HSTATE_ICESHIELD;

		_health = amount / period;
		if (_health <= 0)
		{
			_health = 1;
		}
	}

	~filter_Iceshield() {}

	void Heartbeat(int tick)
	{
		_counter += tick;
		if (_counter > 2 || tick >= _timeout)
		{
			_parent.Heal(_health * _counter, 0, 0);
			_counter -= 3;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _health << _addon << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _health >> _addon >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNEHPLEAK); 
		_parent.SetSelfSilentSeal(true);
		_parent.SetMeleeSeal(true);
		_parent.SetRootSeal(true);
		_parent.SendClientNotifyRoot(1);
		_parent.EnhanceDefense(_addon);
		_parent.UpdateDefenseData();
		_parent.IncVisibleState(VSTATE_ICESHIELD);
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNEHPLEAK);
		_parent.SetSelfSilentSeal(false);
		_parent.SetMeleeSeal(false);
		_parent.SetRootSeal(false);
		_parent.SendClientDispelRoot(1);
		_parent.ImpairDefense(_addon);
		_parent.UpdateDefenseData();
		_parent.DecVisibleState(VSTATE_ICESHIELD);
	}
};

class filter_Cycsubattack : public timeout_filter
{
protected:
	int _amount;
	int _sub;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Cycsubattack() {}
public:
	DECLARE_SUBSTANCE(filter_Cycsubattack);

	filter_Cycsubattack(
		object_interface object,
		int amount,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _amount(0), _counter(0), _sub(amount / period)
	{
		_filter_id = buffid + FILTER_CYCSUBATTACK;
		_buff_id = HSTATE_CYCSUBATTACK;

	}

	~filter_Cycsubattack() {}

	void Heartbeat(int tick)
	{
		_counter += tick;
		if (_counter > 1 || tick >= _timeout)
		{
			_amount += _counter * _sub;
			_parent.ImpairDamage(_counter * _sub);
			_parent.UpdateAttackData();
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _amount << _sub << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _amount >> _sub >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		_parent.EnhanceDamage(_amount);
		_parent.UpdateAttackData();
	}
};

class filter_BarAdddogeaccu : public filter
{
protected:
	int _accuinc;
	int _dodgeinc;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE 
	};

	filter_BarAdddogeaccu() {}
public:
	DECLARE_SUBSTANCE(filter_BarAdddogeaccu);

	filter_BarAdddogeaccu(
		object_interface object,
		int accuinc,
		int dodgeinc,
		int buffid) : filter(object, FILTER_MASK), _accuinc(accuinc), _dodgeinc(dodgeinc)
	{
		_filter_id = buffid + FILTER_BARADDDOGEACCU;
		_buff_id = HSTATE_BARADDDOGEACCU;
	}

	~filter_BarAdddogeaccu() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _accuinc << _dodgeinc;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _accuinc >> _dodgeinc;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceArmor(_dodgeinc);
		_parent.UpdateDefenseData();
		_parent.EnhanceAttack(_accuinc);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairArmor(_dodgeinc);
		_parent.UpdateDefenseData();
		_parent.ImpairAttack(_accuinc);
		_parent.UpdateAttackData();
	}

};

//15
class filter_Dimming : public timeout_filter
{
protected:
	int inc_attack_dodge;
	int inc_skill_dodge;
	int inc_speed;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Dimming() {}
public:
	DECLARE_SUBSTANCE(filter_Dimming);

	filter_Dimming(
		object_interface object,
		float ratio,
		int value,
		int period,
		int amount,
		int buffid) : timeout_filter(object, period, FILTER_MASK),
		inc_attack_dodge(amount), inc_skill_dodge(value), inc_speed(ratio)
	{
		_filter_id = buffid + FILTER_DIMMING;
		_buff_id = HSTATE_DIMMING;

	}

	~filter_Dimming() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << inc_attack_dodge << inc_skill_dodge << inc_speed;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> inc_attack_dodge >> inc_skill_dodge >> inc_speed;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceArmor(inc_attack_dodge);
		_parent.UpdateDefenseData();
		_parent.EnhanceSkillArmor(inc_skill_dodge);
		_parent.UpdateAttackData();
		_parent.EnhanceSpeed(inc_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.SetDimState(true);
	}

	void OnDetach()
	{
		_parent.ImpairArmor(inc_attack_dodge);
		_parent.UpdateDefenseData();
		_parent.EnhanceSkillArmor(-inc_skill_dodge);
		_parent.UpdateAttackData();
		_parent.ImpairSpeed(inc_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.SetDimState(false);
	}
};

class filter_Inchurt : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Inchurt() {}
public:
	DECLARE_SUBSTANCE(filter_Inchurt);

	filter_Inchurt(
		object_interface object,
		float ratio,
		int period) : timeout_filter(object, period, FILTER_MASK), _ratio(ratio + 1.0)
	{
		_filter_id = FILTER_INCHURT;
		_buff_id = HSTATE_INCHURT;

	}

	~filter_Inchurt() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Cycsubdefence : public timeout_filter
{
protected:
	int _amount;
	int _sub;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT 
	};

	filter_Cycsubdefence() {}
public:
	DECLARE_SUBSTANCE(filter_Cycsubdefence);

	filter_Cycsubdefence(
		object_interface object,
		int amount,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK),
		_sub(amount / period), _amount(0), _counter(0)
	{
		_filter_id = buffid + FILTER_CYCSUBDEFENCE;
		_buff_id = HSTATE_CYCSUBDEFENCE;

	}

	~filter_Cycsubdefence() {}

	void Heartbeat(int tick)
	{
		_counter += tick;
		if (_counter > 1 || tick >= _timeout)
		{
			_amount += _counter * _sub;
			_parent.ImpairDefense(_counter * _sub);
			_parent.UpdateDefenseData();
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _amount << _sub << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _amount >> _sub >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		_parent.EnhanceDefense(_amount);
		_parent.UpdateDefenseData();
	}
};

class filter_Darkuniform : public filter
{
protected:
	int inc_dark_light;
	int dec_dark_skill_cd;
	int spirit_gen_interval;
	bool flag;
	int counter;
	int anti;
	int mp;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Darkuniform() {}
public:
	DECLARE_SUBSTANCE(filter_Darkuniform);

	filter_Darkuniform(
		object_interface object,
		float ratio,
		int value,
		int amount,
		int time,
		int buffid) : filter(object, FILTER_MASK), 
					inc_dark_light(value), dec_dark_skill_cd(ratio), spirit_gen_interval(amount / 1000),
					flag(0), counter(0), anti(buffid), mp(time)
	{
		_filter_id = FILTER_DARKUNIFORM;
		_buff_id = HSTATE_DARKUNIFORM;
	}

	~filter_Darkuniform() {}

	void Heartbeat(int tick)
	{
		if (!_is_deleted)
		{
			_parent.IncDarkLight(inc_dark_light);
			if (_parent.GetDarkLight() >= 0)
				_is_deleted = 1;
			if (--counter <= 0)
			{
				_parent.AddDarkLightSpirit(3);
				counter = spirit_gen_interval;
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << inc_dark_light << dec_dark_skill_cd << spirit_gen_interval << flag << counter << anti << mp;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> inc_dark_light >> dec_dark_skill_cd >> spirit_gen_interval >> flag >> counter >> anti >> mp;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(CLASS_HUPO);
		_parent.RemoveFilter(CLASS_GUILIN);
		_parent.RemoveFilter(CLASS_ADDATTACK4);
		_parent.RemoveFilter(CLASS_ADDANTI2);
		_parent.RemoveFilter(CLASS_TONGXIN);
		_parent.SetDarkLightForm(5);
		if (_parent.GetDarkLight() < 0)
		{
			_parent.EnhanceMaxMP(mp);
			for (int i = 0; i <= 5; ++i)
			{
				_parent.EnhanceResistance(i, anti);
			}
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			counter = spirit_gen_interval;
			_parent.GetSkillWrapper().DecDarkSkillCoolTime(dec_dark_skill_cd);
			_parent.GetSkillWrapper().DecLightSkillCoolTime(dec_dark_skill_cd);
			flag = 1;
			for (int i = 0; i <= 2; ++i)   //这个循环很诡异  haokaixin
			{
				_parent.AddDarkLightSpirit(3);
			}
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		_parent.ClearDarkLightForm();
		if (flag)
		{
			_parent.ImpairMaxMP(mp);
			for (int i = 0; i <= 5; ++i)
			{
				_parent.ImpairResistance(i, anti);
			}
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			_parent.GetSkillWrapper().IncDarkSkillCoolTime(dec_dark_skill_cd);
			_parent.GetSkillWrapper().IncLightSkillCoolTime(dec_dark_skill_cd);
		}
	}

};

class filter_Addantisilent : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addantisilent() {}
public:
	DECLARE_SUBSTANCE(filter_Addantisilent);

	filter_Addantisilent(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _addend(v)
	{
		_filter_id = buffid + FILTER_ADDANTISILENT;
		_buff_id = HSTATE_ADDANTISILENT;

	}

	~filter_Addantisilent() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance(3, _addend);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance(3, _addend);
		_parent.UpdateMagicData();
	}
};

//20
class filter_Addattack4 : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addattack4() {}
public:
	DECLARE_SUBSTANCE(filter_Addattack4);

	filter_Addattack4(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _addend(r)
	{
		_filter_id = buffid + FILTER_ADDATTACK4;
		_buff_id = HSTATE_ADDATTACK4;

	}

	~filter_Addattack4() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDamage(_addend);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairDamage(_addend);
		_parent.UpdateAttackData();
	}
};

class filter_Hpleak2 : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpleak2() {}
public:
	DECLARE_SUBSTANCE(filter_Hpleak2);

	filter_Hpleak2(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _addtion(addtion)
	{
		_counter = 0;
		_damage = damage / period;
		if (_damage <= 0)
		{
			_damage = 1;
		}
		_timeout = damage / _damage;

		_filter_id = buffid + FILTER_HPLEAK2;
		_buff_id = HSTATE_HPLEAK2;
	}

	~filter_Hpleak2() {}

	void Heartbeat(int tick)
	{
		_counter += tick;
		if (_counter > 1 || tick >= _timeout)
		{
			int damage = _counter * _damage;
			if (_timeout <= tick)
			{
				damage += _addtion;
			}
			_parent.BeHurt(_performer,_pinfo,damage,_invader,_mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		_performer.type = player.type;
		_performer.id = player.id;
		memcpy(&_pinfo, &info, sizeof(_pinfo));
		_mode = mode;
		_invader = invader;
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 1)
		{
			_damage /= 2;
			_addtion /= 2;
		}
		if (_damage <= 1)
		{
			_is_deleted = 1;
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _damage >> _addtion >> _performer >> _counter;
		ar.push_back(&_pinfo,sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPLEAK);
	}
};

class filter_Setskilllevel : public timeout_filter
{
protected:
	int sid[3];
	int destlevel;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Setskilllevel() {}
public:
	DECLARE_SUBSTANCE(filter_Setskilllevel);

	filter_Setskilllevel(
		object_interface object,
		int period,
		int value,
		int amount,
		int ratio,
		int buffid) : timeout_filter(object, period, FILTER_MASK), destlevel(buffid)
	{
		sid[0] = value;
		sid[1] = amount;
		sid[2] = ratio;
		_filter_id = FILTER_SETSKILLLEVEL;
		_buff_id = HSTATE_SETSKILLLEVEL;
	}

	~filter_Setskilllevel() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar.pop_back(sid, sizeof(sid));
		ar << destlevel;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar.push_back(sid, sizeof(sid));
		ar >> destlevel;
		return true;
	}

	void OnAttach()
	{
		if (destlevel < 0)
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
	}
};

class filter_Hpregain : public timeout_filter
{
protected:
	int _ratio;
	int _addend;
	int _count;
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Hpregain() {}
public:
	DECLARE_SUBSTANCE(filter_Hpregain);

	filter_Hpregain(
		object_interface object,
		float ratio,
		int addend,
		int max,
		int prob,
		int period) : timeout_filter(object, period, FILTER_MASK),
					_ratio(ratio), _addend(addend), _count(max), _prob(prob)
	{
		_filter_id = FILTER_HPREGAIN;
		_buff_id = HSTATE_HPREGAIN;
	}

	~filter_Hpregain() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _addend << _count << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _addend >> _count >> _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Darkness : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Darkness() {}
public:
	DECLARE_SUBSTANCE(filter_Darkness);

	filter_Darkness(object_interface object, int period)
		: timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_DARKNESS;
		_buff_id = HSTATE_DARKNESS;
	}

	~filter_Darkness() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.SetDarkState(true);
		_parent.IncVisibleState(VSTATE_DARKNESS);
	}

	void OnDetach()
	{
		_parent.SetDarkState(false);
		_parent.DecVisibleState(VSTATE_DARKNESS);
	}
};

//25
class filter_Triggerskill : public filter
{
protected:
	//struct trigger_skill { short skillid; short skilllevel; int time; };  //add in skill.h
	trigger_skill ts;
	std::map<int,trigger_skill> trigger_skill_map;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Triggerskill() {}
public:
	DECLARE_SUBSTANCE(filter_Triggerskill);

	filter_Triggerskill(
		object_interface object,
		int period,
		int value,
		int amount) : filter(object, FILTER_MASK)
	{
		ts.skillid = value;
		ts.skilllevel = amount;
		ts.time = period;
		_filter_id = FILTER_TRIGGERSKILL;
		_buff_id = HSTATE_DARKUNIFORM;
	}

	~filter_Triggerskill() {}

	void Heartbeat(int tick)
	{
		for (std::map<int, trigger_skill>::iterator it = trigger_skill_map.begin(); it != trigger_skill_map.end(); )
		{
			it->second.time -= tick;
			if (it->second.time > 0)
			{
				it++;
			}
			else
			{
				_parent.GetSkillWrapper().ClearTriggerSkill(it->second.skillid, _parent);
				trigger_skill_map.erase(it++);
			}
		}
		if (trigger_skill_map.empty())
		{
			_is_deleted = true;
		}
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrlen)
	{
		if (ctrlname == 10 && ctrlen == 8)
		{
			trigger_skill* tsa = (trigger_skill*)ctrlval;
			_parent.GetSkillWrapper().InsertTriggerSkill(tsa->skillid, tsa->skilllevel, _parent);
			_parent.SendClientTriggerSkillTime(tsa->skillid, tsa->time);
			trigger_skill_map[tsa->skillid] = *tsa;
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << trigger_skill_map.size();
		for (std::map<int, trigger_skill>::iterator it = trigger_skill_map.begin(); it != trigger_skill_map.end(); it++)
		{
			ar << it->second.skillid << it->second.skilllevel << it->second.time;
		}
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);  
		size_t size;
		ar >> size;

		for (int i = 0; i < size; i++)
		{
			trigger_skill ts;
			ar >> ts.skillid >> ts.skilllevel >> ts.time;
			std::pair<int, trigger_skill> it(ts.skillid,ts);
			trigger_skill_map.insert(it);
		}
		return true;
	}

	void OnAttach()
	{
		_parent.GetSkillWrapper().InsertTriggerSkill(ts.skillid, ts.skilllevel, _parent);
		_parent.SendClientTriggerSkillTime(ts.skillid,ts.time);
		trigger_skill_map[ts.skillid].skillid = ts.skillid;
		trigger_skill_map[ts.skillid].time = ts.time;
	}

	void OnDetach()
	{
		for (std::map<int, trigger_skill>::iterator it = trigger_skill_map.begin(); it != trigger_skill_map.end(); it++)
		{
			_parent.GetSkillWrapper().ClearTriggerSkill(it->second.skillid, _parent);
		}
		trigger_skill_map.clear();

	}

};

class filter_Expboost : public timeout_filter
{
protected:
	int _exp;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Expboost() {}
public:
	DECLARE_SUBSTANCE(filter_Expboost);

	filter_Expboost(
		object_interface object,
		int exp,
		int period) : timeout_filter(object, period, FILTER_MASK), _counter(0)
	{
		_filter_id = FILTER_EXPBOOST;
		_buff_id = HSTATE_EXPBOOST; 
		_exp = exp / period;
		if (_exp <= 0)
		{
			_exp = 1;
		}
	}

	~filter_Expboost() {}

	void Heartbeat(int tick)
	{
		_counter += tick;
		if (_counter > 14 || tick >= _timeout)
		{
			_parent.ReceiveTaskExp(_counter * _exp);
			_counter -= 15;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _exp << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _exp >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Ningjin : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Ningjin() {}
public:
	DECLARE_SUBSTANCE(filter_Ningjin);

	filter_Ningjin(
		object_interface object,
		float ratio,
		int period) : timeout_filter(object, period, FILTER_MASK), _ratio(ratio)
	{
		_filter_id = FILTER_NINGJIN;
		_buff_id = HSTATE_NINGJIN;
	}

	~filter_Ningjin() {}

	void TranslateSendAttack(
		const XID& target,
		attack_msg& msg,
		char type,
		float radius,
		int mc)
	{
		msg.damage_factor = msg.damage_factor + _ratio;
		_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Barexpboost : public filter
{
protected:
	int _skill_level;
	int _period;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT
	};

	filter_Barexpboost() {}
public:
	DECLARE_SUBSTANCE(filter_Barexpboost);

	filter_Barexpboost(
		object_interface object,
		int period,
		int skill_level,
		int buffid) : filter(object, FILTER_MASK), _skill_level(skill_level), _period(period), _counter(0)
	{
		_filter_id = buffid + FILTER_BAREXPBOOST;
		_buff_id = HSTATE_BAREXPBOOST;
	}

	~filter_Barexpboost() {}

	void Heartbeat(int tick)   //haokaixin  这个函数懒得去格式化了
	{
		int exp; // [esp+4h] [ebp-24h]
		int reborn_cnt; // [esp+Ch] [ebp-1Ch]
		float m2; // [esp+10h] [ebp-18h]
		float m1; // [esp+14h] [ebp-14h]
		int k2_0; // [esp+18h] [ebp-10h]
		int k1_0; // [esp+1Ch] [ebp-Ch]
		//__int16 player_level; // [esp+22h] [ebp-6h]

		_counter += tick;
		if (_counter > 14)
		{
			short player_level = _parent.GetBasicProp().level;
			if (player_level <= 149)
			{
				if (player_level > 59)
				{
					k1_0 = 0;
					k2_0 = 1;
				}
				else
				{
					k1_0 = 1;
					k2_0 = 0;
				}
				reborn_cnt = _parent.RebornCount();
				m1 = (long double)this->_skill_level * 0.02 * ((long double)reborn_cnt * 0.5 + 1.0);
				m2 = (long double)this->_skill_level * 0.02 * ((long double)reborn_cnt * 1.5 + 1.0);
				exp = (int)((long double)(int)((long double)k1_0
					* m1
					* ((long double)player_level * 0.45 * (long double)player_level
						+ (long double)(11 * player_level)
						+ 900.0)
					+ (long double)k2_0
					* m2
					* ((long double)(11 * player_level * player_level)
						- (long double)player_level * 1333.3
						+ 44500.0))
					* (long double)this->_counter
					/ (long double)this->_period);
				if (exp > 0)
					_parent.ReceiveTaskExp(exp);
			}
			_counter -= 15;
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _skill_level << _period << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _skill_level >> _period >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Barmpgen : public filter
{
protected:
	int _health;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT
	};

	filter_Barmpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Barmpgen);

	filter_Barmpgen(
		object_interface object,
		int period,
		int health,
		int buffid) : filter(object, FILTER_MASK),  _counter(0)
	{
		_filter_id = buffid + FILTER_BARMPGEN;
		_buff_id = HSTATE_BARMPGEN;
		
		_health = health / period;
		if (_health <= 0)
		{
			_health = 1;
		}
	}

	~filter_Barmpgen() {}

	void Heartbeat(int tick)   //haokaixin  这个函数懒得去格式化了
	{
		_counter += tick;
		if (_counter > 1)
		{
			_parent.InjectMana(_counter * _health, false);
			_counter -= 2;
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _health << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _health >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_MPGEN);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_MPGEN);
	}
};

//30
class filter_Shadowhide : public timeout_filter
{
protected:
	int mpcost;
	int shadow_ability;
	float inc_speed;
	int _count;
	int is_timeprotected;
	int timeprotectedcount;
	int attackcount;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED | FILTER_MASK_TRANSLATE_SEND_ENCHANT  | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Shadowhide() {}
public:
	DECLARE_SUBSTANCE(filter_Shadowhide);

	filter_Shadowhide(
		object_interface object,
		float ratio,
		int value,
		int period,
		int amount) : timeout_filter(object, period, FILTER_MASK), _count(0)
	{
		_filter_id = FILTER_SHADOWHIDE;
		_buff_id = HSTATE_SHADOWHIDE;  
		this->mpcost = value;
		this->shadow_ability = amount;
		this->inc_speed = ratio;
		this->is_timeprotected = 1;
		this->timeprotectedcount = 0;
		this->attackcount = 0;
	}

	~filter_Shadowhide() {}

	void Heartbeat(int tick)
	{
		object_interface* v2; // [esp-Ch] [ebp-14h]

		if (timeprotectedcount > 1)
			is_timeprotected = 0;
		++timeprotectedcount;
		_count += tick;
		if (_count > 1)
		{
			if (mpcost <= 0 || _parent.GetBasicProp().mp >= mpcost)
			{
				_count = 0;
				_parent.DrainMana(mpcost);
				timeout_filter::Heartbeat(tick);
			}
			else
			{
				_is_deleted = 1;
			}
		}
	}

	void BeAttacked(const XID& target, int dmg, bool crit)
	{
		if (attackcount > 10)
			_is_deleted = 1;
		if (dmg > _parent.GetExtendProp().max_hp * 0.050000001 || !dmg)
		{
			++attackcount;
		}
		if (!is_timeprotected)
		{
			if (dmg > _parent.GetExtendProp().max_hp * 0.050000001 || !dmg)
			{
				_is_deleted = 1;
			}
		}
	}

	void TranslateSendAttack(const XID& target, attack_msg* const msg, char type, float r, int mc)
	{
		if (target.id != msg->ainfo.attacker.id)
			_is_deleted = 1;
	}

	void TranslateSendEnchant(const XID& target, enchant_msg* const msg)
	{
		if (target.id != msg->ainfo.attacker.id)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)  //很奇怪，反编译发现这个filter没有调用父类Save和Load  haokaixin
	{
		//timeout_filter::Save(ar);
		ar << mpcost << shadow_ability << inc_speed;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//timeout_filter::Load(ar);
		ar >> mpcost >> shadow_ability >> inc_speed;
		return true;
	}

	void OnAttach()
	{
		_parent.SetInvisible(shadow_ability);
		_parent.EnhanceSpeed(inc_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.ClearInvisible(shadow_ability);
		_parent.ImpairSpeed(inc_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Shadowdance : public timeout_filter
{
protected:
	int mpcost;
	int shadow_ability;
	float inc_speed;
	int _count;
	int _period;
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Shadowdance() {}
public:
	DECLARE_SUBSTANCE(filter_Shadowdance);

	filter_Shadowdance(
		object_interface object,
		float ratio,
		int prob,
		int value,
		int period,
		int amount) : timeout_filter(object, period, FILTER_MASK), _count(0)
	{
		_filter_id = FILTER_SHADOWDANCE;
		_buff_id = HSTATE_SHADOWDANCE;
		mpcost = value;
		shadow_ability = amount;
		inc_speed = ratio;
		_period = period;
		_prob = prob;
	}

	~filter_Shadowdance() {}

	void Heartbeat(int tick)
	{
		_count += tick;
		if (_count > 1)
		{
			if (mpcost <= 0
				|| _parent.GetBasicProp().mp >= mpcost)
			{
				if (!_parent.IsFilterExist(FILTER_SHADOWHIDE))
				{
					_parent.AddFilter(new filter_Shadowhide(_parent, inc_speed, mpcost, _period, shadow_ability));
				}
				this->_count = 0;
				timeout_filter::Heartbeat(tick);
			}
			else
			{
				_is_deleted = 1;
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << mpcost << shadow_ability << inc_speed << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> mpcost >> shadow_ability >> inc_speed >> _prob;
		return true;
	}

	void OnAttach()
	{
		if (_parent.CanSetInvisible())
		{
			_parent.AddFilter(new filter_Shadowhide(_parent, inc_speed, mpcost, _period, shadow_ability));
		}
	}

	void OnDetach()
	{
		_parent.RemoveFilter(FILTER_SHADOWHIDE);
	}
};

class filter_Powerless : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Powerless() {}
public:
	DECLARE_SUBSTANCE(filter_Powerless);

	filter_Powerless(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_POWERLESS;
		_buff_id = HSTATE_POWERLESS;
	}

	~filter_Powerless() {}

	void OnAttach()
	{
		_parent.SetMeleeSeal( true);
		_parent.SetImmuneMask(IMMUNESILENT);
	}

	void OnDetach()
	{
		_parent.SetMeleeSeal( false);
		_parent.ClearImmuneMask(IMMUNESILENT);
	}
};

class filter_Subhp : public timeout_filter
{
protected:
	int _value;
	int _limit;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT 
	};

	filter_Subhp() {}
public:
	DECLARE_SUBSTANCE(filter_Subhp);

	filter_Subhp(
		object_interface object,
		int value,
		int amount,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _value(value), _limit(amount)
	{
		_filter_id = buffid + FILTER_SUBHP;
		_buff_id = HSTATE_SUBHP;
	}

	~filter_Subhp() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value << _limit;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value >> _limit;
		return true;
	}

	void OnAttach()
	{
		if (_parent.GetExtendProp().max_hp < _value + _limit)
		{
			_value = _parent.GetExtendProp().max_hp - _limit;
		}
		if (_value <= 0)
		{
			_is_deleted = 1;
		}
		else
		{
			_parent.ImpairMaxHP(_value);
			_parent.UpdateHPMP();
		}
	}

	void OnDetach()
	{
		if (_value > 0)
		{
			_parent.EnhanceMaxHP(_value);
			_parent.UpdateHPMP();
		}
	}
};

class filter_Teamenchant : public filter
{
protected:
	int skillid;
	int skilllevel;
	char _force;
	float radius;
	int mc;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Teamenchant() {}
public:
	DECLARE_SUBSTANCE(filter_Teamenchant);

	filter_Teamenchant(
		object_interface object,
		int value,
		int ratio,
		float amount,
		char force,
		int buff_id) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_TEAMENCHANT;
		//_buff_id = HSTATE_TEAMENCHANT;  //这个类没有设置这个属性  haokaixin

		skillid = value;
		skilllevel = ratio;
		_force = force;
		radius = amount;
		mc = buff_id;
	}

	~filter_Teamenchant() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << skillid << skilllevel << _force << radius << mc;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> skillid >> skilllevel >> _force >> radius >> mc;
		return true;
	}

	void OnAttach()
	{
		if (_parent.IsInTeam())
		{
			const SkillStub* stub = SkillStub::GetStub(skillid);
			if (stub)
			{
				enchant_msg msg;
				memset(&msg, 0, sizeof(msg));
				msg.attack_range = radius;
				if (msg.attack_range <= 0.0)
					msg.attack_range = _parent.GetExtendProp().attack_range;
				msg.skill = skillid;
				msg.skill_level = skilllevel;
				msg.force_attack = _force;
				msg.helpful = stub->type == 2;
				if (stub->GetTargettype())
					_parent.TeamEnchantZombie(msg, 0, 0, mc);
				else
					_parent.TeamEnchant(msg, 0, 0);
			}
			else
			{
				_is_deleted = 1;
			}
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
	}
};

//35
class filter_Immunesleep : public timeout_filter
{
protected:
	int _value;
	int _limit;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Immunesleep() {}
public:
	DECLARE_SUBSTANCE(filter_Immunesleep);

	filter_Immunesleep(
		object_interface object,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_IMMUNESLEEP;
		_buff_id = HSTATE_IMMUNESLEEP;
	}

	~filter_Immunesleep() {}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNESLEEP); 
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNESLEEP);
	}
};

class filter_Decaccuracy : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decaccuracy() {}
public:
	DECLARE_SUBSTANCE(filter_Decaccuracy);

	filter_Decaccuracy(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK), _ratio(r)
	{
		_filter_id = buffid + FILTER_DECACCURACY;
		_buff_id = HSTATE_DECACCURACY;
	}

	~filter_Decaccuracy() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairScaleAttack(_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceScaleAttack(_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Skillreflect : public timeout_filter
{
protected:
	int reflectProb;
	int reflectCnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_REFLECTED | FILTER_MASK_HEARTBEAT
	};

	filter_Skillreflect() {}
public:
	DECLARE_SUBSTANCE(filter_Skillreflect);

	filter_Skillreflect(
		object_interface object,
		int prob,
		int value,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_SKILLREFLECT;
		_buff_id = HSTATE_SKILLREFLECT;
		this->reflectProb = prob;
		this->reflectCnt = value;
	}

	~filter_Skillreflect() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << reflectProb << reflectCnt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> reflectProb >> reflectCnt;
		return true;
	}

	void OnAttach()
	{
		_parent.SetSkillReflect( 1, this->reflectProb);
	}

	void OnDetach()
	{
		_parent.SetSkillReflect(0, this->reflectProb);
	}
};

class filter_Wraptimer : public timeout_filter
{
protected:
	int _cycle;
	int _duration ;
	int _counter;
	int _active;
	int _buffcnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Wraptimer() {}
public:
	DECLARE_SUBSTANCE(filter_Wraptimer);

	filter_Wraptimer(
		object_interface object,
		int duration,
		int cycle,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_WRAPTIMER;
		_buff_id = HSTATE_WRAPTIMER;
		this->_cycle = cycle;
		this->_duration = duration;
		this->_counter = 0;
		this->_active = 0;
		this->_buffcnt = 0;
	}

	~filter_Wraptimer() {}

	void Heartbeat(int tick)
	{
		if (!this->_counter)
		{
			_parent.SetRootSeal(1);
			_parent.SendClientNotifyRoot(2);
			_parent.UpdateBuff(HSTATE_WRAP,_filter_id, filter::GetTimeout(),1);
			_parent.IncVisibleState(VSTATE_WRAP);
			this->_active = 1;
		}
		this->_counter += tick;
		if (this->_active)
			this->_buffcnt += tick;
		if (this->_buffcnt >= this->_cycle && this->_active)
		{
			this->_buffcnt = 0;
			this->_active = 0;
			_parent.SetRootSeal( 0);
			_parent.SendClientDispelRoot( 2);
			_parent.RemoveBuff(5, _filter_id);
			_parent.DecVisibleState(VSTATE_WRAP);
		}
		if (this->_counter >= this->_duration)
			this->_counter = 0;
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _cycle << _duration << _counter << _active << _buffcnt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _cycle >> _duration >> _counter >> _active >> _buffcnt;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		if (this->_active)
		{
			_parent.SetRootSeal(0);
			_parent.SendClientDispelRoot( 2);
			_parent.RemoveBuff(5, _filter_id);
			_parent.DecVisibleState(VSTATE_WRAP);
		}
	}
};

class filter_Returnpos : public filter
{
protected:
	int skillid;
	int skilllevel;
	char _force;
	float radius;
	int mc;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Returnpos() {}
public:
	DECLARE_SUBSTANCE(filter_Returnpos);

	filter_Returnpos(
		object_interface object) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_RETURNPOS;
		//_buff_id = HSTATE_RETURNPOS;  //这个类没有设置这个属性  haokaixin
	}

	~filter_Returnpos() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		int tag = _parent.GetSkillWrapper().GetRecordMapId();
		A3DVECTOR rpos = _parent.GetSkillWrapper().GetRecordPos();
		A3DVECTOR pos = _parent.GetPos();
		float dist = sqrtf(rpos.squared_distance(pos));
		if (dist * dist <= 10000.0)
		{
			if (tag == _parent.GetTag())
			{
				XID empty;
				if (_parent.CanCharge(empty,rpos,3,dist))
					_parent.Charge(empty, rpos, 2);
			}
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
	}
};

//40
class filter_Paralysis : public timeout_filter
{
protected:
	int praySpeedRatio;
	int speedRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Paralysis() {}
public:
	DECLARE_SUBSTANCE(filter_Paralysis);

	filter_Paralysis(
		object_interface object,
		int period,
		int ratio,
		int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->praySpeedRatio = ratio;
		this->speedRatio = value;
		_filter_id = FILTER_PARALYSIS;
		_buff_id = HSTATE_PARALYSIS;
	}

	~filter_Paralysis() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << praySpeedRatio << speedRatio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> praySpeedRatio >> speedRatio;
		return true;
	}

	void OnAttach()
	{
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.ImpairScaleSpeed(speedRatio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.EnhanceScaleSpeed(speedRatio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Autoresurrect : public timeout_filter
{
protected:
	float exp_ratio;
	float hp_ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_AFTER_DEATH |  FILTER_MASK_HEARTBEAT
	};

	filter_Autoresurrect() {}
public:
	DECLARE_SUBSTANCE(filter_Autoresurrect);

	filter_Autoresurrect(
		object_interface object,
		int period,
		float value,
		float ratio) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_AUTORESURRECT;
		_buff_id = HSTATE_AUTORES;
		this->exp_ratio = ratio;
		this->hp_ratio = value;
	}

	~filter_Autoresurrect() {}

	void AfterDeath(bool is_duel)
	{
		_parent.Resurrect(this->exp_ratio, this->hp_ratio);
		_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << exp_ratio << hp_ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> exp_ratio >> hp_ratio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Inchpgen : public timeout_filter
{
protected:
	int _inc;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Inchpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Inchpgen);

	filter_Inchpgen(
		object_interface object,
		int inc,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_INCHPGEN;
		_buff_id = HSTATE_INCHPGEN;
		this->_inc = inc;
	}

	~filter_Inchpgen() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _inc;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _inc;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleHPGen(_inc);
		_parent.UpdateHPMPGen();
	}

	void OnDetach()
	{
		_parent.ImpairScaleHPGen(_inc);
		_parent.UpdateHPMPGen();
	}
};

class filter_Vipguilin : public filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE
	};

	filter_Vipguilin() {}
public:
	DECLARE_SUBSTANCE(filter_Vipguilin);

	filter_Vipguilin(
		object_interface object,
		int ratio,
		int r,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;
		_filter_id = buffid + FILTER_VIPGUILIN;
		_buff_id = HSTATE_VIPGUILIN;
	}

	~filter_Vipguilin() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDefense(_ratio);
		_parent.UpdateDefenseData();
		_parent.EnhanceDefense(_addend);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDefense(_ratio);
		_parent.UpdateDefenseData();
		_parent.ImpairDefense(_addend);
		_parent.UpdateDefenseData();
	}
};

class filter_Incskilldamage : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Incskilldamage() {}
public:
	DECLARE_SUBSTANCE(filter_Incskilldamage);

	filter_Incskilldamage(
		object_interface object,
		float ratio,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_INCSKILLDAMAGE;
		_buff_id = HSTATE_INCSKILLDAMAGE;
		this->_ratio = ratio;
	}

	~filter_Incskilldamage() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

//45
class filter_Mirrorimage : public timeout_filter
{
protected:
	int mirrorCnt;
	int genMirrorInterval;
	int currMirrorCnt;
	int counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_RECV_MSG
	};

	filter_Mirrorimage() {}
public:
	DECLARE_SUBSTANCE(filter_Mirrorimage);

	filter_Mirrorimage(
		object_interface object,
		int period,
		int value,
		int amount) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_MIRRORIMAGE;
		_buff_id = HSTATE_MIRRORIMAGE;
		this->mirrorCnt = value;
		this->genMirrorInterval = amount;
		this->currMirrorCnt = value;
		this->counter = amount;
	}

	~filter_Mirrorimage() {}

	void Heartbeat(int tick)
	{
		this->counter -= tick;
		if (this->counter <= 0)
		{
			if (this->currMirrorCnt < this->mirrorCnt)
				_parent.SendMirrorImageCnt( ++this->currMirrorCnt);
			this->counter = this->genMirrorInterval;
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << mirrorCnt << genMirrorInterval << currMirrorCnt << counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> mirrorCnt >> genMirrorInterval >> currMirrorCnt >> counter;
		return true;
	}

	void OnAttach()
	{
		_parent.SendMirrorImageCnt(this->mirrorCnt);
	}

	void OnDetach()
	{
		_parent.SendMirrorImageCnt(0);
	}
};

class filter_Addcharm : public filter
{
protected:
	int _prob;
	int _skill_id;
	int _skill_level;
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Addcharm() {}
public:
	DECLARE_SUBSTANCE(filter_Addcharm);

	filter_Addcharm(
		object_interface object,
		int prob,
		int value,
		int amount,
		int ratio) : filter(object, FILTER_MASK)
	{
		this->_prob = prob;
		this->_skill_id = value;
		this->_skill_level = amount;
		this->_addend = ratio;
		_filter_id = FILTER_ADDCHARM;
		_buff_id = HSTATE_ADDCHARM;
	}

	~filter_Addcharm() {}

	void TranslateSendAttack(const XID& target,attack_msg& atkmsg,char type,float radius,int mc)
	{
		if (rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->_skill_id;
			msg.skill_level = this->_skill_level;
			msg.force_attack = atkmsg.force_attack;
			int stype = SkillWrapper::GetType(this->_skill_id);
			msg.attack_range = _parent.GetExtendProp().attack_range;
			msg.skill_var[0] = _parent.GetExtendProp().resistance[1];
			msg.skill_var[1] = _parent.GetExtendProp().resistance[2];
			msg.skill_var[2] = _parent.GetExtendProp().resistance[3];
			msg.skill_var[3] = _parent.GetExtendProp().resistance[0];
			if (stype == 2)
				msg.helpful = 1;
			std::vector<exclude_target> empty;
			if (type == 1)
			{
				_parent.RegionEnchant1(_parent.GetPos(), radius, msg, mc, empty);
			}
			else if (type > 1)
			{
				if (type == 2)
				{
					_parent.RegionEnchant2(_parent.GetPos(), radius, msg, mc, empty);
				}
				else if (type == 3)
				{
					_parent.RegionEnchant3(_parent.GetPos(), radius, msg, mc, empty);
				}
			}
			else if (!type)
			{
				_parent.Enchant(target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _prob << _skill_id << _skill_level << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _prob >> _skill_id >> _skill_level >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_ADDWEAK);
		_parent.RemoveFilter(FILTER_ADDWRAP);
		_parent.RemoveFilter(FILTER_ADDSLOW);
		_parent.EnhanceResistance( 3, this->_addend);
		_parent.UpdateMagicData();
		_parent.IncVisibleState(VSTATE_ADDCHARM);
	}

	void OnDetach()
	{
		_parent.ImpairResistance( 3, this->_addend);
		_parent.UpdateMagicData();
		_parent.DecVisibleState(VSTATE_ADDCHARM);
	}
};

class filter_Deccrithurt : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Deccrithurt() {}
public:
	DECLARE_SUBSTANCE(filter_Deccrithurt);

	filter_Deccrithurt(
		object_interface object,
		float r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_DECCRITHURT;
		_buff_id = HSTATE_DECCRITHURT;
		this->_ratio = r;
	}

	~filter_Deccrithurt() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairCritDmg(this->_ratio);
	}

	void OnDetach()
	{
		_parent.EnhanceCritDmg(this->_ratio);
	}
};

class filter_Decdamage : public timeout_filter
{
protected:
	float _ratio;
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Decdamage() {}
public:
	DECLARE_SUBSTANCE(filter_Decdamage);

	filter_Decdamage(
		object_interface object,
		float ratio,
		int prob,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_DECDAMAGE;
		_buff_id = HSTATE_DECDAMAGE;
		this->_ratio = ratio;
		this->_prob = prob;
	}

	~filter_Decdamage() {}

	void AdjustDamage(damage_entry& dmg, const XID& attacker, const attack_msg& msg)
	{
		if (rand() % 100 < this->_prob)
			dmg.damage = dmg.damage - (this->_ratio * dmg.damage);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio << _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Familyincattack : public timeout_filter
{
protected:
	int _ratio;
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Familyincattack() {}
public:
	DECLARE_SUBSTANCE(filter_Familyincattack);

	filter_Familyincattack(
		object_interface object,
		int r,
		int v,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_FAMILYINCATTACK;
		_buff_id = HSTATE_FAMILYINCATTACK;
		this->_ratio = r;
		this->_value = v;
	}

	~filter_Familyincattack() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio << _value;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage( this->_ratio);
		_parent.EnhanceDamage( this->_value);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage( this->_ratio);
		_parent.ImpairDamage( this->_value);
		_parent.UpdateAttackData();
	}
};

//50
class filter_Incfatalratio : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incfatalratio() {}
public:
	DECLARE_SUBSTANCE(filter_Incfatalratio);

	filter_Incfatalratio(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_INCFATALRATIO;
		_buff_id = HSTATE_INCFATALRATIO;
		this->_ratio = r;
	}

	~filter_Incfatalratio() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceAntiCritRate( -this->_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiCritRate(this->_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Activeonfilteradd : public timeout_filter
{
protected:
	int _param;
	int skill_id;
	int skill_level;
	bool self;
	char _force;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ON_FILTER_ADD | FILTER_MASK_HEARTBEAT
	};

	filter_Activeonfilteradd() {}
public:
	DECLARE_SUBSTANCE(filter_Activeonfilteradd);

	filter_Activeonfilteradd(
		object_interface object,
		int period,
		int prob,
		int ratio,
		int value,
		int amount,
		int buff_id,
		char force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_param = ratio;
		this->skill_id = value;
		this->skill_level = amount;
		this->_force = force;
		_filter_id = buff_id + FILTER_ACTIVE;
		_buff_id = HSTATE_ACTIVE;
		this->self = prob == 0;
	}

	~filter_Activeonfilteradd() {}

	void EnchantSkill(const XID& target,bool help)
	{
		enchant_msg msg;
		memset(&msg, 0, sizeof(msg));
		msg.skill = this->skill_id;
		msg.skill_level = this->skill_level;
		msg.force_attack = this->_force;
		msg.helpful = help;
		msg.attack_range = _parent.GetExtendProp().attack_range;
		msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
		_parent.Enchant(target, msg);
	}
	
	void OnFilterAdd(int filter_id,const XID& caster)
	{
		if (filter_id > 4300)
			filter_id = 10 * ((filter_id - 4300) / 10) + 4300;
		if (filter_id == this->_param)
		{
			if (self)
			{
				XID target = _parent.GetSelfID();
				filter_Activeonfilteradd::EnchantSkill(target, self);
			}
			else
			{
				filter_Activeonfilteradd::EnchantSkill(caster, 0);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _param << skill_id << skill_level << self << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _param >> skill_id >> skill_level >> self >> _force;
		return true;
	}

	void OnAttach()
	{
		if (this->self && (this->_param > MULTI_FILTER_BEGIN 
			&& _parent.IsMultiFilterExist((_param - MULTI_FILTER_BEGIN) / 10)
			|| _parent.IsFilterExist(_param)))
		{
			filter_Activeonfilteradd::EnchantSkill(_parent.GetSelfID(), 1);
		}
	}

	void OnDetach()
	{
	}
};

class filter_Crazy : public timeout_filter
{
protected:
	int _hcost;
	int _mcost;
	int _addon;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Crazy() {}
public:
	DECLARE_SUBSTANCE(filter_Crazy);

	filter_Crazy(
		object_interface object,
		int hcost,
		int mcost,
		int addon,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_CRAZY;
		_buff_id = HSTATE_CRAZY;
		this->_hcost = hcost;
		this->_mcost = mcost;
		this->_addon = addon;
	}

	~filter_Crazy() {}

	void TranslateSendAttack(const XID& target,attack_msg* const msg,char type,float radius,int mc)
	{
		if (_parent.GetBasicProp().hp <= this->_hcost
			&& _parent.GetBasicProp().mp <= this->_mcost)
		{
			_is_deleted = 1;
		}
		else
		{
			if (_parent.GetBasicProp().hp > this->_hcost)
				_parent.DecHP(this->_hcost);
			if (_parent.GetBasicProp().mp > this->_mcost)
				_parent.DrainMana( this->_mcost);
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _hcost << _mcost << _addon;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _hcost >> _mcost >> _addon;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDamage( this->_addon);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairDamage( this->_addon);
		_parent.UpdateAttackData();
	}
};

class filter_Adddefence : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Adddefence() {}
public:
	DECLARE_SUBSTANCE(filter_Adddefence);

	filter_Adddefence(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		_filter_id = buffid + FILTER_ADDDEFENCE;
		_buff_id = HSTATE_ADDDEFENCE;
	}

	~filter_Adddefence() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDefense(_addend);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairDefense(_addend);
		_parent.UpdateDefenseData();
	}
};

class filter_Hupo : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hupo() {}
public:
	DECLARE_SUBSTANCE(filter_Hupo);

	filter_Hupo(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_ADDDEFENCE;
		_buff_id = HSTATE_ADDDEFENCE;  
		this->_addend = r;
		this->_ratio = ratio;
	}

	~filter_Hupo() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.EnhanceDamage(_addend);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.ImpairDamage(_addend);
		_parent.UpdateAttackData();
	}
};

//55
class filter_Immuneweak : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Immuneweak() {}
public:
	DECLARE_SUBSTANCE(filter_Immuneweak);

	filter_Immuneweak(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_IMMUNEWEAK;
		_buff_id = HSTATE_IMMUNEWEAK;
	}

	~filter_Immuneweak() {}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNEWEAK); 
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNEWEAK);
	}
};

class filter_Guishen2debuff : public timeout_filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int crit;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Guishen2debuff() {}
public:
	DECLARE_SUBSTANCE(filter_Guishen2debuff);

	filter_Guishen2debuff(object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->crit = buffid;
		_filter_id = FILTER_GUISHEN2DEBUFF;
		_buff_id = HSTATE_GUISHEN2;
	}

	~filter_Guishen2debuff() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << crit;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> crit;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_GUISHEN1);
		_parent.RemoveFilter(FILTER_GUISHEN2);
		_parent.RemoveFilter(FILTER_TUOLING1);
		_parent.RemoveFilter(FILTER_TUOLING2);
		_parent.RemoveFilter(FILTER_GUISHEN1DEBUFF);
		_parent.RemoveFilter(FILTER_TUOLING1DEBUFF);
		_parent.RemoveFilter(FILTER_TUOLING2DEBUFF);
		_parent.ScalePlayer(0, this->scaleRatio);
		_parent.EnhanceScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		praySpeedRatio = this->praySpeedRatio;
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.EnhanceCrit(this->crit);
	}

	void OnDetach()
	{
		_parent.ScalePlayer( 1, this->scaleRatio);
		_parent.ImpairScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		praySpeedRatio = this->praySpeedRatio;
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.ImpairCrit(this->crit);
	}
};

class filter_Hpleak1 : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpleak1() {}
public:
	DECLARE_SUBSTANCE(filter_Hpleak1);

	filter_Hpleak1(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;
		_timeout = damage / this->_damage;

		_filter_id = buffid + FILTER_HPLEAK1;
		_buff_id = HSTATE_HPLEAK1;
	}

	~filter_Hpleak1() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.BeHurt(_performer,_pinfo,damage,_invader,_mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 1)
		{
			this->_damage /= 2;
			this->_addtion /= 2;
		}
		if (this->_damage <= 1)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.push_back(&_pinfo,sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _damage >> _addtion << _performer << _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPLEAK);
	}
};

class filter_Subantiweak : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantiweak() {}
public:
	DECLARE_SUBSTANCE(filter_Subantiweak);

	filter_Subantiweak(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = v;
		_filter_id = buffid + FILTER_SUBANTIWEAK;
		_buff_id = HSTATE_SUBANTIWEAK;
	}

	~filter_Subantiweak() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(1, this->_sub);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(1, this->_sub);
		_parent.UpdateMagicData();
	}
};

class filter_Dechurt2 : public timeout_filter
{
protected:
	float _ratio;
	int _max;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Dechurt2() {}
public:
	DECLARE_SUBSTANCE(filter_Dechurt2);

	filter_Dechurt2(
		object_interface object,
		float ratio,
		int max,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_DECHURT2;
		_buff_id = HSTATE_DECHURT2;
		this->_ratio = ratio;
		this->_max = max;
	}

	~filter_Dechurt2() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		int dec = dmg.damage * this->_ratio;
		if (dec >= this->_max)
		{
			_is_deleted = 1;
			dec = this->_max;
		}
		dmg.damage = dmg.damage - dec;
		this->_max -= dec;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _max;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _max;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_DECHURT2);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_DECHURT2);
	}
};

//60
class filter_Incskillaccu : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incskillaccu() {}
public:
	DECLARE_SUBSTANCE(filter_Incskillaccu);

	filter_Incskillaccu(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = FILTER_DECHURT2;
		_buff_id = HSTATE_DECHURT2;
	}

	~filter_Incskillaccu() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceSkillAttack(_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceSkillAttack( -this->_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Summonskill : public filter
{
protected:
	int skillid;
	int skilllevel;
	int summonid;
	XID _target;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Summonskill() {}
public:
	DECLARE_SUBSTANCE(filter_Summonskill);

	filter_Summonskill(
		object_interface object,
		int value,
		int amount,
		int ratio,
		const XID& target) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_SUMMONSKILL;
		//_buff_id = HSTATE_SUMMONSKILL;
		this->summonid = value;
		this->skillid = amount;
		this->skilllevel = ratio;
		this->_target.type = target.type;
		this->_target.id = target.id;
	}

	~filter_Summonskill() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << skillid << skilllevel << summonid << _target;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> skillid >> skilllevel >> summonid >> _target;
		return true;
	}

	void OnAttach()
	{
		_parent.SummonCastSkill(summonid,_target,skillid,skilllevel);
	}

	void OnDetach()
	{
	}
};

class filter_Immunesilent : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Immunesilent() {}
public:
	DECLARE_SUBSTANCE(filter_Immunesilent);

	filter_Immunesilent(
		object_interface object,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_IMMUNESILENT;
		_buff_id = HSTATE_IMMUNESILENT;
	}

	~filter_Immunesilent() {}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNESILENT);
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNESILENT);
	}
};

class filter_Juniordarkform : public filter
{
protected:
	int inc_dark_light;
	int dec_dark_skill_cd;
	int flag;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Juniordarkform() {}
public:
	DECLARE_SUBSTANCE(filter_Juniordarkform);

	filter_Juniordarkform(
		object_interface object,
		float ratio,
		int value) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_JUNIORDARKFORM;
		_buff_id = HSTATE_JUNIORDARK;
		this->inc_dark_light = value;
		this->dec_dark_skill_cd = (int)ratio;
		this->flag = 0;
	}

	~filter_Juniordarkform() {}

	void Heartbeat(int tick)
	{
		_parent.IncDarkLight( this->inc_dark_light);
		if ((int)_parent.GetDarkLight() >= 0)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << inc_dark_light << dec_dark_skill_cd << flag;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> inc_dark_light >> dec_dark_skill_cd >> flag;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_JUNIORLIGHTFORM);
		_parent.RemoveFilter(FILTER_SENIORDARKFORM);
		_parent.RemoveFilter(FILTER_SENIORLIGHTFORM);
		_parent.RemoveFilter(FILTER_DARKUNIFORM);
		_parent.RemoveFilter(FILTER_LIGHTUNIFORM);
		_parent.SetDarkLightForm(1);
		if (_parent.GetDarkLight() < 0)
		{
			dec_dark_skill_cd = this->dec_dark_skill_cd;
			 _parent.GetSkillWrapper().DecDarkSkillCoolTime(dec_dark_skill_cd);
			this->flag = 1;
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		_parent.ClearDarkLightForm();
		if (this->flag)
		{
			_parent.GetSkillWrapper().IncDarkSkillCoolTime(dec_dark_skill_cd);
		}
	}
};

class filter_Crazycurse : public timeout_filter
{
protected:
	int _hcost;
	int _mcost;
	XID _performer;
	attacker_info_t _pinfo;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Crazycurse() {}
public:
	DECLARE_SUBSTANCE(filter_Crazycurse);

	filter_Crazycurse(
		object_interface object,
		int hcost,
		int mcost,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_hcost = hcost;
		this->_mcost = mcost;  
		_filter_id = FILTER_CRAZYCURSE;
		_buff_id = HSTATE_CRAZY;
	}

	~filter_Crazycurse() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void TranslateSendAttack(const XID& target,attack_msg& msg,char type,float r,int mc)
	{
		_parent.BeHurt(this->_performer,this->_pinfo,this->_hcost,this->_invader,this->_mode);
		if (_parent.GetBasicProp().mp <= this->_mcost)
		{
			_parent.DrainMana(_parent.GetBasicProp().mp);
		}
		else
		{
			_parent.DrainMana(_mcost);
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _hcost << _mcost << _performer;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _hcost >> _mcost >> _performer;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

//65
class filter_Puppetform : public timeout_filter
{
protected:
	int _num;
	int _need_mp;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_HEARTBEAT 
	};

	filter_Puppetform() {}
public:
	DECLARE_SUBSTANCE(filter_Puppetform);

	filter_Puppetform(
		object_interface object,
		int period,
		int num,
		int mp) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_num = num;
		this->_need_mp = mp;
		_filter_id = FILTER_CRAZYCURSE;
		_buff_id = HSTATE_CRAZY;
	}

	~filter_Puppetform() {}

	void Heartbeat(int tick)
	{
		object_interface* v2; // [esp-Ch] [ebp-14h]

		if (_parent.GetBasicProp().mp >= this->_need_mp)
			_parent.DrainMana(_need_mp);
		else
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _num << _need_mp;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _num >> _need_mp;
		return true;
	}

	void OnAttach()
	{
		_parent.SetPuppetForm( 1, this->_num);
		_parent.ChangeShape(6);
	}

	void OnDetach()
	{
		_parent.SetPuppetForm( 0, this->_num);
		_parent.ChangeShape( 0);
	}
};

class filter_Sharelifewithsummon : public timeout_filter
{
protected:
	float _shareRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Sharelifewithsummon() {}
public:
	DECLARE_SUBSTANCE(filter_Sharelifewithsummon);

	filter_Sharelifewithsummon(
		object_interface object,
		float ratio,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_shareRatio = ratio;
		_filter_id = FILTER_SHARELIFEWITHSUMMON;
		_buff_id = HSTATE_SHARELIFEWITHSUMMON;
	}

	~filter_Sharelifewithsummon() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		float summondmg = dmg.damage * this->_shareRatio;
		dmg.damage = dmg.damage - summondmg;
		_parent.DecHP(_parent.GetSummonID(), dmg.damage);  //第二个参数不明  猜   haokaixin
	}

	virtual bool Save(archive& ar)   //这个类也没有调用父类   haokaixin
	{
		//timeout_filter::Save(ar);
		ar << _shareRatio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//timeout_filter::Load(ar);
		ar >> _shareRatio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Gtmpgen : public filter
{
protected:
	int _health;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT
	};

	filter_Gtmpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Gtmpgen);

	filter_Gtmpgen(
		object_interface object,
		int period,
		int health,
		int buffid) : filter(object, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_GTMPGEN;
		_buff_id = HSTATE_GTMPGEN;
		this->_counter = 0;
		this->_health = health / period;
		if (this->_health <= 0)
			this->_health = 1;
	}

	~filter_Gtmpgen() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1)
		{
			_parent.InjectMana(this->_counter * this->_health, 0);
			this->_counter -= 2;
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _health << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _health >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_MPGEN);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_MPGEN);
	}
};

class filter_Silenttimer : public timeout_filter
{
protected:
	int _cycle;
	int _duration;
	int _counter;
	int _active;
	int _buffcnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Silenttimer() {}
public:
	DECLARE_SUBSTANCE(filter_Silenttimer);

	filter_Silenttimer(
		object_interface object,
		int period,
		int cycle,
		int duration) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_cycle = cycle;
		this->_duration = duration;
		this->_counter = 0;
		this->_active = 0;
		this->_buffcnt = 0;
		_filter_id = FILTER_SILENTTIMER;
		_buff_id = HSTATE_SILENTTIMER;
	}

	~filter_Silenttimer() {}

	void Heartbeat(int tick)
	{
		if (!this->_counter)
		{
			_parent.SetSilentSeal(1);
			_parent.UpdateBuff(4, _filter_id,this->_cycle,1);
			_parent.IncVisibleState(VSTATE_SILENT);
			this->_active = 1;
		}
		this->_counter += tick;
		if (this->_active)
			this->_buffcnt += tick;
		if (this->_buffcnt >= this->_cycle && this->_active)
		{
			_parent.SetSilentSeal(0);
			_parent.RemoveBuff(4, _filter_id);
			_parent.DecVisibleState(VSTATE_SILENT);
			this->_active = 0;
			this->_buffcnt = 0;
		}
		if (this->_counter >= this->_duration)
			this->_counter = 0;
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _cycle << _duration << _counter << _active;
		return true;
	}

	virtual bool Load(archive& ar)  //此类  Load和Save反编译显示  不匹配
	{
		timeout_filter::Load(ar);
		ar >> _cycle >> _duration >> _counter >>_active;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		if (this->_active)
		{
			_parent.SetSilentSeal(0);
			_parent.RemoveBuff( 4, _filter_id);
			_parent.DecVisibleState(VSTATE_SILENT);
		}
	}
};

class filter_Subattack : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subattack() {}
public:
	DECLARE_SUBSTANCE(filter_Subattack);

	filter_Subattack(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = r;
		_filter_id = buffid + FILTER_SUBATTACK;
		_buff_id = HSTATE_SUBATTACK;
	}

	~filter_Subattack() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)  
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairDamage(this->_sub);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceDamage(this->_sub);
		_parent.UpdateAttackData();
	}
};

//70
class filter_Crippleddebuff : public timeout_filter
{
protected:
	int dec_skill_lvl;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Crippleddebuff() {}
public:
	DECLARE_SUBSTANCE(filter_Crippleddebuff);

	filter_Crippleddebuff(
		object_interface object,
		int period,
		int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->dec_skill_lvl = value;
		_filter_id = FILTER_CRIPPLEDDEBUFF;
		_buff_id = HSTATE_CRIPPLED;
	}

	~filter_Crippleddebuff() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << dec_skill_lvl;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> dec_skill_lvl;
		return true;
	}

	void OnAttach()
	{
		_parent.GetSkillWrapper().SetDecSkillLevel( dec_skill_lvl);
	}

	void OnDetach()
	{
		_parent.GetSkillWrapper().SetDecSkillLevel( 0);
	}
};

class filter_Familyincmp : public timeout_filter
{
protected:
	int _ratio;
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Familyincmp() {}
public:
	DECLARE_SUBSTANCE(filter_Familyincmp);

	filter_Familyincmp(
		object_interface object,
		int r,
		int v,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		this->_value = v;
		_filter_id = FILTER_FAMILYINCMP;
		_buff_id = HSTATE_FAMILYINCMP;
	}

	~filter_Familyincmp() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMaxMP(this->_ratio);
		_parent.EnhanceMaxMP(this->_value);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairScaleMaxMP(this->_ratio);
		_parent.ImpairMaxMP(this->_value);
		_parent.UpdateHPMP();
	}
};

class filter_Subdefence : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subdefence() {}
public:
	DECLARE_SUBSTANCE(filter_Subdefence);

	filter_Subdefence(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = r;
		_filter_id = buffid + FILTER_SUBDEFENCE;
		_buff_id = HSTATE_SUBDEFENCE;
	}

	~filter_Subdefence() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairDefense( this->_sub);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.EnhanceDefense(this->_sub);
		_parent.UpdateDefenseData();
	}
};

class filter_Makecrit : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Makecrit() {}
public:
	DECLARE_SUBSTANCE(filter_Makecrit);

	filter_Makecrit(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_MAKECRIT;
		_buff_id = HSTATE_MAKECRIT;
	}

	~filter_Makecrit() {}

	void TranslateSendAttack(const XID& target,attack_msg& msg,char type,float r,int mc)
	{
		msg.crit_rate = 10000;
		_is_deleted = 1;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Addattack3 : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addattack3() {}
public:
	DECLARE_SUBSTANCE(filter_Addattack3);

	filter_Addattack3(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		_filter_id = buffid + FILTER_ADDATTACK3;
		_buff_id = HSTATE_ADDATTACK3;
	}

	~filter_Addattack3() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDamage(_addend);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairDamage(_addend);
		_parent.UpdateAttackData();
	}
};

//75
class filter_Cursed : public timeout_filter
{
protected:
	int _maxcount;
	int _count;
	int _damage;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Cursed() {}
public:
	DECLARE_SUBSTANCE(filter_Cursed);

	filter_Cursed(
		object_interface object,
		int damage,
		int max,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_maxcount = max;
		this->_count = 0;
		this->_damage = damage;
		_filter_id = FILTER_CURSED;
		_buff_id = HSTATE_CURSED;
	}

	~filter_Cursed() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		dmg.damage = (++this->_count * this->_damage) + dmg.damage;
		if (this->_count >= this->_maxcount)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _maxcount << _count << _damage;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _maxcount >> _count >> _damage;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Rejectdebuff : public timeout_filter
{
protected:
	int _probability;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Rejectdebuff() {}
public:
	DECLARE_SUBSTANCE(filter_Rejectdebuff);

	filter_Rejectdebuff(
		object_interface object,
		int probability,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_probability = probability;
		_filter_id = FILTER_REJECTDEBUFF;
		_buff_id = HSTATE_REJECTDEBUFF;
	}

	~filter_Rejectdebuff() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _probability;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _probability;
		return true;
	}

	void OnAttach()
	{
		_parent.GetSkillWrapper().SetRejectRate(_probability);
	}

	void OnDetach()
	{
		_parent.GetSkillWrapper().SetRejectRate(0);
	}
};

class filter_Huju : public filter
{
protected:
	int _attack_ratio;
	int _defend_ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE
	};

	filter_Huju() {}
public:
	DECLARE_SUBSTANCE(filter_Huju);

	filter_Huju(
		object_interface object,
		int attack_ratio,
		int defend_ratio) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_HUJU;
		_buff_id = HSTATE_HUJU;
		this->_attack_ratio = attack_ratio;
		this->_defend_ratio = defend_ratio;
	}

	~filter_Huju() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _attack_ratio << _defend_ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _attack_ratio >> _defend_ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairScaleDamage(_attack_ratio);
		_parent.UpdateAttackData();
		_parent.EnhanceScaleDefense(_defend_ratio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.EnhanceScaleDamage(_attack_ratio);
		_parent.UpdateAttackData();
		_parent.ImpairScaleDefense(_defend_ratio);
		_parent.UpdateDefenseData();
	}
};

class filter_Subantidizzy : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantidizzy() {}
public:
	DECLARE_SUBSTANCE(filter_Subantidizzy);

	filter_Subantidizzy(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = v;
		_filter_id = buffid + FILTER_SUBANTIDIZZY;
		_buff_id = HSTATE_SUBANTIDIZZY;
	}

	~filter_Subantidizzy() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance( 0, this->_sub);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance( 0, this->_sub);
		_parent.UpdateMagicData();
	}
};

class filter_Adddodge2 : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Adddodge2() {}
public:
	DECLARE_SUBSTANCE(filter_Adddodge2);

	filter_Adddodge2(
		object_interface object,
		int inc,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = inc;
		_filter_id = buffid + FILTER_ADDDODGE2;
		_buff_id = HSTATE_ADDDODGE2;
	}

	~filter_Adddodge2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceArmor(_addend);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairArmor(_addend);
		_parent.UpdateDefenseData();
	}
};

//80
class filter_Subantisleep : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantisleep() {}
public:
	DECLARE_SUBSTANCE(filter_Subantisleep);

	filter_Subantisleep(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = v;
		_filter_id = buffid + FILTER_SUBANTISLEEP;
		_buff_id = HSTATE_SUBANTISLEEP;
	}

	~filter_Subantisleep() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(4, this->_sub);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(4, this->_sub);
		_parent.UpdateMagicData();
	}
};

class filter_Activebecrit : public timeout_filter
{
protected:
	int _prob;
	int skill_id;
	int skill_level;
	bool self;
	char _force;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED | FILTER_MASK_HEARTBEAT
	};

	filter_Activebecrit() {}
public:
	DECLARE_SUBSTANCE(filter_Activebecrit);

	filter_Activebecrit(
		object_interface object,
		int period,
		int prob,
		int value,
		int amount,
		int buffid,
		int force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_prob = prob;
		this->skill_id = value;
		this->skill_level = amount;
		this->_force = force;
		this->self = buffid == 0;
		_filter_id = FILTER_ACTIVEBECRIT;
		_buff_id = HSTATE_ACTIVE;
	}

	~filter_Activebecrit() {}

	void BeAttacked(const XID& target,int dmg,bool crit)
	{
		if (crit && rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->skill_id;
			msg.skill_level = this->skill_level;
			msg.force_attack = this->_force;
			msg.helpful = this->self;
			msg.attack_range = _parent.GetExtendProp().attack_range;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
			if (this->self)
			{
				_parent.Enchant(_parent.GetSelfID(), msg);
			}
			else
			{
				_parent.Enchant(target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _prob << skill_id << skill_level << self << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _prob >> skill_id >> skill_level >> self >> _force;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Subantislow : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantislow() {}
public:
	DECLARE_SUBSTANCE(filter_Subantislow);

	filter_Subantislow(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = v;
		_filter_id = buffid + FILTER_SUBANTISLOW;
		//_buff_id = HSTATE_SUBANTISLOW;
	}

	~filter_Subantislow() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(5, this->_sub);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(5, this->_sub);
		_parent.UpdateMagicData();
	}
};

class filter_Sharelifewithclone : public timeout_filter
{
protected:
	float shareRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Sharelifewithclone() {}
public:
	DECLARE_SUBSTANCE(filter_Sharelifewithclone);

	filter_Sharelifewithclone(
		object_interface object,
		float ratio,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->shareRatio = ratio;
		_filter_id = FILTER_SHARELIFEWITHCLONE;
		_buff_id = HSTATE_SHARELIFEWITHCLONE;
	}

	~filter_Sharelifewithclone() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		float  clonedmg = dmg.damage * shareRatio;
		dmg.damage = dmg.damage - clonedmg;
		_parent.DecHP(_parent.GetCloneID(), clonedmg);   //这里第二个参数还是不清楚 反汇编是_parent  haokaixin
	}

	virtual bool Save(archive& ar)
	{
		//timeout_filter::Save(ar);
		ar << shareRatio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//timeout_filter::Load(ar);
		ar >> shareRatio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Xinmo : public timeout_filter
{
protected:
	int _addend;
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Xinmo() {}
public:
	DECLARE_SUBSTANCE(filter_Xinmo);

	filter_Xinmo(
		object_interface object,
		float ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;
		_filter_id = buffid + FILTER_XINMO;
		_buff_id = HSTATE_XINMO;
	}

	~filter_Xinmo() {}

	void TranslateSendAttack(const XID& target,attack_msg& msg,char type,float r,int mc)
	{
		if (msg.skill_id > 0 && msg.damage_factor > (long double)this->_ratio)
			msg.damage_factor = msg.damage_factor - this->_ratio;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairDefense(_addend);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.EnhanceDefense(_addend);
		_parent.UpdateDefenseData();
	}
};

//85
class filter_Addcommon : public timeout_filter
{
protected:
	int _prob;
	int _skill_id;
	int _skill_level;
	int _var[16];

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Addcommon() {}
public:
	DECLARE_SUBSTANCE(filter_Addcommon);

	filter_Addcommon(
		object_interface object,
		int period,
		int prob,
		int value,
		int amount,
		int buffid,
		int* var) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_prob = prob;
		this->_skill_id = value;
		this->_skill_level = amount;
		memcpy(this->_var, var, sizeof(this->_var));
		_filter_id = buffid + FILTER_ADDCOMMON;
		_buff_id = HSTATE_ADDCOMMON;
	}

	~filter_Addcommon() {}

	void TranslateSendAttack(const XID& target, attack_msg& atkmsg, char type, float radius, int mc)
	{
		if (rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->_skill_id;
			msg.skill_level = this->_skill_level;
			msg.force_attack = atkmsg.force_attack;
			int stype = SkillWrapper::GetType(this->_skill_id);
			msg.attack_range = _parent.GetExtendProp().attack_range;
			if (stype == 2)
				msg.helpful = 1;
			memcpy(msg.skill_var, this->_var, sizeof(msg.skill_var));
			std::vector<exclude_target> empty;
			if (type == 1)
			{
				_parent.RegionEnchant1( _parent.GetPos(), radius, msg, mc, empty);
			}
			else if (type > 1)
			{
				if (type == 2)
				{
					_parent.RegionEnchant2(_parent.GetPos(), radius, msg, mc, empty);
				}
				else if (type == 3)
				{
					_parent.RegionEnchant3(_parent.GetPos(), radius, msg, mc, empty);
				}
			}
			else if (!type)
			{
				_parent.Enchant(target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _prob << _skill_id >> _skill_level;
		ar.push_back(_var, sizeof(_var));
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _prob >> _skill_id >> _skill_level;
		ar.pop_back(_var, sizeof(_var));
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_ADDCOMMON);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_ADDCOMMON);
	}
};

class filter_Delaycast : public timeout_filter
{
protected:
	int _skillid;
	int _skilllevel;
	int _force;
	XID _performer;
	attacker_info_t _pinfo;
	int _var[16];

	bool _invader;
	char _mode;
	int _attacker_faction;
	int _target_faction;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Delaycast() {}
public:
	DECLARE_SUBSTANCE(filter_Delaycast);

	filter_Delaycast(
		object_interface object,
		int period,
		int value,
		int ratio,
		int buffid,
		char force,
		int* var) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_skillid = value;
		this->_skilllevel = ratio;
		this->_force = force;
		memcpy(this->_var, var, sizeof(this->_var));

		_filter_id = buffid + FILTER_DELAYCAST;
		_buff_id = HSTATE_DELAYCAST;
	}

	~filter_Delaycast() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader,
		int attacker_faction,
		int target_faction)
	{
		int id; // edx

		id = player.id;
		this->_performer.type = player.type;
		this->_performer.id = id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
		this->_attacker_faction = attacker_faction;
		this->_target_faction = target_faction;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _skillid << _skilllevel >> _force >> _performer;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _attacker_faction >> _target_faction;
		ar.push_back(&_var, sizeof(_var));
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _skillid >> _skilllevel >> _force >> _performer;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _attacker_faction >> _target_faction;
		ar.push_back(&_var, sizeof(_var));
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		enchant_msg msg;
		memset(&msg, 0, sizeof(msg));
		msg.skill = this->_skillid;
		msg.skill_level = this->_skilllevel;
		msg.force_attack = this->_force;
		int type = SkillWrapper::GetType(this->_skillid);
		msg.attack_range = _parent.GetExtendProp().attack_range;
		memcpy(msg.skill_var, this->_var, sizeof(msg.skill_var));
		if (type == 2)
			msg.helpful = 1;
		if (msg.helpful)
		{
			_parent.Enchant(_parent.GetSelfID(), msg);
		}
		else
		{
			memcpy(&msg.ainfo, &this->_pinfo, sizeof(this->_pinfo));
			msg.is_invader = this->_invader;
			msg.attacker_mode = this->_mode;
			msg.attacker_faction = this->_attacker_faction;
			msg.target_faction = this->_target_faction;
			_parent.SendEnchantMsg(_parent.GetSelfID(), msg);
		}
	}
};

class filter_Addspirit : public filter
{
protected:
	int spirit_type;
	int spirit_cnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Addspirit() {}
public:
	DECLARE_SUBSTANCE(filter_Addspirit);

	filter_Addspirit(
		object_interface object,
		int ratio,
		int amount) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_ADDSPIRIT;
		//_buff_id = HSTATE_ADDSPIRIT;
		this->spirit_type = ratio;
		this->spirit_cnt = amount;
	}

	~filter_Addspirit() {}

	virtual bool Save(archive& ar)   //反编译发现没有保存东西  haokaixin
	{
		filter::Save(ar);
		//ar << spirit_type << spirit_cnt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		//ar >> spirit_type >> spirit_cnt;
		return true;
	}

	void OnAttach()
	{
		for (int i = 0; i < this->spirit_cnt; ++i)
			_parent.AddDarkLightSpirit( this->spirit_type);
	}

	void OnDetach()
	{
	}
};

class filter_Longxiang : public filter
{
protected:
	int _attack_ratio;
	int _defend_ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE 
	};

	filter_Longxiang() {}
public:
	DECLARE_SUBSTANCE(filter_Longxiang);

	filter_Longxiang(
		object_interface object,
		int attack_ratio,
		int defend_ratio) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_LONGXIANG;
		_buff_id = HSTATE_LONGXIANG;
		this->_attack_ratio = attack_ratio;
		this->_defend_ratio = defend_ratio;
	}

	~filter_Longxiang() {}

	virtual bool Save(archive& ar)   
	{
		filter::Save(ar);
		ar << _attack_ratio << _defend_ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _attack_ratio >> _defend_ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(this->_attack_ratio);
		_parent.UpdateAttackData();
		_parent.ImpairScaleDefense( this->_defend_ratio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(this->_attack_ratio);
		_parent.UpdateAttackData();
		_parent.EnhanceScaleDefense( this->_defend_ratio);
		_parent.UpdateDefenseData();
	}
};

class filter_Subhp2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subhp2() {}
public:
	DECLARE_SUBSTANCE(filter_Subhp2);

	filter_Subhp2(
		object_interface object,
		int value,
		int period,
		int amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;
		_can_overlay = 1;
		_max_overlay_cnt = amount;

		_filter_id = FILTER_SUBHP2;
		_buff_id = HSTATE_SUBHP2;
	}

	~filter_Subhp2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		if (_parent.GetExtendProp().max_hp < this->_value)
			this->_value = _parent.GetExtendProp().max_hp;
		if (this->_value <= 0)
		{
			_is_deleted = 1;
		}
		else
		{
			_parent.ImpairMaxHP( this->_value);
			_parent.UpdateHPMP();
		}
	}

	void OnDetach()
	{
		if (this->_value > 0)
		{
			_parent.EnhanceMaxHP(this->_value);
			_parent.UpdateHPMP();
		}
	}
};

//90
class filter_Inchp2 : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Inchp2() {}
public:
	DECLARE_SUBSTANCE(filter_Inchp2);

	filter_Inchp2(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_INCHP2;
		_buff_id = HSTATE_INCHP2;
	}

	~filter_Inchp2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMaxHP(this->_ratio);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairScaleMaxHP( this->_ratio);
		_parent.UpdateHPMP();
	}
};

class filter_Instantskill : public timeout_filter
{
protected:
	int prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Instantskill() {}
public:
	DECLARE_SUBSTANCE(filter_Instantskill);

	filter_Instantskill(
		object_interface object,
		int period,
		int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->prob = value;
		_filter_id = FILTER_INSTANTSKILL;
		_buff_id = HSTATE_INSTANTSKILL;
	}

	~filter_Instantskill() {}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrlen)
	{
		if (ctrlname == 11 && ctrlen == 1)
			*(unsigned char*)ctrlval = rand() % 100 < this->prob;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Hpleak6 : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpleak6() {}
public:
	DECLARE_SUBSTANCE(filter_Hpleak6);

	filter_Hpleak6(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;
		_timeout = damage / this->_damage;

		_filter_id = FILTER_INSTANTSKILL;
		_buff_id = HSTATE_INSTANTSKILL;
	}

	~filter_Hpleak6() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.BeHurt(_performer,_pinfo,damage,_invader,_mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat( tick);
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrlen)
	{
		if (ctrlname == 1)
		{
			this->_damage /= 2;
			this->_addtion /= 2;
		}
		if (this->_damage <= 1)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _damage >> _addtion >> _performer >> _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPLEAK);
	}
};

class filter_Guishen1 : public timeout_filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int hpRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Guishen1() {}
public:
	DECLARE_SUBSTANCE(filter_Guishen1);

	filter_Guishen1(
		object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->hpRatio = buffid;

		_filter_id = FILTER_GUISHEN1;
		_buff_id = HSTATE_GUISHEN1;
	}

	~filter_Guishen1() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << hpRatio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> hpRatio;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(CLASS_CREDITBOOST);
		_parent.RemoveFilter(CLASS_DECSKILLDODGE);
		_parent.RemoveFilter(CLASS_ADDATTACK2);
		_parent.ScalePlayer( 0, this->scaleRatio);
		_parent.EnhanceScaleDamage( this->atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.EnhanceScaleMaxHP(this->hpRatio);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ScalePlayer(1, this->scaleRatio);
		_parent.ImpairScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().DecPrayTime( praySpeedRatio);
		_parent.ImpairScaleMaxHP(this->hpRatio);
		_parent.UpdateHPMP();
	}
};

class filter_Blessed : public timeout_filter
{
protected:
	int _count;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_TRANSLATE_ENCHANT | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_RECV_MSG
	};

	filter_Blessed() {}
public:
	DECLARE_SUBSTANCE(filter_Blessed);

	filter_Blessed(object_interface object, int n, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_count = n;
		_filter_id = FILTER_GUISHEN1;
		_buff_id = HSTATE_GUISHEN1;
	}

	~filter_Blessed() {}

	void TranslateRecvAttack(const XID& attacker,attack_msg& msg)
	{
		if (msg.ainfo.team_id <= 0
			|| msg.ainfo.team_id != _parent.GetTeamLeader()
			|| attacker.id == _parent.GetDuelTargetID())
		{
			if (--this->_count <= 0)
				_is_deleted = 1;
			msg.attack_state |= IMMUNEWEAK;
		}
	}

	void TranslateRecvEnchant(const XID& attacker,enchant_msg& msg)
	{
		if (!msg.helpful
			&& (msg.ainfo.team_id <= 0
				|| msg.ainfo.team_id != _parent.GetTeamLeader()
				|| attacker.id == _parent.GetDuelTargetID()))
		{
			if (--this->_count <= 0)
				_is_deleted = 1;
			msg.attack_state |= IMMUNEWEAK;
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _count;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _count;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_BLESSED);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_BLESSED);
	}
};

//95
class filter_Stateexchangewithclone : public filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE
	};

	filter_Stateexchangewithclone() {}
public:
	DECLARE_SUBSTANCE(filter_Stateexchangewithclone);

	filter_Stateexchangewithclone(
		object_interface object) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_STATEEXCHANGEWITHCLONE;
		//_buff_id = HSTATE_STATEEXCHANGEWITHCLONE;
	}

	~filter_Stateexchangewithclone() {}

	virtual bool Save(archive& ar)
	{
		return true;
	}

	virtual bool Load(archive& ar)
	{
		return true;
	}

	void OnAttach()
	{
		_parent.ExchangeStatus(_parent.GetCloneID());
	}

	void OnDetach()
	{
	}
};

class filter_Chihun : public timeout_filter
{
protected:
	int _hpgen_ratio;
	int _attack_ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_RECV_MSG
	};

	filter_Chihun() {}
public:
	DECLARE_SUBSTANCE(filter_Chihun);

	filter_Chihun(
		object_interface object,
		int period,
		int attack_ratio,
		int hpgen_ratio,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_hpgen_ratio = hpgen_ratio;
		this->_attack_ratio = attack_ratio;
		_filter_id = buffid + FILTER_CHIHUN;
		_buff_id = HSTATE_CHIHUN;
	}

	~filter_Chihun() {}

	void TranslateRecvAttack(const XID& attacker, attack_msg& msg)
	{
		msg.crit_rate = 0;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _hpgen_ratio << _attack_ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _hpgen_ratio >> _attack_ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMaxHP(_hpgen_ratio);
		_parent.UpdateHPMP();
		_parent.EnhanceScaleDamage(_attack_ratio);
		_parent.UpdateAttackData();
		_parent.SetImmuneMask(IMMUNESLEEP);
		_parent.SetImmuneMask(IMMUNESILENT);
		_parent.SetImmuneMask(IMMUNEWRAP);
		_parent.SetImmuneMask(IMMUNEWEAK);
		_parent.SetImmuneMask(IMMUNEDIZZY);
		_parent.SetImmuneMask(IMMUNESLOW);
		_parent.SetImmuneMask(IMMUNEHPLEAK);
		_parent.SetImmuneMask(IMMUNEMPLEAK);
		_parent.SetImmuneMask(IMMUNEMPDISPERSE);
		_parent.SetImmuneMask(IMMUNEDIET);
		_parent.SetImmuneMask(IMMUNEPULL);
		_parent.SetImmuneMask(IMMUNEFROZE);
	}

	void OnDetach()
	{
		_parent.ImpairScaleMaxHP(_hpgen_ratio);
		_parent.UpdateHPMP();
		_parent.ImpairScaleDamage(_attack_ratio);
		_parent.UpdateAttackData();
		_parent.ClearImmuneMask(IMMUNESLEEP);
		_parent.ClearImmuneMask(IMMUNESILENT);
		_parent.ClearImmuneMask(IMMUNEWRAP);
		_parent.ClearImmuneMask(IMMUNEWEAK);
		_parent.ClearImmuneMask(IMMUNEDIZZY);
		_parent.ClearImmuneMask(IMMUNESLOW);
		_parent.ClearImmuneMask(IMMUNEHPLEAK);
		_parent.ClearImmuneMask(IMMUNEMPLEAK);
		_parent.ClearImmuneMask(IMMUNEMPDISPERSE);
		_parent.ClearImmuneMask(IMMUNEDIET);
		_parent.ClearImmuneMask(IMMUNEPULL);
		_parent.ClearImmuneMask(IMMUNEFROZE);
	}
};

class filter_Pulling : public timeout_filter
{
protected:
	XID _target;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Pulling() {}
public:
	DECLARE_SUBSTANCE(filter_Pulling);

	filter_Pulling(
		object_interface object,
		int period,
		const XID& target) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_target.type = target.type;
		this->_target.id = target.id;
		_filter_id = FILTER_PULLING;
		_buff_id = HSTATE_PULLING;
	}

	~filter_Pulling() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _target;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _target;
		return true;
	}

	void OnAttach()
	{
		_parent.SetSilentSeal(1);
		_parent.SetMeleeSeal(1);
		_parent.SetRootSeal(1);
		_parent.SendClientNotifyRoot(18);
		_parent.PlayerPulling(1);
	}

	void OnDetach()
	{
		_parent.SetSilentSeal(0);
		_parent.SetMeleeSeal(0);
		_parent.SetRootSeal(0);
		_parent.SendClientDispelRoot(18);
		_parent.PlayerPulling(0);
		_parent.NotifyCancelPull(_target);
	}
};

class filter_Cycle : public timeout_filter
{
protected:
	int skillTickCnt;
	int sid;
	int slevel;
	int _counter;
	float _radius;
	char _force;

	int startSkillId;
	//std::vector<XID> cycleMembers;  //我就感觉这玩意儿没有使用过 haokaixin

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Cycle() {}
public:
	DECLARE_SUBSTANCE(filter_Cycle);

	filter_Cycle(
		object_interface object,
		int period,
		int value,
		int amount,
		int ratio,
		float radius,
		int buffid,
		char force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->skillTickCnt = value;
		this->sid = amount;
		this->slevel = ratio;
		this->_radius = radius;
		this->_force = force;
		this->startSkillId = buffid;
		this->_counter = this->skillTickCnt;

		_filter_id = FILTER_CYCLE;
		_buff_id = HSTATE_CYCLE;
	}

	~filter_Cycle() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter >= this->skillTickCnt)
		{
			this->_counter = 0;
			std::vector<XID> list;
			_parent.GetPlayerInCircleOfDoom(list);
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->sid;
			msg.skill_level = this->slevel;
			msg.force_attack = this->_force;
			msg.helpful = 1;
			msg.attack_range = this->_radius;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
			for (int i = 0; i < list.size(); ++i)
			{
				_parent.Enchant(list[i], msg);
			}
			_parent.Enchant(_parent.GetSelfID(), msg);
			timeout_filter::Heartbeat(tick);
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << skillTickCnt << sid << slevel << _counter << _radius << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> skillTickCnt >> sid >> slevel >> _counter >> _radius >> _force;
		return true;
	}

	void OnAttach()
	{
		_parent.CircleOfDoomStartup();
		if (this->startSkillId > 0)
		{
			std::vector<XID> list;
			_parent.GetPlayerInCircleOfDoom(list);
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->startSkillId;
			msg.skill_level = 1;
			msg.force_attack = this->_force;
			msg.helpful = 1;
			msg.attack_range = this->_radius;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
			for (int i = 0; i < list.size(); ++i)
			{
				_parent.Enchant(list[i], msg);
			}
			_parent.Enchant(_parent.GetSelfID(), msg);
		}
	}

	void OnDetach()
	{
		_parent.CircleOfDoomStop();
	}
};

class filter_Barhpgen : public filter
{
protected:
	int _health;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT
	};

	filter_Barhpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Barhpgen);

	filter_Barhpgen(
		object_interface object,
		int period,
		int health,
		int buffid) : filter(object, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_BARHPGEN;
		_buff_id = HSTATE_BARHPGEN;
		this->_counter = 0;
		this->_health = health / period;
		if (this->_health <= 0)
			this->_health = 1;
	}

	~filter_Barhpgen() {}

	void Heartbeat(int tick)  //这里没有调用父类  haokaixin
	{
		this->_counter += tick;
		if (this->_counter > 1)
		{
			_parent.Heal(this->_counter * this->_health, 0, 0);
			this->_counter -= 2;
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _health << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _health >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPGEN);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPGEN);
	}
};

//100
class filter_Incskilllevel : public filter
{
protected:
	int sid[3];
	int inclevel;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_SKILL | FILTER_MASK_HEARTBEAT
	};

	filter_Incskilllevel() {}
public:
	DECLARE_SUBSTANCE(filter_Incskilllevel);

	filter_Incskilllevel(
		object_interface object,
		int period,
		int value,
		int amount,
		int ratio,
		int buffid) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_INCSKILLLEVEL;
		_buff_id = HSTATE_INCSKILLLEVEL;
		this->inclevel = buffid;
		this->sid[0] = value;
		this->sid[1] = amount;
		this->sid[2] = ratio;
	}

	~filter_Incskilllevel() {}

	void AdjustSkill(int skill_id, short& level)
	{
		for (int i = 0; i <= 2; ++i)
		{
			if (this->sid[i] == skill_id)
			{
				level += inclevel;
				printf("Adjust Level inc to level: %d\n", level);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar.push_back(sid, sizeof(sid));
		ar << inclevel;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar.pop_back(sid, sizeof(sid));
		ar >> inclevel;
		return true;
	}

	void OnAttach()
	{
		if (this->inclevel < 0)
			_is_deleted = 1;
	}

	void OnDetach()
	{
	}
};

class filter_Addwrap : public filter
{
protected:
	int _prob;
	int _skill_id;
	int _skill_level;
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Addwrap() {}
public:
	DECLARE_SUBSTANCE(filter_Addwrap);

	filter_Addwrap(
		object_interface object,
		int prob,
		int value,
		int amount,
		int ratio) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_ADDWRAP;
		_buff_id = HSTATE_ADDWRAP;
		this->_prob = prob;
		this->_skill_id = value;
		this->_skill_level = amount;
		this->_addend = ratio;
	}

	~filter_Addwrap() {}

	void TranslateSendAttack(
		const XID& target,attack_msg& atkmsg,
		char type,float radius,int mc)
	{
		if (rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->_skill_id;
			msg.skill_level = this->_skill_level;
			msg.force_attack = atkmsg.force_attack;
			msg.attack_range = _parent.GetExtendProp().attack_range;
			msg.skill_var[0] = _parent.GetExtendProp().resistance[1];
			msg.skill_var[1] = _parent.GetExtendProp().resistance[2];
			msg.skill_var[2] = _parent.GetExtendProp().resistance[3];
			msg.skill_var[3] = _parent.GetExtendProp().resistance[0];
			if (SkillWrapper::GetType(this->_skill_id) == 2)
				msg.helpful = 1;
			std::vector<exclude_target> empty;
			if (type == 1)
			{
				_parent.RegionEnchant1( _parent.GetPos(), radius, msg, mc, empty);
			}
			else if (type > 1)
			{
				if (type == 2)
				{
					_parent.RegionEnchant2(_parent.GetPos(), radius, msg, mc, empty);
				}
				else if (type == 3)
				{
					_parent.RegionEnchant3( _parent.GetPos(), radius,msg, mc, empty);
				}
			}
			else if (!type)
			{
				_parent.Enchant( target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _prob << _skill_id << _skill_level << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _prob >> _skill_id >> _skill_level >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_BLESSED);
		_parent.RemoveFilter(FILTER_ADDCHARM);
		_parent.RemoveFilter(FILTER_ADDSLOW);
		_parent.EnhanceResistance( 2, this->_addend);
		_parent.UpdateMagicData();
		_parent.IncVisibleState(VSTATE_ADDWRAP);
	}

	void OnDetach()
	{
		_parent.ImpairResistance(2, this->_addend);
		_parent.UpdateMagicData();
		_parent.DecVisibleState(VSTATE_ADDWRAP);
	}
};

class filter_Addattack2 : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addattack2() {}
public:
	DECLARE_SUBSTANCE(filter_Addattack2);

	filter_Addattack2(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		_filter_id = buffid + FILTER_ADDATTACK2;
		_buff_id = HSTATE_ADDATTACK2;
	}

	~filter_Addattack2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDamage(this->_addend);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairDamage(this->_addend);
		_parent.UpdateAttackData();
	}
};

class filter_Deccritrate : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Deccritrate() {}
public:
	DECLARE_SUBSTANCE(filter_Deccritrate);

	filter_Deccritrate(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_DECCRITRATE;
		_buff_id = HSTATE_DECCRITRATE;
	}

	~filter_Deccritrate() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairCrit(this->_ratio);
	}

	void OnDetach()
	{
		_parent.EnhanceCrit(this->_ratio);
	}
};

class filter_Inchp : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Inchp() {}
public:
	DECLARE_SUBSTANCE(filter_Inchp);

	filter_Inchp(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_INCHP;
		_buff_id = HSTATE_INCHP;
	}

	~filter_Inchp() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMaxHP(this->_ratio);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairScaleMaxHP(this->_ratio);
		_parent.UpdateHPMP();
	}
};

//105
class filter_Decskilldodge : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decskilldodge() {}
public:
	DECLARE_SUBSTANCE(filter_Decskilldodge);

	filter_Decskilldodge(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_DECSKILLDODGE;
		_buff_id = HSTATE_DECSKILLDODGE;
	}

	~filter_Decskilldodge() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceSkillArmor( -this->_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceSkillArmor( this->_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Incattack3 : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incattack3() {}
public:
	DECLARE_SUBSTANCE(filter_Incattack3);

	filter_Incattack3(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_INCATTACK3;
		_buff_id = HSTATE_INCATTACK3;
	}

	~filter_Incattack3() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Activecrit : public timeout_filter
{
protected:
	int _prob;
	int skill_id;
	int skill_level;
	bool self;
	char _force;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_CRITENEMY | FILTER_MASK_HEARTBEAT
	};

	filter_Activecrit() {}
public:
	DECLARE_SUBSTANCE(filter_Activecrit);

	filter_Activecrit(
		object_interface object,
		int period,
		int prob,
		int value,
		int amount,
		int buffid,
		char force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_prob = prob;
		this->skill_id = value;
		this->skill_level = amount;
		this->_force = force;  
		this->self = buffid == 0;

		_filter_id = buffid +  FILTER_ACTIVEBECRIT;  //为啥是这个  不应该是这个吗 FILTER_ACTIVECRIT   haokaixin
		_buff_id = HSTATE_ACTIVE;
	}

	~filter_Activecrit() {}

	void CritEnemy(const XID& target)
	{
		if (rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->skill_id;
			msg.skill_level = this->skill_level;
			msg.force_attack = this->_force;
			msg.helpful = this->self;
			msg.attack_range = _parent.GetExtendProp().attack_range;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
			if (this->self)
			{
				_parent.Enchant(_parent.GetSelfID(), msg);
			}
			else
			{
				_parent.Enchant(target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _prob << skill_id << skill_level << self << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _prob >> skill_id >> skill_level >> self >> _force;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Fly : public timeout_filter
{
protected:
	float speed;
	int darklight;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Fly() {}
public:
	DECLARE_SUBSTANCE(filter_Fly);

	filter_Fly(object_interface object, int period, float value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->speed = value;
		this->darklight = 0;

		_filter_id = FILTER_FLY;  
		_buff_id = HSTATE_FLY;
	}

	~filter_Fly() {}

	void Heartbeat(int tick)
	{
	}

	virtual bool Save(archive& ar)   //没有保存darklight  haokaixin
	{
		timeout_filter::Save(ar);
		ar << speed;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> speed;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceOverrideSpeed( this->speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.ActiveFlyState(1);
		this->darklight = _parent.GetDarkLight();
		if (this->darklight < 0)
			_parent.IncVisibleState(VSTATE_SENIORDARK);
		else
			_parent.IncVisibleState(VSTATE_SENIORLIGHT);
	}

	void OnDetach()
	{
		_parent.ImpairOverrideSpeed( this->speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.DeactiveFlyState( 1);
		if (this->darklight < 0)
			_parent.DecVisibleState(VSTATE_SENIORDARK);
		else
			_parent.DecVisibleState(VSTATE_SENIORLIGHT);
	}
};

class filter_Pull : public timeout_filter
{
protected:
	float speed;
	float stop_dist;
	XID _performer;
	attacker_info_t _pinfo;
	bool _invader;
	char _mode;
	bool _error;
	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Pull() {}
public:
	DECLARE_SUBSTANCE(filter_Pull);

	filter_Pull(
		object_interface object,
		int period,
		float value,
		float amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->speed = value;
		this->stop_dist = amount;
		this->_error = 0;

		_filter_id = FILTER_PULL;
		_buff_id = HSTATE_PULL;
	}

	~filter_Pull() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		A3DVECTOR tpos;
		float tbody;
		if (_parent.QueryObject(this->_performer, tpos, tbody) == 1)
		{
			if (tpos.horizontal_distance(_parent.GetPos()) >= this->stop_dist * this->stop_dist)
				_parent.PlayerFollowTarget(_performer,speed,stop_dist);
			timeout_filter::Heartbeat(tick);
		}
	}

	virtual bool Save(archive& ar)  
	{
		timeout_filter::Save(ar);
		ar << speed << stop_dist << _performer;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode << _error;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> speed >> stop_dist >> _performer;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode >> _error;
		return true;
	}

	void OnAttach()
	{
		float tbody; // [esp+1Ch] [ebp-1Ch] BYREF
		A3DVECTOR tpos;
		if (_parent.QueryObject(this->_performer, tpos, tbody) == 1)
		{
			A3DVECTOR near;
			if (_parent.CollisionDetect(tpos,near,0.2))
			{
				_parent.SetSilentSeal(1);
				_parent.SetMeleeSeal(1);
				_parent.SetRootSeal(1);
				_parent.SendClientSelfNotifyRoot(17);
				_parent.PlayerFollowTarget(this->_performer,this->speed,this->stop_dist);
				_parent.PlayerBePulled(this->_performer, 1);
			}
			else
			{
				this->_error = 1;
				_is_deleted = 1;
			}
		}
		else
		{
			this->_error = 1;
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		if (!this->_error)
		{
			_parent.SetSilentSeal(0);
			_parent.SetMeleeSeal(0);
			_parent.SetRootSeal(0);
			_parent.SendClientDispelRoot(17);
			_parent.StopPlayerFollowTarget();
			_parent.PlayerBePulled(this->_performer, 0);
		}
	}
};

//110
class filter_Addhp : public timeout_filter
{
protected:
	float _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addhp() {}
public:
	DECLARE_SUBSTANCE(filter_Addhp);

	filter_Addhp(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;

		_filter_id = FILTER_PULL;
		_buff_id = HSTATE_PULL;
	}

	~filter_Addhp() {}

	virtual bool Save(archive& ar)   
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMaxHP(_addend);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairMaxHP(_addend);
		_parent.UpdateHPMP();
	}
};

class filter_Addantiweak : public timeout_filter
{
protected:
	float _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addantiweak() {}
public:
	DECLARE_SUBSTANCE(filter_Addantiweak);

	filter_Addantiweak(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = v;

		_filter_id = buffid + FILTER_ADDANTIWEAK;
		_buff_id = HSTATE_ADDANTIWEAK;
	}

	~filter_Addantiweak() {}

	virtual bool Save(archive& ar)  
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance(1, this->_addend);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance(1, this->_addend);
		_parent.UpdateMagicData();
	}
};

class filter_Incanti : public timeout_filter
{
protected:
	float _var;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incanti() {}
public:
	DECLARE_SUBSTANCE(filter_Incanti);

	filter_Incanti(object_interface object, int v, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_var = v;

		_filter_id = FILTER_INCANTI;
		_buff_id = HSTATE_INCANTI;
	}

	~filter_Incanti() {}

	virtual bool Save(archive& ar)   
	{
		timeout_filter::Save(ar);
		ar << _var;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _var;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleResistance( 0, this->_var);
		_parent.EnhanceScaleResistance( 1, this->_var);
		_parent.EnhanceScaleResistance( 2, this->_var);
		_parent.EnhanceScaleResistance( 3, this->_var);
		_parent.EnhanceScaleResistance( 4, this->_var);
		_parent.EnhanceScaleResistance( 5, this->_var);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleResistance(0, this->_var);
		_parent.ImpairScaleResistance(1, this->_var);
		_parent.ImpairScaleResistance(2, this->_var);
		_parent.ImpairScaleResistance(3, this->_var);
		_parent.ImpairScaleResistance(4, this->_var);
		_parent.ImpairScaleResistance(5, this->_var);
		_parent.UpdateMagicData();
	}
};

class filter_Familyinchp : public timeout_filter
{
protected:
	int _ratio;
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Familyinchp() {}
public:
	DECLARE_SUBSTANCE(filter_Familyinchp);

	filter_Familyinchp(
		object_interface object,
		int r,
		int v,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		this->_value = v;

		_filter_id = FILTER_FAMILYINCHP;
		_buff_id = HSTATE_FAMILYINCHP;
	}

	~filter_Familyinchp() {}

	virtual bool Save(archive& ar)  
	{
		timeout_filter::Save(ar);
		ar << _ratio << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMaxHP(_ratio);
		_parent.EnhanceMaxHP(_value);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairScaleMaxHP(_ratio);
		_parent.ImpairMaxHP(_value);
		_parent.UpdateHPMP();
	}
};

class filter_Accumdamage : public timeout_filter
{
protected:
	int _max_damage;
	int _acc_damage;
	float _ratio;
	XID _performer;
	attacker_info_t _pinfo;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED | FILTER_MASK_HEARTBEAT
	};

	filter_Accumdamage() {}
public:
	DECLARE_SUBSTANCE(filter_Accumdamage);

	filter_Accumdamage(
		object_interface object,
		float ratio,
		int max_damage,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_max_damage = max_damage;

		_filter_id = FILTER_ACCUMDAMAGE;
		_buff_id = HSTATE_ACCUMDAMAGE;
	}

	~filter_Accumdamage() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void BeAttacked(const XID& target,int dmg,bool crit)
	{
		this->_acc_damage += (int)((long double)dmg * this->_ratio);
		if (this->_acc_damage > this->_max_damage)
			this->_acc_damage = this->_max_damage;
	}

	virtual bool Save(archive& ar)   
	{
		timeout_filter::Save(ar);
		ar << _max_damage << _acc_damage << _ratio << _performer;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _max_damage >> _acc_damage >> _ratio >> _performer;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		_parent.BeHurt(_performer,_pinfo,_acc_damage,_invader,_mode);
	}
};

//115
class filter_Spicy : public timeout_filter
{
protected:
	int _hp;
	int _mp;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Spicy() {}
public:
	DECLARE_SUBSTANCE(filter_Spicy);

	filter_Spicy(object_interface object, int hp, int mp, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_hp = hp / period;
		this->_mp = mp / period;
		if (this->_hp <= 0)
			this->_hp = 1;
		if (this->_mp <= 0)
			this->_mp = 1;

		_filter_id = FILTER_SPICY;
		_buff_id = HSTATE_SPICY;
	}

	~filter_Spicy() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			if (_parent.GetBasicProp().hp > this->_hp * this->_counter)
				_parent.DecHP(this->_hp * this->_counter);
			_parent.InjectMana(this->_mp * this->_counter, 0);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)   
	{
		timeout_filter::Save(ar);
		ar << _hp << _mp << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _hp >> _mp >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Asfire : public filter
{
protected:
	int _add_attack;
	int _weak_resist;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE
	};

	filter_Asfire() {}
public:
	DECLARE_SUBSTANCE(filter_Asfire);

	filter_Asfire(
		object_interface object,
		int add_attack,
		int weak_resist) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_ASFIRE;
		_buff_id = HSTATE_ASFIRE;
		this->_add_attack = add_attack;
		this->_weak_resist = weak_resist;
	}

	~filter_Asfire() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _add_attack << _weak_resist;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _add_attack >> _weak_resist;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(4171);
		_parent.RemoveFilter(4172);
		_parent.RemoveFilter(4174);
		_parent.EnhanceDamage(_add_attack);
		_parent.UpdateAttackData();
		_parent.EnhanceResistance(1, _weak_resist);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairDamage(_add_attack);
		_parent.UpdateAttackData();
		_parent.ImpairResistance(1, _weak_resist);
		_parent.UpdateMagicData();
	}
};

class filter_Addslow : public filter
{
protected:
	int _prob;
	int _skill_id;
	int _skill_level;
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Addslow() {}
public:
	DECLARE_SUBSTANCE(filter_Addslow);

	filter_Addslow(
		object_interface object,
		int prob,
		int value,
		int amount,
		int ratio) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_ADDSLOW;
		_buff_id = HSTATE_ADDSLOW;
		this->_prob = prob;
		this->_skill_id = value;
		this->_skill_level = amount;
		this->_addend = ratio;
	}

	~filter_Addslow() {}

	void TranslateSendAttack(const XID& target,attack_msg& atkmsg,char type,float radius,int mc)
	{
		if (rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->_skill_id;
			msg.skill_level = this->_skill_level;
			msg.force_attack = atkmsg.force_attack;
			msg.attack_range = _parent.GetExtendProp().attack_range;
			if (SkillWrapper::GetType(this->_skill_id) == 2)
				msg.helpful = 1;
			msg.skill_var[0] = _parent.GetExtendProp().resistance[1];
			msg.skill_var[1] = _parent.GetExtendProp().resistance[2];
			msg.skill_var[2] = _parent.GetExtendProp().resistance[3];
			msg.skill_var[3] = _parent.GetExtendProp().resistance[0];

			std::vector<exclude_target> empty;
			if (type == 1)
			{
				_parent.RegionEnchant1(_parent.GetPos(), radius, msg, mc, empty);
			}
			else if (type > 1)
			{
				if (type == 2)
				{
					_parent.RegionEnchant2(_parent.GetPos(), radius, msg, mc, empty);
				}
				else if (type == 3)
				{
					_parent.RegionEnchant3(_parent.GetPos(), radius, msg, mc, empty);
				}
			}
			else if (!type)
			{
				_parent.Enchant(target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _prob << _skill_id << _skill_level << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _prob >> _skill_id >> _skill_level >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_ADDWEAK);
		_parent.RemoveFilter(FILTER_ADDWRAP);
		_parent.RemoveFilter(FILTER_ADDCHARM);
		_parent.EnhanceResistance(0, this->_addend);
		_parent.UpdateMagicData();
		_parent.IncVisibleState(VSTATE_ADDSLOW);
	}

	void OnDetach()
	{
		_parent.ImpairResistance(0, this->_addend);
		_parent.UpdateMagicData();
		_parent.DecVisibleState(VSTATE_ADDSLOW);
	}
};

class filter_Dechurt : public timeout_filter
{
protected:
	float _ratio;
	int _max;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Dechurt() {}
public:
	DECLARE_SUBSTANCE(filter_Dechurt);

	filter_Dechurt(
		object_interface object,
		float ratio,
		int max,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_max = max;

		_filter_id = FILTER_DECHURT;
		_buff_id = HSTATE_DECHURT;
	}

	~filter_Dechurt() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg* const msg)
	{
		int dec = (int)(dmg.damage * this->_ratio);
		if ((long double)dec > dmg.damage)
			dec = (int)dmg.damage;
		if (dec >= this->_max)
		{
			_is_deleted = 1;
			dec = this->_max;
		}
		dmg.damage = dmg.damage - (long double)dec;
		this->_max -= dec;
	}

	virtual bool Save(archive& ar) 
	{
		timeout_filter::Save(ar);
		ar << _ratio << _max;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _max;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(1);  //VSTATE_DECHURT ?  haokaixin
	}

	void OnDetach()
	{
		_parent.DecVisibleState(1);
	}
};

class filter_Douhun : public filter
{
protected:
	float _ratio_mp2hp;
	int _add_crit_rate;
	float _add_crit_hurt;
	float _discount;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_ADJUST_MANA_COST
	};

	filter_Douhun() {}
public:
	DECLARE_SUBSTANCE(filter_Douhun);

	filter_Douhun(
		object_interface object,
		float ratio_mp2hp,
		int add_crit_rate,
		float add_crit_hurt,
		float discount) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_DOUHUN;
		_buff_id = HSTATE_DOUHUN;
		this->_ratio_mp2hp = ratio_mp2hp;
		this->_add_crit_rate = add_crit_rate;
		this->_add_crit_hurt = add_crit_hurt;
		this->_discount = discount;
	}

	~filter_Douhun() {}

	void AdjustManaCost(int& mana)
	{
		int dechp = (int)(mana * this->_ratio_mp2hp * this->_discount);
		int hp = _parent.GetBasicProp().hp;
		if (dechp >= hp)
			dechp = hp - 1;
		mana -= (int)((long double)dechp / this->_discount);
		if (dechp > 0)
			_parent.DecHP(dechp);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio_mp2hp << _add_crit_rate << _add_crit_hurt << _discount;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio_mp2hp >> _add_crit_rate >> _add_crit_hurt >> _discount;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_GANGDAN);  //4170 ? FILTER_DOUHUN = 4169  haokaixin
		_parent.EnhanceCrit(_add_crit_rate);
		_parent.EnhanceCritDmg(_add_crit_hurt);
	}

	void OnDetach()
	{
		_parent.ImpairCrit(_add_crit_rate);
		_parent.ImpairCritDmg(_add_crit_hurt);
	}
};

//120
class filter_Addantidizzy : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addantidizzy() {}
public:
	DECLARE_SUBSTANCE(filter_Addantidizzy);

	filter_Addantidizzy(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = v;

		_filter_id = buffid + FILTER_ADDANTIDIZZY;
		_buff_id = HSTATE_ADDANTIDIZZY;
	}

	~filter_Addantidizzy() {}

	virtual bool Save(archive& ar) 
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance( 0, this->_addend);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance( 0, this->_addend);
		_parent.UpdateMagicData();
	}
};

class filter_Aoshi : public timeout_filter
{
protected:
	int _addend;
	int _addgen;
	int _ratio;
	int _amount;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Aoshi() {}
public:
	DECLARE_SUBSTANCE(filter_Aoshi);

	filter_Aoshi(
		object_interface object,
		int ratio,
		int amount,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;
		this->_amount = amount;
		this->_counter = 0;

		_filter_id = buffid + FILTER_AOSHI;
		_buff_id = HSTATE_AOSHI;

		this->_addgen = r / period;
		if (this->_addgen <= 0)
			this->_addgen = 1;
	}

	~filter_Aoshi() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int addhp = this->_counter * this->_addend;
			if (_timeout <= tick)
				addhp += this->_amount;
			_parent.Heal(addhp, 0, 0);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar) 
	{
		timeout_filter::Save(ar);
		ar << _addend << _addgen << _ratio << _amount << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _addgen >> _ratio >> _amount >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
		_parent.EnhanceScaleDefense(this->_ratio);
		_parent.UpdateDefenseData();
		_parent.EnhanceScaleMaxHP(this->_ratio);
		_parent.UpdateHPMP();
		_parent.EnhanceScaleMaxMP(this->_ratio);
		_parent.UpdateHPMP();
		_parent.EnhanceDamage(this->_addend);
		_parent.UpdateAttackData();
		_parent.EnhanceDefense(this->_addend);
		_parent.UpdateDefenseData();
		_parent.EnhanceMaxHP(this->_amount);
		_parent.UpdateHPMP();
		_parent.EnhanceMaxMP(this->_amount);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
		_parent.ImpairScaleDefense(this->_ratio);
		_parent.UpdateDefenseData();
		_parent.ImpairScaleMaxHP(this->_ratio);
		_parent.UpdateHPMP();
		_parent.ImpairScaleMaxMP(this->_ratio);
		_parent.UpdateHPMP();
		_parent.ImpairDamage(this->_addend);
		_parent.UpdateAttackData();
		_parent.ImpairDefense(this->_addend);
		_parent.UpdateDefenseData();
		_parent.ImpairMaxHP(this->_amount);
		_parent.UpdateHPMP();
		_parent.ImpairMaxMP(this->_amount);
		_parent.UpdateHPMP();
	}
};

class filter_Adddodge : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Adddodge() {}
public:
	DECLARE_SUBSTANCE(filter_Adddodge);

	filter_Adddodge(
		object_interface object,
		int inc,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = inc;

		_filter_id = buffid + FILTER_ADDDODGE;
		_buff_id = HSTATE_ADDDODGE;
	}

	~filter_Adddodge() {}

	virtual bool Save(archive& ar)  
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceArmor(_addend);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairArmor(_addend);
		_parent.UpdateDefenseData();
	}
};

class filter_Focusanti : public timeout_filter
{
protected:
	int _var[5];

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Focusanti() {}
public:
	DECLARE_SUBSTANCE(filter_Focusanti);

	filter_Focusanti(
		object_interface object,
		float ratio,
		int type,
		int value,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		int sum = 0;
		for (int i = 0; i <= 4; ++i)
		{
			this->_var[i] = _parent.GetExtendProp().resistance[i];
			sum += this->_var[i];
			this->_var[i] -= value;
		}
		this->_var[type % 5] += value - (int)((long double)sum * ratio);

		_filter_id = FILTER_FOCUSANTI;
		_buff_id = HSTATE_FOCUSANTI;
	}

	~filter_Focusanti() {}

	virtual bool Save(archive& ar)   
	{
		timeout_filter::Save(ar);
		ar << _var[0] << _var[1] << _var[2] << _var[3] << _var[4];
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _var[0] >> _var[1] >> _var[2] >> _var[3] >> _var[4];
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(0, this->_var[0]);
		_parent.ImpairResistance(1, this->_var[1]);
		_parent.ImpairResistance(2, this->_var[2]);
		_parent.ImpairResistance(3, this->_var[3]);
		_parent.ImpairResistance(4, this->_var[4]);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(0, this->_var[0]);
		_parent.EnhanceResistance(1, this->_var[1]);
		_parent.EnhanceResistance(2, this->_var[2]);
		_parent.EnhanceResistance(3, this->_var[3]);
		_parent.EnhanceResistance(4, this->_var[4]);
		_parent.UpdateMagicData();
	}
};

class filter_Addanti2 : public timeout_filter
{
protected:
	int _var;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addanti2() {}
public:
	DECLARE_SUBSTANCE(filter_Addanti2);

	filter_Addanti2(object_interface object, int v, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_var = v;

		_filter_id = FILTER_ADDANTI2;
		_buff_id = HSTATE_ADDANTI2;
	}

	~filter_Addanti2() {}

	virtual bool Save(archive& ar)   
	{
		timeout_filter::Save(ar);
		ar << _var;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _var;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance(0, this->_var);
		_parent.EnhanceResistance(1, this->_var);
		_parent.EnhanceResistance(2, this->_var);
		_parent.EnhanceResistance(3, this->_var);
		_parent.EnhanceResistance(4, this->_var);
		_parent.EnhanceResistance(5, this->_var);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance(0, this->_var);
		_parent.ImpairResistance(1, this->_var);
		_parent.ImpairResistance(2, this->_var);
		_parent.ImpairResistance(3, this->_var);
		_parent.ImpairResistance(4, this->_var);
		_parent.ImpairResistance(5, this->_var);
		_parent.UpdateMagicData();
	}
};

//125
class filter_Guishen2 : public timeout_filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int crit;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Guishen2() {}
public:
	DECLARE_SUBSTANCE(filter_Guishen2);

	filter_Guishen2(
		object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->crit = buffid;

		_filter_id = FILTER_GUISHEN2;
		_buff_id = HSTATE_GUISHEN2;
	}

	~filter_Guishen2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << crit;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> crit;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter( 4208);
		_parent.RemoveFilter( 4209);
		_parent.RemoveFilter( 4216);
		_parent.ScalePlayer(0, this->scaleRatio);
		_parent.EnhanceScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		praySpeedRatio = this->praySpeedRatio;
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.EnhanceCrit(this->crit);
	}

	void OnDetach()
	{
		_parent.ScalePlayer(1, this->scaleRatio);
		_parent.ImpairScaleDamage( this->atkRatio);
		_parent.UpdateAttackData();
		praySpeedRatio = this->praySpeedRatio;
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.ImpairCrit(this->crit);
	}
};

class filter_Speedto : public timeout_filter
{
protected:
	float _add_speed;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Speedto() {}
public:
	DECLARE_SUBSTANCE(filter_Speedto);

	filter_Speedto(
		object_interface object,
		int period,
		float value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_add_speed = value;

		_filter_id = buffid + FILTER_SPEEDTO;
		_buff_id = HSTATE_SPEEDTO;
	}

	~filter_Speedto() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _add_speed;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _add_speed;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceSpeed(_add_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.ImpairSpeed(_add_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Yanyu : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Yanyu() {}
public:
	DECLARE_SUBSTANCE(filter_Yanyu);

	filter_Yanyu(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_YANYU;
		_buff_id = HSTATE_YANYU;
	}

	~filter_Yanyu() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.EnhanceDefense(_addend);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.EnhanceScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.ImpairDefense(_addend);
		_parent.UpdateDefenseData();
	}
};

class filter_Banruo : public timeout_filter
{
protected:
	int _addend;
	int _ratio;
	int _amount;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Banruo() {}
public:
	DECLARE_SUBSTANCE(filter_Banruo);

	filter_Banruo(
		object_interface object,
		int ratio,
		int amount,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_amount = amount;
		this->_counter = 0;
		this->_addend = r / period;
		if (this->_addend <= 0)
			this->_addend = 1;

		_filter_id = buffid + FILTER_BANRUO;
		_buff_id = HSTATE_BANRUO;
	}

	~filter_Banruo() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int addhp = this->_counter * this->_addend;
			if (_timeout <= tick)
				addhp += this->_amount;
			_parent.Heal(addhp, 0, 0);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio << _amount << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio >> _amount >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.IncVisibleState(9);  //VSTATE_HPGEN
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(_ratio);
		_parent.UpdateAttackData();
		_parent.DecVisibleState(9);
	}
};

class filter_Cycdecdefence : public timeout_filter
{
protected:
	int _amount;
	int _ratio;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Cycdecdefence() {}
public:
	DECLARE_SUBSTANCE(filter_Cycdecdefence);

	filter_Cycdecdefence(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r / period;
		this->_amount = 0;
		this->_counter = 0;

		_filter_id = buffid + FILTER_CYCDECDEFENCE;
		_buff_id = HSTATE_CYCDECDEFENCE;
	}

	~filter_Cycdecdefence() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			this->_amount += this->_counter * this->_ratio;
			_parent.ImpairScaleDefense(_counter * _ratio);
			_parent.UpdateDefenseData();
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _amount << _ratio << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _amount >> _ratio >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		_parent.EnhanceScaleDefense(_amount);
		_parent.UpdateDefenseData();
	}
};

//130
class filter_Cycleskill : public timeout_filter
{
protected:
	int		_skill_id;
	int		_skill_level;
	char	_skill_type;
	float	_radius;
	int		_coverage;
	int		_mpcost;
	int		_hpcost;
	int		_dpcost;
	int		_inkcost;
	char	_force;
	float	_ratio;
	int		_plus;
	int		_period;
	int		_var[16];
	int		_crit_rate;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Cycleskill() {}
public:
	DECLARE_SUBSTANCE(filter_Cycleskill);

	filter_Cycleskill(
		object_interface object,
		int skill_id,
		int skill_level,
		char skill_type,
		float radius,
		int coverage,
		int mpcost,
		int hpcost,
		int dpcost,
		int inkcost,
		char force,
		float ratio,
		int plus,
		int period,
		int* var,
		int crit_rate) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_skill_id = skill_id;
		this->_skill_level = skill_level;
		this->_skill_type = skill_type;
		this->_radius = radius;
		this->_coverage = coverage;
		this->_mpcost = mpcost;
		this->_hpcost = hpcost;
		this->_dpcost = dpcost;
		this->_inkcost = inkcost;
		this->_force = force;
		this->_ratio = ratio;
		this->_plus = plus;
		this->_period = period;
		this->_crit_rate = crit_rate;
		memcpy(this->_var, var, sizeof(this->_var));

		_filter_id = FILTER_CYCLESKILL;
		_buff_id = HSTATE_CYCLESKILL;
	}

	~filter_Cycleskill() {}

	virtual bool Save(archive& ar)   //没有保存_crit_rate  haokaixin
	{
		timeout_filter::Save(ar);
		ar << _skill_id << _skill_level << _skill_type << _radius << _coverage;
		ar << _mpcost << _hpcost << _dpcost << _inkcost << _force;
		ar << _ratio << _plus << _period;
		ar.push_back(_var,sizeof(_var));
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _skill_id >> _skill_level >> _skill_type >> _radius >> _coverage;
		ar >> _mpcost >> _hpcost >> _dpcost >> _inkcost >> _force;
		ar >> _ratio >> _plus >> _period;
		ar.pop_back(_var, sizeof(_var));
		return true;
	}

	void OnAttach()
	{
		_parent.CastCycleSkill(
			this->_skill_id,
			this->_skill_level,
			this->_skill_type,
			this->_radius,
			this->_coverage,
			this->_mpcost,
			this->_hpcost,
			this->_dpcost,
			this->_inkcost,
			this->_force,
			this->_ratio,
			this->_plus,
			this->_period,
			this->_var,
			this->_crit_rate);
		_parent.SetMeleeSeal(1);
		_parent.SetCycleState(1);
	}

	void OnDetach()
	{
		_parent.SetMeleeSeal(0);
		_parent.SetCycleState(0);
	}
};

class filter_Humanbomb : public timeout_filter
{
protected:
	int		_damage;
	float	_hpreduceratio;
	int		_radius;
	char	_force;
	int		_total;

	bool _invader;
	char _mode;

	XID _performer;
	attacker_info_t _pinfo;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Humanbomb() {}
public:
	DECLARE_SUBSTANCE(filter_Humanbomb);

	filter_Humanbomb(
		object_interface object,
		float hpreduceratio,
		int damage,
		int radius,
		int period,
		int buffid,
		char force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_hpreduceratio = hpreduceratio;
		this->_damage = damage;
		this->_radius = radius;
		this->_force = force;
		this->_total = buffid;

		_filter_id = FILTER_CYCLESKILL;
		_buff_id = HSTATE_CYCLESKILL;
	}

	~filter_Humanbomb() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	virtual bool Save(archive& ar)   
	{
		timeout_filter::Save(ar);
		ar << _damage << _hpreduceratio << _radius << _force << _total << _performer;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _damage >> _hpreduceratio >> _radius >> _force >> _total >> _performer;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		_parent.TransferAttack(_performer,_force,_damage,_radius,_total);
		_parent.BeHurt(_performer,_pinfo, _parent.GetExtendProp().max_hp * this->_hpreduceratio,
			_invader,_mode);
	}
};

class filter_Aloof : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_TRANSLATE_ENCHANT | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_RECV_MSG
	};

	filter_Aloof() {}
public:
	DECLARE_SUBSTANCE(filter_Aloof);

	filter_Aloof(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_ALOOF;
		_buff_id = HSTATE_ALOOF;
	}

	~filter_Aloof() {}

	void TranslateRecvAttack(const XID& attacker,attack_msg& msg)
	{
		msg.attack_state |= IMMUNEWEAK;
	}

	void TranslateRecvEnchant(const XID& attacker,enchant_msg& msg)
	{
		msg.attack_state |= IMMUNEWEAK;
	}

	void OnAttach()
	{
		_parent.SetMeleeSeal(1);
		_parent.SetSilentSeal(1);
	}

	void OnDetach()
	{
		_parent.SetMeleeSeal(0);
		_parent.SetSilentSeal(0);
	}
};

class filter_Juniorlightform : public filter
{
protected:
	int dec_dark_light;
	int dec_light_skill_cd;
	bool flag;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Juniorlightform() {}
public:
	DECLARE_SUBSTANCE(filter_Juniorlightform);

	filter_Juniorlightform(
		object_interface object,
		float ratio,
		int value) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_JUNIORLIGHTFORM;
		_buff_id = HSTATE_JUNIORLIGHT;
		this->dec_dark_light = value;
		this->dec_light_skill_cd = (int)ratio;
		this->flag = 0;
	}

	~filter_Juniorlightform() {}

	void Heartbeat(int tick)
	{
		_parent.DecDarkLight(dec_dark_light);
		if (_parent.GetDarkLight() <= 0)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)  //一个都没存取   haokaixin
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(4234);
		_parent.RemoveFilter(4236);
		_parent.RemoveFilter(4237);
		_parent.RemoveFilter(4238);
		_parent.RemoveFilter(4239);
		_parent.SetDarkLightForm(2);
		if (_parent.GetDarkLight() > 0)
		{
			dec_light_skill_cd = this->dec_light_skill_cd;
			_parent.GetSkillWrapper().DecLightSkillCoolTime(dec_light_skill_cd);
			this->flag = 1;
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		_parent.ClearDarkLightForm();
		if (this->flag)
		{
			dec_light_skill_cd = this->dec_light_skill_cd;
			_parent.GetSkillWrapper().IncLightSkillCoolTime(dec_light_skill_cd);
		}
	}
};

class filter_Cibei : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Cibei() {}
public:
	DECLARE_SUBSTANCE(filter_Cibei);

	filter_Cibei(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;

		_filter_id = buffid+FILTER_CIBEI;
		_buff_id = HSTATE_CIBEI;
	}

	~filter_Cibei() {}

	virtual bool Save(archive& ar)  
	{
		filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMaxHP(_addend);
		_parent.UpdateHPMP();
		_parent.EnhanceScaleMaxHP(_ratio);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairMaxHP(_addend);
		_parent.UpdateHPMP();
		_parent.ImpairScaleMaxHP(_ratio);
		_parent.UpdateHPMP();
	}
};

//135
class filter_Decdrugeffect : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decdrugeffect() {}
public:
	DECLARE_SUBSTANCE(filter_Decdrugeffect);

	filter_Decdrugeffect(
		object_interface object,
		float ratio,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = (int)(ratio * 100.0);

		_filter_id = buffid + FILTER_DECDRUGEFFECT;
		_buff_id = HSTATE_DECDRUGEFFECT;
	}

	~filter_Decdrugeffect() {}

	virtual bool Save(archive& ar)  //没有调用父类  haokaixin
	{
		//filter::Save(ar);
		ar <<  _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//filter::Load(ar);
		ar >>  _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.SetHealEffect(-this->_ratio);
	}

	void OnDetach()
	{
		_parent.SetHealEffect(this->_ratio);
	}
};

class filter_Incmpgen : public timeout_filter
{
protected:
	int _inc;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incmpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Incmpgen);

	filter_Incmpgen(
		object_interface object,
		int inc,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_inc = inc;

		_filter_id = buffid + FILTER_INCMPGEN;
		_buff_id = HSTATE_INCMPGEN;
	}

	~filter_Incmpgen() {}

	virtual bool Save(archive& ar)  
	{
		filter::Save(ar);
		ar << _inc;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _inc;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMPGen(this->_inc);
		_parent.UpdateHPMPGen();
	}

	void OnDetach()
	{
		_parent.ImpairScaleMPGen(this->_inc);
		_parent.UpdateHPMPGen();
	}
};

class filter_Decdefence : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decdefence() {}
public:
	DECLARE_SUBSTANCE(filter_Decdefence);

	filter_Decdefence(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_DECDEFENCE;
		_buff_id = HSTATE_DECDEFENCE;
	}

	~filter_Decdefence() {}

	virtual bool Save(archive& ar)  
	{
		filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairScaleDefense(this->_ratio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.EnhanceScaleDefense(this->_ratio);
		_parent.UpdateDefenseData();
	}
};

class filter_Goback : public timeout_filter
{
protected:
	A3DVECTOR pos;
	int tag;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Goback() {}
public:
	DECLARE_SUBSTANCE(filter_Goback);

	filter_Goback(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_GOBACK;
		_buff_id = HSTATE_GOBACK;
	}

	~filter_Goback() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << pos << tag;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> pos >> tag;
		return true;
	}

	void OnAttach()
	{
		this->pos = _parent.GetPos();
		this->tag = _parent.GetTag();
	}

	void OnDetach()
	{
		if (this->tag == _parent.GetTag())
			_parent.Teleport(this->tag, this->pos);
	}
};

class filter_Bleeding : public timeout_filter
{
protected:
	int _damage; 
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Bleeding() {}
public:
	DECLARE_SUBSTANCE(filter_Bleeding);

	filter_Bleeding(
		object_interface object,
		int period,
		int damage,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage;

		_filter_id = buffid + FILTER_BLEEDING;
		_buff_id = HSTATE_BLEEDING;
	}

	~filter_Bleeding() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_damage - _parent.GetExtendProp().defense;
			if (damage > 1)
				_parent.BeHurt(_performer,_pinfo,damage,_invader,_mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _damage << _performer << _counter;
		ar.push_back(&_pinfo,sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _damage >> _performer >> _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

//140
class filter_Divinityfury : public timeout_filter
{
protected:
	int _genspeed;
	int _addition;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Divinityfury() {}
public:
	DECLARE_SUBSTANCE(filter_Divinityfury);

	filter_Divinityfury(
		object_interface object,
		int dptotal,
		int addition,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_genspeed = dptotal / period;
		this->_addition = addition;
		this->_counter = 0;

		_filter_id = buffid + FILTER_DIVINITYFURY;
		_buff_id = HSTATE_DIVINITYFURY;
	}

	~filter_Divinityfury() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int deity = this->_counter * this->_genspeed;
			if (_timeout <= tick)
				deity += this->_addition;
			_parent.InjectDeity( deity, 0);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _genspeed << _addition << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _genspeed >> _addition >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Equipeffect : public timeout_filter
{
protected:
	char _equip_type;
	char _level;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Equipeffect() {}
public:
	DECLARE_SUBSTANCE(filter_Equipeffect);

	filter_Equipeffect(
		object_interface object,
		int period,
		char equip_type,
		char level) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_equip_type = equip_type;
		this->_level = level;

		_filter_id = FILTER_EQUIPEFFECT;
		_buff_id = HSTATE_EQUIPEFFECT;
	}

	~filter_Equipeffect() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _equip_type << _level;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _equip_type >> _level;
		return true;
	}

	void OnAttach()
	{
		if (!this->_equip_type)
			_parent.SetExtraEquipEffectState(1, this->_level);
	}

	void OnDetach()
	{
		if (!this->_equip_type)
			_parent.SetExtraEquipEffectState(0, 0);
	}
};

class filter_Frozen : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Frozen() {}
public:
	DECLARE_SUBSTANCE(filter_Frozen);

	filter_Frozen(
		object_interface object,
		float ratio,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_FROZEN;
		_buff_id = HSTATE_FROZEN;
	}

	~filter_Frozen() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.SetFrozenState( 1);
		_parent.SetSilentSeal( 1);
		_parent.SetMeleeSeal( 1);
		_parent.SetRootSeal( 1);
		_parent.SendClientNotifyRoot( 4);
		_parent.IncVisibleState(VSTATE_FROZEN);
	}

	void OnDetach()
	{
		_parent.SetFrozenState( 0);
		_parent.SetSilentSeal( 0);
		_parent.SetMeleeSeal( 0);
		_parent.SetRootSeal( 0);
		_parent.SendClientDispelRoot( 4);
		_parent.DecVisibleState(VSTATE_FROZEN);
	}
};

class filter_Hpleak3 : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpleak3() {}
public:
	DECLARE_SUBSTANCE(filter_Hpleak3);

	filter_Hpleak3(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;
		_timeout = damage / this->_damage;

		_filter_id = buffid + FILTER_HPLEAK3;
		_buff_id = HSTATE_HPLEAK3;
	}

	~filter_Hpleak3() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.BeHurt(_performer,_pinfo,damage,_invader,_mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 1)
		{
			this->_damage /= 2;
			this->_addtion /= 2;
		}
		if (this->_damage <= 1)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.push_back(&_pinfo,sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _damage >> _addtion >> _performer >> _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPLEAK);
	}
};

class filter_Addantislow : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addantislow() {}
public:
	DECLARE_SUBSTANCE(filter_Addantislow);

	filter_Addantislow(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = v;

		_filter_id = buffid + FILTER_ADDANTISLOW;
		//_buff_id = HSTATE_ADDANTISLOW;
	}

	~filter_Addantislow() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance(5, this->_addend);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance(5, this->_addend);
		_parent.UpdateMagicData();
	}
};

//145
class filter_Addweak : public filter
{
protected:
	int _prob;
	int _skill_id;
	int _skill_level;
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Addweak() {}
public:
	DECLARE_SUBSTANCE(filter_Addweak);

	filter_Addweak(
		object_interface object,
		int prob,
		int value,
		int amount,
		int ratio) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_ADDWEAK;
		_buff_id = HSTATE_ADDWEAK;
		
		this->_prob = prob;
		this->_skill_id = value;
		this->_skill_level = amount;
		this->_addend = ratio;
	}

	~filter_Addweak() {}

	void TranslateSendAttack(const XID& target,attack_msg& atkmsg,char type,float radius,int mc)
	{
		if (rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->_skill_id;
			msg.skill_level = this->_skill_level;
			msg.force_attack = atkmsg.force_attack;
			msg.attack_range = _parent.GetExtendProp().attack_range;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
			msg.skill_var[0] = _parent.GetExtendProp().resistance[1];
			msg.skill_var[1] = _parent.GetExtendProp().resistance[2];
			msg.skill_var[2] = _parent.GetExtendProp().resistance[3];
			msg.skill_var[3] = _parent.GetExtendProp().resistance[0];
			if (SkillWrapper::GetType(this->_skill_id) == 2)
				msg.helpful = 1;
			std::vector<exclude_target> empty;
			if (type == 1)
			{
				_parent.RegionEnchant1(_parent.GetPos(), radius, msg, mc, empty);
			}
			else if (type > 1)
			{
				if (type == 2)
				{
					_parent.RegionEnchant2(_parent.GetPos(), radius, msg, mc, empty);
				}
				else if (type == 3)
				{
					_parent.RegionEnchant3(_parent.GetPos(), radius, msg, mc, empty);
				}
			}
			else if (!type)
			{
				_parent.Enchant(target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)  //一个都没存取   haokaixin
	{
		filter::Save(ar);
		ar << _prob << _skill_id << _skill_level << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _prob >> _skill_id >> _skill_level >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_ADDWRAP);
		_parent.RemoveFilter(FILTER_ADDCHARM);
		_parent.RemoveFilter(FILTER_ADDSLOW);
		_parent.EnhanceResistance(1, this->_addend);
		_parent.UpdateMagicData();
		_parent.IncVisibleState(VSTATE_ADDWEAK);
	}

	void OnDetach()
	{
		_parent.ImpairResistance(1, this->_addend);
		_parent.UpdateMagicData();
		_parent.DecVisibleState(VSTATE_ADDWEAK);
	}
};

class filter_Decdodge : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decdodge() {}
public:
	DECLARE_SUBSTANCE(filter_Decdodge);

	filter_Decdodge(
		object_interface object,
		int ratio,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_DECDODGE;
		_buff_id = HSTATE_DECDODGE;
	}

	~filter_Decdodge() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairScaleArmor(_ratio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.EnhanceScaleArmor(_ratio);
		_parent.UpdateDefenseData();
	}
};

class filter_Mpleak : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Mpleak() {}
public:
	DECLARE_SUBSTANCE(filter_Mpleak);

	filter_Mpleak(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;
		_timeout = damage / this->_damage;

		_filter_id = buffid + FILTER_MPLEAK;
		_buff_id = HSTATE_MPLEAK;
	}

	~filter_Mpleak() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.DrainMana(damage);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _damage << _addtion << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _damage >> _addtion >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_MPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_MPLEAK);
	}
};

class filter_Addspot : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addspot() {}
public:
	DECLARE_SUBSTANCE(filter_Addspot);

	filter_Addspot(
		object_interface object,
		int value,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;

		_filter_id = buffid + FILTER_ADDSPOT;
		_buff_id = HSTATE_ADDSPOT;
	}

	~filter_Addspot() {}

	virtual bool Save(archive& ar)  //没调用父类    haokaixin
	{
		//filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceAntiInvisible(this->_value);
	}

	void OnDetach()
	{
		_parent.ImpairAntiInvisible(this->_value);
	}
};

class filter_Zhaoqi : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Zhaoqi() {}
public:
	DECLARE_SUBSTANCE(filter_Zhaoqi);

	filter_Zhaoqi(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_ZHAOQI;
		_buff_id = HSTATE_ZHAOQI;
	}

	~filter_Zhaoqi() {}

	virtual bool Save(archive& ar)  
	{
		filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMaxMP(this->_addend);
		_parent.UpdateHPMP();
		_parent.EnhanceScaleMaxMP( this->_ratio);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairMaxMP(this->_addend);
		_parent.UpdateHPMP();
		_parent.ImpairScaleMaxMP( this->_ratio);
		_parent.UpdateHPMP();
	}
};

//150
class filter_Seed : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Seed() {}
public:
	DECLARE_SUBSTANCE(filter_Seed);

	filter_Seed(object_interface object, int amount, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_SEED;
		_buff_id = HSTATE_SEED;
		_max_overlay_cnt = amount;
		_can_overlay = true;
	}

	~filter_Seed() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Subantisilent : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantisilent() {}
public:
	DECLARE_SUBSTANCE(filter_Subantisilent);

	filter_Subantisilent(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = v;

		_filter_id = buffid + FILTER_SUBANTISILENT;
		_buff_id = HSTATE_SUBANTISILENT;
	}

	~filter_Subantisilent() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(3, this->_sub);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(3, this->_sub);
		_parent.UpdateMagicData();
	}
};

class filter_Tuoling2 : public timeout_filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int antiCrit;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Tuoling2() {}
public:
	DECLARE_SUBSTANCE(filter_Tuoling2);

	filter_Tuoling2(
		object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->antiCrit = buffid;

		_filter_id = FILTER_TUOLING2;
		_buff_id = HSTATE_TUOLING2;
	}

	~filter_Tuoling2() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << antiCrit;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> antiCrit;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_GUISHEN1);
		_parent.RemoveFilter(FILTER_GUISHEN2);
		_parent.RemoveFilter(FILTER_TUOLING1);
		_parent.ScalePlayer(0, this->scaleRatio);
		_parent.ImpairScaleDamage(this->atkRatio);
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.EnhanceAntiCritRate(this->antiCrit);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ScalePlayer(1, this->scaleRatio);
		_parent.EnhanceScaleDamage(this->atkRatio);
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.EnhanceAntiCritRate(-this->antiCrit);
		_parent.UpdateAttackData();
	}
};

class filter_Sleeptimer : public timeout_filter
{
protected:
	int _cycle;
	int _duration;
	int _counter;
	int _active;
	int _buffcnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Sleeptimer() {}
public:
	DECLARE_SUBSTANCE(filter_Sleeptimer);

	filter_Sleeptimer(
		object_interface object,
		int duration,
		int cycle,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_cycle = cycle;
		this->_duration = duration;
		this->_counter = 0;
		this->_active = 0;
		this->_buffcnt = 0;

		_filter_id = FILTER_SLEEPTIMER;
		_buff_id = HSTATE_SLEEPTIMER;
	}

	~filter_Sleeptimer() {}

	void Heartbeat(int tick)
	{
		if (!this->_counter)
		{
			_parent.SetSilentSeal(1);
			_parent.SetMeleeSeal(1);
			_parent.SetRootSeal(1);
			_parent.SendClientNotifyRoot(0);
			int Timeout = filter::GetTimeout();
			_parent.UpdateBuff(1, _filter_id,Timeout,1);
			_parent.IncVisibleState(VSTATE_DIZZY);
			this->_active = 1;
		}
		this->_counter += tick;
		if (this->_active)
			this->_buffcnt += tick;
		if (this->_buffcnt >= this->_cycle && this->_active)
		{
			Deactivate();
			this->_buffcnt = 0;
		}
		if (this->_counter >= this->_duration)
			this->_counter = 0;
		timeout_filter::Heartbeat(tick);
	}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		if (this->_active)
			Deactivate();
	}

	void Deactivate()
	{
		this->_active = 0;
		_parent.SendClientDispelRoot(0);
		_parent.SetSilentSeal(0);
		_parent.SetMeleeSeal(0);
		_parent.SetRootSeal(0);
		_parent.RemoveBuff(2, _filter_id);
		_parent.DecVisibleState(6u);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _cycle << _duration << _counter << _active << _buffcnt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _cycle >> _duration >> _counter >> _active >> _buffcnt;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		if (this->_active)
			Deactivate();
	}
};

class filter_Miracleburstfo : public timeout_filter
{
protected:
	int _ratio;
	int _dpdecspeed;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Miracleburstfo() {}
public:
	DECLARE_SUBSTANCE(filter_Miracleburstfo);

	filter_Miracleburstfo(
		object_interface object,
		int r,
		int period,
		int buffid,
		int dpcost) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		this->_dpdecspeed = dpcost / period;
		this->_counter = 0;

		_filter_id = buffid + FILTER_MIRACLEBURSTFO;
		_buff_id = HSTATE_MIRACLEBURSTFO;
	}

	~filter_Miracleburstfo() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			if (!_parent.DecDeity(_counter * _dpdecspeed))
				_is_deleted = 1;
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio << _dpdecspeed << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio >> _dpdecspeed >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.SetFuryState(1);
		_parent.EnhanceScaleDeityPower(this->_ratio);
		_parent.EnhanceCultAttack(1, this->_ratio);
		_parent.EnhanceScaleDamage(this->_ratio);
		_parent.EnhanceScaleDmgReduce(this->_ratio);
		_parent.EnhanceScaleIgnDmgReduce(this->_ratio);
		_parent.UpdateAttackData();
		_parent.IncVisibleState(VSTATE_MIRACLEBURSTFO);
	}

	void OnDetach()
	{
		_parent.SetFuryState(0);
		_parent.EnhanceScaleDeityPower(-this->_ratio);
		_parent.EnhanceCultAttack(1, -this->_ratio);
		_parent.ImpairScaleDamage(this->_ratio);
		_parent.ImpairScaleDmgReduce(this->_ratio);
		_parent.ImpairScaleIgnDmgReduce(this->_ratio);
		_parent.UpdateAttackData();
		_parent.DecVisibleState(VSTATE_MIRACLEBURSTFO);
	}
};

//155
class filter_Hpgen : public timeout_filter
{
protected:
	int _health;
	int _addtion;
	char _counter;
	bool _can_heal_bloodpool;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Hpgen);

	filter_Hpgen(
		object_interface object,
		int health,
		int addtion,
		int period,
		int buffid,
		int can_heal_bloodpool) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_health = health / period;
		if (this->_health <= 0)
			this->_health = 1;
		this->_addtion = addtion;
		this->_can_heal_bloodpool = can_heal_bloodpool != 0;

		_filter_id = buffid + FILTER_HPGEN;
		_buff_id = HSTATE_HPGEN;
	}

	~filter_Hpgen() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int health = this->_counter * this->_health;
			if (_timeout <= tick)
				health += this->_addtion;
			_parent.Heal(health, 0, this->_can_heal_bloodpool);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _health << _addtion << _counter << _can_heal_bloodpool;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _health >> _addtion >> _counter >> _can_heal_bloodpool;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPGEN);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPGEN);
	}
};

class filter_Aswind : public filter
{
protected:
	float _add_speed;
	int _slow_resist;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE
	};

	filter_Aswind() {}
public:
	DECLARE_SUBSTANCE(filter_Aswind);

	filter_Aswind(
		object_interface object,
		float add_speed,
		int slow_resist) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_ADDWEAK;
		_buff_id = HSTATE_ADDWEAK;

		this->_add_speed = add_speed;
		this->_slow_resist = slow_resist;
	}

	~filter_Aswind() {}

	virtual bool Save(archive& ar)  
	{
		filter::Save(ar);
		ar << _add_speed << _slow_resist;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _add_speed >> _slow_resist;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_ASFOREST);
		_parent.RemoveFilter(FILTER_ASFIRE);
		_parent.RemoveFilter(FILTER_ASHILL);
		_parent.EnhanceResistance(5, this->_slow_resist);
		_parent.UpdateMagicData();
		_parent.EnhanceSpeed(this->_add_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.ImpairResistance(5, this->_slow_resist);
		_parent.UpdateMagicData();
		_parent.ImpairSpeed(this->_add_speed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Limitcyclearea : public filter
{
protected:
	A3DVECTOR pos;
	float radius;
	int time;
	int _performer;
	std::map<int, int> idxMap;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Limitcyclearea() {}
public:
	DECLARE_SUBSTANCE(filter_Limitcyclearea);

	filter_Limitcyclearea(
		object_interface object,
		int period,
		const A3DVECTOR& attackerpos,
		float value) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_LIMITCYCLEAREA;
		_buff_id = HSTATE_JAILLIMITCYCLEAREA;

		this->pos = attackerpos;
		this->radius = value;
		this->time = period;
	}

	~filter_Limitcyclearea() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader)
	{
		this->_performer = player.id;
	}

	void Heartbeat(int tick)
	{
		std::map<int, int>::iterator it = idxMap.begin();
		while (1)
		{
			if (it == idxMap.end())
				break;
			it->second -= tick;
			if (it->second > 0)
			{
				++it;
			}
			else
			{
				idxMap.erase(it++);
			}
		}
		if (idxMap.empty())
			_is_deleted = 1;
	}

	void OnModify(int ctrlname,void* ctrlval,size_t ctrlen)
	{
		cycle_area* ca = (cycle_area*)ctrlval;
		if (ctrlname == 8 && ctrlen == 24)
		{
			_parent.AddLimitCycleArea(ca->owner,ca->center,ca->radius);
			idxMap[ca->owner] = ca->time;
		}
		else if (ctrlname == 9 && ctrlen == 4)
		{
			std::map<int, int>::iterator it = idxMap.find(ca->owner);
			if (it !=idxMap.end())
			{
				_parent.RemoveLimitCycleArea(ca->owner);
				idxMap.erase(it++);
			}
		}
	}

	virtual bool Save(archive& ar) 
	{
		filter::Save(ar);
		ar << pos << radius << time << _performer << idxMap.size();
		for (std::map<int, int>::iterator it = idxMap.begin(); it != idxMap.end(); it++)
		{
			ar << it->first << it->second;
		}
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		size_t size;
		ar >> pos >> radius >> time >> _performer >> size ;
		for (int i = 0; i < size; i++)
		{
			int idx, t;
			ar >> idx >> t;
			idxMap[idx] = t;
		}
		return true;
	}

	void OnAttach()
	{
		_parent.AddLimitCycleArea(this->_performer,this->pos,this->radius);
		idxMap[_performer] = this->time;
	}

	void OnDetach()
	{
		idxMap.clear();
		_parent.ClearLimitCycleArea();
	}
};

class filter_CatchPet : public timeout_filter
{
protected:
	XID _attacker;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_CatchPet() {}
public:
	DECLARE_SUBSTANCE(filter_CatchPet);

	filter_CatchPet(
		object_interface object,
		int period,
		const XID& attacker) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_attacker.type = attacker.type;
		this->_attacker.id = attacker.id;

		_filter_id = FILTER_CATCHPET;
		//_buff_id = HSTATE_CATCHPET;
	}

	~filter_CatchPet() {}

	void OnAttach()
	{
		_parent.PlayerCatchPet(_attacker);
		_parent.IncVisibleState(VSTATE_CATCHPET);

	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_CATCHPET);

	}
};

class filter_Permitcyclearea : public filter
{
protected:
	A3DVECTOR pos;
	float radius;
	int time;
	int _performer;
	std::map<int, int> idxMap;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Permitcyclearea() {}
public:
	DECLARE_SUBSTANCE(filter_Permitcyclearea);

	filter_Permitcyclearea(
		object_interface object,
		int period,
		const A3DVECTOR& attackerpos,
		float value) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_PERMITCYCLEAREA;
		_buff_id = HSTATE_JAILPERMITCYCLEAREA;

		this->time = period;
		this->pos = attackerpos;
		this->radius = value;
	}

	~filter_Permitcyclearea() {}

	void SetUp(const XID& player, const attacker_info_t& info, char mode, bool invader)
	{
		this->_performer = player.id;
		printf("Permit Area: object_id: %d\n", this->_performer);
	}

	void Heartbeat(int tick)
	{
		std::map<int, int>::iterator it = idxMap.begin();
		while (1)
		{
			if (it == idxMap.end())
				break;
			it->second -= tick;
			if (it->second > 0)
			{
				++it;
			}
			else
			{
				idxMap.erase(it++);
			}
		}
		if (idxMap.empty())
			_is_deleted = 1;
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrlen)
	{
		cycle_area* ca = (cycle_area*)ctrlval;
		if (ctrlname == 8 && ctrlen == 24)
		{
			_parent.AddPermitCycleArea(ca->owner, ca->center, ca->radius);
			idxMap[ca->owner] = ca->time;
		}
		else if (ctrlname == 9 && ctrlen == 4)
		{
			std::map<int, int>::iterator it = idxMap.find(ca->owner);
			if (it != idxMap.end())
			{
				_parent.RemovePermitCycleArea(ca->owner);
				idxMap.erase(it++);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << pos << radius << time << _performer << idxMap.size();
		for (std::map<int, int>::iterator it = idxMap.begin(); it != idxMap.end(); it++)
		{
			ar << it->first << it->second;
		}
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		size_t size;
		ar >> pos >> radius >> time >> _performer >> size;
		for (int i = 0; i < size; i++)
		{
			int idx, t;
			ar >> idx >> t;
			idxMap[idx] = t;
		}
		return true;
	}

	void OnAttach()
	{
		_parent.AddPermitCycleArea(this->_performer, this->pos, this->radius);
		idxMap[_performer] = this->time;
	}

	void OnDetach()
	{
		idxMap.clear();
		_parent.ClearPermitCycleArea();
	}
};

//120
class filter_Subantidizzy2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantidizzy2() {}
public:
	DECLARE_SUBSTANCE(filter_Subantidizzy2);

	filter_Subantidizzy2(
		object_interface object,
		int v,
		int period,
		int overlay_cnt) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = v;
		_max_overlay_cnt = overlay_cnt;
		_can_overlay = 1;

		_filter_id = FILTER_SUBANTIDIZZY2;
		_buff_id = HSTATE_SUBANTIDIZZY2;
	}

	~filter_Subantidizzy2() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance( 0, this->_value);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(0, this->_value);
		_parent.UpdateMagicData();
	}
};

class filter_Icon : public timeout_filter
{
protected:
	int _vstate;
	int _hstate;

	enum
	{
		FILTER_MASK =  FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Icon() {}
public:
	DECLARE_SUBSTANCE(filter_Icon);

	filter_Icon(
		object_interface object,
		int period,
		int hstate,
		int vstate,
		int filterid,
		int mask) : timeout_filter(object, period, mask | FILTER_MASK)
	{
		this->_vstate = vstate;
		this->_hstate = hstate;

		_filter_id = filterid;
		//_buff_id = ;
	}

	~filter_Icon() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _vstate << _hstate;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _vstate >> _hstate;
		return true;
	}

	void OnAttach()
	{
		if (this->_vstate)
			_parent.IncVisibleState(_vstate);
		if (this->_hstate)
		{
			_parent.UpdateBuff(_hstate, _filter_id, GetTimeout(),1);
		}
	}

	void OnDetach()
	{
		if (this->_vstate)
			_parent.DecVisibleState(_vstate);
		if (this->_hstate)
			_parent.RemoveBuff(_hstate, _filter_id);
	}
};

class filter_Fogstate : public timeout_filter
{
protected:
	float _addend;
	int time;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Fogstate() {}
public:
	DECLARE_SUBSTANCE(filter_Fogstate);

	filter_Fogstate(
		object_interface object,
		float r,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->time = period;

		_filter_id = FILTER_FOGSTATE;
		_buff_id = HSTATE_FOGSTATE;
	}

	~filter_Fogstate() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addend << time;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend >> time;
		return true;
	}

	void OnAttach()
	{
		_parent.ForbidAttack();
		_parent.EnhanceSpeed(_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.SetInvincibleFilter(1, this->time, 1);

		_parent.AddFilter(new filter_Icon(_parent, this->time, HSTATE_INVINCIBLE, VSTATE_INVINCIBLE, FILTER_ICON, 0));
		_parent.ChangeShape(4);
	}

	void OnDetach()
	{
		_parent.AllowAttack();
		_parent.ImpairSpeed(_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.SetInvincibleFilter( 0, time, 1);
		_parent.ChangeShape(0);
	}
};

class filter_Dodge : public timeout_filter
{
protected:
	float _speed;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Dodge() {}
public:
	DECLARE_SUBSTANCE(filter_Dodge);

	filter_Dodge(object_interface object, int period, float s) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_speed = s;

		_filter_id = FILTER_DODGE;
		//_buff_id = HSTATE_DODGE;
	}

	~filter_Dodge() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _speed;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _speed;
		return true;
	}

	void OnAttach()
	{
		_parent.SetOverSpeed( this->_speed);
	}

	void OnDetach()
	{
		_parent.SetOverSpeed( 0.0);
	}
};

class filter_Hpleak4 : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpleak4() {}
public:
	DECLARE_SUBSTANCE(filter_Hpleak4);

	filter_Hpleak4(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;

		_timeout = damage / this->_damage;

		_filter_id = buffid + FILTER_HPLEAK4;
		_buff_id = HSTATE_HPLEAK4;
	}

	~filter_Hpleak4() {}

	void SetUp(const XID& player, const attacker_info_t& info, char mode, bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.BeHurt(_performer, _pinfo, damage, _invader, _mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 1)
		{
			this->_damage /= 2;
			this->_addtion /= 2;
		}
		if (this->_damage <= 1)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _damage >> _addtion >> _performer >> _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPLEAK);
	}
};
//125
class filter_Addantiwrap : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addantiwrap() {}
public:
	DECLARE_SUBSTANCE(filter_Addantiwrap);

	filter_Addantiwrap(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = v;

		_filter_id = buffid + FILTER_ADDANTIWRAP;
		_buff_id = HSTATE_ADDANTIWRAP;
	}

	~filter_Addantiwrap() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance(2, this->_addend);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance(2, this->_addend);
		_parent.UpdateMagicData();
	}
};

class filter_Rmhumanform : public filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK
	};

	filter_Rmhumanform() {}
public:
	DECLARE_SUBSTANCE(filter_Rmhumanform);

	filter_Rmhumanform(object_interface object) : filter(object, FILTER_MASK)
	{
		_filter_id = FILTER_RMHUMANFORM;
		_buff_id = HSTATE_RMHUMANFORM;

	}

	~filter_Rmhumanform() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.ForbidAttack();
		_parent.ChangeShape(8);
	}

	void OnDetach()
	{
		_parent.AllowAttack();
		_parent.ChangeShape(0);
	}
};

class filter_Ignite : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Ignite() {}
public:
	DECLARE_SUBSTANCE(filter_Ignite);

	filter_Ignite(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;
		this->_counter = 0;
		_timeout = period;

		_filter_id = buffid + FILTER_IGNITE;
		_buff_id = HSTATE_IGNITE;
	}

	~filter_Ignite() {}

	void SetUp(const XID& player, const attacker_info_t& info, char mode, bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 0 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.BeHurt(_performer, _pinfo, damage, _invader, _mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 1)
		{
			this->_damage /= 2;
			this->_addtion /= 2;
		}
		if (this->_damage <= 1)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _damage >> _addtion >> _performer >> _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.SetIgniteState(1);
		_parent.IncVisibleState(VSTATE_IGNITE);
	}

	void OnDetach()
	{
		_parent.SetIgniteState(0);
		_parent.DecVisibleState(VSTATE_IGNITE);
	}
};

class filter_Diet : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Diet() {}
public:
	DECLARE_SUBSTANCE(filter_Diet);

	filter_Diet(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_DIET;
		_buff_id = HSTATE_DIET;
	}

	~filter_Diet() {}

	void OnAttach()
	{
		_parent.SetDietSeal(1);
	}

	void OnDetach()
	{
		_parent.SetDietSeal(0);
	}
};

class filter_Attack2hp : public timeout_filter
{
protected:
	float _ratio;
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Attack2hp() {}
public:
	DECLARE_SUBSTANCE(filter_Attack2hp);

	filter_Attack2hp(
		object_interface object,
		float ratio,
		int period,
		int prob) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_prob = prob;

		_filter_id = FILTER_ATTACK2HP;
		_buff_id = HSTATE_HPSUCK;
	}

	~filter_Attack2hp() {}

	void TranslateSendAttack(const XID& target,attack_msg& msg,char type,float r,int mc)
	{
		if (rand() % 100 < this->_prob)
		{
			int addhp = msg.damage_high * this->_ratio;
			if (addhp > 1)
				_parent.Heal(addhp, 0, 0);
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio >> _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};
//130
class filter_Tuoling1 : public timeout_filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int defenseRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Tuoling1() {}
public:
	DECLARE_SUBSTANCE(filter_Tuoling1);

	filter_Tuoling1(
		object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->defenseRatio = buffid;

		_filter_id = FILTER_TUOLING1;
		_buff_id = HSTATE_TUOLING1;
	}

	~filter_Tuoling1() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << defenseRatio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> defenseRatio;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_GUISHEN1);
		_parent.RemoveFilter(FILTER_GUISHEN2);
		_parent.RemoveFilter(FILTER_TUOLING2);
		_parent.ScalePlayer( 0, this->scaleRatio);
		_parent.ImpairScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.EnhanceScaleDefense(defenseRatio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ScalePlayer(1, this->scaleRatio);
		_parent.EnhanceScaleDamage(atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.ImpairScaleDefense(defenseRatio);
		_parent.UpdateDefenseData();
	}
};

class filter_Incattack : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incattack() {}
public:
	DECLARE_SUBSTANCE(filter_Incattack);

	filter_Incattack(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_INCATTACK;
		_buff_id = HSTATE_INCATTACK;
	}

	~filter_Incattack() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Foxstate : public filter
{
protected:
	float _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Foxstate() {}
public:
	DECLARE_SUBSTANCE(filter_Foxstate);

	filter_Foxstate(object_interface object, float r) : filter(object, FILTER_MASK)
	{
		this->_addend = r;

		_filter_id = FILTER_FOXSTATE;
		_buff_id = HSTATE_FOXSTATE;

	}

	~filter_Foxstate() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.ForbidAttack();
		_parent.EnhanceSpeed( this->_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.SetImmuneMask(IMMUNESLOW);
		_parent.SetImmuneMask(IMMUNEWRAP);
		_parent.ChangeShape( 3);
	}

	void OnDetach()
	{
		_parent.AllowAttack();
		_parent.ImpairSpeed( this->_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
		_parent.ClearImmuneMask(IMMUNESLOW);
		_parent.ClearImmuneMask(IMMUNEWRAP);
		_parent.ChangeShape( 0);
	}
};

class filter_Holyaura : public timeout_filter
{
protected:
	int _sid;
	int _slevel;
	float _radius;
	int _coverage;
	int _mpcost;
	int _counter;

	char _vstate;
	int _var[15];
	int _dpcost;
	int _hpcost;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Holyaura() {}
public:
	DECLARE_SUBSTANCE(filter_Holyaura);

	filter_Holyaura(
		object_interface object,
		int skillid,
		int level,
		float radius,
		int coverage,
		int mpcost,
		int period,
		int* var,
		int dpcost,
		int hpcost) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sid = skillid;
		this->_slevel = level;
		this->_radius = radius;
		this->_coverage = coverage;
		this->_mpcost = mpcost;
		this->_counter = 5;

		this->_vstate = 1;
		memcpy(this->_var, var, sizeof(this->_var));
		this->_dpcost = dpcost;
		this->_hpcost = hpcost;

		_filter_id = FILTER_HOLYAURA;
		_buff_id = HSTATE_HOLYAURA;
	}

	~filter_Holyaura() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 4)
		{
			if (this->_mpcost <= 0
				|| _parent.GetBasicProp().mp >= this->_mpcost)
			{
				if (this->_dpcost <= 0
					|| _parent.GetBasicProp().dp >= this->_dpcost)
				{
					if (this->_hpcost <= 0
						|| _parent.GetBasicProp().hp > this->_hpcost)
					{
						_parent.DrainMana(_mpcost);
						_parent.DecDeity(_dpcost);
						_parent.DecHP(_hpcost);
						enchant_msg msg;
						memset(&msg, 0, sizeof(msg));
						msg.skill = this->_sid;
						msg.skill_level = this->_slevel;
						msg.force_attack = 0;
						msg.helpful = 1;
						msg.attack_range = this->_radius;
						msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
						memcpy(msg.skill_var, this->_var, sizeof(msg.skill_var));

						_parent.GetSkillWrapper().SetSkillTalent(_sid, msg.skill_modify, _parent);
						if (_parent.IsInTeam())
						{
							_parent.TeamEnchant(msg, 0, 0);
						}
						else
						{
							_parent.Enchant(_parent.GetSelfID(), msg);
						}
						this->_counter = 0;
						timeout_filter::Heartbeat(tick);
					}
					else
					{
						_is_deleted = 1;
					}
				}
				else
				{
					_is_deleted = 1;
				}
			}
			else
			{
				_is_deleted = 1;
			}
		}
	}

	void SetVisible(char id)
	{
		if (id <= 0 || id > 49)
			_vstate = 1;
		else
			_vstate = id;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sid << _slevel << _radius << _coverage << _mpcost << _counter << _vstate;
		ar.pop_back(_var,sizeof(_var));
		ar << _dpcost << _hpcost;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sid >> _slevel >> _radius >> _coverage >> _mpcost >> _counter >> _vstate;
		ar.pop_back(_var, sizeof(_var));
		ar >> _dpcost >> _hpcost;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(_vstate);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(_vstate);
	}
};

class filter_Puppetskill : public timeout_filter
{
protected:
	int _skill_id;
	int _immune_prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Puppetskill() {}
public:
	DECLARE_SUBSTANCE(filter_Puppetskill);

	filter_Puppetskill(
		object_interface object,
		int period,
		int prob,
		int skill_id) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_skill_id = skill_id;
		this->_immune_prob = prob;

		_filter_id = FILTER_PUPPETSKILL;
		_buff_id = HSTATE_PUPPETSKILL;
	}

	~filter_Puppetskill() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _skill_id << _immune_prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _skill_id >> _immune_prob;
		return true;
	}

	void OnAttach()
	{
		_parent.SetPuppetSkill(this->_skill_id, this->_immune_prob);
	}

	void OnDetach()
	{
		_parent.SetPuppetSkill( 0, 0);
	}
};

//135
class filter_Inccritrate : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Inccritrate() {}
public:
	DECLARE_SUBSTANCE(filter_Inccritrate);

	filter_Inccritrate(
		object_interface object,
		int r,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = FILTER_INCCRITRATE;
		_buff_id = HSTATE_INCCRITRATE;
	}

	~filter_Inccritrate() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceCrit(_ratio);
	}

	void OnDetach()
	{
		_parent.ImpairCrit(_ratio);
	}
};

class filter_Immuneslow : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Immuneslow() {}
public:
	DECLARE_SUBSTANCE(filter_Immuneslow);

	filter_Immuneslow(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_INCCRITRATE;
		_buff_id = HSTATE_INCCRITRATE;
	}

	~filter_Immuneslow() {}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNESLOW);
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNESLOW);
	}
};

class filter_Incmp : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incmp() {}
public:
	DECLARE_SUBSTANCE(filter_Incmp);

	filter_Incmp(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_INCMP;
		_buff_id = HSTATE_INCMP;
	}

	~filter_Incmp() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMaxMP( this->_ratio);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairScaleMaxMP(this->_ratio);
		_parent.UpdateHPMP();
	}
};

class filter_Barehanded : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Barehanded() {}
public:
	DECLARE_SUBSTANCE(filter_Barehanded);

	filter_Barehanded(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_BAREHANDED;
		_buff_id = HSTATE_BAREHANDED;
	}

	~filter_Barehanded() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		this->_sub = _parent.GetCurWeapon().damage_low;
		_parent.ImpairDamage(_sub);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceDamage(_sub);
		_parent.UpdateAttackData();
	}
};

class filter_Turnbuff : public timeout_filter
{
protected:
	int turnMaxCnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Turnbuff() {}
public:
	DECLARE_SUBSTANCE(filter_Turnbuff);

	filter_Turnbuff(
		object_interface object,
		int period,
		int amount,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->turnMaxCnt = amount;

		_filter_id = buffid + FILTER_TURNBUFF;
		_buff_id = HSTATE_TURNBUFF;
	}

	~filter_Turnbuff() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << turnMaxCnt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> turnMaxCnt;
		return true;
	}

	void OnAttach()
	{
		_parent.ClearRandomSpecFilter(IMMUNEFROZE, this->turnMaxCnt);
		_parent.SetTurnBuffState(1);
		_parent.IncVisibleState(VSTATE_TURNBUFF);
	}

	void OnDetach()
	{
		_parent.SetTurnBuffState(0);
		_parent.DecVisibleState(VSTATE_TURNBUFF);
	}
};

//140 
class filter_Seniordarkform : public filter
{
protected:
	int inc_dark_light;
	int dec_dark_skill_cd;
	int spirit_gen_interval;
	bool flag;
	int counter;
	int anti;
	int mp;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Seniordarkform() {}
public:
	DECLARE_SUBSTANCE(filter_Seniordarkform);

	filter_Seniordarkform(
		object_interface object,
		float ratio,
		int value,
		int amount,
		int time,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->inc_dark_light = value;
		this->dec_dark_skill_cd = (int)ratio;
		this->spirit_gen_interval = amount / 1000;
		this->flag = 0;
		this->counter = 0;
		this->anti = buffid;
		this->mp = time;

		_filter_id = FILTER_SENIORDARKFORM;
		_buff_id = HSTATE_SENIORDARK;

	}

	~filter_Seniordarkform() {}

	void Heartbeat(int tick)
	{
		if (!_is_deleted)
		{
			_parent.IncDarkLight(this->inc_dark_light);
			if ((int)_parent.GetDarkLight() >= 0)
				_is_deleted = 1;
			if (--this->counter <= 0)
			{
				_parent.AddDarkLightSpirit(1);
				this->counter = this->spirit_gen_interval;
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << inc_dark_light << dec_dark_skill_cd << spirit_gen_interval << flag << counter << anti << mp;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> inc_dark_light >> dec_dark_skill_cd >> spirit_gen_interval >> flag >> counter >> anti >> mp;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_JUNIORDARKFORM);
		_parent.RemoveFilter(FILTER_JUNIORLIGHTFORM);
		_parent.RemoveFilter(FILTER_SENIORLIGHTFORM);
		_parent.RemoveFilter(FILTER_DARKUNIFORM);
		_parent.RemoveFilter(FILTER_LIGHTUNIFORM);
		_parent.SetDarkLightForm(3);
		if (_parent.GetDarkLight() < 0)
		{
			this->counter = this->spirit_gen_interval;
			dec_dark_skill_cd = this->dec_dark_skill_cd;
			_parent.GetSkillWrapper().DecDarkSkillCoolTime(dec_dark_skill_cd);
			this->flag = 1;
			_parent.EnhanceMaxMP(this->mp);
			for (int i = 0; i <= 5; ++i)
				_parent.EnhanceResistance(i, this->anti);
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			for (int ia = 0; ia <= 2; ++ia)
				_parent.AddDarkLightSpirit(1);
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		_parent.ClearDarkLightForm();
		if (this->flag)
		{
			_parent.ImpairMaxMP(this->mp);
			for (int i = 0; i <= 5; ++i)
				_parent.ImpairResistance(i, this->anti);
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			dec_dark_skill_cd = this->dec_dark_skill_cd;
			_parent.GetSkillWrapper().IncDarkSkillCoolTime(dec_dark_skill_cd);
		}
	}
};

class filter_Decfatalratio : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decfatalratio() {}
public:
	DECLARE_SUBSTANCE(filter_Decfatalratio);

	filter_Decfatalratio(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_DECFATALRATIO;
		_buff_id = HSTATE_DECFATALRATIO;
	}

	~filter_Decfatalratio() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceAntiCritRate(this->_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiCritRate(-this->_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Talismaneffectsbless : public filter
{
protected:
	int _level;
	char _force;
	int _var[16];

	std::vector<short> skills;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Talismaneffectsbless() {}
public:
	DECLARE_SUBSTANCE(filter_Talismaneffectsbless);

	filter_Talismaneffectsbless(
		object_interface object,
		int level,
		char force,
		int* var,
		const std::vector<short int>& refineSkillList) : filter(object, FILTER_MASK)
	{
		this->_level = level;
		this->_force = force;
		memcpy(this->_var, var, sizeof(this->_var));
		for (int i = 0; i < refineSkillList.size(); ++i)
		{
			skills.push_back(refineSkillList[i]);
		}

		_filter_id = FILTER_TALISMANEFFECTBLESS;
		//_buff_id = HSTATE_TALISMANEFFECTBLESS;

	}

	~filter_Talismaneffectsbless() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _force << _level;
		ar.push_back(_var,sizeof(_var));
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _force >> _level;
		ar.pop_back(_var, sizeof(_var));
		return true;
	}

	void OnAttach()
	{
		for (int i = 0; i < skills.size(); ++i)
		{
			short skillid = skills[i];
			const SkillStub* stub = SkillStub::GetStub(skillid);
			if (stub && stub->type == 2 && stub->rangetype == 5)
			{
				enchant_msg msg;
				memset(&msg, 0, sizeof(msg));
				msg.skill = skillid;
				msg.skill_level = this->_level;
				msg.force_attack = this->_force;
				msg.attack_range = _parent.GetExtendProp().attack_range;
				msg.helpful = 1;
				memcpy(msg.skill_var, this->_var, sizeof(msg.skill_var));

				_parent.GetSkillWrapper().SetSkillTalent(skillid, msg.skill_modify, _parent);
				_parent.Enchant(_parent.GetSelfID(), msg);
			}
		}
	}

	void OnDetach()
	{
	}
};

class filter_Posexchangewithclone : public filter
{
protected:


	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE
	};

	filter_Posexchangewithclone() {}
public:
	DECLARE_SUBSTANCE(filter_Posexchangewithclone);

	filter_Posexchangewithclone(object_interface object) : filter(object, FILTER_MASK)
	{

		_filter_id = FILTER_POSEXCHANGEWITHCLONE;
		//_buff_id = HSTATE_POSEXCHANGEWITHCLONE;

	}

	~filter_Posexchangewithclone() {}

	virtual bool Save(archive& ar)
	{
		return true;
	}

	virtual bool Load(archive& ar)
	{
		return true;
	}

	void OnAttach()
	{
		_parent.ExchangePos(_parent.GetCloneID());
	}

	void OnDetach()
	{
	}
};

class filter_Gtaddattackdefense : public filter
{
protected:
	int attack_ratio;
	int defense_ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE 
	};

	filter_Gtaddattackdefense() {}
public:
	DECLARE_SUBSTANCE(filter_Gtaddattackdefense);

	filter_Gtaddattackdefense(
		object_interface object,
		int value,
		int amount,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->attack_ratio = amount;
		this->defense_ratio = value;

		_filter_id = buffid + FILTER_GTADDATTACKDEFENSE;
		_buff_id = HSTATE_GTADDATTACKDEFENSE;

	}

	~filter_Gtaddattackdefense() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << attack_ratio << defense_ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> attack_ratio >> defense_ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(this->attack_ratio);
		_parent.EnhanceScaleDefense(this->defense_ratio);
		_parent.UpdateAttackData();
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(this->attack_ratio);
		_parent.ImpairScaleDefense(this->defense_ratio);
		_parent.UpdateAttackData();
		_parent.UpdateDefenseData();
	}
};

//145 
class filter_Subantisilent2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantisilent2() {}
public:
	DECLARE_SUBSTANCE(filter_Subantisilent2);

	filter_Subantisilent2(
		object_interface object,
		int v,
		int period,
		int overlay_cnt) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = v; 
		_max_overlay_cnt = overlay_cnt;
		_can_overlay = 1;

		_filter_id = FILTER_SUBANTISILENT2;
		_buff_id = HSTATE_SUBANTISILENT2;
	}

	~filter_Subantisilent2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(3, this->_value);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(3, this->_value);
		_parent.UpdateMagicData();
	}
};

class filter_Scopedamage : public filter
{
protected:
	int _value;
	float _ratio;
	float _radius;
	char _forceattack;
	int _coverage;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH 
	};

	filter_Scopedamage() {}
public:
	DECLARE_SUBSTANCE(filter_Scopedamage);

	filter_Scopedamage(
		object_interface object,
		int value,
		float ratio,
		float radius,
		char forceattack,
		int coverage) : filter(object, FILTER_MASK)
	{
		this->_value = value;
		this->_ratio = ratio;
		this->_radius = radius;
		this->_forceattack = forceattack;
		this->_coverage = coverage;

		_filter_id = FILTER_SCOPEDAMAGE;
		//_buff_id = HSTATE_SCOPEDAMAGE;

	}

	~filter_Scopedamage() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _value << _ratio << _radius << _forceattack << _coverage;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _value >> _ratio >> _radius >> _forceattack >> _coverage;
		return true;
	}

	void OnAttach()
	{
		std::vector<exclude_target> empty;
		int playercnt = _parent.GetRegionPlayerCount(_radius, empty);
		if (playercnt > 0)
		{
			if (playercnt > this->_coverage)
				playercnt = this->_coverage;
			attack_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.force_attack = this->_forceattack;
			msg.spec_damage = playercnt * _parent.GetExtendProp().damage_high * this->_ratio + _value * playercnt;
			msg.skill_id = 0;
			msg.attack_rate = _parent.GetExtendProp().attack;
			XID xid(-1, -1);
			_parent.FillAttackMsg(xid, msg, 0);
			std::vector<exclude_target> empty_0;
			_parent.RegionAttack1(_parent.GetPos(), _radius, msg, 0, _coverage, empty_0, 1);
		}
	}

	void OnDetach()
	{
	}
};

class filter_Skillmirror : public timeout_filter
{
protected:
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Skillmirror() {}
public:
	DECLARE_SUBSTANCE(filter_Skillmirror);

	filter_Skillmirror(
		object_interface object,
		int period,
		int prob) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_prob = prob;

		_filter_id = FILTER_SKILLMIRROR;
		_buff_id = HSTATE_SKILLMIRROR;
	}

	~filter_Skillmirror() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _prob;
		return true;
	}

	void OnAttach()
	{
		_parent.SetSkillMirror( 1, this->_prob);
	}

	void OnDetach()
	{
		_parent.SetSkillMirror( 0, this->_prob);
	}
};

class filter_Addantisleep : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addantisleep() {}
public:
	DECLARE_SUBSTANCE(filter_Addantisleep);

	filter_Addantisleep(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = v;

		_filter_id = buffid + FILTER_ADDANTISLEEP;
		_buff_id = HSTATE_ADDANTISLEEP;
	}

	~filter_Addantisleep() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance( 4, this->_addend);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance( 4, this->_addend);
		_parent.UpdateMagicData();
	}
};

class filter_Deathscatter : public timeout_filter
{
protected:
	float _radius;
	int _probability;
	float _ratio;
	int _plus;
	int _total;
	char _force;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BEFORE_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Deathscatter() {}
public:
	DECLARE_SUBSTANCE(filter_Deathscatter);

	filter_Deathscatter(
		object_interface object,
		float radius,
		int probability,
		float ratio,
		int plus,
		int total,
		char force,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_radius = radius;
		this->_probability = probability;
		this->_ratio = ratio;
		this->_plus = plus;
		this->_total = total;
		this->_force = force;

		_filter_id = FILTER_DEATHSCATTER;
		_buff_id = HSTATE_DEATHSCATTER;
	}

	~filter_Deathscatter() {}

	void BeforeDeath(bool is_duel)
	{
		if (!is_duel && rand() % 100 <= this->_probability)
		{
			attack_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.damage_low = _parent.GetExtendProp().damage_low * _ratio + _plus;
			msg.damage_high = _parent.GetExtendProp().damage_high * _ratio + _plus;
			msg.force_attack = this->_force;
			msg.attack_rate = _parent.GetExtendProp().attack;
			msg.crit_rate = _parent.GetExtendProp().crit_rate;
			msg.crit_factor = _parent.GetExtendProp().crit_damage;
			XID xid(-1, -1);
			_parent.FillAttackMsg(xid, msg, 0);

			std::vector<exclude_target> empty;
			_parent.RegionAttack1(_parent.GetPos(), _radius, msg, 0, _total, empty, 1);
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _radius << _probability << _ratio << _plus << _total << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _radius >> _probability >> _ratio >> _plus >> _total >> _force;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

//150
class filter_Spiritdrag : public timeout_filter
{
protected:
	int speed;
	int stop_dist;
	XID _performer;
	attacker_info_t _pinfo;
	bool _invader;
	char _mode;
	bool _error;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_TRANSLATE_ENCHANT | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_RECV_MSG
	};

	filter_Spiritdrag() {}
public:
	DECLARE_SUBSTANCE(filter_Spiritdrag);

	filter_Spiritdrag(
		object_interface object,
		int period,
		float ratio,
		float value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->speed = ratio;
		this->stop_dist = value * value;
		this->_error = 0;

		_filter_id = FILTER_SPIRITDRAG;
		_buff_id = HSTATE_SPIRITDRAG;
	}

	~filter_Spiritdrag() {}

	void SetUp(const XID& player, const attacker_info_t& info, char mode, bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		if (!this->_error)
		{
			A3DVECTOR tpos; 
			float tbody = 0;
			if (_parent.QueryObject(this->_performer, tpos, tbody) == 1)
			{
				float range = tpos.horizontal_distance(_parent.GetPos());
				if (range >= 4.0)
					_parent.PlayerFollowTarget(
						this->_performer,
						this->speed,
						2.0);
				if (range >= this->stop_dist)
					_is_deleted = 1;
				timeout_filter::Heartbeat(tick);
			}
		}
	}

	void TranslateRecvAttack(const XID& attacker,attack_msg& msg)
	{
		if (_performer == attacker)
			_is_deleted = 1;
	}

	void TranslateRecvEnchant(const XID& attacker,enchant_msg& msg)
	{
		if (_performer == attacker)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << speed << stop_dist << _performer ;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode << _error;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> speed >> stop_dist >> _performer ;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode >> _error;
		return true;
	}

	void OnAttach()
	{
		float tbody = 0; 

		A3DVECTOR tpos;
		if (_parent.QueryObject(this->_performer, tpos, tbody) == 1)
		{
			A3DVECTOR near;
			if (_parent.CollisionDetect(tpos,near,0.2))
			{
				_parent.SetSilentSeal( 1);
				_parent.SetMeleeSeal( 1);
				_parent.SetRootSeal( 1);
				_parent.SendClientNotifyRoot( 0x10u);
				_parent.PlayerFollowTarget(_performer,speed,2.0);
				_parent.PlayerBeSpiritDraged( this->_performer, 1);
			}
			else
			{
				this->_error = 1;
				_is_deleted = 1;
			}
		}
		else
		{
			this->_error = 1;
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		if (!this->_error)
		{
			_parent.SetSilentSeal( 0);
			_parent.SetMeleeSeal( 0);
			_parent.SetRootSeal( 0);
			_parent.SendClientDispelRoot( 16);
			_parent.StopPlayerFollowTarget();
			_parent.PlayerBeSpiritDraged( this->_performer, 0);
		}
	}
};

class filter_Immunewrap : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Immunewrap() {}
public:
	DECLARE_SUBSTANCE(filter_Immunewrap);

	filter_Immunewrap(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_IMMUNEWRAP;
		_buff_id = HSTATE_IMMUNEWRAP;
	}

	~filter_Immunewrap() {}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNEWRAP);
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNEWRAP);
	}
};

class filter_Turndebuff : public timeout_filter
{
protected:
	int turnMaxCnt;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Turndebuff() {}
public:
	DECLARE_SUBSTANCE(filter_Turndebuff);

	filter_Turndebuff(
		object_interface object,
		int period,
		int amount,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->turnMaxCnt = amount;

		_filter_id = buffid + FILTER_TURNDEBUFF;
		_buff_id = HSTATE_TURNDEBUFF;
	}

	~filter_Turndebuff() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << turnMaxCnt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> turnMaxCnt;
		return true;
	}

	void OnAttach()
	{
		_parent.ClearRandomSpecFilter(0x10000000, this->turnMaxCnt);
		_parent.SetTurnDebuffState(1);
		_parent.IncVisibleState(VSTATE_TURNDEBUFF);
	}

	void OnDetach()
	{
		_parent.SetTurnDebuffState(0);
		_parent.DecVisibleState(VSTATE_TURNDEBUFF);
	}
};

class filter_Retort : public timeout_filter
{
protected:
	float _ratio;
	int _prob;
	char _force;
	int _retort_prob;
	int _max_damage;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Retort() {}
public:
	DECLARE_SUBSTANCE(filter_Retort);

	filter_Retort(
		object_interface object,
		float ratio,
		int prob,
		int period,
		char force,
		int value,
		int amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_prob = prob;
		this->_force = force;
		this->_retort_prob = value;
		this->_max_damage = amount;

		_filter_id = FILTER_RETORT;
		_buff_id = HSTATE_RETORT;
	}

	~filter_Retort() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& message)
	{
		if (rand() % 100 <= this->_retort_prob)
		{
			float damage = dmg.damage * this->_ratio;
			if (this->_max_damage > 0 && damage > this->_max_damage)
				damage = this->_max_damage;
			if (damage > 1)
			{
				attack_msg msg;
				memset(&msg, 0, sizeof(msg));
				msg.damage_low = 0;
				msg.damage_high = 0;
				msg.force_attack = this->_force;
				msg.skill_id = 0;
				msg.attack_rate = 99999;
				msg.spec_damage = damage;
				_parent.SetRetortState();
				XID xid(-1, -1);
				_parent.FillAttackMsg(xid, msg, 0);
				_parent.Attack(attacker, msg, 0);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio << _prob << _force << _retort_prob << _max_damage;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio >> _prob >> _force >> _retort_prob >> _max_damage;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Asforest : public filter
{
protected:
	int _add_dodge;
	int _add_skill_dodge;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE 
	};

	filter_Asforest() {}
public:
	DECLARE_SUBSTANCE(filter_Asforest);

	filter_Asforest(
		object_interface object,
		int add_dodge,
		int add_skill_dodge) : filter(object, FILTER_MASK)
	{
		this->_add_dodge = add_dodge;
		this->_add_skill_dodge = add_skill_dodge;

		_filter_id = FILTER_ASFOREST;
		_buff_id = HSTATE_ASFOREST;

	}

	~filter_Asforest() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _add_dodge << _add_skill_dodge;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _add_dodge >> _add_skill_dodge;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_ASWIND);
		_parent.RemoveFilter(FILTER_ASFIRE);
		_parent.RemoveFilter(FILTER_ASHILL);
		_parent.EnhanceArmor(this->_add_dodge);
		_parent.UpdateDefenseData();
		_parent.EnhanceSkillArmor(this->_add_skill_dodge);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairArmor(this->_add_dodge);
		_parent.UpdateDefenseData();
		_parent.EnhanceSkillArmor(-this->_add_skill_dodge);
		_parent.UpdateAttackData();
	}
};

//155 
class filter_Addspeed : public timeout_filter
{
protected:
	float _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addspeed() {}
public:
	DECLARE_SUBSTANCE(filter_Addspeed);

	filter_Addspeed(
		object_interface object,
		float r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;

		_filter_id = buffid + FILTER_ADDSPEED;
		_buff_id = HSTATE_ADDSPEED;
	}

	~filter_Addspeed() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceSpeed(this->_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.ImpairSpeed(this->_addend);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Jail : public timeout_filter
{
protected:
	float limitAreaRadius;
	float permitAreaRadius;

	int paSkillId;
	int paSkillLevel;
	int laSkillId;
	int laSkillLevel;
	char _force;
	char _counter; 
	bool permitRemove;
	
	std::vector<XID> jailMembers;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Jail() {}
public:
	DECLARE_SUBSTANCE(filter_Jail);

	filter_Jail(
		object_interface object,
		int period,
		float prob,
		int value,
		int amount,
		int ratio,
		int buffid,
		char force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->paSkillId = value;
		this->paSkillLevel = amount;
		this->laSkillId = ratio;
		this->laSkillLevel = buffid;
		this->_force = force;
		this->permitAreaRadius = prob;
		this->limitAreaRadius = prob + 6.0;
		this->_counter = 2;

		_filter_id = buffid + FILTER_JAIL;
		//_buff_id = HSTATE_JAIL;
	}

	~filter_Jail() {}

	void Heartbeat(int tick)
	{
		timeout_filter::Heartbeat(tick);
		if (!_is_deleted)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->laSkillId;
			msg.skill_level = this->laSkillLevel;
			msg.force_attack = this->_force;
			msg.attack_range = this->permitAreaRadius;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;

			std::vector<exclude_target> empty;
			_parent.RegionEnchant1(_parent.GetPos(), limitAreaRadius, msg, 500, empty);
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << limitAreaRadius << permitAreaRadius << paSkillId << paSkillLevel;
		ar << laSkillId << laSkillLevel << _force << permitRemove;
		ar << jailMembers.size();
		for (int i = 0; i < jailMembers.size(); i++)
		{
			ar << jailMembers[i];
		}
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> limitAreaRadius >> permitAreaRadius >> paSkillId >> paSkillLevel;
		ar >> laSkillId >> laSkillLevel >> _force >> permitRemove;
		int size;
		ar >> size;
		for (int i = 0; i < size; i++)
		{
			XID xid;
			ar >> xid;
			jailMembers.push_back(xid);
		}
		return true;
	}

	void OnAttach()
	{
		_parent.GetPlayerInJail(jailMembers, _parent.GetPos(),permitAreaRadius,1140457472);
		if (jailMembers.size())
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->paSkillId;
			msg.skill_level = this->paSkillLevel;
			msg.force_attack = this->_force;
			msg.attack_range = this->permitAreaRadius;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
			for (int i = 0; i < jailMembers.size(); ++i)
			{
				_parent.Enchant(jailMembers[i], msg);
			}
		}
	}

	void OnDetach()
	{
		for (int i = 0; i < jailMembers.size(); ++i)
		{
			_parent.SendRemovePermitCycleArea(jailMembers[i]);
		}
	}
};

class filter_Addrage2 : public filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_AFTER_DEATH
	};

	filter_Addrage2() {}
public:
	DECLARE_SUBSTANCE(filter_Addrage2);

	filter_Addrage2(object_interface object, float ratio) : filter(object, FILTER_MASK)
	{
		this->_ratio = ratio;

		_filter_id = FILTER_ADDRAGE2;
		_buff_id = HSTATE_ADDRAGE2;

	}

	~filter_Addrage2() {}

	void DamageEnemy(const XID& target, int damage)
	{
		int value = (int)((long double)damage * this->_ratio);
		if (value > 0)
			_parent.AddRage( value);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Attack2mp : public timeout_filter
{
protected:
	float _ratio;
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Attack2mp() {}
public:
	DECLARE_SUBSTANCE(filter_Attack2mp);

	filter_Attack2mp(
		object_interface object,
		float ratio,
		int period,
		int prob) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_prob = prob;

		_filter_id = FILTER_ATTACK2MP;
		_buff_id = HSTATE_MPSUCK;
	}

	~filter_Attack2mp() {}

	void TranslateSendAttack(const XID& target,attack_msg& msg,char type,float r,int mc)
	{
		if (rand() % 100 < this->_prob)
		{
			int addmp = msg.damage_high * this->_ratio;
			if (addmp > 1)
				_parent.InjectMana(addmp, 0);
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Immunedizzy : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT 
	};

	filter_Immunedizzy() {}
public:
	DECLARE_SUBSTANCE(filter_Immunedizzy);

	filter_Immunedizzy(
		object_interface object,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_IMMUNEDIZZY;
		_buff_id = HSTATE_IMMUNEDIZZY;
	}

	~filter_Immunedizzy() {}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNEDIZZY);
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNEDIZZY);
	}
};

//160
class filter_Buxiu : public timeout_filter
{
protected:
	int _rage;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Buxiu() {}
public:
	DECLARE_SUBSTANCE(filter_Buxiu);

	filter_Buxiu(object_interface object, int value, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_rage = value;

		_filter_id = FILTER_BUXIU;
		_buff_id = HSTATE_BUXIU;
	}

	~filter_Buxiu() {}

	void Heartbeat(int tick)
	{
		_parent.AddRage(_rage);
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _rage;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _rage;
		return true;
	}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNEDAMAGE);
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNEDAMAGE);
	}
};

class filter_Tiangu : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Tiangu() {}
public:
	DECLARE_SUBSTANCE(filter_Tiangu);

	filter_Tiangu(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_TIANGU;
		_buff_id = HSTATE_TIANGU;
	}

	~filter_Tiangu() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleMaxMP(this->_ratio);
		_parent.UpdateHPMP();
		_parent.EnhanceMaxHP(this->_addend);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairScaleMaxMP(this->_ratio);
		_parent.UpdateHPMP();
		_parent.ImpairMaxHP(this->_addend);
		_parent.UpdateHPMP();
	}
};

class filter_Dodgeregain : public timeout_filter
{
protected:
	float _ratio;
	int _addend;
	int _maxcount;
	int _count;
	int _prob;

	std::vector<XID> jailMembers;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Dodgeregain() {}
public:
	DECLARE_SUBSTANCE(filter_Dodgeregain);

	filter_Dodgeregain(
		object_interface object,
		float ratio,
		int addend,
		int max,
		int prob,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_addend = addend;
		this->_maxcount = max;
		this->_count = 0;
		this->_prob = prob;

		_filter_id = FILTER_DODGEREGAIN;
		_buff_id = HSTATE_DODGEREGAIN;
	}

	~filter_Dodgeregain() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		if (this->_count < this->_maxcount)
		{
			if (rand() % 100 <= this->_prob)
			{
				float hp = _parent.GetBasicProp().hp;
				if (this->_ratio > hp / _parent.GetExtendProp().max_hp)
				{
					_parent.EnhanceArmor( this->_addend);
					_parent.UpdateDefenseData();
					++this->_count;
				}
			}
		}
		else
		{
			_is_deleted = 1;
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _addend << _maxcount << _count << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _addend >> _maxcount >> _count >> _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		if (this->_count)
		{
			_parent.ImpairArmor(this->_count * this->_addend);
			_parent.UpdateDefenseData();
		}
	}
};

class filter_Subantiwrap : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantiwrap() {}
public:
	DECLARE_SUBSTANCE(filter_Subantiwrap);

	filter_Subantiwrap(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = buffid + FILTER_SUBANTIWRAP;
		_buff_id = HSTATE_SUBANTIWRAP;

		this->_sub = v;

	}

	~filter_Subantiwrap() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(2, this->_sub);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(2, this->_sub);
		_parent.UpdateMagicData();
	}
};

class filter_Swift : public timeout_filter
{
protected:
	int praySpeedRatio;
	int speedRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Swift() {}
public:
	DECLARE_SUBSTANCE(filter_Swift);

	filter_Swift(
		object_interface object,
		int period,
		int ratio,
		int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->praySpeedRatio = ratio;
		this->speedRatio = value;

		_filter_id = FILTER_SWIFT;
		_buff_id = HSTATE_SWIFT;
	}

	~filter_Swift() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << praySpeedRatio << speedRatio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> praySpeedRatio >> speedRatio;
		return true;
	}

	void OnAttach()
	{
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.EnhanceScaleSpeed(this->speedRatio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.ImpairScaleSpeed(this->speedRatio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

//165
class filter_Bloodthirsty : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_DAMAGEENEMY | FILTER_MASK_HEARTBEAT
	};

	filter_Bloodthirsty() {}
public:
	DECLARE_SUBSTANCE(filter_Bloodthirsty);

	filter_Bloodthirsty(
		object_interface object,
		int period,
		float ratio,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_BLOODTHIRSTY;
		_buff_id = HSTATE_BLOODTHIRSTY;
	}

	~filter_Bloodthirsty() {}

	void DamageEnemy(const XID& target, int damage)
	{
		int hp = (int)((long double)damage * this->_ratio);
		if (hp > 0)
			_parent.Heal(hp, 0, 0);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.SetBloodThirstyState(1);
	}

	void OnDetach()
	{
		_parent.SetBloodThirstyState(0);
	}
};

class filter_Ashill : public filter
{
protected:
	int _add_defence;
	int _dec_crit_rate;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE 
	};

	filter_Ashill() {}
public:
	DECLARE_SUBSTANCE(filter_Ashill);

	filter_Ashill(
		object_interface object,
		int add_defence,
		int dec_crit_rate) : filter(object, FILTER_MASK)
	{
		this->_add_defence = add_defence;
		this->_dec_crit_rate = dec_crit_rate;

		_filter_id = FILTER_ASHILL;
		_buff_id = HSTATE_ASHILL;

	}

	~filter_Ashill() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _add_defence << _dec_crit_rate;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _add_defence >> _dec_crit_rate;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(4171);
		_parent.RemoveFilter(4172);
		_parent.RemoveFilter(4173);
		_parent.EnhanceAntiCritRate(this->_dec_crit_rate);
		_parent.UpdateAttackData();
		_parent.EnhanceDefense(this->_add_defence);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiCritRate(-this->_dec_crit_rate);
		_parent.ImpairDefense(this->_add_defence);
		_parent.UpdateAttackData();
		_parent.UpdateDefenseData();
	}
};

class filter_Addhp2 : public timeout_filter
{
protected:
	int _addend;
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addhp2() {}
public:
	DECLARE_SUBSTANCE(filter_Addhp2);

	filter_Addhp2(
		object_interface object,
		int r,
		int period,
		int buffid,
		float amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = amount;

		_filter_id = buffid + FILTER_ADDHP2;
		_buff_id = HSTATE_ADDHP2;
	}

	~filter_Addhp2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMaxHP(this->_addend);
		_parent.UpdateHPMP();
		_parent.EnhanceAntiCritDamage(this->_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairMaxHP(this->_addend);
		_parent.UpdateHPMP();
		_parent.EnhanceAntiCritDamage(*(int*)&_ratio ^ IMMUNEDAMAGE);
		_parent.UpdateAttackData();
	}
};

class filter_Incmountspeed : public timeout_filter
{
protected:
	float incspeed;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incmountspeed() {}
public:
	DECLARE_SUBSTANCE(filter_Incmountspeed);

	filter_Incmountspeed(
		object_interface object,
		int period,
		float value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->incspeed = value;

		_filter_id = buffid + FILTER_INCMOUNTSPEED;
		_buff_id = HSTATE_INCMOUNTSPEED;
	}

	~filter_Incmountspeed() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << incspeed;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> incspeed;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMountSpeed(incspeed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.ImpairMountSpeed(incspeed);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Recordpos : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Recordpos() {}
public:
	DECLARE_SUBSTANCE(filter_Recordpos);

	filter_Recordpos(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_RECORDPOS;
		_buff_id = HSTATE_RECORDPOS;
	}

	~filter_Recordpos() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.GetSkillWrapper().RecordPos(_parent);
	}

	void OnDetach()
	{
		_parent.GetSkillWrapper().ClearRecordPos();
	}
};

//170
class filter_Addmp : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addmp() {}
public:
	DECLARE_SUBSTANCE(filter_Addmp);

	filter_Addmp(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;

		_filter_id = buffid + FILTER_ADDMP;
		_buff_id = HSTATE_ADDMP;
	}

	~filter_Addmp() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMaxMP( this->_addend);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairMaxMP( this->_addend);
		_parent.UpdateHPMP();
	}
};

class filter_Subanti : public timeout_filter
{
protected:
	int _var;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subanti() {}
public:
	DECLARE_SUBSTANCE(filter_Subanti);

	filter_Subanti(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_var = v;

		_filter_id = buffid + FILTER_SUBANTI;
		_buff_id = HSTATE_SUBANTI;
	}

	~filter_Subanti() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _var;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _var;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(0, this->_var);
		_parent.ImpairResistance(1, this->_var);
		_parent.ImpairResistance(2, this->_var);
		_parent.ImpairResistance(3, this->_var);
		_parent.ImpairResistance(4, this->_var);
		_parent.ImpairResistance(5, this->_var);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(0, this->_var);
		_parent.EnhanceResistance(1, this->_var);
		_parent.EnhanceResistance(2, this->_var);
		_parent.EnhanceResistance(3, this->_var);
		_parent.EnhanceResistance(4, this->_var);
		_parent.EnhanceResistance(5, this->_var);
		_parent.UpdateMagicData();
	}
};

class filter_Cycdecattack : public timeout_filter
{
protected:
	int _amount;
	int _ratio;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Cycdecattack() {}
public:
	DECLARE_SUBSTANCE(filter_Cycdecattack);

	filter_Cycdecattack(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r / period;
		this->_amount = 0;
		this->_counter = 0;

		_filter_id = buffid + FILTER_CYCDECATTACK;
		_buff_id = HSTATE_CYCDECATTACK;
	}

	~filter_Cycdecattack() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			this->_amount += this->_counter * this->_ratio;
			_parent.ImpairScaleDamage(this->_counter * this->_ratio);
			_parent.UpdateAttackData();
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _amount << _ratio << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _amount >> _ratio >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_WEAK);
	}

	void OnDetach()
	{
		_parent.EnhanceScaleDamage(this->_amount);
		_parent.UpdateAttackData();
		_parent.DecVisibleState(VSTATE_WEAK);
	}
};

class filter_Gangdan : public filter
{
protected:
	float _ratio_hp2mp;
	int _dec_crit_rate;
	float _dec_crit_hurt;
	float _discount;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_ADJUST_DAMAGE
	};

	filter_Gangdan() {}
public:
	DECLARE_SUBSTANCE(filter_Gangdan);

	filter_Gangdan(
		object_interface object,
		float ratio_hp2mp,
		int dec_crit_rate,
		float dec_crit_hurt,
		float discount) : filter(object, FILTER_MASK)
	{
		this->_ratio_hp2mp = ratio_hp2mp;
		this->_dec_crit_rate = dec_crit_rate;
		this->_dec_crit_hurt = dec_crit_hurt;
		this->_discount = discount;

		_filter_id = FILTER_GANGDAN;
		_buff_id = HSTATE_GANGDAN;

	}

	~filter_Gangdan() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		int decmp = (int)(dmg.damage * this->_ratio_hp2mp * this->_discount);
		int mp = _parent.GetBasicProp().mp;
		if (decmp > mp)
			decmp = mp;
		_parent.DrainMana(decmp);
		dmg.damage = dmg.damage - (long double)(int)((long double)decmp / this->_discount);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio_hp2mp << _dec_crit_rate << _dec_crit_hurt << _discount;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio_hp2mp >> _dec_crit_rate >> _dec_crit_hurt >> _discount;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(4169);
		_parent.EnhanceAntiCritRate(this->_dec_crit_rate);
		_parent.EnhanceAntiCritDamage(this->_dec_crit_hurt);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiCritRate( -this->_dec_crit_rate);
		_parent.EnhanceAntiCritDamage(*(int*)&_dec_crit_hurt ^ 0x80000000);
		_parent.UpdateAttackData();
	}
};

class filter_Fashen : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Fashen() {}
public:
	DECLARE_SUBSTANCE(filter_Fashen);

	filter_Fashen(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_FASHEN;
		_buff_id = HSTATE_FASHEN;
	}

	~filter_Fashen() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceCrit(this->_ratio);
		_parent.EnhanceMaxHP(this->_addend);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairCrit(this->_ratio);
		_parent.ImpairMaxHP(this->_addend);
		_parent.UpdateHPMP();
	}
};

//175 
class filter_Bloodthirsty2 : public timeout_filter
{
protected:
	float hpratio;
	int damage;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED | FILTER_MASK_HEARTBEAT
	};

	filter_Bloodthirsty2() {}
public:
	DECLARE_SUBSTANCE(filter_Bloodthirsty2);

	filter_Bloodthirsty2(
		object_interface object,
		int period,
		float ratio) : timeout_filter(object, period, FILTER_MASK)
	{
		this->hpratio = ratio;

		_filter_id = FILTER_BLOODTHIRSTY2;
		//_buff_id = HSTATE_BLOODTHIRSTY2;
	}

	~filter_Bloodthirsty2() {}

	void BeAttacked(const XID& target,int dmg,bool crit)
	{
		this->damage = dmg;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << hpratio << damage;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> hpratio >> damage;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		float hp = damage * hpratio;
		if (hp > 0)
			_parent.Heal(hp, 0, 0);
	}
};

class filter_Miracleburstxian : public timeout_filter
{
protected:
	int _ratio;
	int _dpdecspeed;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Miracleburstxian() {}
public:
	DECLARE_SUBSTANCE(filter_Miracleburstxian);

	filter_Miracleburstxian(
		object_interface object,
		int ratio,
		int period,
		int buffid,
		int dpcost) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_dpdecspeed = dpcost / period;
		this->_counter = 0;

		_filter_id = buffid + FILTER_MIRACLEBURSTXIAN;
		_buff_id = HSTATE_MIRACLEBURSTXIAN;
	}

	~filter_Miracleburstxian() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			if (! _parent.DecDeity(_counter * this->_dpdecspeed))
				_is_deleted = 1;
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _dpdecspeed << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _dpdecspeed >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.SetFuryState(1);
		_parent.EnhanceScaleDeityPower(this->_ratio);
		_parent.EnhanceCultAttack(2, this->_ratio);
		_parent.EnhanceScaleDamage(this->_ratio);
		_parent.EnhanceScaleDmgReduce(this->_ratio);
		_parent.EnhanceScaleIgnDmgReduce(this->_ratio);
		_parent.UpdateAttackData();
		_parent.IncVisibleState(VSTATE_MIRACLEBURSTXIAN);
	}

	void OnDetach()
	{
		_parent.SetFuryState(0);
		_parent.EnhanceScaleDeityPower(-this->_ratio);
		_parent.EnhanceCultAttack(2, -this->_ratio);
		_parent.ImpairScaleDamage(this->_ratio);
		_parent.ImpairScaleDmgReduce(this->_ratio);
		_parent.ImpairScaleIgnDmgReduce(this->_ratio);
		_parent.UpdateAttackData();
		_parent.DecVisibleState(VSTATE_MIRACLEBURSTXIAN);
	}
};

class filter_Magicshield : public timeout_filter
{
protected:
	float _ratio;
	int _max;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Magicshield() {}
public:
	DECLARE_SUBSTANCE(filter_Magicshield);

	filter_Magicshield(
		object_interface object,
		float ratio,
		int max,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_max = max;

		_filter_id = FILTER_MAGICSHIELD;
		_buff_id = HSTATE_MAGICSHIELD;
	}

	~filter_Magicshield() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		int dec = (int)(dmg.damage * this->_ratio);
		int mp = _parent.GetBasicProp().mp;
		if (mp > dec)
		{
			if (this->_max <= dec)
			{
				_is_deleted = 1;
				dec = this->_max;
			}
		}
		else
		{
			_is_deleted = 1;
			dec = mp;
		}
		dmg.damage = dmg.damage - dec;
		this->_max -= dec;
		_parent.DrainMana(dec);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _max;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _max;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_MAGICSHIELD);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_MAGICSHIELD);
	}
};

class filter_Addanti : public timeout_filter
{
protected:
	int _var;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addanti() {}
public:
	DECLARE_SUBSTANCE(filter_Addanti);

	filter_Addanti(object_interface object, int v, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_var = v;

		_filter_id = FILTER_ADDANTI;
		_buff_id = HSTATE_ADDANTI;
	}

	~filter_Addanti() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _var;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _var;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceResistance(0, this->_var);
		_parent.EnhanceResistance(1, this->_var);
		_parent.EnhanceResistance(2, this->_var);
		_parent.EnhanceResistance(3, this->_var);
		_parent.EnhanceResistance(4, this->_var);
		_parent.EnhanceResistance(5, this->_var);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairResistance(0, this->_var);
		_parent.ImpairResistance(1, this->_var);
		_parent.ImpairResistance(2, this->_var);
		_parent.ImpairResistance(3, this->_var);
		_parent.ImpairResistance(4, this->_var);
		_parent.ImpairResistance(5, this->_var);
		_parent.UpdateMagicData();
	}
};

class filter_Incdefence : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incdefence() {}
public:
	DECLARE_SUBSTANCE(filter_Incdefence);

	filter_Incdefence(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_INCDEFENCE;
		_buff_id = HSTATE_INCDEFENCE;
	}

	~filter_Incdefence() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDefense(_ratio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDefense(_ratio);
		_parent.UpdateDefenseData();
	}
};

//180
class filter_Recoverhp : public timeout_filter
{
protected:
	int _speed;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Recoverhp() {}
public:
	DECLARE_SUBSTANCE(filter_Recoverhp);

	filter_Recoverhp(
		object_interface object,
		int period,
		int recoverHpSpeed) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_speed = recoverHpSpeed;

		_filter_id = FILTER_RECOVERHP;
		_buff_id = HSTATE_RECOVERHP;
	}

	~filter_Recoverhp() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1)
		{
			_parent.Heal(this->_speed, 0, 0);
			this->_counter -= 2;
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		//ar << _speed << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		//ar >> _speed >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_RECOVERYHP);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_RECOVERYHP);
	}
};

class filter_Addattack : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addattack() {}
public:
	DECLARE_SUBSTANCE(filter_Addattack);

	filter_Addattack(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;

		_filter_id = buffid + FILTER_ADDATTACK;
		_buff_id = HSTATE_ADDATTACK;
	}

	~filter_Addattack() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDamage(this->_addend);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairDamage(this->_addend);
		_parent.UpdateAttackData();
	}
};

class filter_Guilin : public timeout_filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Guilin() {}
public:
	DECLARE_SUBSTANCE(filter_Guilin);

	filter_Guilin(
		object_interface object,
		int ratio,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_GUILIN;
		_buff_id = HSTATE_GUILIN;
	}

	~filter_Guilin() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDefense(this->_ratio);
		_parent.UpdateDefenseData();
		_parent.EnhanceDefense(this->_addend);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDefense(this->_ratio);
		_parent.UpdateDefenseData();
		_parent.ImpairDefense(this->_addend);
		_parent.UpdateDefenseData();
	}
};

class filter_Decfatalhurt : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decfatalhurt() {}
public:
	DECLARE_SUBSTANCE(filter_Decfatalhurt);

	filter_Decfatalhurt(
		object_interface object,
		float r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_DECFATALHURT;
		_buff_id = HSTATE_DECFATALHURT;
	}

	~filter_Decfatalhurt() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceAntiCritDamage(_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiCritDamage(*(int*)&_ratio ^ 0x80000000);
		_parent.UpdateAttackData();
	}
};

class filter_Firing : public timeout_filter
{
protected:
	int		_amount;
	int		_damage;
	float		_radius;

	XID _performer;
	attacker_info_t _pinfo;

	char _counter;
	char	_force;
	bool _invader;
	char _mode;
	int _attacker_faction;
	int _target_faction;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Firing() {}
public:
	DECLARE_SUBSTANCE(filter_Firing);

	filter_Firing(
		object_interface object,
		int period,
		int amount,
		int value,
		float ratio,
		int buffid,
		char force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_amount = amount;
		this->_damage = value;
		this->_radius = ratio;
		this->_force = force;
		this->_counter = 0;

		_filter_id = buffid + FILTER_FIRING;
		_buff_id = HSTATE_FIRING;
	}

	~filter_Firing() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader,int attacker_faction,int target_faction)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
		this->_attacker_faction = attacker_faction;
		this->_target_faction = target_faction;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			if (this->_damage > 0)
				_parent.BeHurt(_performer,_pinfo,_damage,_invader,_mode);
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = 3869;
			msg.skill_level = 1;
			msg.force_attack = this->_force;
			msg.attack_range = _parent.GetExtendProp().attack_range;
			msg.skill_var[0] = this->_damage;
			memcpy(&msg, &this->_pinfo, sizeof(_pinfo));
			msg.attacker_faction = this->_attacker_faction;
			msg.target_faction = this->_target_faction;

			std::vector<exclude_target> empty;
			_parent.SendRegionEnchant1Msg(_parent.GetPos(), _radius, msg, _amount, empty);
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _amount << _damage << _radius << _performer << _counter ;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _amount >> _damage >> _radius >> _performer >> _counter ;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode >> _force;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_FIRING);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_FIRING);
	}
};

//185
class filter_Incdrugeffect : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incdrugeffect() {}
public:
	DECLARE_SUBSTANCE(filter_Incdrugeffect);

	filter_Incdrugeffect(
		object_interface object,
		float ratio,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = (int)(ratio * 100.0);

		_filter_id = buffid + FILTER_INCDRUGEFFECT;
		_buff_id = HSTATE_INCDRUGEFFECT;
	}

	~filter_Incdrugeffect() {}

	virtual bool Save(archive& ar)
	{
		//timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.SetHealEffect(this->_ratio);
	}

	void OnDetach()
	{
		_parent.SetHealEffect(-this->_ratio);
	}
};

class filter_Decanti : public timeout_filter
{
protected:
	int _var;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decanti() {}
public:
	DECLARE_SUBSTANCE(filter_Decanti);

	filter_Decanti(object_interface object, int v, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_var = v;

		_filter_id = FILTER_DECANTI;
		_buff_id = HSTATE_DECANTI;
	}

	~filter_Decanti() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _var;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _var;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairScaleResistance(0, this->_var);
		_parent.ImpairScaleResistance(1, this->_var);
		_parent.ImpairScaleResistance(2, this->_var);
		_parent.ImpairScaleResistance(3, this->_var);
		_parent.ImpairScaleResistance(4, this->_var);
		_parent.ImpairScaleResistance(5, this->_var);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceScaleResistance(0, this->_var);
		_parent.EnhanceScaleResistance(1, this->_var);
		_parent.EnhanceScaleResistance(2, this->_var);
		_parent.EnhanceScaleResistance(3, this->_var);
		_parent.EnhanceScaleResistance(4, this->_var);
		_parent.EnhanceScaleResistance(5, this->_var);
		_parent.UpdateMagicData();
	}
};

class filter_Lightuniform : public filter
{
protected:
	int dec_dark_light;
	int dec_light_skill_cd;
	int spirit_gen_interval;
	bool flag;
	int counter;
	int anti;
	int mp;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Lightuniform() {}
public:
	DECLARE_SUBSTANCE(filter_Lightuniform);

	filter_Lightuniform(
		object_interface object,
		float ratio,
		int value,
		int amount,
		int time,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->dec_dark_light = value;
		this->dec_light_skill_cd = (int)ratio;
		this->spirit_gen_interval = amount / 1000;
		this->flag = this->flag;
		this->counter = 0;
		this->anti = buffid;
		this->mp = time;

		_filter_id = FILTER_LIGHTUNIFORM;
		_buff_id = HSTATE_LIGHTUNIFORM;

	}

	~filter_Lightuniform() {}

	void Heartbeat(int tick)
	{
		if (!_is_deleted)
		{
			_parent.DecDarkLight(this->dec_dark_light);
			if (_parent.GetDarkLight() <= 0)
				_is_deleted = 1;
			if (--this->counter <= 0)
			{
				_parent.AddDarkLightSpirit(3);
				this->counter = this->spirit_gen_interval;
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << dec_dark_light << dec_light_skill_cd << spirit_gen_interval << flag;
		ar << counter << anti << mp;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> dec_dark_light >> dec_light_skill_cd >> spirit_gen_interval >> flag;
		ar >> counter >> anti >> mp;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_JUNIORDARKFORM);
		_parent.RemoveFilter(FILTER_JUNIORLIGHTFORM);
		_parent.RemoveFilter(FILTER_SENIORDARKFORM);
		_parent.RemoveFilter(FILTER_SENIORLIGHTFORM);
		_parent.RemoveFilter(FILTER_DARKUNIFORM);
		_parent.SetDarkLightForm(6);
		if (_parent.GetDarkLight() > 0)
		{
			this->counter = this->spirit_gen_interval;
			_parent.GetSkillWrapper().DecDarkSkillCoolTime(dec_light_skill_cd);
			_parent.GetSkillWrapper().DecLightSkillCoolTime(dec_light_skill_cd);
			this->flag = 1;
			_parent.EnhanceMaxMP(this->mp);
			for (int i = 0; i <= 5; ++i)
				_parent.EnhanceResistance(i, this->anti);
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			for (int ia = 0; ia <= 2; ++ia)
				_parent.AddDarkLightSpirit(3);
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		_parent.ClearDarkLightForm();
		if (this->flag)
		{
			_parent.ImpairMaxMP(this->mp);
			for (int i = 0; i <= 5; ++i)
				_parent.ImpairResistance(i, this->anti);
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			_parent.GetSkillWrapper().IncDarkSkillCoolTime(dec_light_skill_cd);
			_parent.GetSkillWrapper().IncLightSkillCoolTime(dec_light_skill_cd);
		}
	}
};

class filter_Bloodshield : public timeout_filter
{
protected:
	float		_ratio;
	int		_addend;
	int		_maxcount;
	int		_count;
	int		_prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Bloodshield() {}
public:
	DECLARE_SUBSTANCE(filter_Bloodshield);

	filter_Bloodshield(
		object_interface object,
		float ratio,
		int addend,
		int max,
		int prob,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_addend = addend;
		this->_maxcount = max;
		this->_count = 0;
		this->_prob = prob;

		_filter_id = FILTER_BLOODSHIELD;
		_buff_id = HSTATE_BLOODSHIELD;
	}

	~filter_Bloodshield() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		if (rand() % 100 <= this->_prob)
		{
			float hp = _parent.GetBasicProp().hp;
			if (this->_ratio > hp / _parent.GetExtendProp().max_hp)
			{
				if (this->_count < this->_maxcount)
				{
					_parent.EnhanceDefense(this->_addend);
					_parent.UpdateDefenseData();
					++this->_count;
				}
				else
				{
					_is_deleted = 1;
				}
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _addend << _maxcount << _count << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _addend >> _maxcount >> _count >> _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
		if (this->_count)
		{
			_parent.ImpairDefense( this->_count * this->_addend);
			_parent.UpdateDefenseData();
		}
	}
};

class filter_Incfatalhurt : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incfatalhurt() {}
public:
	DECLARE_SUBSTANCE(filter_Incfatalhurt);

	filter_Incfatalhurt(
		object_interface object,
		float r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_INCFATALHURT;
		_buff_id = HSTATE_INCFATALHURT;
	}

	~filter_Incfatalhurt() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceAntiCritDamage(*(int*)&_ratio ^ 0x80000000);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiCritDamage(this->_ratio);
		_parent.UpdateAttackData();
	}
};

//190 
class filter_Inccrithurt : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Inccrithurt() {}
public:
	DECLARE_SUBSTANCE(filter_Inccrithurt);

	filter_Inccrithurt(
		object_interface object,
		float r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_INCCRITHURT;
		_buff_id = HSTATE_INCCRITHURT;
	}

	~filter_Inccrithurt() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceAntiCritDamage(*(int*)&_ratio ^ 0x80000000);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiCritDamage(this->_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Dizzy : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Dizzy() {}
public:
	DECLARE_SUBSTANCE(filter_Dizzy);

	filter_Dizzy(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{
		_filter_id = FILTER_DIZZY;
		_buff_id = HSTATE_DIZZY;
	}

	~filter_Dizzy() {}

	void OnAttach()
	{
		_parent.SetSilentSeal(1);
		_parent.SetMeleeSeal(1);
		_parent.SetRootSeal(1);
		_parent.SendClientNotifyRoot(1);
		_parent.IncVisibleState(VSTATE_DIZZY);
	}

	void OnDetach()
	{
		_parent.SetSilentSeal(0);
		_parent.SetMeleeSeal(0);
		_parent.SetRootSeal(0);
		_parent.SendClientDispelRoot(1);
		_parent.DecVisibleState(VSTATE_DIZZY);
	}
};

class filter_Sleep : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED | FILTER_MASK_HEARTBEAT
	};

	filter_Sleep() {}
public:
	DECLARE_SUBSTANCE(filter_Sleep);

	filter_Sleep(object_interface object, int period, int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;

		_filter_id = FILTER_SLEEP;
		_buff_id = HSTATE_SLEEP;
	}

	~filter_Sleep() {}

	void BeAttacked(const XID& target, int dmg, bool crit)
	{
		if (dmg >= this->_value)
			_is_deleted = 1;
	}

	void OnAttach()
	{
		_parent.SendClientNotifyRoot(0);
		_parent.SetSilentSeal(1);
		_parent.SetMeleeSeal(1);
		_parent.SetRootSeal(1);
		_parent.IncVisibleState(VSTATE_SLEEP);
	}

	void OnDetach()
	{
		_parent.SendClientDispelRoot(0);
		_parent.SetSilentSeal(0);
		_parent.SetMeleeSeal(0);
		_parent.SetRootSeal(0);
		_parent.DecVisibleState(VSTATE_SLEEP);
	}
};

class filter_Decattack : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decattack() {}
public:
	DECLARE_SUBSTANCE(filter_Decattack);

	filter_Decattack(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_DECATTACK;
		_buff_id = HSTATE_WEAK;   //未知原因   haokaixin
	}

	~filter_Decattack() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_WEAK);
		_parent.ImpairScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_WEAK);
		_parent.EnhanceScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Wrap : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Wrap() {}
public:
	DECLARE_SUBSTANCE(filter_Wrap);

	filter_Wrap(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_WRAP;
		_buff_id = HSTATE_WRAP;
	}

	~filter_Wrap() {}

	void OnAttach()
	{
		_parent.SetRootSeal(1);
		_parent.SendClientNotifyRoot(2);
		_parent.IncVisibleState(VSTATE_WRAP);
	}

	void OnDetach()
	{
		_parent.SetRootSeal(0);
		_parent.SendClientDispelRoot(2);
		_parent.DecVisibleState(VSTATE_WRAP);
	}
};

//195 
class filter_Slow : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Slow() {}
public:
	DECLARE_SUBSTANCE(filter_Slow);

	filter_Slow(object_interface object, int r, int period, int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_SLOW;
		_buff_id = HSTATE_SLOW; 
	}

	~filter_Slow() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_SLOW);
		_parent.ImpairScaleSpeed(this->_ratio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_SLOW);
		_parent.EnhanceScaleSpeed(this->_ratio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Silent : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Silent() {}
public:
	DECLARE_SUBSTANCE(filter_Silent);

	filter_Silent(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_SILENT;
		_buff_id = HSTATE_SILENT;
	}

	~filter_Silent() {}

	void OnAttach()
	{
		_parent.SetSilentSeal( 1);
		_parent.IncVisibleState(VSTATE_SILENT);
	}

	void OnDetach()
	{
		_parent.SetSilentSeal( 0);
		_parent.DecVisibleState(VSTATE_SILENT);
	}
};

class filter_Dsleep : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED | FILTER_MASK_HEARTBEAT
	};

	filter_Dsleep() {}
public:
	DECLARE_SUBSTANCE(filter_Dsleep);

	filter_Dsleep(object_interface object, int period, int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;

		_filter_id = FILTER_DSLEEP;
		_buff_id = HSTATE_DSLEEP;
	}

	~filter_Dsleep() {}

	void BeAttacked(const XID& target, int dmg, bool crit)
	{
		if (dmg >= this->_value)
			_is_deleted = 1;
	}

	void OnAttach()
	{
		_parent.SendClientNotifyRoot( 0);
		_parent.SetSilentSeal( 1);
		_parent.SetMeleeSeal( 1);
		_parent.SetRootSeal( 1);
		_parent.IncVisibleState(VSTATE_SLEEP);
	}

	void OnDetach()
	{
		_parent.SendClientDispelRoot( 0);
		_parent.SetSilentSeal( 0);
		_parent.SetMeleeSeal( 0);
		_parent.SetRootSeal( 0);
		_parent.DecVisibleState(VSTATE_SLEEP);
	}
};

class filter_Subhide : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subhide() {}
public:
	DECLARE_SUBSTANCE(filter_Subhide);

	filter_Subhide(
		object_interface object,
		int value,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;

		_filter_id = buffid + FILTER_SUBHIDE;
		_buff_id = HSTATE_SUBHIDE;
	}

	~filter_Subhide() {}


	virtual bool Save(archive& ar)
	{
		//timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairInvisible( this->_value);
	}

	void OnDetach()
	{
		_parent.EnhanceInvisible( this->_value);
	}
};

class filter_Incskilldodge : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incskilldodge() {}
public:
	DECLARE_SUBSTANCE(filter_Incskilldodge);

	filter_Incskilldodge(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_INCSKILLDODGE;
		_buff_id = HSTATE_INCSKILLDODGE;
	}

	~filter_Incskilldodge() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceSkillArmor( this->_ratio);
		_parent.UpdateAttackData( );
	}

	void OnDetach()
	{
		_parent.EnhanceSkillArmor( -this->_ratio);
		_parent.UpdateAttackData( );
	}
};

//200  
class filter_Divinityburst : public timeout_filter
{
protected:
	int _dpscale;
	int _dp;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Divinityburst() {}
public:
	DECLARE_SUBSTANCE(filter_Divinityburst);

	filter_Divinityburst(
		object_interface object,
		int ratio,
		int value,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_dpscale = ratio;
		this->_dp = value;

		_filter_id = FILTER_DIVINITYBURST;
		_buff_id = HSTATE_DIVINITYBURST;
	}

	~filter_Divinityburst() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _dpscale << _dp;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _dpscale >> _dp;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDeityPower( this->_dpscale);
		_parent.EnhanceDeityPower( this->_dp);
		_parent.UpdateAttackData();
		_parent.UpdateDP();
	}

	void OnDetach()
	{
		_parent.EnhanceDeityPower( -this->_dp);
		_parent.EnhanceScaleDeityPower( -this->_dpscale);
		_parent.UpdateAttackData();
		_parent.UpdateDP();
	}
};

class filter_Deadlybless : public timeout_filter
{
protected:
	float _ratio;
	int _count;
	int _prob;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Deadlybless() {}
public:
	DECLARE_SUBSTANCE(filter_Deadlybless);

	filter_Deadlybless(
		object_interface object,
		float ratio,
		int max,
		int prob,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;
		this->_count = max;
		this->_prob = prob;

		_filter_id = FILTER_DEADLYBLESS;
		_buff_id = HSTATE_DEADLYBLESS;
	}

	~filter_Deadlybless() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		if (this->_count > 0)
		{
			if (rand() % 100 <= this->_prob)
			{
				int hp = _parent.GetBasicProp().hp;
				if (this->_ratio >= hp / _parent.GetExtendProp().max_hp)
				{
					if (dmg.damage >= hp)
						dmg.ignore_attack = 1;
					--this->_count;
				}
			}
		}
		else
		{
			_is_deleted = 1;
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _count << _prob;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _count >> _prob;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Coldinjure : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Coldinjure() {}
public:
	DECLARE_SUBSTANCE(filter_Coldinjure);

	filter_Coldinjure(
		object_interface object,
		float ratio,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_COLDINJURE;
		_buff_id = HSTATE_COLDINJURE;
	}

	~filter_Coldinjure() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		dmg.damage = (this->_ratio + 1.0) * dmg.damage;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.SetColdInjureState( 1);
		_parent.IncVisibleState(VSTATE_COLDINJURE);
	}

	void OnDetach()
	{
		_parent.SetColdInjureState( 0);
		_parent.DecVisibleState(VSTATE_COLDINJURE);
	}
};

class filter_Evilaura : public timeout_filter
{
protected:
	int _sid;
	int _slevel;
	float _radius;
	int _coverage;
	int _mpcost;
	int _counter;

	char _vstate;
	int _var[16];
	int _dpcost;
	int _hpcost;

	float _ratio;
	int _plus;
	char _force;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Evilaura() {}
public:
	DECLARE_SUBSTANCE(filter_Evilaura);

	filter_Evilaura(
		object_interface object,
		int skillid,
		int level,
		float radius,
		int coverage,
		int mpcost,
		float ratio,
		int plus,
		int period,
		char force,
		int* var,
		int dpcost,
		int hpcost) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sid = skillid;
		this->_slevel = level;
		this->_radius = radius;
		this->_coverage = coverage;
		this->_mpcost = mpcost;
		this->_ratio = ratio;
		this->_plus = plus;
		this->_counter = 5;

		this->_vstate = 1;
		this->_force = force;
		memcpy(this->_var, var, sizeof(this->_var));
		this->_dpcost = dpcost;
		this->_hpcost = hpcost;

		_filter_id = FILTER_EVILAURA;
		_buff_id = HSTATE_EVILAURA;
	}

	~filter_Evilaura() {}

	void Heartbeat(int tick)   //后面来补   haokaixin
	{
		this->_counter += tick;
		if (this->_counter > 4)
		{
			if (this->_mpcost <= 0 || _parent.GetBasicProp().mp >= this->_mpcost)
			{
				if (this->_dpcost <= 0 || _parent.GetBasicProp().dp >= this->_dpcost)
				{
					if (this->_hpcost <= 0 || _parent.GetBasicProp().hp > this->_hpcost)
					{
						_parent.DrainMana(this->_mpcost);
						_parent.DecDeity(this->_dpcost);
						_parent.DecHP(this->_hpcost);
						if (this->_ratio <= 0.001 && this->_plus <= 0)
						{
							enchant_msg msg;
							memset(&msg, 0, sizeof(msg));
							msg.skill = this->_sid;
							msg.skill_level = this->_slevel;
							msg.force_attack = this->_force;
							msg.helpful = 0;
							msg.attack_range = this->_radius;
							msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
							memcpy(&msg.skill_modify[2], _var,sizeof(_var));

							_parent.GetSkillWrapper().SetSkillTalent(_sid, msg.skill_modify, _parent);
							std::vector<exclude_target> empty_0;

							_parent.RegionEnchant1(_parent.GetPos(), _radius, msg, _coverage,empty_0);
						}
						else
						{
							attack_msg msg;
							memset(&msg, 0, sizeof(msg));
							msg.damage_low = _parent.GetExtendProp().damage_low * this->_ratio + _plus;
							msg.damage_high = _parent.GetExtendProp().damage_high * this->_ratio + _plus;
							msg.force_attack = this->_force;
							msg.skill_id = this->_sid;
							msg.attack_rate = _parent.GetExtendProp().attack;
							msg.attached_skill.skill = this->_sid;
							msg.attached_skill.level = this->_slevel;
							memcpy(msg.skill_var, this->_var, sizeof(msg.skill_var));
							XID xid( -1, -1);
							_parent.FillAttackMsg( xid, msg, 0);

							_parent.GetSkillWrapper().SetSkillTalent(_sid, msg.skill_modify, _parent);

							std::vector<exclude_target> empty;

							_parent.RegionAttack1(_parent.GetPos(), _radius, msg, 0, _coverage, empty, 1);

						}
						this->_counter = 0;
						timeout_filter::Heartbeat(tick);
					}
					else
					{
						_is_deleted = 1;
					}
				}
				else
				{
					_is_deleted = 1;
				}
			}
			else
			{
				_is_deleted = 1;
			}
		}
	}

	void SetVisible(char id)
	{
		if (id <= 0 || id > 49)
			_vstate = 1;
		else
			_vstate = id;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sid << _slevel << _radius << _coverage;
		ar << _mpcost << _ratio << _plus << _counter << _vstate << _force;
		ar.pop_back(_var, sizeof(_var));
		ar << _dpcost << _hpcost;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sid >> _slevel >> _radius >> _coverage;
		ar >> _mpcost >> _ratio >> _plus >> _counter >> _vstate >> _force;
		ar.pop_back(_var, sizeof(_var));
		ar >> _dpcost >> _hpcost;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(this->_vstate);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(this->_vstate);
	}
};

class filter_Submp : public timeout_filter
{
protected:
	int _value;
	int _limit;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Submp() {}
public:
	DECLARE_SUBSTANCE(filter_Submp);

	filter_Submp(
		object_interface object,
		int value,
		int amount,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;
		this->_limit = amount;

		_filter_id = buffid + FILTER_SUBMP;
		_buff_id = HSTATE_SUBMP;
	}

	~filter_Submp() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value << _limit;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value >> _limit;
		return true;
	}

	void OnAttach()
	{
		if (_parent.GetExtendProp().max_mp < this->_value + this->_limit)
			this->_value = _parent.GetExtendProp().max_mp - this->_limit;
		if (this->_value <= 0)
		{
			_is_deleted = 1;
		}
		else
		{
			_parent.ImpairMaxMP(this->_value);
			_parent.UpdateHPMP();
		}
	}

	void OnDetach()
	{
		if (this->_value > 0)
		{
			_parent.EnhanceMaxMP(this->_value);
			_parent.UpdateHPMP();
		}
	}
};

//205
class filter_Addrage1 : public filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED
	};

	filter_Addrage1() {}
public:
	DECLARE_SUBSTANCE(filter_Addrage1);

	filter_Addrage1(object_interface object, float ratio) : filter(object, FILTER_MASK)
	{
		this->_ratio = ratio;

		_filter_id = FILTER_ADDRAGE1;
		_buff_id = HSTATE_ADDRAGE1;

	}

	~filter_Addrage1() {}

	void BeAttacked(const XID& target, int dmg, bool crit)
	{
		int value = dmg * this->_ratio;
		if (value > 0)
			_parent.AddRage(value);
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Mpgen : public timeout_filter
{
protected:
	int _health;
	int _addtion;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Mpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Mpgen);

	filter_Mpgen(
		object_interface object,
		int health,
		int addtion,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_health = health / period;
		if (this->_health <= 0)
			this->_health = 1;
		this->_addtion = addtion;

		_filter_id = buffid + FILTER_MPGEN;
		_buff_id = HSTATE_MPGEN;
	}

	~filter_Mpgen() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int health = this->_counter * this->_health;
			if (_timeout <= tick)
				health += this->_addtion;
			_parent.InjectMana(health, 0);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _health << _addtion << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _health >> _addtion >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_MPGEN);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_MPGEN);
	}
};

class filter_Viphupo : public filter
{
protected:
	int _addend;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE 
	};

	filter_Viphupo() {}
public:
	DECLARE_SUBSTANCE(filter_Viphupo);

	filter_Viphupo(
		object_interface object,
		int ratio,
		int r,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->_addend = r;
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_VIPHUPO;
		_buff_id = HSTATE_VIPHUPO;

	}

	~filter_Viphupo() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addend << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addend >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
		_parent.EnhanceDamage(this->_addend);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
		_parent.ImpairDamage(this->_addend);
		_parent.UpdateAttackData();
	}
};

class filter_Befired : public filter
{
protected:
	XID _performer;
	attacker_info_t _pinfo;
	int _damage;

	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Befired() {}
public:
	DECLARE_SUBSTANCE(filter_Befired);

	filter_Befired(object_interface object, int value) : filter(object, FILTER_MASK)
	{
		this->_damage = value;

		_filter_id = FILTER_BEFIRED;
		//_buff_id = HSTATE_BEFIRED;

	}

	~filter_Befired() {}

	void SetUp(const XID& player,const attacker_info_t& info,char mode,bool invader,int attacker_faction,int target_faction)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _performer ;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode << _damage;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _performer ;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode >> _damage;
		return true;
	}

	void OnAttach()
	{
		_parent.BeHurt(_performer,_pinfo,_damage,_invader,_mode);
	}

	void OnDetach()
	{
	}
};

class filter_Fear : public timeout_filter
{
protected:
	XID _attacker;
	float _distance;
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Fear() {}
public:
	DECLARE_SUBSTANCE(filter_Fear);

	filter_Fear(
		object_interface object,
		int period,
		const XID& attacker,
		float distance,
		int ratio) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_attacker.type = attacker.type;
		this->_attacker.id = attacker.id;
		this->_distance = distance;
		this->_ratio = ratio;

		_filter_id = FILTER_FEAR;
		_buff_id = HSTATE_FEAR;
	}

	~filter_Fear() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& msg)
	{
		_is_deleted = 1;
	}

	void Heartbeat(int tick)
	{
		_parent.Flee(this->_attacker, this->_distance);
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _attacker << _distance << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _attacker >> _distance >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_FEAR);
		_parent.EnhanceScaleSpeed(500);
		_parent.UpdateSpeedData();
		_parent.ImpairScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_FEAR);
		_parent.ImpairScaleSpeed(500);
		_parent.UpdateSpeedData();
		_parent.EnhanceScaleDamage(this->_ratio);
		_parent.UpdateAttackData();
	}
};

//210
class filter_Betaunted2 : public timeout_filter
{
protected:
	XID _attacker;
	int _time;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Betaunted2() {}
public:
	DECLARE_SUBSTANCE(filter_Betaunted2);

	filter_Betaunted2(
		object_interface object,
		const XID& attacker,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_attacker.type = attacker.type;
		this->_attacker.id = attacker.id;
		this->_time = period;

		_filter_id = FILTER_BETAUNTED2;
		_buff_id = HSTATE_BETAUNTED2;
	}

	~filter_Betaunted2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _attacker << _time;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _attacker >> _time;
		return true;
	}

	void OnAttach()
	{
		_parent.BeTaunted2(this->_attacker, this->_time);
		_parent.SetSilentSeal(1);
	}

	void OnDetach()
	{
		_parent.SetSilentSeal(0);
	}
};

class filter_Hpleak5 : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpleak5() {}
public:
	DECLARE_SUBSTANCE(filter_Hpleak5);

	filter_Hpleak5(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;

		_timeout = damage / this->_damage;

		_filter_id = buffid + FILTER_HPLEAK5;
		_buff_id = HSTATE_HPLEAK5;
	}

	~filter_Hpleak5() {}

	void SetUp(const XID& player, const attacker_info_t& info, char mode, bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.BeHurt(_performer, _pinfo, damage, _invader, _mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 1)
		{
			this->_damage /= 2;
			this->_addtion /= 2;
		}
		if (this->_damage <= 1)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _damage >> _addtion >> _performer >> _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPLEAK);
	}
};

class filter_Subantisleep2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantisleep2() {}
public:
	DECLARE_SUBSTANCE(filter_Subantisleep2);

	filter_Subantisleep2(
		object_interface object,
		int v,
		int period,
		int overlay_cnt) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = v;
		_max_overlay_cnt = overlay_cnt;
		_can_overlay = 1;

		_filter_id = FILTER_SUBANTISLEEP2;
		_buff_id = HSTATE_SUBANTISLEEP2;
	}

	~filter_Subantisleep2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(4, this->_value);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(4, this->_value);
		_parent.UpdateMagicData();
	}
};

class filter_Ghostform : public filter
{
protected:
	int _addhp;
	int _addmp;
	int _addattack;
	int _adddefence;
	int _addanti;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK
	};

	filter_Ghostform() {}
public:
	DECLARE_SUBSTANCE(filter_Ghostform);

	filter_Ghostform(
		object_interface object,
		int addhp,
		int addmp,
		int addattack,
		int adddefence,
		int addanti) : filter(object, FILTER_MASK)
	{
		this->_addhp = addhp;
		this->_addmp = addmp;
		this->_addattack = addattack;
		this->_adddefence = adddefence;
		this->_addanti = addanti;

		_filter_id = FILTER_GHOSTFORM;
		_buff_id = HSTATE_GHOSTFORM;

	}

	~filter_Ghostform() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _addhp << _addmp << _addattack << _adddefence << _addanti;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _addhp >> _addmp >> _addattack >> _adddefence >> _addanti;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceMaxHP(this->_addhp);
		_parent.EnhanceMaxMP(this->_addmp);
		_parent.EnhanceDamage(this->_addattack);
		_parent.EnhanceDefense(this->_adddefence);
		_parent.EnhanceResistance(0, this->_addanti);
		_parent.EnhanceResistance(1, this->_addanti);
		_parent.EnhanceResistance(2, this->_addanti);
		_parent.EnhanceResistance(3, this->_addanti);
		_parent.EnhanceResistance(4, this->_addanti);
		_parent.EnhanceResistance(5, this->_addanti);
		_parent.UpdateAllProp();
		_parent.ChangeShape(1);
	}

	void OnDetach()
	{
		_parent.ImpairMaxHP(this->_addhp);
		_parent.ImpairMaxMP(this->_addmp);
		_parent.ImpairDamage(this->_addattack);
		_parent.ImpairDefense(this->_adddefence);
		_parent.ImpairResistance(0, this->_addanti);
		_parent.ImpairResistance(1, this->_addanti);
		_parent.ImpairResistance(2, this->_addanti);
		_parent.ImpairResistance(3, this->_addanti);
		_parent.ImpairResistance(4, this->_addanti);
		_parent.ImpairResistance(5, this->_addanti);
		_parent.UpdateAllProp();
		_parent.ChangeShape(0);
	}
};

class filter_Bloodpool : public timeout_filter
{
protected:
	int _capa;
	int _cur_hp;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BEFORE_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Bloodpool() {}
public:
	DECLARE_SUBSTANCE(filter_Bloodpool);

	filter_Bloodpool(
		object_interface object,
		int period,
		int amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_capa = amount;
		this->_cur_hp = amount;

		_filter_id = FILTER_BLOODPOOL;
		_buff_id = HSTATE_BLOODPOOL;
	}

	~filter_Bloodpool() {}

	void BeforeDamage(int& damage)
	{
		if (damage > 0 && this->_cur_hp > 0)
		{
			if (damage >= this->_cur_hp)
			{
				damage -= this->_cur_hp;
				this->_cur_hp = 0;
			}
			else
			{
				this->_cur_hp -= damage;
				damage = 0;
			}
		}
		_parent.NotifyBloodPoolStatus(1, this->_cur_hp, this->_capa);
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 15)
		{
			*(int*)ctrlval = this->_capa;
		}
		else if (ctrlname == 16)
		{
			this->_cur_hp += *(int*)ctrlval;
			if (this->_cur_hp > this->_capa)
				this->_cur_hp = this->_capa;
			_parent.NotifyBloodPoolStatus(1, this->_cur_hp, this->_capa);
		}
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _capa ;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _capa ;
		return true;
	}

	void OnAttach()
	{
		_parent.NotifyBloodPoolStatus( 1, this->_cur_hp, this->_capa);
	}

	void OnDetach()
	{
		_parent.NotifyBloodPoolStatus( 0, this->_cur_hp, this->_capa);
	}
};

//215 
class filter_Miracleburstmo : public timeout_filter
{
protected:
	int _ratio;
	int _dpdecspeed;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Miracleburstmo() {}
public:
	DECLARE_SUBSTANCE(filter_Miracleburstmo);

	filter_Miracleburstmo(
		object_interface object,
		int r,
		int period,
		int buffid,
		int dpcost) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		this->_dpdecspeed = dpcost / period;
		this->_counter = 0;

		_filter_id = buffid + FILTER_MIRACLEBURSTMO;
		_buff_id = HSTATE_MIRACLEBURSTMO;
	}

	~filter_Miracleburstmo() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			if (! _parent.DecDeity(_counter * this->_dpdecspeed))
				_is_deleted = 1;
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio << _dpdecspeed << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio >> _dpdecspeed >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.SetFuryState(1);
		_parent.EnhanceScaleDeityPower(this->_ratio);
		_parent.EnhanceCultAttack(0, this->_ratio);
		_parent.EnhanceScaleDamage(this->_ratio);
		_parent.EnhanceScaleDmgReduce(this->_ratio);
		_parent.EnhanceScaleIgnDmgReduce(this->_ratio);
		_parent.UpdateAttackData();
		_parent.IncVisibleState(VSTATE_MIRACLEBURSTMO);
	}

	void OnDetach()
	{
		_parent.SetFuryState(0);
		_parent.EnhanceScaleDeityPower(-this->_ratio);
		_parent.EnhanceCultAttack(0, -this->_ratio);
		_parent.ImpairScaleDamage(this->_ratio);
		_parent.ImpairScaleDmgReduce(this->_ratio);
		_parent.ImpairScaleIgnDmgReduce(this->_ratio);
		_parent.UpdateAttackData();
		_parent.DecVisibleState(VSTATE_MIRACLEBURSTMO);
	}
};

class filter_Seniorlightform : public filter
{
protected:
	int dec_dark_light;
	int dec_light_skill_cd;
	int spirit_gen_interval;
	bool flag;
	int counter;
	int anti;
	int mp;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Seniorlightform() {}
public:
	DECLARE_SUBSTANCE(filter_Seniorlightform);

	filter_Seniorlightform(
		object_interface object,
		float ratio,
		int value,
		int amount,
		int time,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->dec_dark_light = value;
		this->dec_light_skill_cd = (int)ratio;
		this->spirit_gen_interval = amount / 1000;
		this->flag = this->flag;
		this->counter = 0;
		this->anti = buffid;
		this->mp = time;

		_filter_id = FILTER_SENIORLIGHTFORM;
		_buff_id = HSTATE_SENIORLIGHT;

	}

	~filter_Seniorlightform() {}

	void Heartbeat(int tick)
	{
		if (!_is_deleted)
		{
			_parent.DecDarkLight(this->dec_dark_light);
			if (_parent.GetDarkLight() <= 0)
				_is_deleted = 1;
			if (--this->counter <= 0)
			{
				_parent.AddDarkLightSpirit(2);
				this->counter = this->spirit_gen_interval;
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << dec_dark_light << dec_light_skill_cd << spirit_gen_interval << flag;
		ar << counter << anti << mp;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> dec_dark_light >> dec_light_skill_cd >> spirit_gen_interval >> flag;
		ar >> counter >> anti >> mp;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_JUNIORDARKFORM);
		_parent.RemoveFilter(FILTER_JUNIORLIGHTFORM);
		_parent.RemoveFilter(FILTER_SENIORDARKFORM);
		_parent.RemoveFilter(FILTER_DARKUNIFORM);
		_parent.RemoveFilter(FILTER_LIGHTUNIFORM);
		_parent.SetDarkLightForm(4);
		if (_parent.GetDarkLight() > 0)
		{
			this->counter = this->spirit_gen_interval;
			_parent.GetSkillWrapper().DecLightSkillCoolTime(dec_light_skill_cd);
			this->flag = 1;
			_parent.EnhanceMaxMP(this->mp);
			for (int i = 0; i <= 5; ++i)
				_parent.EnhanceResistance(i, this->anti);
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			for (int ia = 0; ia <= 2; ++ia)
				_parent.AddDarkLightSpirit(3);
		}
		else
		{
			_is_deleted = 1;
		}
	}

	void OnDetach()
	{
		_parent.ClearDarkLightForm();
		if (this->flag)
		{
			_parent.ImpairMaxMP(this->mp);
			for (int i = 0; i <= 5; ++i)
				_parent.ImpairResistance(i, this->anti);
			_parent.UpdateHPMP();
			_parent.UpdateMagicData();
			_parent.GetSkillWrapper().IncLightSkillCoolTime(dec_light_skill_cd);
		}
	}
};

class filter_Hpleak : public timeout_filter
{
protected:
	int _damage;
	int _addtion;
	XID _performer;
	attacker_info_t _pinfo;
	char _counter;
	bool _invader;
	char _mode;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Hpleak() {}
public:
	DECLARE_SUBSTANCE(filter_Hpleak);

	filter_Hpleak(
		object_interface object,
		int period,
		int damage,
		int addtion,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_counter = 0;
		this->_damage = damage / period;
		if (this->_damage <= 0)
			this->_damage = 1;
		this->_addtion = addtion;

		_timeout = damage / this->_damage;

		_filter_id = buffid + FILTER_HPLEAK;
		_buff_id = HSTATE_HPLEAK;
	}

	~filter_Hpleak() {}

	void SetUp(const XID& player, const attacker_info_t& info, char mode, bool invader)
	{
		this->_performer.type = player.type;
		this->_performer.id = player.id;
		memcpy(&this->_pinfo, &info, sizeof(this->_pinfo));
		this->_mode = mode;
		this->_invader = invader;
	}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1 || tick >= _timeout)
		{
			int damage = this->_counter * this->_damage;
			if (_timeout <= tick)
				damage += this->_addtion;
			_parent.BeHurt(_performer, _pinfo, damage, _invader, _mode);
			this->_counter -= 2;
		}
		timeout_filter::Heartbeat(tick);
	}

	void OnModify(int ctrlname, void* ctrlval, size_t ctrllen)
	{
		if (ctrlname == 1)
		{
			this->_damage /= 2;
			this->_addtion /= 2;
		}
		if (this->_damage <= 1)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _damage << _addtion << _performer << _counter;
		ar.push_back(&_pinfo, sizeof(_pinfo));
		ar << _invader << _mode;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _damage >> _addtion >> _performer >> _counter;
		ar.pop_back(&_pinfo, sizeof(_pinfo));
		ar >> _invader >> _mode;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPLEAK);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPLEAK);
	}
};

class filter_Decskilldamage : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_SEND_MSG
	};

	filter_Decskilldamage() {}
public:
	DECLARE_SUBSTANCE(filter_Decskilldamage);

	filter_Decskilldamage(
		object_interface object,
		float ratio,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = ratio;

		_filter_id = buffid + FILTER_DECSKILLDAMAGE;
		_buff_id = HSTATE_DECSKILLDAMAGE;
	}

	~filter_Decskilldamage() {}

	void AdjustDamage(damage_entry& dmg, const XID& attacker, const attack_msg& msg)
	{
		dmg.damage = (this->_ratio + 1.0) * dmg.damage;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.SetColdInjureState(1);
		_parent.IncVisibleState(VSTATE_COLDINJURE);
	}

	void OnDetach()
	{
		_parent.SetColdInjureState(0);
		_parent.DecVisibleState(VSTATE_COLDINJURE);
	}
};

class filter_Gthpgen : public filter
{
protected:
	int _health;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT
	};

	filter_Gthpgen() {}
public:
	DECLARE_SUBSTANCE(filter_Gthpgen);

	filter_Gthpgen(
		object_interface object,
		int period,
		int health,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->_counter = 0;
		this->_health = health / period;
		if (this->_health <= 0)
			this->_health = 1;

		_filter_id = buffid + FILTER_GTHPGEN;
		_buff_id = HSTATE_GTHPGEN;

	}

	~filter_Gthpgen() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 1)
		{
			_parent.Heal(this->_counter * this->_health, 0, 0);
			this->_counter -= 2;
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _health << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _health >> _counter;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_HPGEN);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_HPGEN);
	}
};

//220
class filter_Decmp : public filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Decmp() {}
public:
	DECLARE_SUBSTANCE(filter_Decmp);

	filter_Decmp(object_interface object, int r, int period) : filter(object, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = FILTER_DECMP;
		//_buff_id = HSTATE_DECMP;

	}

	~filter_Decmp() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairScaleMaxMP(this->_ratio);
	}

	void OnDetach()
	{
		_parent.EnhanceScaleMaxMP(this->_ratio);
	}
};

class filter_Powerup : public timeout_filter
{
protected:
	int _incdef;
	int _decatt;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Powerup() {}
public:
	DECLARE_SUBSTANCE(filter_Powerup);

	filter_Powerup(
		object_interface object,
		int inc,
		int dec,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_incdef = inc;
		this->_decatt = dec;

		_filter_id = FILTER_POWERUP;
		_buff_id = HSTATE_POWERUP;
	}

	~filter_Powerup() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _incdef << _decatt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _incdef >> _decatt;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDefense(this->_incdef);
		_parent.ImpairScaleDamage(this->_decatt);
		_parent.UpdateDefenseData();
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDefense(this->_incdef);
		_parent.EnhanceScaleDamage(this->_decatt);
		_parent.UpdateDefenseData();
		_parent.UpdateAttackData();
	}
};

class filter_Shouyi : public timeout_filter
{
protected:
	int _adddef;
	int _subatt;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Shouyi() {}
public:
	DECLARE_SUBSTANCE(filter_Shouyi);

	filter_Shouyi(
		object_interface object,
		int inc,
		int dec,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_adddef = inc;
		this->_subatt = dec;

		_filter_id = FILTER_SHOUYI;
		_buff_id = HSTATE_SHOUYI;
	}

	~filter_Shouyi() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _adddef << _subatt;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _adddef >> _subatt;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDefense( this->_adddef);
		_parent.ImpairDamage( this->_subatt);
		_parent.UpdateDefenseData();
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairDefense( this->_adddef);
		_parent.EnhanceDamage( this->_subatt);
		_parent.UpdateDefenseData();
		_parent.UpdateAttackData();
	}
};

class filter_Incheal : public timeout_filter
{
protected:
	float _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT 
	};

	filter_Incheal() {}
public:
	DECLARE_SUBSTANCE(filter_Incheal);

	filter_Incheal(
		object_interface object,
		float r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_INCHEAL;
		//_buff_id = HSTATE_INCHEAL;
	}

	~filter_Incheal() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Creditboost : public timeout_filter
{
protected:
	int _index;
	int _credit;
	char _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Creditboost() {}
public:
	DECLARE_SUBSTANCE(filter_Creditboost);

	filter_Creditboost(
		object_interface object,
		int index,
		int credit,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_credit = credit / period;
		if (this->_credit <= 0)
			this->_credit = 1;
		this->_index = index;

		_filter_id = FILTER_CREDITBOOST;
		_buff_id = HSTATE_CREDITBOOST;
	}

	~filter_Creditboost() {}

	void Heartbeat(int tick)
	{
		this->_counter += tick;
		if (this->_counter > 14 || tick >= _timeout)
		{
			_parent.ModifyRegionReputation(_index,_counter * _credit);
			this->_counter -= 15;
		}
		timeout_filter::Heartbeat(tick);
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _index << _credit << _counter;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _index >> _credit >> _counter;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

//225
class filter_Hurtscatter : public filter
{
protected:
	float _radius;
	int _probability;
	float _ratio;
	int _plus;
	int _total;
	char _force;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_ADJUST_DAMAGE | FILTER_MASK_HEARTBEAT
	};

	filter_Hurtscatter() {}
public:
	DECLARE_SUBSTANCE(filter_Hurtscatter);

	filter_Hurtscatter(
		object_interface object,
		float radius,
		int probability,
		float ratio,
		int plus,
		int total,
		char force,
		int period) : filter(object, FILTER_MASK)
	{
		this->_radius = radius;
		this->_probability = probability;
		this->_ratio = ratio;
		this->_plus = plus;
		this->_total = total;
		this->_force = force;

		_filter_id = FILTER_HURTSCATTER;
		_buff_id = HSTATE_HURTSCATTER;

	}

	~filter_Hurtscatter() {}

	void AdjustDamage(damage_entry& dmg,const XID& attacker,const attack_msg& a4)
	{
		if (dmg.damage >= 0.0 && rand() % 100 <= this->_probability)
		{
			attack_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.damage_low = _parent.GetExtendProp().damage_low * this->_ratio + _plus;
			msg.damage_high = _parent.GetExtendProp().damage_high * this->_ratio + _plus;
			msg.force_attack = this->_force;
			msg.attack_rate = _parent.GetExtendProp().attack;
			XID xid(-1, -1);
			_parent.FillAttackMsg(xid, msg, 0);
			std::vector<exclude_target> empty;
			_parent.RegionAttack1(_parent.GetPos() , _radius, msg, 0, _total, empty, 1);
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _radius << _probability << _ratio << _plus << _total << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _radius >> _probability >> _ratio >> _plus >> _total >> _force;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Incattack2 : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incattack2() {}
public:
	DECLARE_SUBSTANCE(filter_Incattack2);

	filter_Incattack2(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;
		_filter_id = buffid + FILTER_INCATTACK2;
		_buff_id = HSTATE_INCATTACK2;
	}

	~filter_Incattack2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDamage(_ratio);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDamage(_ratio);
		_parent.UpdateAttackData();
	}
};

class filter_Slow2 : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Slow2() {}
public:
	DECLARE_SUBSTANCE(filter_Slow2);

	filter_Slow2(object_interface object, int r, int period, int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_SLOW2;
		_buff_id = HSTATE_SLOW2;
	}

	~filter_Slow2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_SLOW);
		_parent.ImpairScaleSpeed(this->_ratio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_SLOW);
		_parent.EnhanceScaleSpeed(this->_ratio);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}
};

class filter_Incdefence2 : public timeout_filter
{
protected:
	int _ratio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Incdefence2() {}
public:
	DECLARE_SUBSTANCE(filter_Incdefence2);

	filter_Incdefence2(
		object_interface object,
		int r,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_ratio = r;

		_filter_id = buffid + FILTER_INCDEFENCE2;
		_buff_id = HSTATE_INCDEFENCE2;
	}

	~filter_Incdefence2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _ratio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _ratio;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceScaleDefense(_ratio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairScaleDefense(_ratio);
		_parent.UpdateDefenseData();
	}
};

class filter_Immunetransform : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Immunetransform() {}
public:
	DECLARE_SUBSTANCE(filter_Immunetransform);

	filter_Immunetransform(
		object_interface object,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_IMMUNETRANSFORM;
		_buff_id = HSTATE_IMMUNETRANSFORM;
	}

	~filter_Immunetransform() {}

	void OnAttach()
	{
		_parent.SetImmuneMask(IMMUNETRANSFORM);
	}

	void OnDetach()
	{
		_parent.ClearImmuneMask(IMMUNETRANSFORM);
	}
};

//230
class filter_Addantitransform : public timeout_filter
{
protected:
	int _addend;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addantitransform() {}
public:
	DECLARE_SUBSTANCE(filter_Addantitransform);

	filter_Addantitransform(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_addend = v;

		_filter_id = buffid + FILTER_ADDANTITRANSFORM;
		_buff_id = HSTATE_ADDANTITRANSFORM;
	}

	~filter_Addantitransform() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _addend;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _addend;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceAntiTransform( this->_addend);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.ImpairAntiTransform(this->_addend);
		_parent.UpdateMagicData();
	}
};

class filter_Subantitransform : public timeout_filter
{
protected:
	int _sub;

	enum
	{
		FILTER_MASK = FILTER_MASK_WEAK | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantitransform() {}
public:
	DECLARE_SUBSTANCE(filter_Subantitransform);

	filter_Subantitransform(
		object_interface object,
		int v,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_sub = v;

		_filter_id = buffid + FILTER_SUBANTITRANSFORM;
		_buff_id = HSTATE_SUBANTITRANSFORM;
	}

	~filter_Subantitransform() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _sub;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _sub;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairAntiTransform(this->_sub);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceAntiTransform(this->_sub);
		_parent.UpdateMagicData();
	}
};

class filter_Drawclonelife : public timeout_filter
{
protected:
	int drawhp;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Drawclonelife() {}
public:
	DECLARE_SUBSTANCE(filter_Drawclonelife);

	filter_Drawclonelife(
		object_interface object,
		int value,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->drawhp = value;

		_filter_id = FILTER_DRAWCLONELIFE;
		_buff_id = HSTATE_DRAWCLONELIFE;
	}

	~filter_Drawclonelife() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << drawhp;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> drawhp;
		return true;
	}

	void OnAttach()
	{
		_parent.DecHP(_parent.GetCloneID(), this->drawhp);   //猜的  haokaixin
		_parent.EnhanceMaxHP( this->drawhp);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ImpairMaxHP(this->drawhp);
		_parent.UpdateHPMP();
	}
};

class filter_Backstab : public timeout_filter
{
protected:
	int inc_crit;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_TRANSLATE_ENCHANT | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_RECV_MSG
	};

	filter_Backstab() {}
public:
	DECLARE_SUBSTANCE(filter_Backstab);

	filter_Backstab(
		object_interface object,
		int value,
		int period) : timeout_filter(object, period, FILTER_MASK)
	{
		this->inc_crit = value;

		_filter_id = FILTER_BACKSTAB;
		_buff_id = HSTATE_BACKSTAB;
	}

	~filter_Backstab() {}

	void TranslateRecvAttack(const XID& attacker, attack_msg& msg)
	{
		_is_deleted = 1;
	}

	void TranslateRecvEnchant(const XID& attacker, enchant_msg& msg)
	{
		_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << inc_crit;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> inc_crit;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceCrit(this->inc_crit);
	}

	void OnDetach()
	{
		_parent.ImpairCrit(this->inc_crit);
	}
};

class filter_Critcurse : public timeout_filter
{
protected:
	int max_effect_cnt;
	int _counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_RECV_MSG
	};

	filter_Critcurse() {}
public:
	DECLARE_SUBSTANCE(filter_Critcurse);

	filter_Critcurse(
		object_interface object,
		int period,
		int amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->max_effect_cnt = amount;
		this->_counter = amount;

		_filter_id = FILTER_CRITCURSE;
		_buff_id = HSTATE_CRITCURSE;
	}

	~filter_Critcurse() {}

	void TranslateRecvAttack(const XID& attacker,attack_msg& msg)
	{
		--this->_counter;
		msg.crit_rate = 10000;
		if (this->_counter <= 0)
			_is_deleted = 1;
	}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << max_effect_cnt /*<< _counter*/;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> max_effect_cnt /*>> _counter*/;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

//235 
class filter_Subspot : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subspot() {}
public:
	DECLARE_SUBSTANCE(filter_Subspot);

	filter_Subspot(
		object_interface object,
		int value,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;

		_filter_id = buffid + FILTER_SUBSPOT;
		_buff_id = HSTATE_SUBSPOT;
	}

	~filter_Subspot() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairAntiInvisible(this->_value);
	}

	void OnDetach()
	{
		_parent.EnhanceAntiInvisible(this->_value);
	}
};

class filter_Addhide : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Addhide() {}
public:
	DECLARE_SUBSTANCE(filter_Addhide);

	filter_Addhide(
		object_interface object,
		int value,
		int period,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;

		_filter_id = buffid + FILTER_ADDHIDE;
		_buff_id = HSTATE_ADDHIDE;
	}

	~filter_Addhide() {}

	virtual bool Save(archive& ar)
	{
		//timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		//timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceInvisible(this->_value);
	}

	void OnDetach()
	{
		_parent.ImpairInvisible(this->_value);
	}
};

class filter_Crippled : public timeout_filter
{
protected:
	int dec_skill_lvl;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Crippled() {}
public:
	DECLARE_SUBSTANCE(filter_Crippled);

	filter_Crippled(
		object_interface object,
		int period,
		int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->dec_skill_lvl = value;

		_filter_id = FILTER_CRIPPLED;
		_buff_id = HSTATE_CRIPPLED;
	}

	~filter_Crippled() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << dec_skill_lvl;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> dec_skill_lvl;
		return true;
	}

	void OnAttach()
	{
		_parent.GetSkillWrapper().SetDecSkillLevel(dec_skill_lvl);
	}

	void OnDetach()
	{
		_parent.GetSkillWrapper().SetDecSkillLevel(0);
	}
};

class filter_Guishen1debuff : public filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int hpRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Guishen1debuff() {}
public:
	DECLARE_SUBSTANCE(filter_Guishen1debuff);

	filter_Guishen1debuff(
		object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : filter(object, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->hpRatio = buffid;

		_filter_id = FILTER_GUISHEN1DEBUFF;
		_buff_id = HSTATE_GUISHEN1;

	}

	~filter_Guishen1debuff() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << hpRatio ;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> hpRatio;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_GUISHEN1);
		_parent.RemoveFilter(FILTER_GUISHEN2);
		_parent.RemoveFilter(FILTER_TUOLING1);
		_parent.RemoveFilter(FILTER_TUOLING2);
		_parent.RemoveFilter(FILTER_GUISHEN2DEBUFF);
		_parent.RemoveFilter(FILTER_TUOLING1DEBUFF);
		_parent.RemoveFilter(FILTER_TUOLING2DEBUFF);
		_parent.ScalePlayer(0, this->scaleRatio);
		_parent.EnhanceScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().IncPrayTime( praySpeedRatio);
		_parent.EnhanceScaleMaxHP(this->hpRatio);
		_parent.UpdateHPMP();
	}

	void OnDetach()
	{
		_parent.ScalePlayer(1, this->scaleRatio);
		_parent.ImpairScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.ImpairScaleMaxHP(this->hpRatio);
		_parent.UpdateHPMP();
	}
};

class filter_Tuoling1debuff : public timeout_filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int defenseRatio;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Tuoling1debuff() {}
public:
	DECLARE_SUBSTANCE(filter_Tuoling1debuff);

	filter_Tuoling1debuff(
		object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->defenseRatio = buffid;

		_filter_id = FILTER_TUOLING1DEBUFF;
		_buff_id = HSTATE_TUOLING1;

	}

	~filter_Tuoling1debuff() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << defenseRatio;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> defenseRatio;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_GUISHEN1);
		_parent.RemoveFilter(FILTER_GUISHEN2);
		_parent.RemoveFilter(FILTER_TUOLING1);
		_parent.RemoveFilter(FILTER_TUOLING2);
		_parent.RemoveFilter(FILTER_GUISHEN1DEBUFF);
		_parent.RemoveFilter(FILTER_GUISHEN2DEBUFF);
		_parent.RemoveFilter(FILTER_TUOLING2DEBUFF);
		_parent.ScalePlayer(0, this->scaleRatio);
		_parent.ImpairScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.EnhanceScaleDefense(this->defenseRatio);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ScalePlayer(1, this->scaleRatio);
		_parent.EnhanceScaleDamage(this->atkRatio);
		_parent.UpdateAttackData();
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.ImpairScaleDefense(this->defenseRatio);
		_parent.UpdateDefenseData();
	}
};

//240
class filter_Tuoling2debuff : public timeout_filter
{
protected:
	int scaleRatio;
	int atkRatio;
	int praySpeedRatio;
	int antiCrit;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Tuoling2debuff() {}
public:
	DECLARE_SUBSTANCE(filter_Tuoling2debuff);

	filter_Tuoling2debuff(
		object_interface object,
		int period,
		int ratio,
		int amount,
		int value,
		int buffid) : timeout_filter(object, period, FILTER_MASK)
	{
		this->scaleRatio = ratio;
		this->atkRatio = amount;
		this->praySpeedRatio = value;
		this->antiCrit = buffid;

		_filter_id = FILTER_TUOLING2DEBUFF;
		_buff_id = HSTATE_TUOLING2;

	}

	~filter_Tuoling2debuff() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << scaleRatio << atkRatio << praySpeedRatio << antiCrit;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> scaleRatio >> atkRatio >> praySpeedRatio >> antiCrit;
		return true;
	}

	void OnAttach()
	{
		_parent.RemoveFilter(FILTER_GUISHEN1);
		_parent.RemoveFilter(FILTER_GUISHEN2);
		_parent.RemoveFilter(FILTER_TUOLING1);
		_parent.RemoveFilter(FILTER_TUOLING2);
		_parent.RemoveFilter(FILTER_GUISHEN1DEBUFF);
		_parent.RemoveFilter(FILTER_GUISHEN2DEBUFF);
		_parent.RemoveFilter(FILTER_TUOLING1DEBUFF);
		_parent.ScalePlayer(0, this->scaleRatio);
		_parent.ImpairScaleDamage(this->atkRatio);
		_parent.GetSkillWrapper().DecPrayTime(praySpeedRatio);
		_parent.EnhanceAntiCritRate(this->antiCrit);
		_parent.UpdateAttackData();
	}

	void OnDetach()
	{
		_parent.ScalePlayer(1, this->scaleRatio);
		_parent.EnhanceScaleDamage(this->atkRatio);
		_parent.GetSkillWrapper().IncPrayTime(praySpeedRatio);
		_parent.EnhanceAntiCritRate(-this->antiCrit);
		_parent.UpdateAttackData();
	}
};

class filter_Pzmaxspeed : public filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Pzmaxspeed() {}
public:
	DECLARE_SUBSTANCE(filter_Pzmaxspeed);

	filter_Pzmaxspeed(object_interface object) : filter(object, FILTER_MASK)
	{

		_filter_id = FILTER_PZMAXSPEED;
		//_buff_id = HSTATE_PZMAXSPEED;

	}

	~filter_Pzmaxspeed() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.pz_maxspeed();
	}

	void OnDetach()
	{
	}
};

class filter_Pzhalfspeed : public filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Pzhalfspeed() {}
public:
	DECLARE_SUBSTANCE(filter_Pzhalfspeed);

	filter_Pzhalfspeed(object_interface object) : filter(object, FILTER_MASK)
	{

		_filter_id = FILTER_PZHALFSPEED;
		//_buff_id = HSTATE_PZHALFSPEED;

	}

	~filter_Pzhalfspeed() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.pz_halfspeed();
	}

	void OnDetach()
	{
	}
};

class filter_Pzhill : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Pzhill() {}
public:
	DECLARE_SUBSTANCE(filter_Pzhill);

	filter_Pzhill(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_PZHILL;
		_buff_id = HSTATE_PZHILL;
	}

	~filter_Pzhill() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.pz_ashill(1);
	}

	void OnDetach()
	{
		_parent.pz_ashill(0);
	}
};

class filter_Pznomove : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Pznomove() {}
public:
	DECLARE_SUBSTANCE(filter_Pznomove);

	filter_Pznomove(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_PZNOMOVE;
		_buff_id = HSTATE_PZNOMOVE;
	}

	~filter_Pznomove() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.pz_nomove(1);
	}

	void OnDetach()
	{
		_parent.pz_nomove(0);
	}
};

//245
class filter_Pzchaos : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Pzchaos() {}
public:
	DECLARE_SUBSTANCE(filter_Pzchaos);

	filter_Pzchaos(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_PZCHAOS;
		_buff_id = HSTATE_PZCHAOS;
	}

	~filter_Pzchaos() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.pz_chaos(1);
	}

	void OnDetach()
	{
		_parent.pz_chaos(0);
	}
};

class filter_Pzvoid : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Pzvoid() {}
public:
	DECLARE_SUBSTANCE(filter_Pzvoid);

	filter_Pzvoid(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_PZVOID;
		_buff_id = HSTATE_PZVOID;
	}

	~filter_Pzvoid() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.pz_void(1);
	}

	void OnDetach()
	{
		_parent.pz_void(0);
	}
};

class filter_Activebeattacked : public timeout_filter
{
protected:
	int _prob;
	int skill_id;
	int skill_level;
	bool self;
	char _force;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_BE_ATTACKED | FILTER_MASK_HEARTBEAT
	};

	filter_Activebeattacked() {}
public:
	DECLARE_SUBSTANCE(filter_Activebeattacked);

	filter_Activebeattacked(
		object_interface object,
		int prob,
		int period,
		int value,
		int ratio,
		int buffid,
		char force) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_prob = prob;
		this->skill_id = value;
		this->skill_level = ratio;
		this->_force = force;
		this->self = buffid == 0;

		_filter_id = FILTER_ACTIVEBEATTACKED;
		//_buff_id = HSTATE_ACTIVEBEATTACKED;

	}

	~filter_Activebeattacked() {}

	void BeAttacked(const XID& target,int dmg,bool crit)
	{
		if (rand() % 100 < this->_prob)
		{
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = this->skill_id;
			msg.skill_level = this->skill_level;
			msg.force_attack = this->_force;
			msg.helpful = this->self;
			msg.attack_range = _parent.GetExtendProp().attack_range;
			msg.skill_reserved1 = _parent.GetExtendProp().damage_high;
			if (this->self)
			{
				_parent.Enchant(_parent.GetSelfID(), msg);
			}
			else
			{
				_parent.Enchant(target, msg);
			}
		}
	}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		ar << _prob << skill_id << skill_level << self << _force;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		ar >> _prob >> skill_id >> skill_level >> self >> _force;
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Darklaser : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Darklaser() {}
public:
	DECLARE_SUBSTANCE(filter_Darklaser);

	filter_Darklaser(
		object_interface object,
		int period,
		int value) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;
		_max_overlay_cnt = 5;
		_can_overlay = 1;

		_filter_id = FILTER_DARKLASER;
		_buff_id = HSTATE_DECHURT;
	}

	~filter_Darklaser() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.EnhanceDefense(this->_value);
		_parent.UpdateDefenseData();
	}

	void OnDetach()
	{
		_parent.ImpairDefense(this->_value);
		_parent.UpdateDefenseData();
	}
};

class filter_Dirstate : public timeout_filter
{
protected:
	int state_id;
	int dir;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT 
	};

	filter_Dirstate() {}
public:
	DECLARE_SUBSTANCE(filter_Dirstate);

	filter_Dirstate(
		object_interface object,
		int period,
		int value,
		int amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->state_id = amount;
		this->dir = value;

		_filter_id = FILTER_DIRSTATE;
		//_buff_id = HSTATE_DIRSTATE;
	}

	~filter_Dirstate() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << state_id << dir;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> state_id >> dir;
		return true;
	}

	void OnAttach()
	{
		_parent.IncDirVisibleState(this->state_id, this->dir);
	}

	void OnDetach()
	{
		_parent.DecDirVisibleState(this->state_id, this->dir);
	}
};

//250 
class filter_Showyb : public timeout_filter
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Showyb() {}
public:
	DECLARE_SUBSTANCE(filter_Showyb);

	filter_Showyb(object_interface object, int period) : timeout_filter(object, period, FILTER_MASK)
	{

		_filter_id = FILTER_SHOWYB;
		//_buff_id = HSTATE_SHOWYB;
	}

	~filter_Showyb() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
		_parent.IncVisibleState(VSTATE_SHOWYB);
	}

	void OnDetach()
	{
		_parent.DecVisibleState(VSTATE_SHOWYB);
	}
};

class filter_Subantiweak2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantiweak2() {}
public:
	DECLARE_SUBSTANCE(filter_Subantiweak2);

	filter_Subantiweak2(
		object_interface object,
		int v,
		int period,
		int overlay_cnt) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = v;
		_max_overlay_cnt = overlay_cnt;
		_can_overlay = 1;

		_filter_id = FILTER_SUBANTIWEAK2;
		_buff_id = HSTATE_SUBANTIWEAK2;
	}

	~filter_Subantiweak2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(1, this->_value);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(1, this->_value);
		_parent.UpdateMagicData();
	}
};

class filter_Subantislow2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Subantislow2() {}
public:
	DECLARE_SUBSTANCE(filter_Subantislow2);

	filter_Subantislow2(
		object_interface object,
		int v,
		int period,
		int overlay_cnt) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = v;
		_max_overlay_cnt = overlay_cnt;
		_can_overlay = 1;

		_filter_id = FILTER_SUBANTISLOW2;
		_buff_id = HSTATE_SUBANTISLOW2;
	}

	~filter_Subantislow2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		_parent.ImpairResistance(5, this->_value);
		_parent.UpdateMagicData();
	}

	void OnDetach()
	{
		_parent.EnhanceResistance(5, this->_value);
		_parent.UpdateMagicData();
	}
};

class filter_Submp2 : public timeout_filter
{
protected:
	int _value;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DEBUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_Submp2() {}
public:
	DECLARE_SUBSTANCE(filter_Submp2);

	filter_Submp2(
		object_interface object,
		int value,
		int period,
		int amount) : timeout_filter(object, period, FILTER_MASK)
	{
		this->_value = value;
		_max_overlay_cnt = amount;
		_can_overlay = 1;

		_filter_id = FILTER_SUBMP2;
		_buff_id = HSTATE_SUBMP2;
	}

	~filter_Submp2() {}

	virtual bool Save(archive& ar)
	{
		timeout_filter::Save(ar);
		ar << _value;
		return true;
	}

	virtual bool Load(archive& ar)
	{
		timeout_filter::Load(ar);
		ar >> _value;
		return true;
	}

	void OnAttach()
	{
		if (_parent.GetExtendProp().max_mp < this->_value)
			this->_value = _parent.GetExtendProp().max_mp;
		if (this->_value <= 0)
		{
			_is_deleted = 1;
		}
		else
		{
			_parent.ImpairMaxMP(this->_value);
			_parent.UpdateHPMP();
		}
	}

	void OnDetach()
	{
		if (this->_value > 0)
		{
			_parent.EnhanceMaxMP(this->_value);
			_parent.UpdateHPMP();
		}
	}
};

class filter_Renma1 : public filter   //这个filter有鸡毛作用?   haokaixin
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Renma1() {}
public:
	DECLARE_SUBSTANCE(filter_Renma1);

	filter_Renma1(object_interface object) : filter(object, FILTER_MASK)
	{

		_filter_id = FILTER_RENMA1;
		//_buff_id = HSTATE_RENMA1;

	}

	~filter_Renma1() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

//255
class filter_Renma2 : public filter   //这个filter又有鸡毛作用?   haokaixin
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Renma2() {}
public:
	DECLARE_SUBSTANCE(filter_Renma2);

	filter_Renma2(object_interface object) : filter(object, FILTER_MASK)
	{

		_filter_id = FILTER_RENMA2;
		//_buff_id = HSTATE_RENMA2;

	}

	~filter_Renma2() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Renma3 : public filter   //这个filter又又有鸡毛作用?   haokaixin
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Renma3() {}
public:
	DECLARE_SUBSTANCE(filter_Renma3);

	filter_Renma3(object_interface object) : filter(object, FILTER_MASK)
	{

		_filter_id = FILTER_RENMA3;
		//_buff_id = HSTATE_RENMA3;

	}

	~filter_Renma3() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

class filter_Renma4 : public filter   //这个filter又又有鸡毛作用?   haokaixin
{
protected:

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH
	};

	filter_Renma4() {}
public:
	DECLARE_SUBSTANCE(filter_Renma4);

	filter_Renma4(object_interface object) : filter(object, FILTER_MASK)
	{

		_filter_id = FILTER_RENMA4;
		//_buff_id = HSTATE_RENMA4;

	}

	~filter_Renma4() {}

	virtual bool Save(archive& ar)
	{
		filter::Save(ar);
		return true;
	}

	virtual bool Load(archive& ar)
	{
		filter::Load(ar);
		return true;
	}

	void OnAttach()
	{
	}

	void OnDetach()
	{
	}
};

/////////////////////skillWrapper

class filter_guiwang_addanticrit : public timeout_filter
{
protected:
	int _heartbeat_counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_guiwang_addanticrit() {}
public:
	DECLARE_SUBSTANCE(filter_guiwang_addanticrit);

	filter_guiwang_addanticrit(object_interface object) : timeout_filter(object, 0, FILTER_MASK)
	{
		this->_heartbeat_counter = 0;
		_max_overlay_cnt = 100;
		_can_overlay = 1;
		_overlay_cnt = 0;

		_filter_id = FILTER_GUIWANG_ADDANTICRIT;
		_buff_id = HSTATE_GUIWANG_ADDANTICRIT;
	}

	~filter_guiwang_addanticrit() {}

	void Heartbeat(int tick)
	{
		if (! _parent.IsFilterExist(FILTER_BLESSED)
			&& ++this->_heartbeat_counter > 3
			&& _overlay_cnt > 0)
		{
			OnDetach();  //+132   猜的   haokaixin
			-- _overlay_cnt;
			OnAttach();  //+128  同上  猜的  haokaixin
			if (_overlay_cnt)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			}
			else
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
			this->_heartbeat_counter = 0;
		}
	}

	void BeAttacked(const XID& target,int dmg,bool crit)
	{
		if (! _parent.IsFilterExist(FILTER_BLESSED)
			&& target.IsPlayerClass()
			&& _overlay_cnt < _max_overlay_cnt)
		{
			OnDetach();  //+132   猜的   haokaixin
			if (_overlay_cnt)
				++_overlay_cnt;
			else
				_overlay_cnt += 20;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateBuff(_buff_id,_filter_id, GetTimeout(),_overlay_cnt);
			this->_heartbeat_counter = 0;
		}
	}

	void BeforeDeath(bool is_duel)
	{
		OnDetach();
		_overlay_cnt = 0;
		_parent.RemoveBuff(_buff_id, _filter_id);
	}

	void OnAttach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.EnhanceAntiCritRate(_overlay_cnt * 2.5);
			_parent.UpdateAttackData();
		}
	}

	void OnDetach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.ImpairAntiCritRate(_overlay_cnt * 2.5);
			_parent.UpdateAttackData();
		}
	}
};

class filter_guiwang_addmaxhp : public timeout_filter
{
protected:
	int _heartbeat_counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_guiwang_addmaxhp() {}
public:
	DECLARE_SUBSTANCE(filter_guiwang_addmaxhp);

	filter_guiwang_addmaxhp(object_interface object) : timeout_filter(object, 0, FILTER_MASK)
	{
		this->_heartbeat_counter = 0;
		_max_overlay_cnt = 100;
		_can_overlay = 1;
		_overlay_cnt = 0;

		_filter_id = FILTER_GUIWANG_ADDMAXHP;
		_buff_id = HSTATE_GUIWANG_ADDMAXHP;
	}

	~filter_guiwang_addmaxhp() {}

	void Heartbeat(int tick)
	{
		if (!_parent.IsFilterExist(FILTER_BLESSED)
			&& ++this->_heartbeat_counter > 3
			&& _overlay_cnt > 0)
		{
			OnDetach();  //+132   猜的   haokaixin
			--_overlay_cnt;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateHPMP();
			if (_overlay_cnt)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			}
			else
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
			this->_heartbeat_counter = 0;
		}
	}

	void BeAttacked(const XID& target, int dmg, bool crit)
	{
		if (!_parent.IsFilterExist(FILTER_BLESSED)
			&& target.IsPlayerClass()
			&& _overlay_cnt < _max_overlay_cnt)
		{
			OnDetach();  //+132   猜的   haokaixin
			if (_overlay_cnt)
				++_overlay_cnt;
			else
				_overlay_cnt += 20;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateHPMP();
			_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			this->_heartbeat_counter = 0;
		}
	}

	void BeforeDeath(bool is_duel)
	{
		OnDetach();
		_overlay_cnt = 0;
		_parent.RemoveBuff(_buff_id, _filter_id);
	}

	void OnAttach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.EnhanceScaleMaxHP(_overlay_cnt * 2.5);
			_parent.UpdateAttackData();
		}
	}

	void OnDetach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.ImpairScaleMaxHP(_overlay_cnt * 2.5);
			_parent.UpdateAttackData();
		}
	}
};

//260 
class filter_guiwang_adddamage : public timeout_filter
{
protected:
	int _heartbeat_counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DAMAGEENEMY | FILTER_MASK_BEFORE_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_guiwang_adddamage() {}
public:
	DECLARE_SUBSTANCE(filter_guiwang_adddamage);

	filter_guiwang_adddamage(object_interface object) : timeout_filter(object, 0, FILTER_MASK)
	{
		this->_heartbeat_counter = 0;
		_max_overlay_cnt = 100;
		_can_overlay = 1;
		_overlay_cnt = 0;

		_filter_id = FILTER_GUIWANG_ADDDAMAGE;
		_buff_id = HSTATE_GUIWANG_ADDDAMAGE;
	}

	~filter_guiwang_adddamage() {}

	void Heartbeat(int tick)
	{
		if (!_parent.IsFilterExist(FILTER_BLESSED)
			&& ++this->_heartbeat_counter > 3
			&& _overlay_cnt > 0)
		{
			OnDetach();  //+132   猜的   haokaixin
			--_overlay_cnt;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateHPMP();
			if (_overlay_cnt)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			}
			else
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
			this->_heartbeat_counter = 0;
		}
	}

	void DamageEnemy(const XID& target,int damage)
	{
		if (target.IsPlayerClass() && _overlay_cnt < _max_overlay_cnt)
		{
			OnDetach();  //+132   猜的   haokaixin
			if (_overlay_cnt)
				++_overlay_cnt;
			else
				_overlay_cnt += 20;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			this->_heartbeat_counter = 0;
		}
	}

	void BeforeDeath(bool is_duel)
	{
		OnDetach();
		_overlay_cnt = 0;
		_parent.RemoveBuff(_buff_id, _filter_id);
	}

	void OnAttach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.EnhanceScaleDamage(_overlay_cnt * 2.5);
			_parent.UpdateAttackData();
		}
	}

	void OnDetach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.ImpairScaleDamage(_overlay_cnt * 2.5);
			_parent.UpdateAttackData();
		}
	}
};

class filter_guiwang_addskillattack : public timeout_filter
{
protected:
	int _heartbeat_counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_DAMAGEENEMY | FILTER_MASK_BEFORE_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_guiwang_addskillattack() {}
public:
	DECLARE_SUBSTANCE(filter_guiwang_addskillattack);

	filter_guiwang_addskillattack(object_interface object) : timeout_filter(object, 0, FILTER_MASK)
	{
		this->_heartbeat_counter = 0;
		_max_overlay_cnt = 100;
		_can_overlay = 1;
		_overlay_cnt = 0;

		_filter_id = FILTER_GUIWANG_ADDSKILLATTACK;
		_buff_id = HSTATE_GUIWANG_ADDSKILLATTACK;
	}

	~filter_guiwang_addskillattack() {}

	void Heartbeat(int tick)
	{
		if (!_parent.IsFilterExist(FILTER_BLESSED)
			&& ++this->_heartbeat_counter > 3
			&& _overlay_cnt > 0)
		{
			OnDetach();  //+132   猜的   haokaixin
			--_overlay_cnt;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateHPMP();
			if (_overlay_cnt)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			}
			else
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
			this->_heartbeat_counter = 0;
		}
	}

	void DamageEnemy(const XID& target, int damage)
	{
		if (target.IsPlayerClass() && _overlay_cnt < _max_overlay_cnt)
		{
			OnDetach();  //+132   猜的   haokaixin
			if (_overlay_cnt)
				++_overlay_cnt;
			else
				_overlay_cnt += 20;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			this->_heartbeat_counter = 0;
		}
	}

	void BeforeDeath(bool is_duel)
	{
		OnDetach();
		_overlay_cnt = 0;
		_parent.RemoveBuff(_buff_id, _filter_id);
	}

	void OnAttach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.EnhanceSkillAttack(_overlay_cnt + _overlay_cnt);
			_parent.UpdateAttackData();
		}
	}

	void OnDetach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.ImpairSkillAttack(_overlay_cnt + _overlay_cnt);
			_parent.UpdateAttackData();
		}
	}
};

class filter_guiwang_addtenaciy : public timeout_filter
{
protected:
	int _heartbeat_counter;

	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};

	filter_guiwang_addtenaciy() {}
public:
	DECLARE_SUBSTANCE(filter_guiwang_addtenaciy);

	filter_guiwang_addtenaciy(object_interface object) : timeout_filter(object, 0, FILTER_MASK)
	{
		this->_heartbeat_counter = 0;
		_max_overlay_cnt = 100;
		_can_overlay = 1;
		_overlay_cnt = 0;

		_filter_id = FILTER_GUIWANG_ADDTENACIY;
		_buff_id = HSTATE_GUIWANG_ADDTENACIY;
	}

	~filter_guiwang_addtenaciy() {}

	void Heartbeat(int tick)
	{
		if (!_parent.IsFilterExist(FILTER_BLESSED)
			&& ++this->_heartbeat_counter > 3
			&& _overlay_cnt > 0)
		{
			OnDetach();  //+132   猜的   haokaixin
			--_overlay_cnt;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateHPMP();
			if (_overlay_cnt)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			}
			else
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
			this->_heartbeat_counter = 0;
		}
	}

	void BeAttacked(const XID& target, int dmg, bool crit)
	{
		if (!_parent.IsFilterExist(FILTER_BLESSED)
			&& target.IsPlayerClass()
			&& _overlay_cnt < _max_overlay_cnt)
		{
			OnDetach();  //+132   猜的   haokaixin
			if (_overlay_cnt)
				++_overlay_cnt;
			else
				_overlay_cnt += 20;
			OnAttach();  //+128  同上  猜的  haokaixin
			_parent.UpdateHPMP();
			_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			this->_heartbeat_counter = 0;
		}
	}

	void BeforeDeath(bool is_duel)
	{
		OnDetach();
		_overlay_cnt = 0;
		_parent.RemoveBuff(_buff_id, _filter_id);
	}

	void OnAttach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.EnhanceResistanceTenaciy(0, _overlay_cnt);
			_parent.EnhanceResistanceTenaciy(1, _overlay_cnt);
			_parent.EnhanceResistanceTenaciy(2, _overlay_cnt);
			_parent.EnhanceResistanceTenaciy(3, _overlay_cnt);
			_parent.EnhanceResistanceTenaciy(4, _overlay_cnt);
			_parent.EnhanceResistanceTenaciy(5, _overlay_cnt);
			_parent.UpdateMagicData();
		}
	}

	void OnDetach()
	{
		if (_overlay_cnt > 0)
		{
			_parent.ImpairResistanceTenaciy(0, _overlay_cnt);
			_parent.ImpairResistanceTenaciy(1, _overlay_cnt);
			_parent.ImpairResistanceTenaciy(2, _overlay_cnt);
			_parent.ImpairResistanceTenaciy(3, _overlay_cnt);
			_parent.ImpairResistanceTenaciy(4, _overlay_cnt);
			_parent.ImpairResistanceTenaciy(5, _overlay_cnt);
			_parent.UpdateMagicData();
		}
	}
};






#endif
