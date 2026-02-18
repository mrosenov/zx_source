#include "skill_filter.h"
#include "invincible_filter.h"
#include "clstab.h"
#include "player_imp.h"

DEFINE_SUBSTANCE(skill_interrupt_filter,filter,CLS_FILTER_SKILL_INTERRUPT)
DEFINE_SUBSTANCE(gather_interrupt_filter,filter,CLS_FILTER_GATHER_INTERRUPT)
DEFINE_SUBSTANCE(sitdown_filter,filter, CLS_FILTER_SITDOWN)
DEFINE_SUBSTANCE(online_agent_filter,filter, CLS_FILTER_ONLINE_AGENT)
DEFINE_SUBSTANCE(active_emote_filter,filter, CLS_FILTER_ACTIVE_EMOTE)

void 
skill_interrupt_filter::AdjustDamage(damage_entry&, const XID &, const attack_msg&)
{
	if(_parent.SessionOnAttacked(_session_id))
	{
		_is_deleted = true;
	}
}

bool 
skill_interrupt_filter::Save(archive & ar)
{
	filter::Save(ar);
	ar <<  _session_id;
	return true;
}

bool 
skill_interrupt_filter::Load(archive & ar)
{
	filter::Load(ar);
	ar >> _session_id;
	return true;
}

void 
gather_interrupt_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	if(msg.target_faction & _parent.GetFaction())
	{
		if(_parent.SessionOnAttacked(_session_id))
		{
			_is_deleted = true;
		}
	}
}

bool 
gather_interrupt_filter::Save(archive & ar)
{
	filter::Save(ar);
	ar <<  _session_id;
	return true;
}

bool 
gather_interrupt_filter::Load(archive & ar)
{
	filter::Load(ar);
	ar >> _session_id;
	return true;
}

void 
sitdown_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	if(_parent.SessionOnAttacked(_session_id))
	{
		_is_deleted = true;
	}
}

bool 
sitdown_filter::Save(archive & ar)
{
	filter::Save(ar);
	ar <<  _session_id;
	return true;
}

bool 
sitdown_filter::Load(archive & ar)
{
	filter::Load(ar);
	ar >> _session_id;
	return true;
}

void 
online_agent_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	if(msg.target_faction & _parent.GetFaction())
	{
		if(_parent.SessionOnAttacked(_session_id))
		{
			_is_deleted = true;
		}
	}
}

void online_agent_filter::OnAttach() 
{
	_parent.UpdateBuff(1006, 1, 0);
}

void online_agent_filter::OnDetach()
{
	_parent.RemoveBuff(1006, 1);
}

void online_agent_filter::Heartbeat(int tick)
{
	_tick_count ++;
	if(_tick_count >= 60)
	{
		//给玩家增加一分钟的经验
		gplayer_imp * pImp = (gplayer_imp *) _parent.GetImpl();
		int64_t exp = gplayer_imp::GetAgentExpBonus(_parent.GetBasicProp().level, pImp->GetRebornCount(), false);
		if(exp >0)
		{
			int real_exp = (exp & 0x7FFFFFFF);
			pImp->ReceiveTaskExp(real_exp);
		}

		_tick_count = 0;
	}
}

void 
active_emote_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	if(_parent.SessionOnAttacked(_session_id))
	{
		_is_deleted = true;
	}
}

bool 
active_emote_filter::Save(archive & ar)
{
	filter::Save(ar);
	ar <<  _session_id;
	return true;
}

bool 
active_emote_filter::Load(archive & ar)
{
	filter::Load(ar);
	ar >> _session_id;
	return true;
}

