#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include <arandomgen.h>
#include "mobactivenpc.h"
#include "world.h"
#include "clstab.h"
#include "ainpc.h"
#include "mobactivedataman.h"
#include "patrol_agent.h"
#include "npcsession.h"

DEFINE_SUBSTANCE(mob_active_imp,gnpc_imp,CLS_MOBACTIVE_NPC_IMP)
DEFINE_SUBSTANCE(mob_active_policy,ai_policy,CLS_NPC_AI_POLICY_MOB_ACTIVE)

mob_active_imp::mob_active_imp()
{
	_active_mode = 0;
	_active_point_num = 0;
	_active_times_max = 0;
	_active_no_disappear = false;
	memset(_active_point,0,sizeof(_active_point));
	_need_equipment = 0;
	_task_in = 0;
	_level = 0;
	_require_gender = 0;
	_character_combo_id = 0;
	_character_combo_id2 = 0;
	_eliminate_equipment = 0;
	_active_ready_time = 0;
	_active_running_time = 0;
	_active_ending_time = 0;
	_path_id = 0;
	_end_new_mob_active_id = 0;
	_move_can_active = 0;
	_path_end_finish_active = false;

	_active_counter = 0;
	_lock = false;
	_lock_id = 0;
	_lock_time_out = 0;
	_mob_active_move_status = MOBACTIVE_CANNOT_MOVE;
	
	for (size_t i = 0; i < 6; i++)
	{
		_pos[i] = false;
	}

	_mob_active_life = 0;
	_mob_active_can_reclaim = false;

}

void mob_active_imp::SetParam(mob_active_statement* mob_active_data)
{
	_active_mode = mob_active_data->active_mode;
	_active_point_num = mob_active_data->active_point_num;
	_active_times_max = mob_active_data->active_times_max;
	_active_no_disappear = mob_active_data->active_no_disappear;
	memcpy(_active_point,mob_active_data->active_point,sizeof(_active_point));
	_need_equipment = mob_active_data->need_equipment;
	_task_in = mob_active_data->task_in;
	_level = mob_active_data->level;
	_require_gender = mob_active_data->require_gender;
	_character_combo_id = mob_active_data->character_combo_id;
	_character_combo_id2 = mob_active_data->character_combo_id2;
	_eliminate_equipment = mob_active_data->eliminate_equipment;
	_active_ready_time = mob_active_data->active_ready_time;
	_active_running_time = mob_active_data->active_running_time;
	_active_ending_time = mob_active_data->active_ending_time;
	_path_id = mob_active_data->path_id;
	_end_new_mob_active_id = mob_active_data->new_mob_active_id;
	_move_can_active = mob_active_data->move_can_active;
	_path_end_finish_active = mob_active_data->path_end_finish_active;

	if (_path_id)
	{
		_mob_active_move_status = MOBACTIVE_STOP_MOVE;
	}
	if (_spawner && _spawner->IsMobActiveReclaim())
	{
		_mob_active_can_reclaim = true;
	}
}

int
mob_active_imp::MessageHandler(const MSG & msg)
{
	switch (msg.message)
	{
		case GM_MSG_NPC_DISAPPEAR:
		{
			return gnpc_imp::MessageHandler(msg);
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_START:
		{
			ASSERT(msg.content_length == sizeof(int) * 5);
			int need_equipment = *(((int*)msg.content) + 0);
			int task_in = *(((int*)msg.content) + 1);
			int level = *(((int*)msg.content)+2);
			int gender = *(((int*)msg.content)+3);
			int cls = *(((int*)msg.content)+4);

			if (need_equipment != _need_equipment || task_in != _task_in || level <= _level
					|| !((gender + 1) & (_require_gender + 1)) || !object_base_info::CheckCls(cls,_character_combo_id,_character_combo_id2) ||  _lock)
			{
				SendErrorMessage(msg.source,S2C::ERR_MOB_ACTIVE_INVALID);
				return 0;
			}

			if (_mob_active_move_status == MOBACTIVE_MOVE && !_move_can_active)
			{
				SendErrorMessage(msg.source,S2C::ERR_MOB_ACTIVE_INVALID);
				return 0;
			}

			if (_player_map.find(msg.source.id) != _player_map.end()) return 0;
			if (_lock)
			{
				SendErrorMessage(msg.source,S2C::ERR_MOB_ACTIVE_LOCKED);
				return 0;
			}
			if(msg.pos.squared_distance(_parent->pos) >= GATHER_RANGE_LIMIT*GATHER_RANGE_LIMIT)
			{
				SendErrorMessage(msg.source,S2C::ERR_OUT_OF_RANGE);
				return 0;
			}

			if (_player_map.size() >= _active_point_num) 
			{
				SendErrorMessage(msg.source,S2C::ERR_MOB_ACTIVE_POINT_MAX);
				return 0;
			}


			if (_active_times_max && _active_counter >= _active_times_max)
			{
				SendErrorMessage(msg.source,S2C::ERR_MOB_ACTIVE_TIMES_MAX);
				return 0;
			}

			//会移动的互动物品
			if (_path_id)
			{
				if (_player_map.size() && _mob_active_move_status == MOBACTIVE_MOVE && _active_ready_time)
				{
					//让互动物品停下来
					MobActiveStopMove();
					_mob_active_move_status = MOBACTIVE_STOP_MOVE;
				}
			}

			addplayer(msg.source.id,msg.pos);
			int emptypos = _player_map[msg.source.id];
			if (_active_ready_time)
			{
				_lock = true;
				_lock_id = msg.source.id;
				_lock_time_out = _active_ready_time;
			}

			int active_mount_point = _active_point[emptypos];
			struct
			{
				int pos;
				int point;
				int ready_time;
				int run_time;
				int end_time;
				int path_id;
				int active_mode;
			} data;
			data.pos = emptypos;
			data.point = active_mount_point;
			data.ready_time = _active_ready_time;
			data.run_time = _active_running_time;
			data.end_time = _active_ending_time;
			data.path_id = _path_id;
			data.active_mode = _active_mode;
			SendTo<0>(GM_MSG_MOB_ACTIVE_STATE_START,msg.source,0,&data,sizeof(data));
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_STATE_CANCEL:
		{
			if (_player_map.find(msg.source.id) == _player_map.end()) return 0;
			delplayer(msg.source.id);
			if (msg.source.id == _lock_id)
			{
				_lock = false;
				_lock_id = 0;
				_lock_time_out = 0;
			}

			if (_player_map.size() == 0 && _mob_active_move_status == MOBACTIVE_MOVE)
			{
				 MobActiveStopMove();
				 _mob_active_move_status = MOBACTIVE_STOP_MOVE;
			}
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_STATE_FINISH:
		{
			if (_player_map.find(msg.source.id) == _player_map.end()) return 0;
			delplayer(msg.source.id);
			if (msg.source.id == _lock_id)
			{
				_lock = false;
				_lock_id = 0;
				_lock_time_out = 0;
			}

			if (_end_new_mob_active_id)
			{
				if (_mob_active_move_status == MOBACTIVE_MOVE)
				{
					MobActiveStopMove();
					_mob_active_move_status = MOBACTIVE_STOP_MOVE;
				}
				_runner->disappear(true);
				BecomeNewMobActive(_end_new_mob_active_id);
				_runner->enter_world();
			}
			else if (_mob_active_move_status == MOBACTIVE_STOP_MOVE)
			{
				if (_player_map.size())
				{
					MobActiveStartMove();
					_mob_active_move_status = MOBACTIVE_MOVE;
				}
			}
			else if (_active_times_max && _active_counter >= _active_times_max && !_end_new_mob_active_id)
			{
				SendTo<0>(GM_MSG_NPC_DISAPPEAR, _parent->ID,0);
			}
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_COUNTER_SUC:
		{
			_active_counter++;
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_SYNC_POS:
		{
			if (_player_map.find(msg.source.id) == _player_map.end()) return 0;
			A3DVECTOR pos = msg.pos;
			pos -= _parent->pos;
			gnpc_imp::StepMove(pos);
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_TELL_POS:
		{
			if (_player_map.find(msg.source.id) == _player_map.end()) return 0;
			_player_pos_map[msg.source.id] = msg.pos;
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_START_MOVE:
		{
			if (_mob_active_move_status == MOBACTIVE_STOP_MOVE)
			{
				MobActiveStartMove();
				_mob_active_move_status = MOBACTIVE_MOVE;
			}
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_STOP_MOVE:
		{
			if (_mob_active_move_status == MOBACTIVE_MOVE)
			{
				MobActiveStopMove();
				_mob_active_move_status = MOBACTIVE_STOP_MOVE;
			}
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_RECLAIM:
		{
			
			if (_mob_active_can_reclaim)
			{
				_mob_active_can_reclaim = false;
				CreateMobActive();
			}
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_PATH_END:
		{
			for (std::map<int,int>::iterator it = _player_map.begin(); it != _player_map.end(); ++it)
			{
				SendTo<0>(GM_MSG_MOB_ACTIVE_PATH_END,XID(GM_TYPE_PLAYER,it->first),0);
			}

		}

	default:
		return gnpc_imp::MessageHandler(msg);
	}
	return 0;
}

void mob_active_imp::OnHeartbeat(size_t tick)
{
	//检测和互动物品交互的玩家是否掉线
	if (_lock_time_out > 0)
	{
		_lock_time_out--;
		if (_lock_time_out == 0)
		{
			_lock = false;
			_lock_id = 0;
		}
	}
	
	world::object_info info;
	for (std::map<int,int>::iterator it = _player_map.begin(); it != _player_map.end();)
	{
		bool rst = _plane->QueryObject(XID(GM_TYPE_PLAYER,it->first),info);
		if (!rst || (info.state & world::QUERY_OBJECT_STATE_ZOMBIE)
				||(info.pos.squared_distance(_parent->pos)) >= GATHER_RANGE_LIMIT * GATHER_RANGE_LIMIT)
		{
			//玩家异常下线
			//通知玩家解除锁定
			SendTo<0>(GM_MSG_MOB_ACTIVE_STATE_CANCEL, XID(GM_TYPE_PLAYER,it->first),0);
			A3DVECTOR playerpos = _player_pos_map[it->first];
			if (rst) playerpos = info.pos;
			_runner->player_mobactive_state_cancel(XID(GM_TYPE_PLAYER,it->first),_parent->ID,it->second,playerpos,_parent->pos);
			if (it->first == _lock_id)
			{
				_lock = 0;
				_lock_id = 0;
				_lock_time_out = 0;
			}
			_pos[_player_map[it->first]] = false;
			_player_map.erase(it++);
			_player_pos_map.erase(it->first);
			continue;
		}
		++it;
	}

	if (_player_map.size() == 0)
	{
		if (_active_times_max && _active_counter >= _active_times_max && !_end_new_mob_active_id)
		{
			SendTo<0>(GM_MSG_NPC_DISAPPEAR, _parent->ID,0);
		}
		else if (_mob_active_move_status == MOBACTIVE_MOVE)
		{
			_mob_active_move_status = MOBACTIVE_STOP_MOVE;
		}
	}

	gnpc_imp::OnHeartbeat(tick);
}

bool mob_active_imp::StepMove(const A3DVECTOR& offset)
{
	//移动时需要通知互动的玩家
	bool bRst = gnpc_imp::StepMove(offset);
	if (bRst && _active_mode == MOBACTIVE_ACTIVE)
	{
		for (std::map<int,int>::iterator it = _player_map.begin();it != _player_map.end(); ++it)
		{
			SendTo<0>(GM_MSG_MOB_ACTIVE_SYNC_POS,XID(GM_TYPE_PLAYER,it->first),0);
		}
	}
	return bRst;
}

bool mob_active_imp::BecomeNewMobActive(int npc_tid)
{

	_runner->disappear(true);

	npc_template * pTemplate = npc_stubs_manager::Get(npc_tid);
	ASSERT(pTemplate);
	if(!pTemplate) return false; 
	if(!pTemplate->mob_active_data) return false;

	int ai_policy_cid = CLS_NPC_AI_POLICY_BASE;
	int aggro_policy = 0;
	SetParam(pTemplate->mob_active_data);

	gnpc* pNPC = GetParent();
	pNPC->tid = npc_tid;
	pNPC->vis_tid = npc_tid;
	pNPC->monster_faction = pTemplate->monster_faction;
	pNPC->msg_mask = gobject::MSG_MASK_ATTACK;
	pNPC->cruise_timer = abase::Rand(0,31);
	_basic = pTemplate->bp;
	_base_prop = pTemplate->ep;
	_base_prop.attack_range += pTemplate->body_size;
	if (pTemplate->mob_active_data->speed > 1e-3)
	{
		_base_prop.walk_speed = pTemplate->mob_active_data->speed;
		_base_prop.run_speed = pTemplate->mob_active_data->speed;
	}
	_cur_item=  pTemplate->ip;
	_en_point.spec_damage = pTemplate->spec_damage;
	_en_point.dmg_reduce = pTemplate->dmg_reduce;
	_base_prop.anti_invisible_rate = pTemplate->bp.level;
	pNPC->anti_invisible_rate = _base_prop.anti_invisible_rate;

	_is_fly = pTemplate->isfly;
	_faction = pTemplate->faction;
	_enemy_faction = pTemplate->enemy_faction;
	_after_death = pTemplate->after_death;
	_immune_state = pTemplate->immune_type;
	_inhabit_mode = 0;
	_drop_no_protected = pTemplate->drop_no_protected;
	_is_boss = pTemplate->is_boss;
	_fix_drop_rate = pTemplate->is_skill_drop_adjust ? 0:1;
	_no_exp_punish = pTemplate->no_exp_punish;
	_player_cannot_attack = pTemplate->player_cannot_attack;
	_init_path_id = 0; 
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTemplate->body_size;
	pNPC->anti_invisible_rate = _base_prop.anti_invisible_rate;
	_share_npc_kill = pTemplate->task_share;		
	_share_drop = pTemplate->item_drop_share;			
	_buffarea_tid = pTemplate->buff_area_id;
	aggro_param aggp;
	ai_param aip;
	memset(&aggp,0,sizeof(aggp));
	memset(&aip,0,sizeof(aip));
	if(ai_policy_cid)
	{
		aip.trigger_policy = pTemplate->trigger_policy;
		aggp.aggro_policy = aggro_policy;	

		aggp.aggro_range = pTemplate->aggro_range;
		aggp.aggro_time = pTemplate->aggro_time; 
		aggp.sight_range = pTemplate->sight_range;
		aggp.enemy_faction = _enemy_faction;
		aggp.faction = _faction;
		aggp.faction_ask_help = pTemplate->monster_faction_ask_help;
		aggp.faction_accept_help = pTemplate->monster_faction_can_help;

		if(aggp.faction_accept_help)
		{
			pNPC->msg_mask |= gobject::MSG_MASK_CRY_FOR_HELP;
		}

		aip.policy_class = ai_policy_cid;

		aip.patrol_mode = pTemplate->patrol_mode;
		aip.primary_strategy = pTemplate->id_strategy; 

		//验证和复制技能
		ASSERT(sizeof(aip.skills) >= sizeof(pTemplate->skills));
		memcpy(&aip.skills,&pTemplate->skills,sizeof(pTemplate->skills));

		aip.path_id = 0;
		aip.path_type = _init_path_type;
		aip.script_data = NULL; //-_-!
		aip.script_size = 0;

	}

	if(ai_policy_cid)
	{
		gnpc_controller * pCtrl = (gnpc_controller*)_commander;
		pCtrl->ReleaseAI();
		pCtrl->CreateAI(aggp,aip);
		//创建AI之后才能设定寿命
	}
	_cur_prop = _base_prop;
	_runner->enter_world();

	return true;
}

void mob_active_imp::MobActiveStartMove()
{
	//SetMovePathID(GetParent()->tid,1);
	
}

void mob_active_imp::MobActiveStopMove()
{
	if (_cur_session && _cur_session->GetGUID() == CLS_SESSION_NPC_PATROL)
	{
		EndCurSession();
	}
}

void mob_active_imp::CreateMobActive()
{
	gnpc * origin = GetParent();
	int spawner_index = origin->spawn_index;
	A3DVECTOR pos = _birth_place;
	char dir = _birth_dir;
	npc_spawner::entry_t ent;
	ent.npc_tid = origin->tid;
	ent.path_type = _init_path_type;
	const int cid[3] = {CLS_MOBACTIVE_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	int ai_policy_cid = CLS_NPC_AI_POLICY_BASE;

	gnpc *pNPC = npc_spawner::CreateMobActiveBase(_spawner,_plane,ent,spawner_index,pos,cid,dir,ai_policy_cid,0,NULL,_mob_active_life);
	if (pNPC)
	{
		_plane->InsertNPC(pNPC);
		pNPC->imp->_runner->enter_world();
		pNPC->imp->OnCreate();
		pNPC->Unlock();
	}
}




/* --------------------------------policy mob_active---------------------- */

void mob_active_policy::Init(const ai_object & self, const ai_param & aip)
{
	ai_policy::Init(self,aip);

	if (_mob_active_path_agent)
	{
		delete _mob_active_path_agent;
		_mob_active_path_agent = NULL;
	}

	mob_active_imp *pImp = (mob_active_imp *)_self->GetImpl();
	if (pImp->_path_id)
	{
		_mob_active_path_agent = new base_patrol_agent();
		if (!_mob_active_path_agent->Init(pImp->_path_id,_self->GetWorldManager(),pImp->_init_path_type))
		{
			delete _mob_active_path_agent;
			_mob_active_path_agent = NULL;
		}
	}
}

int mob_active_policy::GetPathID()
{
	if (_mob_active_path_agent) return -1;
	return _mob_active_path_agent->GetPathID();
}

bool mob_active_policy::GetNextWaypoint(A3DVECTOR & pos)
{
	if(!_mob_active_path_agent) return false;
	bool first_end = false;
	_mob_active_path_agent->GetNextWayPoint(pos,first_end);
	if(first_end) return false;
	return true;
}

void mob_active_policy::OnHeartbeat()
{
	ai_policy::OnHeartbeat();

	if (!InCombat())
	{
		if(!_cur_task && _mob_active_path_agent)
		{
			mob_active_imp *pImp = (mob_active_imp *)_self->GetImpl();
			if (pImp->_mob_active_move_status == mob_active_imp::MOBACTIVE_MOVE)
			{
				A3DVECTOR pos;
				bool first_end = true;
				if (_mob_active_path_agent->GetNextWayPoint(pos,first_end))
				{
					if (first_end)
					{
						if (pImp->_path_id && !pImp->_init_path_type && pImp->_path_end_finish_active)
						{
							XID id;
							_self->GetID(id);
							_self->SendMessage(id,GM_MSG_MOB_ACTIVE_PATH_END);
						}
					}
					else
					{
						A3DVECTOR selfpos;
						_self->GetPos(selfpos);
						if (pos.squared_distance(selfpos) > 1e-3)
						{
							AddPosTask<ai_mob_active_patrol_task>(pos);
						}
					}
				}
			}
		}
		else
		{
		}
	}

}
