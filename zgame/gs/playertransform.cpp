#include "playertransform.h"
#include "transform_filter.h"
#include "player_imp.h"

void 
player_transform::Init(gplayer_imp *pImp)
{
	ASSERT(_template_id > 0);
	DATA_TYPE dt;
	const CHANGE_SHAPE_PROP_CONFIG & ess = *(CHANGE_SHAPE_PROP_CONFIG*)gmatrix::GetDataMan().get_data_ptr(_template_id, ID_SPACE_CONFIG,dt);
	if(dt != DT_CHANGE_SHAPE_PROP_CONFIG || &ess == NULL)
	{
		ASSERT(false);
	}

	_can_skill = !ess.shield_skill;				
	_can_bot = ess.can_run_helper;
	_can_attack = ess.can_fight;
	_can_fly = ess.is_fly;
	_can_use_potion = ess.can_use_medicine;
	_can_change_faction = ess.change_faction;


	_move_mode = ess.move_type;
	_buff_type = ess.is_buff_positive;
	_attack_recover_rate = ess.recover_prop;

	if(_can_change_faction)
	{
		_old_faction = pImp->GetFaction();
		_old_enemy_faction = pImp->GetEnemyFaction();
		pImp->_faction = ess.faction;
		pImp->_enemy_faction = ess.enemy_faction;
		pImp->GetParent()->base_info.faction = pImp->GetFaction();
	}

}

void
player_transform::Reset(gplayer_imp *pImp)
{
	_template_id = 0;
	_transform_type = 0;
	_transform_level = 0;
	_transform_explevel = 0;
	
	if(_can_change_faction)
	{
		pImp->_faction = _old_faction;
		pImp->_enemy_faction = _old_enemy_faction;
		pImp->GetParent()->base_info.faction = pImp->GetFaction();
	}
}

bool
player_transform::OnActiveSkill(gplayer_imp *pImp)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TRANSFORM);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	lua_getfield(L, LUA_GLOBALSINDEX, "Init_Transform_Skill");
	lua_pushinteger(L, _template_id);
	lua_pushinteger(L, (int)_transform_type);
	lua_pushinteger(L, (int)_transform_level);
	lua_pushinteger(L, (int)_transform_explevel);
	lua_pushlightuserdata(L, (void*)pImp);

	if(lua_pcall(L, 5, 1,0))
	{
		printf("error when calling Init_Transform_Skill \n");
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		printf("error when calling Init_Transform_Skill \n");
	}
	lua_pop(L,1);

	return true;
}

void
player_transform::OnDeActiveSkill(gplayer_imp *pImp)
{
	_transform_skill.clear();
}


void
player_transform::StartTransform(gplayer_imp *pImp)
{
	AddTransformFilter(pImp);
}

void
player_transform::ModifyTransform(gplayer_imp* pImp)
{
	ModifyTransformFilter(pImp);
}

void 
player_transform::StopTransform(gplayer_imp *pImp)
{
	RemoveTransformFilter(pImp);
}

void
player_transform::OnActiveTransformState(gplayer_imp *pImp)
{
	OnActiveProp(pImp);
	OnActiveSkill(pImp);

	//加入命令限制
	if(_move_mode == MOVE_DISABLE)
	{
		pImp->_commander->DenyCmd(controller::CMD_MOVE);
	}
	if(!_can_skill)
	{
		pImp->_commander->DenyCmd(controller::CMD_SKILL);
	}
	if(!_can_bot)
	{
		pImp->_commander->DenyCmd(controller::CMD_BOT);
	}
	if(!_can_attack)
	{
		pImp->_commander->DenyCmd(controller::CMD_ATTACK);
	}
	if(!_can_fly)
	{
		pImp->_commander->DenyCmd(controller::CMD_FLY);
	}

	pImp->_commander->DenyCmd(controller::CMD_BIND);
	pImp->_commander->DenyCmd(controller::CMD_MARKET);
		
	pImp->_runner->player_start_transform(_template_id, 1, _transform_type, _transform_level, _transform_explevel);
	pImp->_runner->transform_skill_data();
}

void
player_transform::OnDeactiveTransformState(gplayer_imp *pImp)
{
	OnDeActiveProp(pImp);
	OnDeActiveSkill(pImp);

	//加入命令限制
	if(_move_mode == MOVE_DISABLE)
	{
		pImp->_commander->AllowCmd(controller::CMD_MOVE);
	}
	if(!_can_skill)
	{
		pImp->_commander->AllowCmd(controller::CMD_SKILL);
	}
	if(!_can_bot)
	{
		pImp->_commander->AllowCmd(controller::CMD_BOT);
	}
	if(!_can_attack)
	{
		pImp->_commander->AllowCmd(controller::CMD_ATTACK);
	}
	if(!_can_fly)
	{
		pImp->_commander->AllowCmd(controller::CMD_FLY);
	}

	pImp->_commander->AllowCmd(controller::CMD_BIND);
	pImp->_commander->AllowCmd(controller::CMD_MARKET);


	pImp->_runner->player_start_transform(_template_id, 0, _transform_type, _transform_level, _transform_explevel);
	Reset(pImp);

}

void
player_transform::AddTransformSkill(int skill_id, int skill_level)
{
	for(size_t i = 0; i < _transform_skill.size(); ++i)
	{
		if(_transform_skill[i].skill_id == skill_id) return;
	}

	transform_skill ts;
	ts.skill_id = skill_id;
	ts.skill_level = skill_level;
	_transform_skill.push_back(ts);
}

int 
player_transform::GetSkillLevel(int skill_id)
{
	for(size_t i = 0; i < _transform_skill.size(); ++i)
	{
		if(skill_id == _transform_skill[i].skill_id)
		{
			return _transform_skill[i].skill_level;
		}
	}
	return -1;
}

void
player_transform::GetSkillData(archive & ar)
{
	size_t size = _transform_skill.size();
	ar << size;

	for(size_t i = 0; i < _transform_skill.size(); ++i)
	{
		ar << (short)_transform_skill[i].skill_id;
		ar << (char)_transform_skill[i].skill_level;
	}

	//为了兼容skillwrapper的结构，这里额外压一个size
	ar << (size_t)0;
}


void 
player_item_transform::AddTransformFilter(gplayer_imp *pImp)
{
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_ITEM_TRANSFORM)) return;
	pImp->_filters.AddFilter(new item_transform_filter(pImp,FILTER_INDEX_ITEM_TRANSFORM,_energy_drop_speed,
		_exp_speed,_attack_recover_rate)); 
}

void 
player_item_transform::RemoveTransformFilter(gplayer_imp *pImp)
{
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_ITEM_TRANSFORM))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_ITEM_TRANSFORM);
	}
}

bool
player_item_transform::OnActiveProp(gplayer_imp *pImp)
{
	item & it = pImp->GetEquipInventory()[item::EQUIP_INDEX_MAGIC];
	if(it.type == -1 || it.GetItemType() != item_body::ITEM_TYPE_MAGIC)
	{
		return false;
	}
	it.ActivateProp(item::EQUIP_INDEX_MAGIC, pImp);
	return true;
}

bool
player_item_transform::OnDeActiveProp(gplayer_imp *pImp)
{
	item & it= pImp->GetEquipInventory()[item::EQUIP_INDEX_MAGIC];
	if(it.type == -1 || it.GetItemType() != item_body::ITEM_TYPE_MAGIC)
	{
		return false;
	}
	it.DeactivateProp(item::EQUIP_INDEX_MAGIC, pImp);
	return true;
}

bool
player_normal_transform::OnActiveProp(gplayer_imp *pImp)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TRANSFORM);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	lua_getfield(L, LUA_GLOBALSINDEX, "Init_Normal_Transform_Prop");
	lua_pushinteger(L, _template_id);
	lua_pushinteger(L, (int)_transform_type);
	lua_pushinteger(L, (int)_transform_level);
	lua_pushinteger(L, (int)_transform_explevel);
	lua_pushlightuserdata(L, (void*)pImp);

	if(lua_pcall(L, 5, 1,0))
	{
		printf("error when calling Init_Transform_Skill \n");
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		printf("error when calling Init_Transform_Skill \n");
	}
	lua_pop(L,1);

	pImp->_en_point += _en_point;
	pImp->_en_percent += _en_percent;
	return true;
}

bool
player_normal_transform::OnDeActiveProp(gplayer_imp *pImp)
{
	pImp->_en_point -= _en_point;
	pImp->_en_percent -= _en_percent;
	return true;
}


void 
player_skill_transform::AddTransformFilter(gplayer_imp *pImp)
{
	int buff_mask = (_buff_type == 0 ? filter::FILTER_MASK_DEBUFF : filter::FILTER_MASK_BUFF); 
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_SKILL_TRANSFORM)) return;
	pImp->_filters.AddFilter(new skill_transform_filter(pImp,FILTER_INDEX_SKILL_TRANSFORM,_attack_recover_rate, GetTimeOut(), buff_mask)); 
}

void 
player_skill_transform::RemoveTransformFilter(gplayer_imp *pImp)
{
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_SKILL_TRANSFORM))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_SKILL_TRANSFORM);
	}
}

void 
player_buffarea_transform::AddTransformFilter(gplayer_imp *pImp)
{
	int buff_mask = (_buff_type == 0 ? filter::FILTER_MASK_DEBUFF : filter::FILTER_MASK_BUFF); 
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_BUFFAREA_TRANFORM)) return;
	pImp->_filters.AddFilter(new buffarea_transform_filter(pImp,FILTER_INDEX_BUFFAREA_TRANFORM,_attack_recover_rate, GetTimeOut(), buff_mask, _template_id, _transform_type, _transform_level, _transform_explevel)); 
}

void 
player_buffarea_transform::RemoveTransformFilter(gplayer_imp *pImp)
{
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_BUFFAREA_TRANFORM))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_BUFFAREA_TRANFORM);
	}
}

void 
player_buffarea_transform::ModifyTransformFilter(gplayer_imp* pImp)
{
	buffarea_transform_filter_data data;
	data.template_id = _template_id;
	data.type = _transform_type;
	data.level = _transform_level;
	data.explevel = _transform_explevel;
	data.timeout = GetTimeOut();
	pImp->_filters.ModifyFilter(FILTER_INDEX_TASK_TRANSFORM, FMID_BUFFAREA, &data, sizeof(data));
}

void 
player_task_transform::AddTransformFilter(gplayer_imp *pImp)
{
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_TASK_TRANSFORM)) return;
	pImp->_filters.AddFilter(new task_transform_filter(pImp,FILTER_INDEX_TASK_TRANSFORM,_attack_recover_rate, GetTimeOut())); 
}

void 
player_task_transform::RemoveTransformFilter(gplayer_imp *pImp)
{
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_TASK_TRANSFORM))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_TASK_TRANSFORM);
	}
}

