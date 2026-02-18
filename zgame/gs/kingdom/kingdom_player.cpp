#include "kingdom_player.h"
#include "kingdom_world_manager.h"
#include "../userlogin.h"
#include "../clstab.h"
#include <factionlib.h>
#include "../actsession.h"
#include "kingdom_battleprepare_filter.h"


DEFINE_SUBSTANCE( kingdom_player_imp, gplayer_imp, CLS_KINGDOM_PLAYER_IMP )

void kingdom_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.is_invader = false;
	attack.attacker_faction = _defender_faction;
	attack.target_faction = _attacker_faction;
}

void kingdom_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.is_invader= false;
	enchant.attacker_faction = _defender_faction;
	enchant.target_faction = _attacker_faction;
}
	
int kingdom_player_imp::GetFaction()
{
	return _defender_faction;
}

int kingdom_player_imp::GetEnemyFaction()
{
	return _attacker_faction;
}

int kingdom_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_ATTACK:
		{
			attack_msg ack_msg = *(attack_msg*)msg.content;
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
			XID attacker = ack_msg.ainfo.attacker;
			//这里无需区分玩家和NPC
			//自己不允许攻击
			if(attacker.id == _parent->ID.id) return 0;
			if(ack_msg.ainfo.mafia_id == OI_GetMafiaID()) return 0;
			if(!(ack_msg.target_faction  & (GetFaction())))
			{
				//派系不正确则返回
				return 0;
			}
			if( ack_msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return 0;
			ack_msg.is_invader =  false;
			//试着选择对象
			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(msg,&ack_msg);
		}
		return 0;

		case GM_MSG_ENCHANT:
		{
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
			if( !ech_msg.helpful )
			{
				XID attacker = ech_msg.ainfo.attacker;
				if(attacker.id == _parent->ID.id) return 0;
				if(ech_msg.ainfo.mafia_id == OI_GetMafiaID()) return 0;
				if(!(ech_msg.target_faction  & (GetFaction())))
				{
					return 0;
				}
				if( ech_msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return 0;
				((gplayer_controller*)_commander)->TrySelect(msg.source);
			}
			else
			{
				if( !(ech_msg.attacker_faction & (GetFaction())))
				{
					return 0;
				}
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
		}
		return 0;

		case GM_MSG_ENCHANT_ZOMBIE:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if( !ech_msg.helpful )
			{
				return 0;
			}
			if( !(ech_msg.attacker_faction & (GetFaction())))
			{
				return 0;
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		return 0;


		case GM_MSG_KINGDOM_BATTLE_HALF:
		{
			HandleBattleHalfResult(msg.param, msg.param2);
			return 0; 
		}
		return 0;

		case GM_MSG_KINGDOM_BATTLE_END:
		{
			HandleBattleEnd(msg.param);
			return 0;
		}
		return 0;

		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
		}
		return 0;

		case GM_MSG_TEAM_INVITE:
		{
			ASSERT(msg.content_length == sizeof(msg_team_invite));
			const msg_team_invite * pMsg = (const msg_team_invite*)msg.content;

			if(pMsg->mafia_id != GetParent()->id_mafia)
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_TEAM_CANNOT_INVITE);
				return 0;
			}

			if(!_team.MsgInvite(msg.source,msg.param,pMsg->pickup_flag, pMsg->family_id, pMsg->mafia_id, pMsg->level, pMsg->sectid, pMsg->referid ))
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_TEAM_CANNOT_INVITE);
			}
		
		}
		return 0;

		case GM_MSG_TEAM_APPLY_PARTY:
		{
			ASSERT(msg.content_length == sizeof(msg_apply_party));
			const msg_apply_party * pMsg = (const msg_apply_party*)msg.content;

			if(pMsg->mafia_id != GetParent()->id_mafia)
			{
				return 0;
			}
			_team.ApplyParty(msg.source, pMsg->faction, pMsg->family_id, pMsg->mafia_id, pMsg->level, pMsg->sectid, pMsg->referid);
		}
		return 0;

	}
	return gplayer_imp::MessageHandler( msg );
}

void kingdom_player_imp::PlayerEnterWorld()
{
	kingdom_world_manager * pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	EnableFreePVP(true);

	_kingdom_mafia_id = OI_GetMafiaID();
	if(pManager->GetBattleStatus() == kingdom_world_manager::BS_RUNNING)
	{
	/*
		int start_time = pManager->GetBattleStartstamp();
		int cur_time = g_timer.get_systime();
		int time = 300 - (cur_time - start_time);
		if(time > 0)
		{
			 _filters.AddFilter(new kingdom_battleprepare_filter(this,FILTER_INDEX_KINGDOM_BATTLEPREPARE, time));
		}
	*/
		if(_battle_faction == kingdom_world_manager::BF_ATTACKER)
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
			_attacker_faction = FACTION_BATTLEDEFENCE;
			_defender_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
		}
		else if(_battle_faction == kingdom_world_manager::BF_DEFENDER)
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
			_attacker_faction = FACTION_BATTLEOFFENSE;
			_defender_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		}
		_cur_phase = 1;
	}

	if(pManager->GetBattleStatus() == kingdom_world_manager::BS_RUNNING2)
	{

		if(_battle_faction == kingdom_world_manager::BF_ATTACKER)
		{
			pManager->GetAttackerMafiaFaction(_kingdom_mafia_id, _defender_faction, _attacker_faction); 
			_attacker_faction |= FACTION_BATTLEDEFENCE; 
		}
		else if(_battle_faction == kingdom_world_manager::BF_DEFENDER)
		{
			pManager->GetAttackerMafiaFaction(_kingdom_mafia_id, _defender_faction, _attacker_faction); 
			_timeout = 3;
		}

		_cur_phase = 2;
	}
	gplayer_imp::PlayerEnterWorld();

	if(_cur_phase == 2)
	{
		int half_time = pManager->GetBattleHalfstamp();
		int cur_time = g_timer.get_systime();
		int time = 60 - (cur_time - half_time);
		if(time > 0)
		{
			 _filters.AddFilter(new kingdom_battleprepare_filter(this,FILTER_INDEX_KINGDOM_BATTLEPREPARE, time));
		}
	}
	
	int end_time = pManager->GetBattleEndstamp();
	std::vector<int> attacker_mafia_list;
	int defender_mafia_id = pManager->GetDefenderMafiaID();
	pManager->GetAttackerMafiaID(attacker_mafia_list);
	_runner->enter_kingdom(_battle_faction, 1, _cur_phase, end_time, defender_mafia_id, attacker_mafia_list);
	pManager->PlayerEnter(pPlayer, _kingdom_mafia_id);

	GLog::log(GLOG_INFO, "国王战玩家进入战场: roleid=%d, line_id=%d,  battle_world_tag=%d", _parent->ID.id, gmatrix::Instance()->GetServerIndex(), pManager->GetWorldTag()); 
}


void kingdom_player_imp::PlayerLeaveWorld()
{
	EnableFreePVP( false );
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsBattleOffense() )
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		
	}
	else if( pPlayer->IsBattleDefence() ) 
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_DEFENCE );
	}
	
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	pManager->PlayerLeave(pPlayer,_kingdom_mafia_id);
	gplayer_imp::PlayerLeaveWorld();
	
	_runner->leave_kingdom(1);
	GLog::log(GLOG_INFO, "国王战玩家离开战场: roleid=%d, line_id=%d,  battle_world_tag=%d", _parent->ID.id, gmatrix::Instance()->GetServerIndex(), pManager->GetWorldTag()); 
}


void kingdom_player_imp::PlayerEnterMap(int old_tag,const A3DVECTOR& old_pos)
{
	__PRINTF("国王战战场中不应该出现切换场景\n");
	ASSERT(false);
}

void kingdom_player_imp::PlayerLeaveMap()
{
	__PRINTF("国王战战场中不应该出现切换场景\n");
	ASSERT(false);
}


void kingdom_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{                       
	//设置登出使用的原坐标
	_logout_tag = user._origin_tag;
	_logout_pos = user._origin_pos;
	_logout_pos_flag = 1;

	int mafia_id = OI_GetMafiaID();
	kingdom_world_manager * pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	if(pManager->IsDefenderFaction(mafia_id))
	{
		_battle_faction = kingdom_world_manager::BF_DEFENDER;
	}
	else if(pManager->IsAttackerFaction(mafia_id))
	{
		_battle_faction = kingdom_world_manager::BF_ATTACKER;
	}
	else 
	{
		_timeout = 3;
		GLog::log(GLOG_INFO, "玩家在国王战战场1因为无阵营被踢出: userid= %d, status = %d, timeout = %d, battle_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout,  pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
	}

	if(_battle_faction != 0)
	{
		A3DVECTOR temp(0, 0, 0);
		pManager->GetRandomEntryPos(temp,mafia_id);
		_parent->pos = temp;
	}
}

void kingdom_player_imp::GetLogoutPos(int& world_tag,A3DVECTOR& pos)
{
	world_tag = _logout_tag;
	pos = _logout_pos;
}


bool kingdom_player_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//按照通常的判定来进行
	amsg.force_attack = 0;
	return true;
}

bool kingdom_player_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void kingdom_player_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void kingdom_player_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

void kingdom_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	if( _parent->b_disconnect )
	{
		// 已经被清除了,断线了等待处理中
		return;
	}
	if( _timeout > 0 )
	{
		// 已经在踢出准备状态下，_timeout后一定会踢出，不会在做后面面判断
		--_timeout;
		if( _timeout <= 0  && _is_leaving == false)
		{
			LeaveKingdom();
		}
		return;
	}

	if(_is_leaving)
	{
		++_kick_timeout;
		if(IsCombatState())
		{
			ActiveCombatState(false);
		}
		
		if(_kick_timeout == 30)
		{
			LeaveAbnormalState();
			PlayerLogout(GMSV::PLAYER_LOGOUT_HALF,0);
			GLog::log(GLOG_INFO, "国王战玩家退出异常，被强制退出: userid= %d, status = %d, kick_timeout = %d,  battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());

		}
		else if(_kick_timeout == 45)
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			GLog::log(GLOG_INFO, "国王战玩家退出异常，被强制断线: userid= %d, status = %d, kick_timeout = %d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		}
		return;
	}

	
	// 以下进行判断是否需要踢出玩家
	if( CheckGMPrivilege() )
	{
		// gm不踢
		return;
	}

	
	UpdateKeyNPCInfo();

}


void kingdom_player_imp::UpdateKeyNPCInfo()
{
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	if(pManager->GetBattleStatus() != kingdom_world_manager::BS_RUNNING) return;

	int cur_hp = 0;
	int max_hp = 0;
	int hp_add = 0;
	int hp_dec = 0;
	pManager->OnGetKeyNPCInfo(cur_hp, max_hp, hp_add, hp_dec);
	_runner->kingdom_key_npc_info(cur_hp, max_hp, hp_add, hp_dec);
}

void kingdom_player_imp::PlayerLeaveKingdom()
{
	if(!_pstate.CanLogout() || IsCombatState())
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;
	}

	LeaveKingdom();

}

void kingdom_player_imp::LeaveKingdom()
{
	if(_is_leaving) return;
	_is_leaving = true;
	GNET::SendKingdomLeave(_parent->ID.id, 1, GetWorldTag(),  _logout_tag, _logout_pos.x, _logout_pos.y, _logout_pos.z); 
	_kick_timeout = 0;
}

void kingdom_player_imp::OnDeath(const XID& lastattack,bool is_pariah,bool faction_battle,bool is_hostile_duel, int time)
{
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();

	/*
	int revive_timeout = 0;
	if(pManager->GetBattleStatus() == kingdom_world_manager::BS_RUNNING)
	{
		revive_timeout = 30;
	}
	else if(pManager->GetBattleStatus() == kingdom_world_manager::BS_RUNNING2)
	{
		_last_revive_timeout += 15;
		if(_last_revive_timeout > 60) _last_revive_timeout = 60;
		revive_timeout = _last_revive_timeout;
	}
	*/

	bool kill_much = false;
	if(lastattack.IsPlayerClass()) pManager->OnPlayerDeath( lastattack.id, _parent->ID.id, _battle_faction, kill_much );

	gplayer_imp::OnDeath(lastattack,is_pariah,true,false,30);
	_resurrect_hp_recover = 1.0f;
}

void kingdom_player_imp::OnMineStart(int mine_id)
{
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	pManager->OnMineStart(_parent->ID.id, _kingdom_mafia_id, mine_id);
}

void kingdom_player_imp::OnMineEnd(int item_id)
{
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	pManager->OnMineEnd(_parent->ID.id, _kingdom_mafia_id, item_id);
}

void kingdom_player_imp::HandleBattleHalfResult(int battle_result, int start_time)
{
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	_runner->kingdom_half_end(battle_result, pManager->GetBattleEndstamp());

	if(battle_result ==  kingdom_world_manager::BR_DEFENDER_WIN)
	{
		LeaveKingdom();
	}
	else if(battle_result == kingdom_world_manager::BR_ATTACKER_WIN)
	{
		if(_battle_faction == kingdom_world_manager::BF_DEFENDER) 
		{
			LeaveKingdom();
		}
		else if(_battle_faction == kingdom_world_manager::BF_ATTACKER)
		{
			A3DVECTOR pos(0, 0, 0);
			int world_tag = 0;
			gplayer* pPlayer = GetParent();
			pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_OFFENSE );
			pManager->GetRebornPos(this, pos, world_tag);
			pManager->GetAttackerMafiaFaction(_kingdom_mafia_id, _defender_faction, _attacker_faction); 
			_attacker_faction |= FACTION_BATTLEDEFENCE; 

			LongJump(pos);

			if(IsDead())
			{
				ClearSession();
				session_resurrect_in_town *pSession= new session_resurrect_in_town(this);
				pSession->SetExpReduce(0);
				AddStartSession(pSession);
			}
		}
	}

	int cur_time = g_timer.get_systime();
      	int time = cur_time - start_time + 60;	
	if(time > 0)
	{
		 _filters.AddFilter(new kingdom_battleprepare_filter(this,FILTER_INDEX_KINGDOM_BATTLEPREPARE, time));
	}

	_cur_phase = 2;
}

void kingdom_player_imp::HandleBattleEnd(int battle_result)
{
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();

	_timeout = abase::Rand( pManager->GetKickoutTimeMin(), pManager->GetKickoutTimeMax() );

	ActiveCombatState(false);
	_commander->DenyCmd(controller::CMD_SKILL);
	_commander->DenyCmd(controller::CMD_ATTACK);
	_runner->kickout_instance( _timeout );
	_runner->kingdom_end_result(battle_result);
}

bool kingdom_player_imp::IsResurrectPosActive()
{
	kingdom_world_manager* pManager = (kingdom_world_manager*)_plane->GetWorldManager();
	return pManager->IsRebornActive(this);
}

void kingdom_player_imp::OnUpdateMafiaInfo()
{
	if(OI_GetMafiaID() == 0)
	{
		ActiveCombatState(false);
		LeaveKingdom();
	}
}

gactive_imp::attack_judge kingdom_player_imp::GetHookAttackJudge()
{
	return kingdom_player_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge kingdom_player_imp::GetHookEnchantJudge()
{
	return kingdom_player_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill kingdom_player_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill kingdom_player_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

