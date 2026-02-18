#include "config.h"
#include "aitrigger.h"
#include "aipolicy.h"
#include <arandomgen.h>
#include "actobject.h"
#include "world.h"
#include "obj_interface.h"

namespace ai_trigger
{
bool 
cond_hp_less::Check(policy * self,int)
{
	ai_object * ps = self->GetAIObject();
	return ps->GetHP() < ps->GetMaxHP() * _ratio ;
}

bool 
cond_timer::Check(policy * self,int)
{
	return self->CheckTimer(_timer_id);
}

bool 
cond_random::Check(policy * self,int)
{
	return  abase::RandUniform() < _rate;
}

bool 
cond_path_end::Check(policy * self,int path_id)
{
	return (path_id ==  _path_id);
}

bool 
cond_aggro_count::Check(policy * self,int path_id)
{
	ai_object * ps = self->GetAIObject();
	return (int)ps->GetAggroCount() > _count;
}

bool 
cond_battle_range::Check(policy * self,int path_id)
{
	ai_object * ps = self->GetAIObject();
	ai_policy * pAI = self->GetAIPolicy();
	A3DVECTOR last_pos = pAI->GetLastCombatPos();
	A3DVECTOR cur_pos;
	ps->GetPos(cur_pos);
	return cur_pos.squared_distance(last_pos) > _squared_distance;


}

int 
expr_common_data::GetValue(policy *self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());

	return oi.GetGlobalValue(_key);
}

int
expr_rank_level::GetValue(policy *self)
{
	return gmatrix::GetRankLevel(_index);
}
//---------后面是目标选择策略-------
void 
target_self::GetTarget(policy * self, XID & target)
{
	ai_object * ps = self->GetAIObject();
	ps->GetID(target);
}

void 
target_aggro_first::GetTarget(policy * self, XID & target)
{
	ai_object * ps = self->GetAIObject();
	if(ps->GetAggroCount() > 0)
	{
		ps->GetFirstAggro(target);
	}
	else 
	{
		target.type = -1;
		target.id  = -1;
	}
}

void 
target_aggro_second::GetTarget(policy * self, XID & target)
{
	ai_object * ps = self->GetAIObject();
	int count = ps->GetAggroCount();
	if(count > 1)
	{
		ps->GetAggroEntry(1,target);
	}
	else if(count > 0)
	{
		ps->GetFirstAggro(target);
	}
	else 
	{
		target.type = -1;
		target.id  = -1;
	}
}

void 
target_aggro_second_rand::GetTarget(policy * self, XID & target)
{
	ai_object * ps = self->GetAIObject();
	int count = ps->GetAggroCount();
	if(count > 1)
	{
		int index = abase::Rand(1, count -1);
		ps->GetAggroEntry(index,target);
	}
	else if(count > 0)
	{
		ps->GetFirstAggro(target);
	}
	else 
	{
		target.type = -1;
		target.id  = -1;
	}
}

void 
target_least_hp::GetTarget(policy * self, XID & target)
{
	//这里需要扫描所有的玩家，提出最小hp的玩家
	ai_object * ps = self->GetAIObject();
	int count = ps->GetAggroCount();
	target.type = -1;
	target.id = -1;
	if(count <= 0) return;

	int max_hp = 0x7FFFFFFF;
	for(int i = 0; i < count ; i ++)
	{
		ai_object::target_info info;
		XID id;
		ps->GetAggroEntry(i,id);
		if(ps->QueryTarget(id, info) == ai_object::TARGET_STATE_NORMAL)
		{
			if(info.hp < max_hp)
			{
				target = id;
				max_hp = info.hp;
			}
		}
	}
}

void 
target_most_hp::GetTarget(policy * self, XID & target)
{
	ai_object * ps = self->GetAIObject();
	int count = ps->GetAggroCount();
	target.type = -1;
	target.id = -1;
	if(count <= 0) return;

	int min_hp = -1;
	for(int i = 0; i < count ; i ++)
	{
		ai_object::target_info info;
		XID id;
		ps->GetAggroEntry(i,id);
		if(ps->QueryTarget(id, info) == ai_object::TARGET_STATE_NORMAL)
		{
			if(info.hp > min_hp)
			{
				target = id;
				min_hp = info.hp;
			}
		}
	}
}

void 
target_most_mp::GetTarget(policy * self, XID & target)
{
	ai_object * ps = self->GetAIObject();
	int count = ps->GetAggroCount();
	target.type = -1;
	target.id = -1;
	if(count <= 0) return;

	int min_mp = -1;
	for(int i = 0; i < count ; i ++)
	{
		XID id;
		ps->GetAggroEntry(i,id);
		ai_object::target_info info;
		if(ps->QueryTarget(id, info) == ai_object::TARGET_STATE_NORMAL)
		{
			if(info.mp > min_mp)
			{
				target = id;
				min_mp = info.mp;
			}
		}
	}
}

void 
target_class_combo::GetTarget(policy * self, XID & target)
{
	ai_object * ps = self->GetAIObject();
	int count = ps->GetAggroCount();
	target.type = -1;
	target.id = -1;
	if(count <= 0) return;
	
	XID target_list[16];
	int index = 0;

	for(int i = 0; i < count ; i ++)
	{
		XID id;
		ps->GetAggroEntry(i,id);
		if(!id.IsPlayer()) continue;
		ai_object::target_info info;
		if(ps->QueryTarget(id, info) == ai_object::TARGET_STATE_NORMAL)
		{
			int cls = info.cls;
			if((1 << cls) & _combo_state)
			{
				//符合条件
				target_list[index] = id;
				index ++;
				if(index >= (int)(sizeof(target_list)/sizeof(XID)))
				{
					break;
				}
			}
		}
	}
	if(index > 0)
	{
		target = target_list[abase::Rand(0,index-1)];
	}
	else
	{
		ps->GetFirstAggro(target);
	}
}


bool 
op_attack::DoSomething(policy * self)
{
	ai_policy * pAI = self->GetAIPolicy();
	XID target;
	_target->GetTarget(self,target);
	if(!target.IsActive())
	{
		if(!pAI->DetermineTarget(target))
		{
			target.type = -1;
		}
	}
	if(target.IsActive())
	{
		pAI->AddPrimaryTask(target,_attack_strategy);
	}
	return true;
}

bool 
op_skill::DoSomething(policy * self)
{
	ai_policy * pAI = self->GetAIPolicy();
	XID target;
	_target->GetTarget(self,target);
	if(!target.IsActive())
	{
		if(!pAI->DetermineTarget(target))
		{
			target.type = -1;
		}
	}
	if(target.IsActive())
	{
		ai_task * pTask = new ai_skill_task_2(target, _skill_id, _skill_lvl);
		pTask->Init(self->GetAIObject(),pAI);
		pAI->AddTask(pTask);
	}
	return true;
}

bool 
op_flee::DoSomething(policy * self)
{
	ai_policy * pAI = self->GetAIPolicy();
	XID target;
	_target->GetTarget(self,target);
	if(!target.IsActive())
	{
		if(!pAI->DetermineTarget(target))
		{
			target.type = -1;
		}
	}
	if(target.IsActive())
	{
		 ai_task * pTask = new ai_runaway_task(target);
		 pTask->Init(self->GetAIObject(),pAI);
		 pAI->AddTask(pTask);
	}
	return true;
}

bool 
op_create_timer::DoSomething(policy * self)
{
	self->CreateTimer(_timerid,_interval, _count,_inbattle);
	return true;
}

bool 
op_remove_timer::DoSomething(policy * self)
{
	self->RemoveTimer(_timerid);
	return true;
}

bool 
op_enable_trigger::DoSomething(policy * self)
{
	self->EnableTrigger(_trigger_id,_is_enable);
	return true;
}

bool 
op_exec_trigger::DoSomething(policy * self)
{
	return _trigger->TestTrigger(self);
}

bool 
op_say::DoSomething(policy * self)
{	
	ai_object * ps = self->GetAIObject();
	XID target = XID(-1,-1);
	if(_target) _target->GetTarget(self,target);
	size_t size = _msg.size();
	if(size > 4)
	{
		//检测是否专用语言
		const short * p = (short*)_msg.data();
		if(p[0] == '$' && p[1] == 'F')
		{
			//城战发言本派 三个均为广播 channel
			ps->BattleFactionSay(&p[2],size);
			return true;
		}
		else if(p[0] == '$' && p[1] == 'T')
		{
			//城战广播发言 三个均为广播 channel
			ps->BattleSay(&p[2],size);
			return true;
		}
		else if(p[0] == '$' && p[1] == 'B')
		{	
			//广播发言 三个均为广播 channel
			ps->BroadcastSay(&p[2],size);
			return true;
		}
		else if(p[0] == '$' && p[1] == 'A')
		{
			ps->BroadcastSay(&p[2],size,9);
			return true;
		}
	}
	ps->Say(target,_msg.data(),size,0);
	return true;
}

bool 
op_reset_aggro::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	ps->RegroupAggro();
	return true;
}

bool 
op_swap_aggro::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	ps->SwapAggro(_index1,_index2);
	return true;
}

bool 
op_be_taunted::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	XID target;
	_target->GetTarget(self,target);
	if(target.IsActive())
	{
		ps->BeTaunted(target);
	}
	return true;
}

bool 
op_fade_target::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	XID target;
	_target->GetTarget(self,target);
	if(target.IsActive())
	{
		ps->FadeTarget(target);
	}
	return true;
}

bool 
op_aggro_fade::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	ps->AggroFade();
	return true;
}

bool
op_active_spawner::DoSomething(policy * self)
{
	if(_ctrl_id <= 0) return true;
	ai_object * ps = self->GetAIObject();
	world_manager::ActiveSpawn(ps->GetWorldManager(), _ctrl_id, _is_active_spawner);
	return true;
}

bool 
op_create_minors::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());

	object_interface::minor_param prop;
	memset(&prop,0,sizeof(prop));

	size_t name_size = _name.size();
	void * name_buf = _name.data();
	if(name_size > sizeof(prop.mob_name)) name_size = sizeof(prop.mob_name);

	prop.mob_id = _mob_id;
	prop.vis_id = _vis_id;
	prop.remain_time = _life;
	prop.exp_factor = 1.f;
	prop.drop_rate = 1.f;
	prop.money_scale = 1.f;
	prop.spec_leader_id = XID(0,0);
	prop.parent_is_leader = _has_leader;
	prop.use_parent_faction = false;
	prop.die_with_leader = _has_leader || _die_with_leader;
	if(name_size > 0) 
	{
		prop.mob_name_size = name_size;
		memcpy(prop.mob_name, name_buf, name_size);
	}
//	GLog::log(GLOG_INFO,"AI策略创建了%d个NPC%d(%d) ", _count, _mob_id,_vis_id);
	for(int i = 0; i < _count; i ++)
	{
		oi.CreateMinors(prop, _radius);
	}
	return true;
}

bool 
op_task_trigger::DoSomething(policy * self)
{
//	ai_object * ps = self->GetAIObject();
//	object_interface oi(ps->GetImpl());
//
//	oi.TaskNPCTrigger(_event);
	return true;
}

bool 
op_disappear::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());
	oi.Disappear();
	return true;
}

bool 
op_switch_path::DoSomething(policy * self)
{
	ai_policy * pAI = self->GetAIPolicy();
	pAI->ChangePath(_path_id, _type);
	return true;
}

bool 
op_cajole_mobs::DoSomething(policy * self)
{
//	ai_object * ps = self->GetAIObject();
//	object_interface oi(ps->GetImpl());
//	oi.CajoleMobs(_radius);
	return true;
}

bool 
op_range_skill::DoSomething(policy * self)
{
//	ai_object * ps = self->GetAIObject();
//	object_interface oi(ps->GetImpl());

//	oi.RangeSkill(_skill,_level, _radius);
	return true;
}

bool 
op_combat_reset::DoSomething(policy * self)
{
	ai_policy * pAI = self->GetAIPolicy();
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());
	oi.SetInvincibleFilter(true, 8, false);
	ps->ClearAggro();
	pAI->ClearTask();
	return true;
}

bool 
op_set_common_data::DoSomething (policy *self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());

	oi.PutGlobalValue(_key, _set_value);
	return true;
}
bool 
op_add_common_data::DoSomething (policy *self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());

	oi.ModifyGlobalValue(_key, _add_value);
	return true;
}

bool
op_assign_common_data::DoSomething (policy *self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());

	oi.AssignGlobalValue(_src, _dest);
	return true;
}

bool
op_summon_mineral::DoSomething (policy *self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());

	oi.CreateMines(_id, _count, _lifetime, _range);
	return true;
}

bool
op_drop_item::DoSomething (policy *self)
{
	ai_object * ps = self->GetAIObject();
	object_interface oi(ps->GetImpl());

	oi.DropItem(_item_id, _item_num, _expire_date);
	return true;
}

bool 
op_change_aggro::DoSomething(policy * self)
{
	ai_object * ps = self->GetAIObject();
	
	XID target;
	_target->GetTarget(self,target);
	if(!target.IsActive())
	{
		return false;
	}

	ps->ChangeAggroEntry(target, _change_aggro_value);
	return true;
}

bool 
op_start_event::DoSomething(policy * self)
{
	gmatrix::StartEvent(_event_id);
	GLog::log(GLOG_INFO,"开启事件 event_id=%d", _event_id); 
	return true;
}

bool 
op_stop_event::DoSomething(policy * self)
{
	gmatrix::StopEvent(_event_id);
	GLog::log(GLOG_INFO,"关闭事件 event_id=%d", _event_id); 
	return true;
}

}


