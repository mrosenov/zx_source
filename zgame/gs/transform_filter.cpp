#include "transform_filter.h"
#include "player_imp.h"
#include "statedef.h"


void 
transform_filter::OnAttach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->ActiveTransformState();
	UpdateBuff();
}

void 
transform_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->DeactiveTransformState();
	RemoveBuff();
}

void 
transform_filter::AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg)
{
	if(abase::RandUniform() < _attack_recover_rate)
	{
		_is_deleted = true;
	}
}

void
item_transform_filter::UpdateBuff()
{
	_parent.UpdateBuff(GNET::HSTATE_CHANGEFORM, _filter_id,0); 
}

void
item_transform_filter::RemoveBuff()
{
	_parent.RemoveBuff(GNET::HSTATE_CHANGEFORM, _filter_id);
}

void 
item_transform_filter::AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg)
{
	if(abase::RandUniform() < _attack_recover_rate)
	{
		_is_deleted = true;
	}
	
	if(abase::RandUniform() < MAGIC_DURATION_DEC_PROB)
	{
		gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
		if(pImp->DecMagicDuration(1) <=0)
		{
			_is_deleted = true;
		}
	}
}
	
void 
item_transform_filter::Heartbeat(int)
{
	if(_energy_drop_speed > 0 && _parent.SpendTalismanStamina(_energy_drop_speed) <= 0)
	{
		_is_deleted = true;
	}
	else
	{
		_exp_counter ++;
		if(_exp_counter >= 60)
		{
			if(_exp_speed > 0) _parent.MagicGainExp(_exp_speed);
			_exp_counter = 0;
		}
	}
}

void
item_transform_filter::TranslateSendAttack(const XID & target,attack_msg & msg, char type, float r, int mc)
{
	if(abase::RandUniform() < MAGIC_DURATION_DEC_PROB)
	{
		gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
		if(pImp->DecMagicDuration(1) <= 0)
		{
			_is_deleted = true;
		}
	}
}

void
skill_transform_filter::UpdateBuff()
{
	_parent.UpdateBuff(GNET::HSTATE_TRANSFORM, _filter_id, _timeout); 
}

void
skill_transform_filter::RemoveBuff()
{
	_parent.RemoveBuff(GNET::HSTATE_TRANSFORM, _filter_id);
}

void 
skill_transform_filter::Heartbeat(int tick)
{
	if(_timeout < 0) return;
	if(--_timeout <= 0) 
	{
		_is_deleted = true;
	}
}

void
task_transform_filter::UpdateBuff()
{
	_parent.UpdateBuff(GNET::HSTATE_TRANSFORM_TASK, _filter_id, _timeout); 
}

void
task_transform_filter::RemoveBuff()
{
	_parent.RemoveBuff(GNET::HSTATE_TRANSFORM_TASK, _filter_id);
}


void 
task_transform_filter::Heartbeat(int tick)
{
	if(_timeout < 0) return;
	if(--_timeout <= 0) 
	{
		_is_deleted = true;
	}
}

void buffarea_transform_filter::Heartbeat(int tick)
{
	if(_timeout < 0) return;
	if(--_timeout <= 0) 
	{
		_is_deleted = true;
	}
}

void buffarea_transform_filter::UpdateBuff()
{
	_parent.UpdateBuff(GNET::HSTATE_TRANSFORM, _filter_id, _timeout); 
}

void buffarea_transform_filter::RemoveBuff()
{
	_parent.RemoveBuff(GNET::HSTATE_TRANSFORM, _filter_id);
}
