#ifndef __ONLINEGAME_GS_TRANSFORM_FILTER_H__
#define __ONLINEGAME_GS_TRANSFORM_FILTER_H__

#include "filter.h"
#include "sfilterdef.h"

class transform_filter : public filter
{
protected:
	float _attack_recover_rate;
public:
	transform_filter(gactive_imp * imp, int mask, int filter_id, float attack_recover_rate)
		:filter(object_interface(imp),mask)
	{
		_filter_id = filter_id;
		_attack_recover_rate = attack_recover_rate;
	}

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg);
	virtual void Heartbeat(int) = 0;

protected:
	virtual void UpdateBuff(){}
	virtual void RemoveBuff(){}
};

class item_transform_filter : public transform_filter
{
	int _exp_speed;
	int _exp_counter;
	float _energy_drop_speed;
	enum
	{
		MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_ADJUST_DAMAGE 
		       | FILTER_MASK_TRANSLATE_SEND_MSG  
	};
public:
	item_transform_filter(gactive_imp * imp, int filter_id, float energy_drop_speed, int exp_speed,
			float attack_recover_rate)
		:transform_filter(imp, MASK, filter_id, attack_recover_rate)
	{
		_energy_drop_speed = energy_drop_speed;
		_exp_speed = exp_speed;
		_exp_counter = 0;
	}
	virtual void Heartbeat(int);
	virtual void AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg);
	virtual void TranslateSendAttack(const XID & target,attack_msg & msg, char type, float r, int mc);	

protected:
	virtual void UpdateBuff();
	virtual void RemoveBuff();
};

class skill_transform_filter : public transform_filter
{
	int _timeout;
	enum
	{
		MASK = FILTER_MASK_UNIQUE | FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_ADJUST_DAMAGE 
	};

public:
	skill_transform_filter(gactive_imp * imp, int filter_id, float attack_recover_rate, int timeout, int buff_mask)
		: transform_filter(imp, MASK|buff_mask, filter_id, attack_recover_rate)
	{
		_timeout = timeout;
	}
	
	virtual void Heartbeat(int);

protected:
	virtual void UpdateBuff();
	virtual void RemoveBuff();
};


struct buffarea_transform_filter_data
{
	int template_id;
	int type;
	int level;
	int explevel;
	int timeout;
};

class buffarea_transform_filter : public transform_filter
{
	int _template_id;
	int _type;
	int _level;
	int _explevel;
	int _timeout;
	enum
	{
		MASK = FILTER_MASK_UNIQUE | FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_ADJUST_DAMAGE 
	};

public:
	buffarea_transform_filter(gactive_imp * imp, int filter_id, float attack_recover_rate, int timeout, int buff_mask, int template_id, int type, int level, int explevel)
		: transform_filter(imp, MASK | buff_mask, filter_id, attack_recover_rate) 
	{
		_timeout = timeout;
		_template_id = template_id;
		_type = type;
		_level = level;
		_explevel = explevel;
	}
	void  OnModify(int ctrlname,void * ctrlval,size_t ctrllen)
	{
		if(ctrlname==FMID_BUFFAREA && ctrllen == sizeof(buffarea_transform_filter_data))
		{
			buffarea_transform_filter_data& data = *(buffarea_transform_filter_data*)ctrlval;
			if(data.template_id == _template_id && data.type == _type && data.level == _level && data.explevel == _explevel && data.timeout > _timeout)
			{
				_timeout = data.timeout;
			}
		}
	}
	
	virtual void Heartbeat(int);

protected:
	virtual void UpdateBuff();
	virtual void RemoveBuff();
};

class task_transform_filter : public transform_filter
{
	int _timeout;
	enum
	{
		MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_ADJUST_DAMAGE 
	};

public:
	task_transform_filter(gactive_imp * imp, int filter_id, float attack_recover_rate, int timeout)
		: transform_filter(imp, MASK, filter_id, attack_recover_rate) 
	{
		_timeout = timeout;
	}
	
	virtual void Heartbeat(int);
protected:
	virtual void UpdateBuff();
	virtual void RemoveBuff();
};


#endif


