#include "azone_filter.h"
#include "statedef.h"
#include "clstab.h"
#include <vector>

DEFINE_SUBSTANCE(azone_filter,filter,CLS_FILTER_AZONE)

void 
azone_filter::OnAttach()
{
	_parent.UpdateBuff(GNET::HSTATE_ABSOLUTEAREA, _filter_id, 0); 
	_parent.IncVisibleState(_visible_state);
}

void 
azone_filter::OnDetach()
{
	_parent.RemoveBuff(GNET::HSTATE_ABSOLUTEAREA, _filter_id);
	_parent.DecVisibleState(_visible_state);
}

void 
azone_filter::Heartbeat(int tick)
{
	if(_mp_cost>0 && _parent.GetBasicProp().mp<_mp_cost)
	{
		_is_deleted = true;
		return;
	}
	_parent.DrainMana(_mp_cost);

	enchant_msg	msg;
	memset(&msg,0,sizeof(msg));

	msg.skill        = _skill_id;
	msg.skill_level	 = _skill_level;
	msg.force_attack = _force;
	msg.helpful      = _is_helpful;
	msg.attack_range = _radis;
	msg.skill_reserved1 =  (int)_parent.GetExtendProp().damage_high;
	msg.skill_var[0] = _var1;
	msg.skill_var[1] = _var2;

	_parent.SetSkillTalent(_skill_id, msg.skill_modify);

	if(_is_helpful)
	{
		if(_parent.IsInTeam())
			_parent.TeamEnchant(msg, false, false);
		else
			_parent.Enchant(_parent.GetSelfID(), msg );
	}
	else
	{
		std::vector<exclude_target> empty;
		_parent.RegionEnchant1(_parent.GetPos(), _radis, msg, _count, empty);
	}

}

