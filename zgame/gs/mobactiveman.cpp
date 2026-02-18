#include "mobactiveman.h"
#include "task/taskman.h"
#include "player_imp.h"
#include "mobactivenpc.h"
#include "mobactivedataman.h"
#include "world.h"

void mobactive_manager::SendErrorMessage(gplayer_imp *pImp, int message)
{
	pImp->_runner->error_message(message);
}

bool mobactive_manager::CheckPlayerMobActiveRequest(gplayer_imp *pImp)
{
	if(!pImp->CheckPlayerMobActiveRequest())
	{
		SendErrorMessage(pImp,S2C::ERR_MOB_ACTIVE_INVALID);
		return false;
	}
	
	return true;
}


void mobactive_manager::PlayerMobActive(gplayer_imp *pImp, const C2S::CMD::mobactive_start & ma)
{
	if (_mobactive_bind_target != XID(-1,-1) || !CheckPlayerMobActiveRequest(pImp)) 
	{
		return;
	}
	struct
	{
		int tool;
		int task_id;
		int level;
		int gender;
		int cls;
	} data;

	data.tool = ma.tool_type;
	data.task_id = ma.task_id;
	data.level = pImp->_basic.level;
	data.gender = pImp->GetParent()->base_info.gender;
	data.cls= pImp->_basic.level;

	pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_START,XID(GM_TYPE_NPC,ma.npc_id),0,&data,sizeof(data));
}

void mobactive_manager::SetPlayerMobActiveLeavePos(gplayer_imp* pImp)
{
	if (_mobactive_state == -1) return;
	const mob_active_mount_point_template *mampt = mob_active_mount_point_dataman::Get(_active_mount_point);
	switch (_mobactive_state)
	{
		case MOB_ACTIVE_STATE_START:
		{
			pImp->GetParent()->pos = pImp->GetParent()->before_mobactive_pos;
		}
		break;

		case MOB_ACTIVE_STATE_RUN:
		case MOB_ACTIVE_STATE_ENDING:
		{
			if (_active_mode == PLAYER_ACTIVE)
			{
				if (mampt->goal_pos[1].squared_magnitude() > 1e-3)
				{
					pImp->GetParent()->pos += mampt->goal_pos[1];
				}
			}
			else
			{
				if (_path_id > 0)
				{
					if (mampt->goal_pos[1].squared_magnitude() > 1e-3)
					{
						pImp->GetParent()->pos += mampt->goal_pos[1];
					}
				}
				else
				{
					pImp->GetParent()->pos = pImp->GetParent()->before_mobactive_pos;
				}
			}
		}
		break;

		case MOB_ACTIVE_STATE_FINISH:
		{
			if (_active_mode == MONSTER_ACTIVE)
			{
				if (mampt->goal_pos[0].squared_magnitude()> 1e-3)
				{
					pImp->GetParent()->pos = mampt->goal_pos[0];
				}
				else if (mampt->goal_pos[1].squared_magnitude() > 1e-3)
				{
					pImp->GetParent()->pos += mampt->goal_pos[1];
				}
				else
				{
					pImp->GetParent()->pos = pImp->GetParent()->before_mobactive_pos;
				}
			}
		}
		break;
	}
}

void mobactive_manager::PlayerMobActiveCancel(gplayer_imp *pImp)
{
	if (_mobactive_bind_target == XID(-1,-1)) return;
	pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_STATE_CANCEL,_mobactive_bind_target,0);
	SetPlayerMobActiveLeavePos(pImp);
	MsgMobActiveStateCancel(pImp);
	AllowCmd(pImp);
	pImp->GetParent()->mobactive_id = 0;
	Reset();
}

void mobactive_manager::PlayerMobActiveFinish(gplayer_imp *pImp)
{
	if (_mobactive_bind_target == XID(-1,-1)) return;
	SetPlayerMobActiveLeavePos(pImp);
	gplayer* pPlayer = pImp->GetParent();
	if (_mobactive_state == MOB_ACTIVE_STATE_START)
	{
		MobActiveStateRunCtrl(pImp);
		DeliverMobActiveTask(pImp,MOB_ACTIVE_STATE_RUN);
		SetMobActiveState(MOB_ACTIVE_STATE_ENDING);
		pPlayer->mobactive_state = MOB_ACTIVE_STATE_ENDING;
		MsgMobActiveStateEnding(pImp);
		MobActiveStateEndingCtrl(pImp);
		DeliverMobActiveTask(pImp, MOB_ACTIVE_STATE_ENDING);
	}
	else if (_mobactive_state == MOB_ACTIVE_STATE_RUN)
	{
		SetMobActiveState(MOB_ACTIVE_STATE_ENDING);
		pPlayer->mobactive_state = MOB_ACTIVE_STATE_ENDING;
		MsgMobActiveStateEnding(pImp);
		MobActiveStateEndingCtrl(pImp);
		DeliverMobActiveTask(pImp, MOB_ACTIVE_STATE_ENDING);
	}
}

void mobactive_manager::SetBindTarget(gplayer_imp * pImp, const XID & target)
{
	pImp->GetParent()->mobactive_id = target.id;
	_mobactive_bind_target = target;
}

void mobactive_manager::PlayerBeAttacked(gplayer_imp *pImp)
{
	if (_interrupt_condition < 0) return;
	if (_interrupt_condition & MOB_ACTIVE_INTERRUPT_BY_ATTACK)
	{
		PlayerMobActiveCancel(pImp);
	}
}

void mobactive_manager::PlayerBeMoved(gplayer_imp *pImp)
{
	if (_interrupt_condition < 0) return;
	if (_interrupt_condition & MOB_ACTIVE_INTERRUPT_BY_MOVE)
	{
		PlayerMobActiveCancel(pImp);
	}
}

void mobactive_manager::Heartbeat(gplayer_imp *pImp)
{
	if (_mobactive_bind_target == XID(-1,-1))
	{
		return;
	}
	world::object_info info;
	bool rst = pImp->_plane->QueryObject(_mobactive_bind_target,info);
	if (!rst || (info.state & world::QUERY_OBJECT_STATE_ZOMBIE)
			||(info.pos.squared_distance(pImp->_parent->pos)) >= GATHER_RANGE_LIMIT * GATHER_RANGE_LIMIT)
	{
		//怪物不在地图上,客户端应该早知道,不用发消息告诉客户端
		AllowCmd(pImp);
		pImp->GetParent()->mobactive_id = 0;
		Reset();
		return;
	}
	pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_TELL_POS,_mobactive_bind_target,0);
	if (_time_out >= 0) //如果_active_time[MOB_ACTIVE_STATE_START] = 0,time_out = 0，直接进入过程
	{
		gplayer* pPlayer = pImp->GetParent();
		_time_out--;
		if (_time_out <= 0)
		{
			switch(_mobactive_state)
			{
				case MOB_ACTIVE_STATE_START:
					SetMobActiveState(MOB_ACTIVE_STATE_RUN);
					pPlayer->mobactive_state = MOB_ACTIVE_STATE_RUN;
					MsgMobActiveStateRun(pImp);
					MobActiveStateRunCtrl(pImp);
					DeliverMobActiveTask(pImp,MOB_ACTIVE_STATE_RUN);
					pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_COUNTER_SUC,_mobactive_bind_target,0);
					break;
				case MOB_ACTIVE_STATE_RUN:
					SetMobActiveState(MOB_ACTIVE_STATE_ENDING);
					pPlayer->mobactive_state = MOB_ACTIVE_STATE_ENDING;
					MsgMobActiveStateEnding(pImp);
					MobActiveStateEndingCtrl(pImp);
					DeliverMobActiveTask(pImp, MOB_ACTIVE_STATE_ENDING);
					break;
				case MOB_ACTIVE_STATE_ENDING:
					const mob_active_mount_point_template *mampt = mob_active_mount_point_dataman::Get(_active_mount_point);
					SetMobActiveState(MOB_ACTIVE_STATE_FINISH);
					SetPlayerMobActiveLeavePos(pImp);
					int item_id[5] = {0};
					for (int i = 0; i < 5; i++)
					{
						float op = abase::RandUniform();
						if (op < mampt->drop_table[i].prop) item_id[i] = mampt->drop_table[i].id_obj;
					}
					pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_STATE_FINISH,pImp->GetParent()->ID,0,item_id,sizeof(item_id));	
					pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_STATE_FINISH,_mobactive_bind_target,0);	

					break;
			}
		}
	}
}

void mobactive_manager::MsgMobActiveStateStart(gplayer_imp *pImp)
{
	pImp->_runner->player_mobactive_state_start(pImp->GetParent()->ID,_mobactive_bind_target,_active_pos);
}

void mobactive_manager::MsgMobActiveStateRun(gplayer_imp *pImp)
{
	pImp->_runner->player_mobactive_state_run(pImp->GetParent()->ID,_mobactive_bind_target,_active_pos);
	if (_path_id > 0 && _active_time[MOB_ACTIVE_STATE_START] > 0)
	{
		//让互动物品动起来
		pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_START_MOVE,_mobactive_bind_target,0);
		pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_RECLAIM,_mobactive_bind_target,0);
	}
}

void mobactive_manager::MsgMobActiveStateEnding(gplayer_imp *pImp)
{
	pImp->_runner->player_mobactive_state_ending(pImp->GetParent()->ID,_mobactive_bind_target,_active_pos);
	if (_path_id > 0 && _active_time[MOB_ACTIVE_STATE_ENDING] > 0)
	{
		//让互动物品停下来
		pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_STOP_MOVE,_mobactive_bind_target,0);
	}
}

void mobactive_manager::MsgMobActiveStateFinish(gplayer_imp *pImp)
{
	world::object_info info;
	bool rst = pImp->_plane->QueryObject(_mobactive_bind_target,info);
	if (rst)
	{
		pImp->_runner->player_mobactive_state_finish(pImp->GetParent()->ID,_mobactive_bind_target,_active_pos, pImp->GetParent()->pos,info.pos);
	}
}

void mobactive_manager::MsgMobActiveStateCancel(gplayer_imp *pImp)
{
	world::object_info info;
	bool rst = pImp->_plane->QueryObject(_mobactive_bind_target,info);
	if (rst)
	{
		pImp->_runner->player_mobactive_state_cancel(pImp->GetParent()->ID,_mobactive_bind_target,_active_pos,pImp->GetParent()->pos,info.pos);
	}
}

void mobactive_manager::MobActiveStateStartCtrl(gplayer_imp *pImp)
{
	if (_active_mount_point > 0)
	{
		const mob_active_mount_point_template *mampt = mob_active_mount_point_dataman::Get(_active_mount_point);
		//打开控制器
		if (mampt->activate_ready_controller_id)
		{
			world_manager::ActiveSpawn(pImp->GetWorldManager(),mampt->activate_ready_controller_id ,true);
		}
		if (mampt->deactivate_ready_controller_id)
		{
			world_manager::ActiveSpawn(pImp->GetWorldManager(),mampt->deactivate_ready_controller_id,true);
		}
	}
}

void mobactive_manager::MobActiveStateRunCtrl(gplayer_imp *pImp)
{
}

void mobactive_manager::MobActiveStateEndingCtrl(gplayer_imp *pImp)
{
	if (_active_mount_point > 0)
	{
		const mob_active_mount_point_template *mampt = mob_active_mount_point_dataman::Get(_active_mount_point);
		//打开控制器
		if (mampt->activate_end_controller_id)
		{
			world_manager::ActiveSpawn(pImp->GetWorldManager(),mampt->activate_end_controller_id ,true);
		}
		if (mampt->deactivate_end_controller_id)
		{
			world_manager::ActiveSpawn(pImp->GetWorldManager(),mampt->deactivate_end_controller_id,true);
		}
	}
}
void mobactive_manager::SetMobActiveInterruptCondition()
{
	if (_interrupt_condition == -1)
	{
		const mob_active_mount_point_template *mampt = mob_active_mount_point_dataman::Get(_active_mount_point);
		_interrupt_condition = mampt->interrupt_condition;
	}
}

void mobactive_manager::DeliverMobActiveTask(gplayer_imp *pImp, int state)
{
	if (_mobactive_state == MOB_ACTIVE_STATE_START || _mobactive_state == MOB_ACTIVE_STATE_ENDING)
	{
		int task_id = 0;
		const mob_active_mount_point_template *mampt = mob_active_mount_point_dataman::Get(_active_mount_point);
		if (_mobactive_state == MOB_ACTIVE_STATE_START)
		{
			task_id = mampt->ready_task_out;
		}
		else
		{
			task_id = mampt->end_task_out;
		}
		if (task_id)
		{
			PlayerTaskInterface task_if(pImp);
			if (OnTaskCheckDeliver(&task_if,task_id,0))
			{
				__PRINTF("玩家%d在互动阶段%d接到任务( %d )了...........\n",pImp->GetParent()->ID.id,_mobactive_state,task_id);
			}
		}
	}
}

void mobactive_manager::MobActiveSyncPos(gplayer_imp* pImp)
{
	if (_active_mode == PLAYER_ACTIVE)
	{
		pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_SYNC_POS,_mobactive_bind_target,0);
	}
}

void mobactive_manager::DenyCmd(gplayer_imp* pImp)
{
	pImp->GetParent()->SetExtraState(gplayer::STATE_MOBACTIVE);
	pImp->_commander->DenyCmd(controller::CMD_FLY_POS);
	pImp->_commander->DenyCmd(controller::CMD_SKILL);
	pImp->_commander->DenyCmd(controller::CMD_MARKET);
	pImp->_commander->DenyCmd(controller::CMD_FLY);
	pImp->_commander->DenyCmd(controller::CMD_BOT);
	pImp->_commander->DenyCmd(controller::CMD_BIND);
	pImp->_commander->DenyCmd(controller::CMD_ACTIVE_EMOTE);
}

void mobactive_manager::AllowCmd(gplayer_imp* pImp)
{
	pImp->GetParent()->ClrExtraState(gplayer::STATE_MOBACTIVE);
	pImp->_commander->AllowCmd(controller::CMD_FLY_POS);
	pImp->_commander->AllowCmd(controller::CMD_SKILL);
	pImp->_commander->AllowCmd(controller::CMD_MARKET);
	pImp->_commander->AllowCmd(controller::CMD_FLY);
	pImp->_commander->AllowCmd(controller::CMD_BOT);
	pImp->_commander->AllowCmd(controller::CMD_BIND);
	pImp->_commander->AllowCmd(controller::CMD_ACTIVE_EMOTE);
}
