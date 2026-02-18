#include "summonnpc.h"
#include "clstab.h"
#include "player_imp.h"
#include "ainpc.h"
#include "guardnpc.h"
#include "npcsession.h"

DEFINE_SUBSTANCE(gsummon_imp,gnpc_imp,CLS_SUMMON_IMP)
DEFINE_SUBSTANCE(gsummon_dispatcher,gnpc_dispatcher,CLS_SUMMON_DISPATCHER)
DEFINE_SUBSTANCE(gsummon_policy,ai_policy,CLS_NPC_AI_POLICY_SUMMON)

gsummon_imp::gsummon_imp()
{
	_attack_judge= NULL;
	_enchant_judge= NULL;
	_attack_fill = NULL;
	_enchant_fill = NULL;
	
	memset(&_leader_data, 0, sizeof(_leader_data));

	_summon_type  = summon_manager::SUMMON_TYPE_NONE;
	_aggro_state  = AGGRO_STATE_NONE;
	_move_state = MOVE_STATE_NONE;
	_is_clone = false;
	_is_invisible = false;

	_master_attack_target = 0;

	_corpse_delay = 0;
	_heartbeat_counter = 0;
	_teleport_id = 0;
	_teleport_max_count = 0;
	_teleport_cur_count = 0;
	_teleport_prob = 0;
}

void 
gsummon_imp::Init(world * pPlane,gobject*parent)
{
	_cur_item.attack_cycle = SECOND_TO_TICK(2.0f);
	_cur_item.attack_point = SECOND_TO_TICK(1.0f);
	gnpc_imp::Init(pPlane,parent);
}

void
gsummon_imp::SetAggroState(char state)
{
	_aggro_state = state;
	gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
	aggro_policy * pAggro = pAI->GetAggroCtrl();
	
	switch (state)
	{
		case AGGRO_STATE_NONE:
			break;
		case AGGRO_STATE_DEFENSE:
			pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
			break;
		case AGGRO_STATE_OFFENSE:
			pAggro->SetAggroState(aggro_policy::STATE_NORMAL);
			break;

	}

	//通知policy对象当前的状态
	ai_policy * pPolicy = pAI->GetAICtrl();
	pPolicy->ChangeAggroState(state);

}

void
gsummon_imp::SetMoveState(char state)
{
	_move_state = state;

	gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
	aggro_policy * pAggro = pAI->GetAggroCtrl();

	pAggro->Clear();
	
	ai_policy * pPolicy = pAI->GetAICtrl();
	pPolicy->ChangeStayMode(state);
}

void
gsummon_imp::SetCloneState(char state)
{
	_is_clone = state;

	gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
	ai_policy * pPolicy = pAI->GetAICtrl();

	pPolicy->SetCloneState(state);
}


void 
gsummon_imp::SetAttackHook(attack_judge judge1,enchant_judge judge2,attack_fill fill1, enchant_fill fill2)
{
	_attack_judge = judge1;
	_enchant_judge = judge2;
	_attack_fill = fill1;
	_enchant_fill = fill2;
}

void 
gsummon_imp::RelocatePos(bool is_disappear)
{
	if(_leader_id.IsValid())
	{
		int dis = is_disappear?1:0;
		SendTo2<0>(GM_MSG_SUMMON_RELOCATE_POS,_leader_id,_summon_stamp,dis);
	}
}

void
gsummon_imp::SetLifeTime(int lifetime)
{
	((gnpc_controller*)_commander)->SetLifeTime(lifetime);
}

int 
gsummon_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_CONTROL_TRAP:
		{
			if(_summon_type == summon_manager::SUMMON_TYPE_CONTROL_TRAP)
			{
				XID attacker(-1, -1);
				((gnpc_controller*)_commander)->OnDeath(attacker);
				((gnpc_controller*)_commander)->SetLifeTime(1);
			}
		}
		return 0;

		case GM_MSG_NPC_DISAPPEAR:
		{ 
			if( msg.source == _leader_id || msg.source == _parent->ID )
			{

				//if(msg.source == _parent->ID)
				{
					int npcTid = GetNPCID();
					SendTo2<0>(GM_MSG_SUMMON_NOTIFY_DEATH,_leader_id,(int)_summon_type, _summon_stamp, &npcTid, sizeof(npcTid));
					//SendTo2<0>(GM_MSG_SUMMON_NOTIFY_DEATH,_leader_id,(int)_summon_type, _summon_stamp);
				}

				_runner->stop_move(_parent->pos,0x500,1,C2S::MOVE_MODE_RETURN);
				_runner->disappear(true);
				_commander->Release();
			}
		}
		return 0;
		
		case GM_MSG_SUMMON_CHANGE_POS:
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

		case GM_MSG_ATTACK:
		{
			ASSERT(msg.content_length >= sizeof(attack_msg));

			attack_msg ack_msg = *(attack_msg*)msg.content;
			if(_player_cannot_attack && ack_msg.ainfo.attacker.IsPlayerClass()) return 0;

			//处理一下到来的攻击消息
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);

			//进行决斗的检测
			if(_leader_data.duel_target && _leader_data.duel_target == ack_msg.ainfo.attacker.id)
			{
				//如何处理宠物？
				ack_msg.attacker_mode |= attack_msg::PVP_DUEL;
			}

			//永远不会受到来自主人的伤害
			if(msg.source == _leader_id || ack_msg.ainfo.attacker == _leader_id) return 0;

			if(_attack_judge && !gmatrix::IsBattleFieldServer() && !GetWorldManager()->IsBattleWorld())
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
			if(_is_invisible && !(ack_msg.skill_limit & SKILL_LIMIT_INVISIBLE_NPC)) return 0; 

			HandleAttackMsg(msg,&ack_msg);
		}
		return 0;
		
		case GM_MSG_ENCHANT:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if(_player_cannot_attack && ech_msg.ainfo.attacker.IsPlayerClass()) return 0;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
			if(!ech_msg.helpful)
			{
				//永远不会受到来自主人的伤害
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
			if(_is_invisible && !(ech_msg.skill_limit & SKILL_LIMIT_INVISIBLE_NPC)) return 0; 

			HandleEnchantMsg(msg,&ech_msg);
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
				GLog::log(GLOG_INFO,"召唤怪可能收到了不是自己主人的消息, source_id=%d, leader_id=%d", msg.source.id, _leader_id.id);
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
		return 0;

		case GM_MSG_MASTER_START_ATTACK:
		{
			if( msg.content_length == sizeof(XID) )
			{
				gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
				aggro_policy * pAggro = pAI->GetAggroCtrl();
				if( pAggro->Size() == 0 )
				{
					XID target = *(XID*)msg.content;
					if(target.IsActive())
					{
						pAI->AddAggro(target, _cur_prop.max_hp + 10);
						_master_attack_target = target.id;
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
			_master_attack_target = 0;

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
		
		case GM_MSG_WATCHING_YOU:
		{
			if(msg.content_length == sizeof(msg_watching_t))
			{
				gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
				msg_watching_t * pMsg = (msg_watching_t *)msg.content;
				
				gnpc * pNPC = (gnpc*)this->_parent;
				if(pNPC->anti_invisible_rate < pMsg->invisible_rate) return 0;
				if(msg.source == _leader_id) return 0;
				if(GetWorldManager()->GetCityRegion()->IsInSanctuary(_parent->pos.x, _parent->pos.z) && _leader_data.duel_target != msg.source.id) return 0;

				bool can_attack = true; 
				
				//非跨服服务器
				if(!IsZoneServer())
				{
					//先检查pvp mask
					if(_leader_data.pvp_mask == 0) can_attack = false;
					else if(!(_leader_data.pvp_mask & C2S::FORCE_ATTACK_MAFIA) && _leader_data.mafia_id == pMsg->mafia_id && _leader_data.mafia_id != 0) can_attack = false;
					else if(!(_leader_data.pvp_mask & C2S::FORCE_ATTACK_FAMILY) && _leader_data.family_id == pMsg->family_id && _leader_data.family_id != 0) can_attack = false;
					else if(!(_leader_data.pvp_mask & C2S::FORCE_ATTACK_RED) &&  (pMsg->faction & FACTION_PARIAH)) can_attack = false;
					else if(!(_leader_data.pvp_mask & C2S::FORCE_ATTACK_WHITE) &&  (pMsg->faction & FACTION_WHITE)) can_attack = false;

					//检查决斗和帮战
					if( _leader_data.duel_target == msg.source.id || (_leader_data.mduel_mafia_id != 0 && (_leader_data.mduel_mafia_id == pMsg->mafia_id))) can_attack = true; 

					//检查free pvp模式
					if(_leader_data.free_pvp_mode && !(pMsg->faction & GetEnemyFaction())) can_attack = false;

					if(OI_IsMember(msg.source)) can_attack = false;
	
				}
				//跨服服务器
				else
				{
					if(_leader_data.pvp_mask == 0) can_attack = false;
					else if(!(_leader_data.pvp_mask & C2S::FORCE_ATTACK_ZONE) &&  _leader_data.zone_id == pMsg->zone_id && _leader_data.zone_id != 0) can_attack = false;
				
					if(_leader_data.duel_target == msg.source.id) can_attack = true; 
					if(_leader_data.zone_id != pMsg->zone_id) can_attack = true;
				}


				if(can_attack)
				{
					//强行将改玩家置为敌对阵营
					if(!(pMsg->faction & GetEnemyFaction()))
					{
						pMsg->faction |= GetEnemyFaction();
					}
					pAI->AggroWatch(msg);
				}
			}
			else
			{
				ASSERT(false);
			}
		}
		return 0;
		
		case GM_MSG_HEAL_CLONE:
		{
			if( msg.source != _leader_id ) return 0;
			if(!_is_clone) return 0;
			Heal(msg.param, false, false);
			SetRefreshState();
		}
		return 0;
		
		case GM_MSG_MANA_CLONE:
		{
			if( msg.source != _leader_id ) return 0;
			if(!_is_clone) return 0;
			InjectMana(msg.param, false);
			SetRefreshState();
		}
		return 0;

		case GM_MSG_SUMMON_CAST_SKILL:
		{
			ASSERT(msg.content_length == sizeof(msg_summon_cast_skill));
			const msg_summon_cast_skill * pMsg = (const msg_summon_cast_skill*)msg.content;
			gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
			ai_policy* pPolicy = pAI->GetAICtrl();
			pPolicy->ClearNextTask();
			pPolicy->AddSummonSkillTask(pMsg->skillid, pMsg->skilllevel, pMsg->target);
		}
		return 0;

		case GM_MSG_NPC_TELEPORT_INFO:
	     	{
			if(_summon_type != summon_manager::SUMMON_TYPE_TELEPORT1 && _summon_type != summon_manager::SUMMON_TYPE_TELEPORT2) return 0;

			msg_npc_teleport_info * pMsg = (msg_npc_teleport_info*)msg.content;
			_teleport_id = pMsg->npc_id;
			_teleport_pos = pMsg->pos;
			_teleport_cur_count = 0; 


			gnpc * pNPC = (gnpc*)this->_parent;
			if(_summon_type == summon_manager::SUMMON_TYPE_TELEPORT1)
			{
				pNPC->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
			}
			((gnpc_controller*)_commander)->SetLifeTime(pMsg->lifetime);
			pNPC->teleport_count = 2;
			_runner->teleport_skill_info(_leader_id.id, GetNPCID(), msg.param, 2);  

		}
		return 0;	

		case GM_MSG_SUMMON_TELEPORT_REQUEST:
		{
			if(_summon_type != summon_manager::SUMMON_TYPE_TELEPORT1) return 0;
		       	if(_teleport_id == 0) return 0;
			if(msg.pos.horizontal_distance(_parent->pos) > 5 * 5) return 0;
			if(_teleport_cur_count >= _teleport_max_count) return 0;
			if(msg.param != GetWorldTag()) return 0;
			if(msg.param2 != _leader_data.team_id) return 0;
			if(msg.param2 == 0 &&  msg.source != _leader_id) return 0; 

			if(abase::Rand(0, 100) < _teleport_prob)
			{
				++_teleport_cur_count;
				MSG tele_msg1;
				BuildMessage(tele_msg1,GM_MSG_SUMMON_TELEPORT_REPLY,msg.source, _parent->ID, _parent->pos, 1, &_teleport_pos, sizeof(_teleport_pos)); 
				gmatrix::SendMessage(tele_msg1);

				MSG tele_msg2;
				BuildMessage(tele_msg2,GM_MSG_SUMMON_TELEPORT_SYNC,XID(GM_TYPE_NPC,_teleport_id), _parent->ID, _parent->pos, 0); 
				gmatrix::SendMessage(tele_msg2);

				if(_teleport_cur_count >= _teleport_max_count)
				{
					MSG tele_msg3;
					BuildMessage(tele_msg3,GM_MSG_NPC_DISAPPEAR,_parent->ID, _parent->ID, _parent->pos, 0); 
					gmatrix::SendMessage(tele_msg3);
				}
			}
		}
		return 0;

		case GM_MSG_SUMMON_TELEPORT_SYNC:
		{
			++_teleport_cur_count;
			if(_teleport_cur_count >= _teleport_max_count)
			{
				MSG msg;
				BuildMessage(msg,GM_MSG_NPC_DISAPPEAR,_parent->ID, _parent->ID, _parent->pos, 0); 
				gmatrix::SendMessage(msg);
			}
		}
		return 0;

		case GM_MSG_SUMMON_TRY_TELEPORT:
		{
			if(_summon_type != summon_manager::SUMMON_TYPE_TELEPORT2) return 0;
		       	if(_teleport_id == 0) return 0;
			if(msg.param != GetWorldTag()) return 0;
			if(_teleport_cur_count >= _teleport_max_count) return 0;

			++_teleport_cur_count;

			MSG r_msg;
			BuildMessage(r_msg,GM_MSG_SUMMON_TELEPORT_REPLY,msg.source, _parent->ID, _parent->pos, 2, &_teleport_pos, sizeof(_teleport_pos)); 
			gmatrix::SendMessage(r_msg);

			MSG s_msg;
			BuildMessage(s_msg,GM_MSG_SUMMON_TELEPORT_SYNC,XID(GM_TYPE_NPC,_teleport_id), _parent->ID, _parent->pos, 0); 
			gmatrix::SendMessage(s_msg);

			if(_teleport_cur_count >= _teleport_max_count)
			{
				MSG d_msg;
				BuildMessage(d_msg,GM_MSG_NPC_DISAPPEAR,_parent->ID, _parent->ID, _parent->pos, 0); 
				gmatrix::SendMessage(d_msg);
			}
		}
		return 0;

	default:
		return gnpc_imp::MessageHandler(msg);
	}
}

void gsummon_imp::Die( const XID& attacker, bool is_pariah, char attacker_mode )
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
gsummon_imp::OnDeath(const XID & attacker,bool is_invader, bool bduel )
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
			SendTo<0>(GM_MSG_PLAYER_KILL_SUMMON,attacker,_leader_data.mafia_id);
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
	int npcTid = GetNPCID();
	SendTo2<0>(GM_MSG_SUMMON_NOTIFY_DEATH,_leader_id,(int)_summon_type, _summon_stamp, &npcTid, sizeof(npcTid));
	if( !_leader_data.free_pvp_mode && attacker.IsPlayerClass() && !bduel )
	{
		GMSV::SendUpdateEnemy( _leader_id.id, attacker.id );
	}
}

void 
gsummon_imp::FillAttackMsg(const XID & target, attack_msg & attack,int dec_arrow)
{
	gactive_imp::FillAttackMsg(target,attack);
	// 召唤怪打的等同于主人打
	attack.ainfo.attacker = _leader_id;
	attack.force_attack = _leader_data.pvp_mask;
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
}

void 
gsummon_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	gactive_imp::FillEnchantMsg(target,enchant);
	enchant.ainfo.attacker = _leader_id;
	enchant.force_attack = _leader_data.pvp_mask;
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
gsummon_imp::InitFromMaster(gplayer_imp * pImp)
{
	pImp->SetLeaderData(_leader_data);
}

void 
gsummon_imp::OnHeartbeat(size_t tick)
{
	++_heartbeat_counter;
	if(_heartbeat_counter % 25 == 0)
	{
		SendTo<0>(GM_MSG_SUMMON_HEARTBEAT, _leader_id, (int)_summon_type); 
	}

	gnpc_imp::OnHeartbeat(tick);
}

void 
gsummon_imp::AddAggroToEnemy(const XID & who,int rage)
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

bool 
gsummon_imp::OI_IsPVPEnable()
{
	return _leader_data.pvp_flag_enable && !_leader_data.sanctuary_mode;
}

bool 
gsummon_imp::OI_IsInPVPCombatState()
{
	return _leader_data.pvp_combat_timer;
}

bool 
gsummon_imp::OI_IsInTeam()
{
	return _leader_data.team_count > 0;
}

bool 
gsummon_imp::OI_IsMember(const XID & id)
{
	for(int i = 0; i < _leader_data.team_count; i ++)
	{
		if(_leader_data.teamlist[i] == id) return true;
	}
	return false;
}

int  
gsummon_imp::OI_GetMafiaID()
{
	return _leader_data.mafia_id;
}

int  
gsummon_imp::OI_GetMasterID()
{
	return _leader_data.master_id == _leader_id.id ? 0 : _leader_data.master_id;
}

int
gsummon_imp::OI_GetZoneID()
{
	return _leader_data.zone_id;
}


int 
gsummon_imp::OI_GetPkLevel()
{
	return _leader_data.pk_level;
}

int 
gsummon_imp::OI_GetInvaderCounter()
{
	return _leader_data.invader_counter;
}

int 
gsummon_imp::OI_GetMDuelMafiaID()
{
	return _leader_data.mduel_mafia_id;
}

int 
gsummon_imp::OI_GetDuelTarget()
{
	return _leader_data.duel_target;
}

void
gsummon_imp::SpyTrap()
{
	if(!_is_invisible) return;
	_is_invisible = false;
	gnpc * pNPC = (gnpc*)_parent;
	pNPC->ClrObjectState(gactive_object::STATE_NPC_INVISIBLE);
	_runner->npc_invisible(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////

gsummon_policy::gsummon_policy()
{
	_is_clone = false;
	_aggro_state = 0; 
	_move_state = 0;
	_pathfind_result = 0;
	_stay_pos = A3DVECTOR(0,0,0);
	memset(&_chase_info,0,sizeof(_chase_info));
}

void 
gsummon_policy::ChangeAggroState(int state)
{
	_aggro_state = state;
}

void 
gsummon_policy::ChangeStayMode(int state)
{
	_move_state = state;
	if(_move_state == gsummon_imp::MOVE_STATE_STAY)
	{
		//需要记录当前的坐标
		_self->GetPos(_stay_pos);
	}
}

void 
gsummon_policy::SetCloneState(bool state)
{
	_is_clone = state;
}



void 
gsummon_policy::RollBack()
{
	_self->ActiveCombatState(false);
	EnableCombat(false);
	_self->ClearDamageList();
	_policy_flag = 0;
	ClearTask();
}

void 
gsummon_policy::UpdateChaseInfo(const CChaseInfo * pInfo)
{
	_chase_info = *pInfo;
}

void 
gsummon_policy::FollowMasterResult(int reason) 
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

void 
gsummon_policy::RelocateSummonPos(bool disappear)
{
	_self->RelocatePos(disappear);
	_pathfind_result = 0;
}

bool
gsummon_policy::GatherTarget()
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
gsummon_policy::OnHeartbeat()
{
	//判断寿命
	if(_life > 0)
	{
		if(_life < g_timer.get_systime())
		{
			//让自己强行消失
			XID id;
			_self->GetID(id);
			_self->SendMessage(id,GM_MSG_NPC_DISAPPEAR);
			return ;
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
			if(_aggro_state == gsummon_imp::AGGRO_STATE_OFFENSE)
			{
				if(GatherTarget())
				{
					//发现了敌人，则跳过后面的内容
					return ;
				}
			}

			ai_object::target_info info;
			int target_state;
			float range;

			target_state = _self->QueryTarget(leader,info);
			if(target_state != ai_object::TARGET_STATE_NORMAL)
			{
				//目标不存在 让自己消失
				XID id;
				_self->GetID(id);
				_self->SendMessage(id,GM_MSG_NPC_DISAPPEAR);
				return;
			}

			range = info.pos.horizontal_distance(selfpos);
			if(_move_state == gsummon_imp::MOVE_STATE_STAY)
			{
				//停留模式
				float h = fabs(selfpos.y - info.pos.y);
				if(h > SUMMON_FOLLOW_RANGE_LIMIT || range >= SUMMON_FOLLOW_RANGE_LIMIT * SUMMON_FOLLOW_RANGE_LIMIT || (range < 6.f*6.f && h > 30.f))
				{
					//需要让自己消失，通知master
					RelocateSummonPos(true);
					return ;
				}
			}
			else if(_move_state == gsummon_imp::MOVE_STATE_FOLLOW )
			{
				//跟随模式
				if(range > 150.f * 150.f)
				{
					//需要瞬移
					RelocateSummonPos();
					return;
				}

				float h = fabs(selfpos.y - info.pos.y);
				if(h > SUMMON_FOLLOW_RANGE_LIMIT || range >= SUMMON_FOLLOW_RANGE_LIMIT * SUMMON_FOLLOW_RANGE_LIMIT 
					|| (range < 6.f*6.f && h > 30.f))
				{
					//距离过远或者无法到达
					//需要瞬移
					RelocateSummonPos();
					return ;
				}

				//跟随
				if(!_is_clone)
				{
					if(range > 4.0f * 4.0f) 
					{
						AddTargetTask<ai_summon_follow_master>(leader,&_chase_info);
					}
				}
				else
				{
					if(range > 1.0f * 1.0f) 
					{
						AddTargetTask<ai_clone_follow_master>(leader,&_chase_info);
					}
				}
			}

		}

		if(_pathfind_result >= 5)
		{
			RelocateSummonPos();
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
			_self->SendMessage(id,GM_MSG_NPC_DISAPPEAR);
			return;
		}

		//如果超出距离就直接消失
		float range = info.pos.horizontal_distance(selfpos);
		float h = fabs(selfpos.y - info.pos.y);
		if(h > SUMMON_FOLLOW_RANGE_LIMIT || range >= SUMMON_FOLLOW_RANGE_LIMIT * SUMMON_FOLLOW_RANGE_LIMIT 
			|| (range < 6.f*6.f && h > 40.f))
		{
			//需要让自己消失，通知master
			RelocateSummonPos(_move_state == gsummon_imp::MOVE_STATE_STAY);
			return ;
		}
	}
}

int gsummon_policy::AddSummonSkillTask(int skill_id, int skill_level, const XID& target)
{
	ai_task* pTask = new ai_skill_task_2(target,skill_id,skill_level);
	pTask->Init(_self,this);
	AddTask(pTask);
	return 0;
}
