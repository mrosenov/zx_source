#include "invincible_filter.h"
#include "clstab.h"
#include "actobject.h"
#include "statedef.h"

DEFINE_SUBSTANCE(invincible_filter,filter,CLS_FILTER_INVINCIBLE)
DEFINE_SUBSTANCE(invincible_filter_to_spec_id,filter,CLS_FILTER_INVINCIBLE_SPEC_ID)
DEFINE_SUBSTANCE(invincible_banish_filter,filter,CLS_FILTER_BANISH_INVINCIBLE)
DEFINE_SUBSTANCE(invincible_battleground_filter,filter,CLS_FILTER_BATTLEGROUND_INVINCIBLE)
DEFINE_SUBSTANCE(invincible_logon_filter,filter,CLS_FILTER_LOGON_INVINCIBLE)

void 
invincible_filter::OnAttach()
{
	gobject * pObj = _parent.GetImpl()->_parent; 
	if( (_isAggressive = pObj->msg_mask & gobject::MSG_MASK_PLAYER_MOVE))
	{
		pObj->msg_mask &= ~gobject::MSG_MASK_PLAYER_MOVE;
	}
	if(!_parent.IsPlayerClass())
	{
		_parent.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);
	}
}

void 
invincible_filter::OnDetach()
{
	if(_isAggressive)
	{
		gobject * pObj = _parent.GetImpl()->_parent; 
		pObj->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
	}
}

void 
invincible_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack  = 0;
	if(_immune)
	{
		msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
	}
}       

void 
invincible_filter::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack = 0; 
	if(!msg.helpful && _immune)
	{
		msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
	}
}       

void 
invincible_filter::Heartbeat(int tick)
{
	if(_timeout < 0) return;
	if(--_timeout <= 0) 
	{
		_is_deleted = true;
	}
}

void 
invincible_filter_to_spec_id::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	if(attacker == _who)
	{
		msg.target_faction = 0;
		msg.force_attack  = 0;
	}
}       

void 
invincible_filter_to_spec_id::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
	if(attacker == _who)
	{
		msg.target_faction = 0;
		msg.force_attack = 0; 
	}
}       


void 
invincible_banish_filter::OnAttach()
{
	if(!_parent.IsPlayerClass())
	{
		_parent.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);
	}
}

void 
invincible_banish_filter::OnDetach()
{
}

void 
invincible_banish_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack  = 0;
}       

void 
invincible_banish_filter::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack = 0; 
	msg.helpful = false;
}       

void 
invincible_banish_filter::Heartbeat(int tick)
{
	if(_timeout < 0) return;
	if(--_timeout <= 0) 
	{
		_is_deleted = true;
	}
}


void 
invincible_battleground_filter::OnAttach()
{
	_parent.IncVisibleState(GNET::VSTATE_INVINCIBLE);
	_parent.UpdateBuff(GNET::HSTATE_INVINCIBLE, _filter_id, GetTimeout());
}

void 
invincible_battleground_filter::OnDetach()
{
	_parent.DecVisibleState(GNET::VSTATE_INVINCIBLE);
	_parent.RemoveBuff(GNET::HSTATE_INVINCIBLE, _filter_id);
}

void 
invincible_battleground_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack  = 0;
	msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
}       

void 
invincible_battleground_filter::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack = 0; 
	msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
}      

void 
invincible_logon_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack  = 0;
	msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
}	

void 
invincible_logon_filter::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack = 0; 
	msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
}

void 
invincible_logon_filter::OnAttach()
{
	_parent.IncVisibleState(GNET::VSTATE_INVINCIBLE);
	_parent.UpdateBuff(GNET::HSTATE_INVINCIBLE, _filter_id, _timeout);
}

void 
invincible_logon_filter::OnDetach()
{
	_parent.DecVisibleState(GNET::VSTATE_INVINCIBLE);
	_parent.RemoveBuff(GNET::HSTATE_INVINCIBLE, _filter_id);
}

