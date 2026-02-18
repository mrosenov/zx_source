#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include <arandomgen.h>
#include "npc.h"
#include "world.h"
#include "usermsg.h"
#include "clstab.h"
#include "ainpc.h"
#include "actsession.h"
#include "npcgenerator.h"
#include "item.h"
#include "playertemplate.h"
#include "npc_filter.h"
#include "pathfinding/pathfinding.h"
#include "guardnpc.h"
#include "petnpc.h"
#include "player_imp.h"
#include "petdataman.h"
#include "pet_filter.h"
#include "sfilterdef.h"
#include "cooldowncfg.h"
#include "item/item_petbedge.h"
#include "skillwrapper.h"

DEFINE_SUBSTANCE(gpet_imp,gnpc_imp,CLS_PET_IMP)
DEFINE_SUBSTANCE(gpet_dispatcher,gnpc_dispatcher,CLS_PET_DISPATCHER)
DEFINE_SUBSTANCE(gpet_policy,ai_policy,CLS_NPC_AI_POLICY_PET)

gpet_imp::gpet_imp()
{
	_pet_index = 0;
	_pet_stamp = -1;
	_attack_judge= NULL;
	_enchant_judge= NULL;
	_attack_fill = NULL;
	_enchant_fill = NULL;
	memset(&_leader_data, 0, sizeof(_leader_data));
	_leader_force_attack = 0;
	_aggro_state = PET_AGGRO_DEFENSE;
	_stay_state = PET_MOVE_FOLLOW;
	_notify_master_counter = 0;
	_hp_notified= -1;
	_vp_notified= -1;
	_pet_tid = 0;
	_honor_level = 0;
	_hunger_level = 0;
	_damage_reduce = 0;
	_damage_reduce_percent = 0.f;
	_master_attack_target = 0;
	_old_combat_state = -1;
	_reborn_cnt = 0;
	ClearSkill();
}

void 
gpet_imp::Init(world * pPlane,gobject*parent)
{
	_cur_item.attack_cycle = SECOND_TO_TICK(2.0f);
	_cur_item.attack_point = SECOND_TO_TICK(1.0f);
	gnpc_imp::Init(pPlane,parent);
}

bool gpet_imp::CheckCoolDown( int cooldown_index )
{
	return _cooldown.TestCoolDown( cooldown_index );
}

void gpet_imp::SetCoolDown(int cooldown_index,int cooldown_time)
{
	_cooldown.SetCoolDown(cooldown_index,cooldown_time);
	msg_pet_skill_cooldown_t temp;
	temp.cooldown_index = cooldown_index;
	temp.cooldown_time = cooldown_time;
	SendTo2<0>(GM_MSG_PET_SET_COOLDOWN,_leader_id,_pet_index,_pet_stamp,&temp,sizeof(temp));
}

void gpet_imp::ClrCoolDown(int cooldown_index)
{
	SetCoolDown(cooldown_index,1);
}

void gpet_imp::NotifySkillStillCoolDown()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	CMD::Make<CMD::error_msg>::From(h1,S2C::ERR_PET_SKILL_IN_COOLDOWN);
	send_ls_msg(_leader_data.cs_index,_leader_id.id,_leader_data.cs_sid,h1);
}

void gpet_imp::RelocatePos(bool is_disappear)
{
	if(_leader_id.IsValid())
	{
		int dis = is_disappear?1:0;
		SendTo2<0>(GM_MSG_PET_RELOCATE_POS,_leader_id,_pet_index,_pet_stamp,&dis,sizeof(dis));
	}
}

void gpet_imp::SetHonorLevel(int honor_level)
{
	if(honor_level < pet_data::HONOR_LEVEL_0 || honor_level >= pet_data::HONOR_LEVEL_COUNT)
	{
		_honor_level = pet_data::HONOR_LEVEL_0;
	}
	_honor_level = honor_level;
}

void gpet_imp::SetHungerLevel(int hunger_level)
{
	if(hunger_level < pet_data::HUNGER_LEVEL_0 || hunger_level >= pet_data::HUNGER_LEVEL_COUNT)
	{
		_hunger_level = pet_data::HUNGER_LEVEL_0;
	}
	_hunger_level = hunger_level;
}

void 
gpet_imp::SetAttackHook(attack_judge judge1,enchant_judge judge2,attack_fill fill1, enchant_fill fill2)
{
	_attack_judge = judge1;
	_enchant_judge = judge2;
	_attack_fill = fill1;
	_enchant_fill = fill2;
}

int 
gpet_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_PET_CTRL_CMD:
		if(msg.source == _leader_id)
		{
			if(msg.content_length > sizeof(short))
			{
				DispatchPlayerCommand(msg.param,msg.content,msg.content_length);
			}
			else
			{
				ASSERT(false);
			}
		}
		return 0;

		case GM_MSG_PET_CHANGE_POS:
		{
			if(msg.source == _leader_id) 
			{
				A3DVECTOR pos = *(A3DVECTOR*)msg.content;
				_runner->stop_move(pos,0x500,1,C2S::MOVE_MODE_RETURN);
				pos -= _parent->pos;
				StepMove(pos);
			}
		}
		return 0;

		case GM_MSG_PET_DISAPPEAR:
		{ 
			if( msg.source == _leader_id || msg.source == _parent->ID )
			{
				_runner->stop_move(_parent->pos,0x500,1,C2S::MOVE_MODE_RETURN);
				_runner->disappear();
				_commander->Release();
			}
		}
		return 0;

		case GM_MSG_ATTACK:
		{
			ASSERT(msg.content_length >= sizeof(attack_msg));

			attack_msg ack_msg = *(attack_msg*)msg.content;
			//处理一下到来的攻击消息
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);

			//进行决斗的检测
			if(_leader_data.duel_target && _leader_data.duel_target == ack_msg.ainfo.attacker.id)
			{
				//如何处理宠物？
				ack_msg.attacker_mode |= attack_msg::PVP_DUEL;
			}

			//宠物永远不会受到来自主人的伤害
			if(msg.source == _leader_id || ack_msg.ainfo.attacker == _leader_id) return 0;

			if(_attack_judge&& !gmatrix::IsBattleFieldServer() && !GetWorldManager()->IsBattleWorld())
			{
				//进行特殊的检测
				if(!(*_attack_judge)(this,msg,ack_msg)) return 0;
			}
			else
			{
				//使用通用怪物方式的检测
				if(!ack_msg.force_attack && !(GetFaction() & ack_msg.target_faction))
				{
					return 0;
				}
			}

			if(msg.source.IsPlayerClass() && ack_msg.is_flying) return 0;

			HandleAttackMsg(msg,&ack_msg);
		}
		return 0;
		
		case GM_MSG_ENCHANT:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
			if(!ech_msg.helpful)
			{
				//宠物永远不会受到来自主人的伤害
				if(msg.source == _leader_id || ech_msg.ainfo.attacker == _leader_id) return 0;
				
				//进行决斗的检测
				if(_leader_data.duel_target && _leader_data.duel_target == ech_msg.ainfo.attacker.id)
				{
					//如何处理宠物？
					ech_msg.attacker_mode |= attack_msg::PVP_DUEL;
				}
			}
			else
			{
				//进行决斗的检测
				if(_leader_data.duel_target && _leader_data.duel_target == ech_msg.ainfo.attacker.id)
				{
					//如何处理宠物？
					ech_msg.target_faction = 0xFFFFFFFF;
				}
			}

			if(_enchant_judge&& !gmatrix::IsBattleFieldServer() && !GetWorldManager()->IsBattleWorld()) 
			{
				//进行特殊的检测
				if(!(*_enchant_judge)(this,msg,ech_msg)) return 0;
			}
			else
			{
				//使用通用怪物方式的检测
				if(!ech_msg.helpful)
				{
					if(!ech_msg.force_attack && !(GetFaction() & ech_msg.target_faction))
					{
						return 0;
					}
				}
				else
				{
					if(!(GetFaction() & ech_msg.attacker_faction))
					{
						return 0;
					}
				}
			}
			if(msg.source.IsPlayerClass() && ech_msg.is_flying) return 0;
			HandleEnchantMsg(msg,&ech_msg);
		}
		return 0;

		case GM_MSG_DUEL_HURT:
		{
			if(!_parent->IsZombie()) 
			{
				if(_leader_data.duel_target && _leader_data.duel_target == msg.source.id)
				{
					DoDamage(msg.param);
					if(_basic.hp == 0)
					{
						Die(msg.source,false,0);
					}
				}
			}
		}
		return 0;
		
		case GM_MSG_MASTER_INFO:
		{
			if(msg.content_length == sizeof(leader_prop) && msg.source == _leader_id)
			{
				_leader_data = *(leader_prop*)msg.content;
				_pk_level = _leader_data.pk_level;
			}
			else
			{
				GLog::log(GLOG_INFO,"宠物可能收到了不是自己主人的消息, source_id=%d, leader_id=%d", msg.source.id, _leader_id.id);
			}
		}
		return 0;

		case GM_MSG_PET_INFO_CHANGED:
		{
			if(sizeof(msg_pet_info_changed_t) == msg.content_length && msg.source == _leader_id)
			{
				msg_pet_info_changed_t& info = *(msg_pet_info_changed_t*)msg.content;
				_base_prop = info.base_prop;
				_damage_reduce = info.damage_reduce;
				_damage_reduce_percent = info.damage_reduce_percent;

				//加入装备效果值
				SetHonorLevel(info.honor_level);
				SetHungerLevel(info.hunger_level);
				property_policy::UpdatePet(this);
				property_policy::UpdateLife(this);
				property_policy::UpdateMana(this);
				if(_basic.level != info.level)
				{
					_basic.level = info.level;
					_basic.hp = _cur_prop.max_hp;
					_runner->level_up();
				}
				SetRefreshState();
				NotifySelfDataToMaster();
			}
		}
		return 0;

		case GM_MSG_FEED_PET:
		{
			if( 0 == msg.content_length && msg.source == _leader_id )
			{
				_basic.hp += msg.param;
				if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
				_basic.mp += msg.param2;
				if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;
				SetRefreshState();
				NotifySelfDataToMaster();
			}
		}
		return 0;

		case GM_MSG_PET_HONOR_LEVEL_CHANGED:
		{
			if(0 == msg.content_length && msg.source == _leader_id)
			{
				SetHonorLevel(msg.param);
			}
		}
		return 0;

		case GM_MSG_PET_HUNGER_LEVEL_CHANGED:
		{
			if(0 == msg.content_length && msg.source == _leader_id)
			{
				SetHungerLevel(msg.param);
			}
		}
		return 0;

		case GM_MSG_MASTER_ASK_HELP:
		{
			if(msg.content_length == sizeof(XID))
			{
				XID who = *(XID*)msg.content;
				if(who.IsValid() && msg.source == _leader_id)
				{
					if(_aggro_state == PET_AGGRO_DEFENSE)
					{
						//是防御姿态
						gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
						aggro_policy * pAggro = pAI->GetAggroCtrl();
						ai_policy * pPolicy = pAI->GetAICtrl();
						if(pAggro->AddAggro(who,2,2) == 0)
						{
							pPolicy->OnAggro();
						}
					}
				}
			}
		}
		return 0;

		case GM_MSG_MASTER_START_ATTACK:
		{
			if( msg.content_length == sizeof(XID) )
			{
				if( PET_AGGRO_AUTO == _aggro_state )
				{
					gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
					aggro_policy * pAggro = pAI->GetAggroCtrl();
					if( pAggro->Size() == 0 )
					{
						XID target = *(XID*)msg.content;
						if(target.IsActive())
						{
							char force_attack = msg.param & 0xFF;
							pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
							pAI->AddAggro(target, _cur_prop.max_hp + 10);
							pAggro->SetAggroState(aggro_policy::STATE_FREEZE);
							_leader_force_attack = force_attack;
							_master_attack_target = target.id;
						}
					}
				}
			}
		}
		return 0;

		case GM_MSG_MASTER_DUEL_STOP:
		{
			//决斗结束清除仇恨
			gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
			aggro_policy * pAggro = pAI->GetAggroCtrl();

			pAggro->Clear();

			//清除debuff防止决斗结束导致红名
			_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

		}
		return 0;

		case GM_MSG_GEN_AGGRO:
		{
			if( msg.source == _leader_id ) return 0;
			return gnpc_imp::MessageHandler(msg);
		}
		return 0;

		case GM_MSG_MASTER_GET_PET_PROP:
		{
			msg_master_get_pet_prop *pMsg = (msg_master_get_pet_prop*)(msg.content);
			_runner->sync_pet_prop(pMsg->cs_index, pMsg->uid, pMsg->sid, pMsg->pet_index, _cur_prop);
		}
		return 0;	

	default:
		return gnpc_imp::MessageHandler(msg);
	}
}


void gpet_dispatcher::sync_pet_prop(int cs_index, int uid, int sid, int pet_index, q_extend_prop & prop)
{
	packet_wrapper h1(256);
	using namespace S2C;
	CMD::Make<CMD::player_get_summon_petprop>::From(h1, pet_index, prop);
	send_ls_msg(cs_index, uid, sid, h1.data(), h1.size());
}

void gpet_imp::Die( const XID& attacker, bool is_pariah, char attacker_mode )
{
	bool bDuel = false;
	// 决斗或者是帮派野战中杀死宠物
	if( ( attacker_mode & attack_msg::PVP_DUEL ) || ( attacker_mode & attack_msg::PVP_MAFIA_DUEL ) )
	{
		bDuel = true;
	}
	//filter调用死亡前的处理
	_filters.EF_BeforeDeath(attacker_mode & attack_msg::PVP_DUEL);

	//清除必要的标志
	_silent_seal_flag = 0;
	_diet_seal_flag = 0;
	_melee_seal_flag = 0;
	_root_seal_flag = 0;

	//去除死亡时应该去掉的filter
	_filters.ClearSpecFilter(filter::FILTER_MASK_REMOVE_ON_DEATH);
	OnDeath( attacker, is_pariah, bDuel );
}

void 
gpet_imp::OnDeath(const XID & attacker,bool is_invader, bool bduel )
{
	if(_parent->IsZombie())
	{
		//已经是zombie了
		return ;
	}
	//死亡的操作，是，进入zombie状态，
	_parent->b_zombie = true;

	_buff.ClearBuff();

	//执行策略的OnDeath
	((gnpc_controller*)_commander) -> OnDeath(attacker);
	//清除当前Session 注意必须在commander的OnDeath之后调用，否则可能由于策略中排队的任务会引发新的session和任务的产生
	ClearSession();
	
	if(attacker.type == GM_TYPE_PLAYER && !_leader_data.free_pvp_mode && _leader_data.pk_level <= 0 && _leader_data.invader_counter <= 0)
	{
		if(!(_leader_data.duel_target && _leader_data.duel_target == attacker.id))
		{
			SendTo<0>(GM_MSG_PLAYER_KILL_PET,attacker,_leader_data.mafia_id);
		}
	}

	//发送死亡消息
	_runner->on_death(attacker,_corpse_delay);

	{
		//发送消失的消息，延时发送，
		//因为现在消失的话，可能在心跳中死亡 会有很多额外的处理
		MSG msg;
		BuildMessage(msg,GM_MSG_OBJ_ZOMBIE_END,_parent->ID,_parent->ID,_parent->pos);
		gmatrix::SendMessage(msg);
	}
	//还需要发送一个消息给master
	int kill_by_npc = 0;
	if(attacker.IsPlayerClass()) kill_by_npc = 1;
	SendTo2<0>(GM_MSG_PET_NOTIFY_DEATH,_leader_id,_pet_index,_pet_stamp,&kill_by_npc,sizeof(kill_by_npc));
	if( !_leader_data.free_pvp_mode && attacker.IsPlayerClass() && !bduel )
	{
		GMSV::SendUpdateEnemy( _leader_id.id, attacker.id );
	}
}

void 
gpet_imp::SetAggroState(int state)
{
	gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
	aggro_policy * pAggro = pAI->GetAggroCtrl();
	switch (state)
	{
		default:
		case 0:
			//被动
			pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
			break;
		case 1:
			//主动
			pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
			break;
		case 2:
			//发呆
			pAggro->Clear();
			pAggro->SetAggroState(aggro_policy::STATE_FREEZE);
			break;

	}
	_aggro_state = state;

	//通知policy对象当前的状态
	ai_policy * pPolicy = pAI->GetAICtrl();
	pPolicy->ChangeAggroState(state);
}

void 
gpet_imp::SetStayState(int state)
{
	gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
	aggro_policy * pAggro = pAI->GetAggroCtrl();
	switch (state)
	{
		case 0:
			//跟随
			pAggro->Clear();
			break;
		case 1:
		default:
			//停留
			pAggro->Clear();
			break;
	}

	//清除当前的session
	ClearSession();

	ai_policy * pPolicy = pAI->GetAICtrl();
	_stay_state = state;
	pPolicy->ChangeStayMode(state);
}

void
gpet_imp::LoadCoolDown(void * cd_buf, size_t cd_len)
{
	if(cd_len == 0) return;
	raw_wrapper cd_data(cd_buf, cd_len);

	_cooldown.Load(cd_data);
	_cooldown.ClearExpiredCoolDown();

	/*
	abase::hash_map<int,int> time_list;
	_cooldown.GetCoolDownTime(time_list);

	abase::hash_map<int,int>::iterator iter = time_list.begin();
	for(; iter != time_list.end(); ++iter)
	{
		msg_pet_skill_cooldown_t temp;
		temp.cooldown_index = iter->first;
		temp.cooldown_time = iter->second;
		SendTo2<0>(GM_MSG_PET_SET_COOLDOWN,_leader_id,_pet_index,_pet_stamp,&temp,sizeof(temp));

	}
	*/
}

void gpet_imp::NotifySelfDataToMaster()
{
	msg_pet_notify_hp_vp_t info = {_basic.hp,_cur_prop.max_hp,_basic.mp,_cur_prop.max_mp,_aggro_state,_stay_state,IsCombatState()};
	SendTo2<0>(GM_MSG_PET_NOTIFY_HP_VP,_leader_id,_pet_index,_pet_stamp,&info,sizeof(info));
	_hp_notified = _basic.hp;
	_vp_notified = _basic.mp;
	_notify_master_counter = 0;
}

void gpet_imp::DispatchPlayerCommand(int target,const void* buf,size_t size)
{
	int pet_cmd = *(int*)buf;
	gnpc_ai* pAI = ((gnpc_controller*)_commander)->GetAI();
	if(!pAI) return;

	switch(pet_cmd)
	{
		case 1:
		{
			//attack...
			if(sizeof(int) + sizeof(char) != size) return;
			if(0 == target || -1 == target) return;
			//对自己
			if(target == _parent->ID.id) return;
			//对主人
			if(target == _leader_id.id) return;
			char force_attack = *((char*)buf + sizeof(int));
			XID target_xid;
			MAKE_ID(target_xid,target);
			world::object_info info;
			if(!_plane->QueryObject(target_xid,info)) return;

			aggro_policy* pAggro = pAI->GetAggroCtrl();
			pAggro->Clear();
			pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
			pAI->AddAggro(target_xid,_cur_prop.max_hp + 10);
			_leader_force_attack = force_attack; 
			_master_attack_target = target;
			__PRINTF("玩家: %d 的宠物: %d 攻击目标: %d\n",_leader_id.id,_parent->ID.id,target);
		}
		break;

		case 2:
		{
			//change follow mode
			if(sizeof(int) * 2 != size ) return;
			aggro_policy * pAggro = pAI->GetAggroCtrl();
			int state = ((int*)(buf))[1];
			switch(state)
			{
				case PET_MOVE_FOLLOW:
				{
					//跟随
					pAggro->Clear();
					__PRINTF("玩家: %d 的宠物: %d 进入跟随状态\n",_leader_id.id,_parent->ID.id);
				}
				break;
				
				case PET_STAY_STAY:
				{
					//停留
					pAggro->Clear();
					__PRINTF("玩家: %d 的宠物: %d 进入停留状态\n",_leader_id.id,_parent->ID.id);
				}
				break;

				default:
					return;
			}
			//清除当前的session
			ClearSession();
			ai_policy * pPolicy = pAI->GetAICtrl();
			pPolicy->ClearTask();
			_stay_state = state;
			pPolicy->ChangeStayMode(state);
			//清空内容
			_leader_force_attack = 0; 
			_master_attack_target = 0;
			NotifySelfDataToMaster();
		}
		break;

		case 3:
		{
			//change aggro state
			if(sizeof(int) * 2 != size) return;
			int state = ((int*)(buf))[1];
			aggro_policy* pAggro = pAI->GetAggroCtrl();
			switch(state)
			{
				case PET_AGGRO_DEFENSE:
				{
					//被动
					if(_aggro_state != state) pAggro->Clear();
					pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
					__PRINTF("玩家: %d 的宠物: %d 进入被动状态\n",_leader_id.id,_parent->ID.id);
				}
				break;

				case PET_AGGRO_AUTO:
				{
					//主动
					pAggro->Clear();
					pAggro->SetAggroState(aggro_policy::STATE_FREEZE);
					__PRINTF("玩家: %d 的宠物: %d 进入主动状态\n",_leader_id.id,_parent->ID.id);
				}
				break;

				case PET_AGGRO_PASSIVE:
				{
					//发呆
					pAggro->Clear();
					pAggro->SetAggroState(aggro_policy::STATE_FREEZE);
					__PRINTF("玩家: %d 的宠物: %d 进入发呆状态\n",_leader_id.id,_parent->ID.id);
				}
				break;

				default:
					return;
			}
			_aggro_state = state;
			//通知policy对象当前的状态
			ai_policy * pPolicy = pAI->GetAICtrl();
			pPolicy->ChangeAggroState(state);
			//清空内容
			_leader_force_attack = 0; 
			_master_attack_target = 0;
			NotifySelfDataToMaster();
		}
		break;

		case 4:
		{
			//use skill
			if(sizeof(int) * 2 + sizeof(char) != size) return; // target skill_id force_attack
			int skill_id = ((int*)(buf))[1];
			char force_attack = *((char*)buf + sizeof(int) * 2);
			int skilltype = GNET::SkillWrapper::GetType(skill_id);
			ai_policy* pPolicy = pAI->GetAICtrl();
			pPolicy->ClearNextTask();
			int rst = pPolicy->AddPetSkillTask(skill_id,target);
			if(0 != rst)
			{
				// 考虑是否给提示
				break;
			}
			//如果是合法目标技能使用成功则加入一些仇恨
			if(1 == skilltype || 3 == skilltype)
			{
				//是主动攻击或者主动诅咒,加到仇恨列表里去
				if(target == _leader_id.id || target == _parent->ID.id) break;
				aggro_policy* pAggro = pAI->GetAggroCtrl();
				pAggro->Clear();
				pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
				XID xid(-1,-1);
				MAKE_ID(xid,target);
				world::object_info info;
				if(!_plane->QueryObject(xid,info)) return;

				pAI->AddAggro(xid,_cur_prop.max_hp + 10);
			}
			_leader_force_attack = force_attack;
			_master_attack_target = target;
			__PRINTF("玩家: %d 的宠物: %d 使用技能: %d\n",_leader_id.id,_parent->ID.id,skill_id);
		}
		break;

		case 5:
		{
			//set auto skill
			if(sizeof(int) * 2  + sizeof(char)!= size) return;
			int skill_id = ((int*)(buf))[1];
			char set_flag = *((char*)buf + sizeof(int) * 2);
			ai_policy * pPolicy = pAI->GetAICtrl();
			int skill_level = GetSkillLevel(skill_id);
			if(skill_level <= 0) skill_id = 0;
			pPolicy->SetPetAutoSkill(skill_id,set_flag);
			msg_pet_auto_skill_set_t temp;
			temp.skill_id = skill_id;
			temp.set_flag = set_flag;
			SendTo2<0>(GM_MSG_PET_SET_AUTO_SKILL,_leader_id,_pet_index,_pet_stamp,&temp,sizeof(temp));
			__PRINTF("玩家: %d 的宠物: %d 设置自动技能: %d,%d\n",_leader_id.id,_parent->ID.id,skill_id,set_flag);
		}
		break;

		case 6:
		{
			//emote
			if(sizeof(int) * 2 != size) return;
			int emote_id = ((int*)(buf))[1];
			if(!CheckCoolDown(COOLDOWN_INDEX_EMOTE)) return;
			SetCoolDown(COOLDOWN_INDEX_EMOTE,EMOTE_COOLDOWN_TIME);
			//if( HasEmoteState() )
			//{
			//	TryStopCurSession();
			//}
			_runner->do_emote((unsigned short)emote_id);
			__PRINTF("玩家: %d 的宠物: %d 使用表情: %d\n",_leader_id.id,_parent->ID.id,emote_id);
		}
		break;

		default:
			__PRINTF("玩家: %d 的宠物: %d 收到未知命令\n",_leader_id.id,_parent->ID.id);
		break;
	}
}

bool 
gpet_imp::OI_IsPVPEnable()
{
	return _leader_data.pvp_flag_enable && !_leader_data.sanctuary_mode;
}

bool 
gpet_imp::OI_IsInPVPCombatState()
{
	return _leader_data.pvp_combat_timer;
}

bool 
gpet_imp::OI_IsInTeam()
{
	return _leader_data.team_count > 0;
}

bool 
gpet_imp::OI_IsMember(const XID & id)
{
	for(int i = 0; i < _leader_data.team_count; i ++)
	{
		if(_leader_data.teamlist[i] == id) return true;
	}
	return false;
}

int  
gpet_imp::OI_GetMafiaID()
{
	return _leader_data.mafia_id;
}

int  
gpet_imp::OI_GetMasterID()
{
	return (_leader_data.master_id == _leader_id.id) ? 0 : _leader_data.master_id;
}

int
gpet_imp::OI_GetZoneID()
{
	return _leader_data.zone_id;
}


int gpet_imp::OI_GetPkLevel()
{
	return _leader_data.pk_level;
}

int gpet_imp::OI_GetInvaderCounter()
{
	return _leader_data.invader_counter;
}

int gpet_imp::OI_GetMDuelMafiaID()
{
	return _leader_data.mduel_mafia_id;
}

int gpet_imp::OI_GetDuelTarget()
{
	return _leader_data.duel_target;
}

void 
gpet_imp::FillAttackMsg(const XID & target, attack_msg & attack,int dec_arrow)
{
	gactive_imp::FillAttackMsg(target,attack);
	// 宠物打的等同于主人打
	attack.ainfo.attacker = _leader_id;
	attack.force_attack = _leader_force_attack;
	attack.ainfo.sid = _leader_data.cs_sid;
	attack.ainfo.cs_index = _leader_data.cs_index;
	attack.ainfo.team_id = _leader_data.team_id;
	attack.ainfo.team_seq = _leader_data.team_seq;
	int eff_level =0;
	if(_leader_data.team_count) eff_level = _leader_data.team_efflevel;
	attack.ainfo.level = _leader_data.level;
	attack.ainfo.eff_level = eff_level;
	attack.ainfo.wallow_level = _leader_data.wallow_level;
	if(_leader_data.safe_lock) attack.force_attack &= ~C2S::FORCE_ATTACK_WHITE;
	//调用master的填充接口
	if(_attack_fill)
	{
		(*_attack_fill)(this, attack);
	}
	attack.damage_factor = 1 + pet_manager::GetHungerLevelDamageAdjust(_hunger_level);
	attack.pet_attack_adjust = _leader_data.pet_attack_adjust;
	attack.is_pet = 1;
}

void 
gpet_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	gactive_imp::FillEnchantMsg(target,enchant);
	enchant.ainfo.attacker = _leader_id;
	if(target.id == _master_attack_target)
	{
		enchant.force_attack = _leader_force_attack;
	}
	else
	{
		enchant.force_attack = 0;
	}

	enchant.ainfo.sid = _leader_data.cs_sid;
	enchant.ainfo.cs_index = _leader_data.cs_index;
	enchant.ainfo.team_id = _leader_data.team_id;
	enchant.ainfo.team_seq = _leader_data.team_seq;
	enchant.ainfo.level = _leader_data.level;
	int eff_level =0;
	if(_leader_data.team_count) eff_level = _leader_data.team_efflevel;
	enchant.ainfo.eff_level = eff_level;
	enchant.ainfo.wallow_level = _leader_data.wallow_level;
	if(_leader_data.safe_lock) enchant.force_attack &= ~C2S::FORCE_ATTACK_WHITE;

	//调用master的填充接口
	if(_enchant_fill)
	{
		(*_enchant_fill)(this, enchant);
	}
}

void 
gpet_imp::InitFromMaster(gplayer_imp * pImp)
{
	pImp->SetLeaderData(_leader_data);
}

void gpet_imp::OnHeartbeat(size_t tick)
{
	gnpc_imp::OnHeartbeat(tick);
	_notify_master_counter  ++;
	gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
	aggro_policy * pAggro = pAI->GetAggroCtrl();
	int combat_state = pAggro->Size()?1:0;
	if(combat_state != _old_combat_state)
	{
		_hp_notified = -1;
		_vp_notified = -1;
	}
	_peep_counter ++;
	/*
	if(_peep_counter >= 3)
	{
		//每三秒发一次广播
		_peep_counter = 0;
		gnpc * pNPC = (gnpc*)_parent;
		MSG msg;
		msg_watching_t mwt= {_basic.level,GetFaction()};
		BuildMessage(msg,GM_MSG_WATCHING_YOU,XID(GM_TYPE_NPC,-1),pNPC->ID,pNPC->pos,0,&mwt,sizeof(mwt));
		float tmp = GetWorldManager()->GetMaxMobSightRange();
		_plane->BroadcastMessage(msg,tmp,gobject::MSG_MASK_PLAYER_MOVE);
	}
	*/
	if(_notify_master_counter >= 5)
	{
		//每5秒向master通报一下自己的数据
		NotifySelfDataToMaster();
		_old_combat_state = combat_state;
		return ;
	}
	if(_hp_notified != _basic.hp || _vp_notified != _basic.mp)
	{
		//血量或者精力发生了变化,直接通知master
		NotifySelfDataToMaster();
		_old_combat_state = combat_state;
		return ;
	}
}

void 
gpet_imp::AddAggroToEnemy(const XID & who,int rage)
{
	size_t count = _enemy_list.size();
	if(!count || rage <= 0) return;
	XID list[MAX_PLAYER_ENEMY_COUNT];
	ENEMY_LIST::iterator it = _enemy_list.begin();
	for(size_t i = 0;it != _enemy_list.end();i ++, ++it )
	{
		MAKE_ID(list[i],it->first);
	}

	msg_aggro_info_t info;
	info.source = who;
	info.aggro = rage;
	info.aggro_type = 0;
	info.faction = 0xFFFFFFFF;
	info.level = 0;
	MSG msg;
	BuildMessage(msg,GM_MSG_GEN_AGGRO,XID(-1,-1),who,_parent->pos,0,&info,sizeof(info));

	gmatrix::SendMessage(list, list + count, msg);
}

void 
gpet_imp::AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg)
{
	if(IS_HUMANSIDE(attack->ainfo.attacker))
	{
		dmg.damage -= _damage_reduce;
		dmg.adjust *= (1 - _damage_reduce_percent); 
	}
}

void gpet_imp::NPCEndSkill(unsigned int skill_id,int level,const XID & target)
{
	__PRINTF( "NPCEndSkill(unsigned int skill_id,int level,const XID & target)\n" );
	SendTo<0>( GM_MSG_PET_CAST_SKILL, _leader_id, 1, NULL, 0 );
	gactive_imp::NPCEndSkill( skill_id, level, target );
}


/* ---------------------------policy pet-------------------------------------- */
gpet_policy::gpet_policy()
{
	memset(&_chase_info,0,sizeof(_chase_info));
	_pathfind_result = 0;
	_aggro_state = gpet_imp::PET_AGGRO_DEFENSE;
	_stay_state = gpet_imp::PET_MOVE_FOLLOW;
	_auto_skills.clear();
	_stay_pos = A3DVECTOR(0,0,0);
}

void gpet_policy::DeterminePolicy(const XID& target)
{
	bool bNoSkill = true;
	if(-1 != target.id)
	{
		bNoSkill = true;
		if(AddAutoCastSkill(target) == 0 ) bNoSkill = false;
		if(bNoSkill)
		{
			AddPetPrimaryTask(target,_primary_strategy);
		}
		if(_cur_task) return;
	}
	//无目标 或者处理原目标失败     
	int count = 3;                  
	XID old_target(-1,-1);          
	while(_cur_task == NULL && _self->GetAggroCount() && count > 0)
	{
		bNoSkill = true;
		XID new_target;         
		if(!DetermineTarget(new_target))
		{                       
			break;          
		}               
		if(AddAutoCastSkill(new_target) == 0 ) bNoSkill = false;
		if( bNoSkill )
		{
			AddPetPrimaryTask(new_target,_primary_strategy);
		}
		if(old_target == new_target) break;
		old_target = new_target;
	}                               
}                                       

void 
gpet_policy::AddPetPrimaryTask(const XID & target, int strategy)
{
	int seal_flag = _self->GetSealMode();
	if(seal_flag & ai_object::SEAL_MODE_SILENT)
	{
		ai_task * pTask = NULL;
		switch(strategy)
		{
			case STRATEGY_MELEE:
			case STRATEGY_RANGE:
			case STRATEGY_MAGIC:
			case STRATEGY_MELEE_MAGIC:
				pTask = new ai_silent_runaway_task(target);
				break;
			case STRATEGY_CRITTER:
				pTask = new ai_runaway_task(target);
				break;
			case STRATEGY_FIX:
			case STRATEGY_FIX_MAGIC:
				pTask = new ai_silent_task();
				break;
			case STRATEGY_STUB:
				//do nothing
				break;
			default:
				ASSERT(false);
		}
		if(pTask){
			pTask->Init(_self,this);
			AddTask(pTask);
		}
	}
	else
	{
		switch(strategy)
		{
			case STRATEGY_MELEE:
				AddTargetTask<ai_melee_task>(target);
				break;
			case STRATEGY_RANGE:
				AddTargetTask<ai_range_task>(target);
				break;
			case STRATEGY_MAGIC:
				AddTargetTask<ai_magic_task>(target);
				break;
			case STRATEGY_MELEE_MAGIC:
				AddTargetTask<ai_magic_melee_task>(target);
				break;
			case STRATEGY_CRITTER:
				AddTargetTask<ai_runaway_task>(target);
				break;
			case STRATEGY_FIX:
				AddTargetTask<ai_fix_melee_task>(target);
				break;
			case STRATEGY_STUB:
				//do nothing
				break;
			case STRATEGY_FIX_MAGIC:
				AddTargetTask<ai_fix_magic_task>(target);
				break;
			default:
				ASSERT(false);
		}               
	}               
}

void 
gpet_policy::UpdateChaseInfo(const CChaseInfo * pInfo)
{
	_chase_info = *pInfo;
}

void 
gpet_policy::FollowMasterResult(int reason) 
{
	if(reason)
	{
		// 寻路失败
		_pathfind_result ++;
	}
	else
	{
		_pathfind_result = 0;
	}
}

bool
gpet_policy::GatherTarget()
{
	A3DVECTOR pos;
	_self->GetPos(pos);
	guard_agent::search_target<slice> worker(_self,_self->GetSightRange(),_self->GetEnemyFaction());
	_self->GetImpl()->_plane->ForEachSlice(pos,_self->GetSightRange(),worker);
	XID target;
	if(_self->GetAggroEntry(0,target))
	{
		OnAggro();
		return true;
	}
	return false;
}

void 
gpet_policy::OnHeartbeat()
{
	if(InCombat() && _cur_task && _cur_task->GetTaskType() != ai_task::AI_TASK_TYPE_PET_SKILL &&
			_self->GetImpl()->_session_state != gactive_imp::STATE_SESSION_SKILL &&
			!HasNextTask())
	{
		//如果在战斗状态，则判断是否可以自动使用技能
		XID target;
		if(_self->GetAggroEntry(0,target))
		{
			AddAutoCastSkill(target);
		}
	}
	ai_policy::OnHeartbeat();
	
	XID leader = _self->GetLeaderID();
	A3DVECTOR selfpos;
	_self->GetPos(selfpos);

	//如果没有任务 则试图跟随主人
	if(!InCombat())
	{
		if(!_cur_task)
		{
			// 如果是主动寻敌的状态，那么就进行敌人的收集 
			//if(_aggro_state == gpet_imp::PET_AGGRO_AUTO)
			//{
			//	if(GatherTarget())
			//	{
			//		//发现了敌人，则跳过后面的内容
			//		return ;
			//	}
			//}

			ai_object::target_info info;
			int target_state;
			float range;

			target_state = _self->QueryTarget(leader,info);
			if(target_state != ai_object::TARGET_STATE_NORMAL)
			{
				//目标不存在 让自己消失
				XID id;
				_self->GetID(id);
				_self->SendMessage(id,GM_MSG_PET_DISAPPEAR);
				return;
			}

			range = info.pos.horizontal_distance(selfpos);
			if(_stay_state != 0  )
			{
				//停留模式
				float h = fabs(selfpos.y - info.pos.y);
				if(h > PET_FALLOW_RANGE_LIMIT || range >= PET_FALLOW_RANGE_LIMIT * PET_FALLOW_RANGE_LIMIT || (range < 6.f*6.f && h > 30.f))
				{
					//需要让自己消失，通知master
					RelocatePetPos(true);
					return ;
				}
			}
			else if(_stay_state == 0)
			{
				//跟随模式
				if(range > 150.f * 150.F)
				{
					//需要瞬移
					RelocatePetPos();
					return;
				}

				float h = fabs(selfpos.y - info.pos.y);
				if(h > PET_FALLOW_RANGE_LIMIT || range >= PET_FALLOW_RANGE_LIMIT * PET_FALLOW_RANGE_LIMIT || (range < 6.f*6.f && h > 30.f))
				{
					//距离过远或者无法到达
					//需要瞬移
					RelocatePetPos();
					return ;
				}
				// 让宠物跟的快点, 只要主人动，宠物就动，原来是6.5
				if(range > 0.0f*0.0f)
				{
					//跟随
					AddTargetTask<ai_pet_follow_master>(leader,&_chase_info);
				}
			}

		}

		if(_pathfind_result >= 5)
		{
			RelocatePetPos();
		}
	}
	else
	{
		//also need check master states
		ai_object::target_info info;
		int target_state = _self->QueryTarget(leader,info);
		if(target_state != ai_object::TARGET_STATE_NORMAL)
		{
			//can not find master
			XID id;
			_self->GetID(id);
			_self->SendMessage(id,GM_MSG_PET_DISAPPEAR);
			return;
		}

		//如果超出距离就直接消失
		float range = info.pos.horizontal_distance(selfpos);
		float h = fabs(selfpos.y - info.pos.y);
		if(h > PET_FALLOW_RANGE_LIMIT || range >= PET_FALLOW_RANGE_LIMIT *PET_FALLOW_RANGE_LIMIT || (range < 6.f*6.f && h > 40.f))
		{
			//需要让自己消失，通知master
			RelocatePetPos(true);
			return ;
		}
	}
}

void 
gpet_policy::RelocatePetPos(bool disappear)
{
	_self->RelocatePos(disappear);
	_pathfind_result = 0;
}

void 
gpet_policy::ChangeAggroState(int state)
{
	_aggro_state = state;
}

void 
gpet_policy::ChangeStayMode(int state)
{
	_stay_state = state;
	if(_stay_state == 1)
	{
		//需要记录当前的坐标
		_self->GetPos(_stay_pos);
	}
}

void 
gpet_policy::RollBack()
{
	_self->ActiveCombatState(false);
	EnableCombat(false);
	_self->ClearDamageList();
	_policy_flag = 0;
	if(_stay_state == 1)
	{
		AddPosTask<ai_returnhome_task>(_stay_pos);
	}
}

int 
gpet_policy::GetInvincibleTimeout()
{
	return 0;
}

void gpet_policy::SetPetAutoSkill(int skill_id,int set_flag)
{
	std::vector<int>::iterator it;
	if(set_flag)
	{
		//设置
		for(it = _auto_skills.begin();it != _auto_skills.end();++it)
		{
			if(*it == skill_id) return;
		}
		_auto_skills.push_back(skill_id);
	}
	else
	{
		//取消
		for(it = _auto_skills.begin();it != _auto_skills.end();++it)
		{
			if(*it == skill_id) break;
		}
		if(it == _auto_skills.end()) return;
		_auto_skills.erase(it);
	}
}

int gpet_policy::AddAutoCastSkill(XID target)
{
	int temp = target.id;
	size_t auto_skill_count = _auto_skills.size();
	for(size_t i = 0;i < auto_skill_count;++i)
	{
		if(_auto_skills[i] && CheckCoolDown(GNET::SkillWrapper::GetCooldownId(_auto_skills[i])))
		{
			int ret = AddPetSkillTask(_auto_skills[i],temp);
			if(0 == ret) return ret;
		}
	}
	return -1;
}

void gpet_policy::SetCoolDown( int cooldown_index, int cooldown_time )
{
	return _self->GetImpl()->SetCoolDown( cooldown_index, cooldown_time );
}

bool gpet_policy::CheckCoolDown( int cooldown_index )
{
	return _self->GetImpl()->CheckCoolDown( cooldown_index );
}

int gpet_policy::AddPetSkillTask(int skill_id,int& target)
{
	//返回值 -1 没有技能 -2 目标有问题 -3 技能类型 -4 冷却
	gpet_imp* pImp = (gpet_imp*)(_self->GetImpl());
	//基础判断
	//根据技能的类别进行调整
	//判断是否需要攻击目标
	int skill_level = pImp->GetSkillLevel(skill_id);
	if(skill_level <= 0)
	{
		__PRINTF("此宠物无此技能,skill_id=%d\n",skill_id);
		return -1;	//此宠物无此技能
	}
	int range_type = GNET::SkillWrapper::RangeType(skill_id);
	int skilltype = GNET::SkillWrapper::GetType(skill_id);
	//确认下技能类型号
	if(7 == range_type)
	{
		//8号是给主人放的技能
		//忽略客户端目标,把目标调整为主人
		target = pImp->_leader_id.id;
	}
	else if(2 == range_type || 5 == range_type)
	{
		//自身类型
		//忽略客户端目标,把目标调整为自己
		target = pImp->_parent->ID.id;
	}
	else if(2 != range_type && 5 != range_type)
	{
		//技能类型：目标不为自己时需要客户端必须给目标
		if(0 == target || -1 == target) return -2;
		//目标不能为自己
		if(target == pImp->_parent->ID.id) return -2;
	}
	if( 1 == skilltype || 3 == skilltype )
	{
		//主动攻击 主动诅咒
		//对自身的技能不可以
		if(5 == range_type) return -3;
		//不可以对自己主人
		if(target == pImp->_leader_id.id) return -2;
	}
	else if(2 == skilltype)
	{
		//主动祝福
		//只能对自己或主人用
		if(8 != range_type) target = pImp->_parent->ID.id; // 不是给主人的就是给自己的，不可以给其他人加
	}
	else
	{
		//宠物不会施放其他类型技能
		return -3;
	}
	if(!CheckCoolDown(GNET::SkillWrapper::GetCooldownId(skill_id)))
	{
		__PRINTF("宠物技能处于冷却状态\n");
		pImp->NotifySkillStillCoolDown();
		return -4;
	}
	//需要消耗魔法了
	int vp_need = GNET::SkillWrapper::GetMpCost(skill_id,skill_level);
	if(vp_need > pImp->_basic.mp)
	{
		return -6;
	}
	pImp->_basic.mp -= vp_need;
	pImp->NotifySelfDataToMaster();
	ai_task* pTask = NULL;
	XID xid(-1,-1);
	MAKE_ID(xid, target);
	//这里真正施放的skill_level要根据honor来计算
	pTask = new ai_pet_skill_task(xid,skill_id,skill_level,range_type);
	pTask->Init(_self,this);
	AddTask(pTask);
	return 0;
}

void gpet_imp::OI_UpdateAllProp()
{
	property_policy::UpdatePet(this);
}

void gpet_imp::OI_UpdataAttackData()
{
	property_policy::UpdatePetAttack(GetObjectClass(),this);
}


