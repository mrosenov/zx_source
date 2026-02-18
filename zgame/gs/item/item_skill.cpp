#include "item_skill.h"
#include "../player_imp.h"
#include "../raid/collision_raid_world_manager.h"
#include "../raid/seek_raid_world_manager.h"

bool item_skill::IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const
{
	if(!_battleground_only) return true;
	if(pImp->GetWorldManager()->GetWorldLimit().can_use_battleground_skill_mater) return true;
	return false;
}
int item_skill::GetItemSkillLevel(int skill_id, gactive_imp *pImp) const
{
	if(pImp->_basic.level < _level_require)
	{
		pImp->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -1;
	}
	if(_skill_id != skill_id) return -1;
	return _skill_level;	
}

int item_skill::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	if(_level_require > obj->_basic.level)
	{
		obj->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -1;
	}	
	
	//在6v6战场禁止使用回城卷
	if(GetTID() == 23865 && (obj->GetWorldManager()->GetRaidType() == RT_COLLISION 
	|| obj->GetWorldManager()->GetRaidType() == RT_COLLISION2
	|| obj->GetWorldManager()->GetRaidType() == RT_SEEK
	|| obj->GetWorldManager()->GetRaidType() == RT_CAPTURE))
	{
		obj->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}

	//判断仙魔佛
	if(_require_cultivation && !(obj->OI_GetCultivation() & _require_cultivation))
	{
		obj->_runner->error_message(S2C::ERR_CULT_NOT_MATCH);
		return false;
	}

	if(!obj->CheckCoolDown(_cooldown_type))
	{
		obj->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	SKILL::Data data(_skill_id);
	data.item_id = GetTID();
	if(obj->_skill.CastRune(data,object_interface(obj,true), _skill_level,  _cooldown_type) == 0)
	{
		return _inconsumable?0:1;
	}
	else
	{
		return 0;
	}
}

