#ifndef __ONLINEGAME_GS_SKILL_FILTER_H__
#define __ONLINEGAME_GS_SKILL_FILTER_H__
#include "filter.h"

class skill_interrupt_filter : public filter
{
	int _session_id;
public:
	DECLARE_SUBSTANCE(skill_interrupt_filter);
	skill_interrupt_filter(gactive_imp * imp,int session_id,int filter_id)
		:filter(object_interface(imp),FILTER_MASK_ADJUST_DAMAGE|FILTER_MASK_UNIQUE|FILTER_MASK_REMOVE_ON_DEATH)
	{
		_filter_id = filter_id;
		_session_id = session_id;
	}

protected:
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void AdjustDamage(damage_entry&,  const XID &, const attack_msg&);
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	skill_interrupt_filter() {}
};

class gather_interrupt_filter : public filter
{
	int _session_id;
public:
	DECLARE_SUBSTANCE(gather_interrupt_filter);
	gather_interrupt_filter(gactive_imp * imp,int session_id,int filter_id)
		:filter(object_interface(imp),FILTER_MASK_TRANSLATE_RECV_MSG|FILTER_MASK_UNIQUE|FILTER_MASK_REMOVE_ON_DEATH)
	{
		_filter_id = filter_id;
		_session_id = session_id;
	}

protected:
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg);
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	gather_interrupt_filter() {}
};

class sitdown_filter : public filter
{
	int _session_id;
public:
	DECLARE_SUBSTANCE(sitdown_filter);
	sitdown_filter(gactive_imp * imp,int session_id,int filter_id)
		:filter(object_interface(imp),FILTER_MASK_TRANSLATE_RECV_MSG|FILTER_MASK_UNIQUE|FILTER_MASK_REMOVE_ON_DEATH)
	{
		_filter_id = filter_id;
		_session_id = session_id;
	}

protected:
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg);
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	sitdown_filter() {}
};

class online_agent_filter : public filter
{
	int _session_id;
	int _tick_count;
public:
	DECLARE_SUBSTANCE(online_agent_filter);
	online_agent_filter(gactive_imp * imp,int session_id,int filter_id)
		:filter(object_interface(imp),FILTER_MASK_TRANSLATE_RECV_MSG|FILTER_MASK_UNIQUE|FILTER_MASK_REMOVE_ON_DEATH|FILTER_MASK_HEARTBEAT)
	{
		_filter_id = filter_id;
		_session_id = session_id;
		_tick_count = 0;
	}

protected:
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg);
	virtual void OnAttach();
	virtual void OnDetach();
	virtual void Heartbeat(int tick);
	online_agent_filter() {}
};


class active_emote_filter : public filter
{
	int _session_id;
public:
	DECLARE_SUBSTANCE(active_emote_filter);
	active_emote_filter(gactive_imp * imp,int session_id,int filter_id)
		:filter(object_interface(imp),FILTER_MASK_TRANSLATE_RECV_MSG|FILTER_MASK_UNIQUE|FILTER_MASK_REMOVE_ON_DEATH)
	{
		_filter_id = filter_id;
		_session_id = session_id;
	}

protected:
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg);
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	active_emote_filter() {}
};


#endif

