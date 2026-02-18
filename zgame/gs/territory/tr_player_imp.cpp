#include <stdio.h>
#include "../clstab.h"
#include "../userlogin.h"
#include "tr_player_imp.h"
#include "tr_world_manager.h"
#include <factionlib.h>
#include "../task/taskman.h"

DEFINE_SUBSTANCE( tr_player_imp, gplayer_imp, CLS_TR_PLAYER_IMP )

void tr_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.is_invader = false;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction |= _attack_faction;
}

void tr_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.is_invader= false;
	enchant.attacker_faction |= _defense_faction;
	enchant.target_faction |= _attack_faction;
}
	
int tr_player_imp::GetFaction()
{
	return _faction | _defense_faction;
}

int tr_player_imp::GetEnemyFaction()
{
	return _enemy_faction | _attack_faction;
}

int tr_player_imp::MessageHandler( const MSG& msg )
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
			return 0;
		}
		break;
		case GM_MSG_ENCHANT:
		{
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
			if( !ech_msg.helpful )
			{
				XID attacker = ech_msg.ainfo.attacker;
				if(attacker.id == _parent->ID.id) return 0;
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
			return 0;
		}
		break;
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
		break;
		
		case GM_MSG_TURRET_NOTIFY_LEADER:
		{
			//每收到一次此种通知 在3秒钟之内都不能进行再次召唤
			_turret_counter = 3;
			return 0;
		}
		break;
	}
	return gplayer_imp::MessageHandler( msg );
}

void tr_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	if(_turret_counter) --_turret_counter;
	tr_world_manager* pManager = (tr_world_manager*)_plane->GetWorldManager();
	if( _parent->b_disconnect )
	{
		// 已经被清除了,断线了等待处理中
		return;
	}
	if( _timeout >= 0 )
	{
		// 已经在踢出准备状态下，_timeout后一定会踢出，不会在做后面面判断
		--_timeout;
		if( _timeout <= 0  && _is_leaving == false)
		{
			LeaveTerritory();
		}
		return;
	}

	if(_task_timeout)
	{
		--_task_timeout;
		if(_task_timeout == 0)
		{
			PlayerTaskInterface  task_if(this);
			OnTaskCheckDeliver(&task_if,21068,0);
			_task_timeout = 10*60; 
		}
	}

	if(_is_leaving)
	{
		++_kick_timeout;
		if(_kick_timeout == 30)
		{
			LeaveAbnormalState();
			PlayerLogout(GMSV::PLAYER_LOGOUT_HALF,0);
			GLog::log(GLOG_INFO, "领土战玩家退出异常，被强制退出: userid= %d, status = %d, kick_timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());

		}
		else if(_kick_timeout == 45)
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			GLog::log(GLOG_INFO, "领土战玩家退出异常，被强制断线: userid= %d, status = %d, kick_timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		}
		return;
	}

	
	// 以下进行判断是否需要踢出玩家
	if( CheckGMPrivilege() )
	{
		// gm不踢
		return;
	}
	if( !pManager->IsBattleRunning() )
	{
		// 判断是否需要发送结果,给奖励
		if(!_battle_result_sent) HandleBattleResult();

		// 非正常状态
		// xx秒后踢出该玩家，发送消息
		_timeout = abase::Rand( pManager->GetKickoutTimeMin(), pManager->GetKickoutTimeMax() );
		__PRINTF( "非正常战场状态%d, %d 秒后踢出玩家\n", pManager->GetBattleStatus(), _timeout );
		GLog::log(GLOG_INFO, "领土战战场非运行状态, 玩家从战场退出: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return;
	}

	// 判断是否是战场里面合法的玩家
	if(!ValidPlayer()) return;
	
	// 处理复活
	HandleDeath();
}

bool tr_player_imp::ValidPlayer()
{
	tr_world_manager* pManager = (tr_world_manager*)_plane->GetWorldManager();
	// 对于级别的判断，级别不符合踢出去
	if( _basic.level < pManager->GetLevelMin() || _basic.level > pManager->GetLevelMax() )
	{
		// 非正常级别玩家进入战场了
		// 3秒后踢出该玩家，发送消息
		_timeout = 3;
		__PRINTF( "玩家级别不在范围内 %d 秒后踢出玩家 %d, 级别:%d, 需求级别范围 %d - %d\n", 
			_timeout, _parent->ID.id, _basic.level, pManager->GetLevelMin(), pManager->GetLevelMax() );
		GLog::log(GLOG_INFO, "玩家在领土战战场因为等级不符合被踢出去了: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}

	return true;

}

void tr_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{                       
	//设置登出使用的原坐标
	_logout_tag = user._origin_tag;
	_logout_pos = user._origin_pos;
	_logout_pos_flag = 1;


	//设置正确的登录坐标	
	tr_world_manager * pManager = (tr_world_manager*)_plane->GetWorldManager();
	if(OI_GetMafiaID() == pManager->GetAttackerMafiaID() && OI_GetMafiaID() != 0 && trole == tr_world_manager::BF_ATTACKER)
	{
		_battle_faction = tr_world_manager::BF_ATTACKER; 
	}
	else if(OI_GetMafiaID() == pManager->GetDefenderMafiaID() && OI_GetMafiaID() != 0 && trole == tr_world_manager::BF_DEFENDER)
	{
		_battle_faction = tr_world_manager::BF_DEFENDER; 
	}
	//处理佣兵逻辑
	else
	{
		for(size_t i = 0; i < _inventory.Size(); ++i)
		{
			item & it = _inventory[i];
			if(it.GetItemType() == item_body::ITEM_TYPE_ASSIST_CARD)
			{
				size_t len;
				const void* buf = it.GetContent(len);
				if(len != sizeof(int) * 2) continue;

				int item_mafia_id = *(int*)buf;
				int item_territory_id = *(int*)((char*)buf +4);

				if(trole == tr_world_manager::BF_ATTACKER && item_mafia_id == pManager->GetAttackerMafiaID() && 
						(item_territory_id == pManager->GetBattleID() || item_territory_id == 0))
				{
					_battle_faction = tr_world_manager::BF_ATTACKER;
					if(item_territory_id == 0) *(int*)((char*)buf +4) = pManager->GetBattleID(); 
					if(it.expire_date == 0)
					{
						it.expire_date = g_timer.get_systime() + 3600;
					}
					_is_assist = true;
					it.Bind();
					break;
				}
				else if(trole == tr_world_manager::BF_DEFENDER && item_mafia_id == pManager->GetDefenderMafiaID() && 
						(item_territory_id == pManager->GetBattleID() || item_territory_id == 0))
				{
					_battle_faction = tr_world_manager::BF_DEFENDER;
					if(item_territory_id == 0) *(int*)((char*)buf +4) = pManager->GetBattleID(); 
					if(it.expire_date == 0)
					{
						it.expire_date = g_timer.get_systime() + 3600;
					}
					_is_assist = true;
					it.Bind();
					break;
				}
			}
		}
	}

	if(_battle_faction != 0)
	{
		A3DVECTOR temp(0,0,0);
		pManager->GetRandomEntryPos(temp,_battle_faction);
		_parent->pos = temp;
	}
	else
	{
		_timeout = 3;
		GLog::log(GLOG_INFO, "玩家在领土战战场因为无阵营被踢出: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
	}
}               

void tr_player_imp::GetLogoutPos(int& world_tag,A3DVECTOR& pos)
{
	world_tag = _logout_tag;
	pos = _logout_pos;
}

void tr_player_imp::PlayerLeaveTerritory()
{
	if(!_pstate.CanLogout() || IsCombatState())
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;
	}

	LeaveTerritory();

}

void tr_player_imp::LeaveTerritory()
{
	if(_is_leaving || _battle_id == 0) return;
	_is_leaving = true;
	GNET::SendTerritoryLeave(_parent->ID.id, _battle_id, _logout_tag, _logout_pos.x, _logout_pos.y, _logout_pos.z);
	_kick_timeout = 0;
}

void tr_player_imp::PlayerEnterWorld()
{
	RecycleBattleItem();
	__PRINTF("玩家: %d 进入领土战战场world %d 啦!!!\n",_parent->ID.id,_plane->GetTag());
	tr_world_manager * pManager = (tr_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	if(_battle_faction == tr_world_manager::BF_ATTACKER)
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		_attack_faction = FACTION_BATTLEDEFENCE;
		_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
	}
	else if(_battle_faction == tr_world_manager::BF_DEFENDER)
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
		_attack_faction = FACTION_BATTLEOFFENSE;
		_defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
	}

	_battle_id = pManager->GetBattleID(); 

	
	if(!_is_assist && pPlayer->rank_mafia == 2)
	{
		int timeout = g_timer.get_systime() - pManager->GetBattleStartstamp();
		if(timeout < 180)
		{
			_task_timeout = 180-timeout;
		}
		else
		{
			_task_timeout = 600 - ((timeout - 180) % 600);
		}
	}
	
	pManager->PlayerEnter(pPlayer, _battle_faction);
	EnableFreePVP(true);
	_runner->enter_territory( _battle_faction, (unsigned char)(pManager->GetBattleType()), _battle_id, pManager->GetBattleEndstamp() );
	gplayer_imp::PlayerEnterWorld();
	
	GLog::log(GLOG_INFO, "领土战玩家进入战场: roleid=%d, line_id=%d, battle_type=%d, battle_id=%d, battle_world_tag=%d",
			_parent->ID.id, gmatrix::Instance()->GetServerIndex(),pManager->GetBattleType(), _battle_id, pManager->GetWorldTag()); 
}

void tr_player_imp::PlayerLeaveWorld()
{
	RecycleBattleItem();
	__PRINTF("玩家: %d 离开领土战战场world %d 啦!!!\n",_parent->ID.id,_plane->GetTag());
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
	
	tr_world_manager* pManager = (tr_world_manager*)_plane->GetWorldManager();
	pManager->PlayerLeave(pPlayer,_battle_faction);
	_runner->leave_territory(); 
	gplayer_imp::PlayerLeaveWorld();
}

void tr_player_imp::PlayerEnterMap(int old_tag,const A3DVECTOR& old_pos)
{
	__PRINTF("领土战战场中不应该出现切换场景\n");
	ASSERT(false);
}

void tr_player_imp::PlayerLeaveMap()
{
	__PRINTF("领土战战场中不应该出现切换场景\n");
	ASSERT(false);
}

void tr_player_imp::OnDeath(const XID& lastattack,bool is_pariah,bool faction_battle,bool is_hostile_duel, int time)
{
	gplayer_imp::OnDeath(lastattack,is_pariah,true);
	_resurrect_hp_recover = 1.0f;
}


int tr_player_imp::CheckUseTurretScroll()
{
	if(_turret_counter >0)
	{
		return S2C::ERR_CONTROL_TOO_MANY_TURRETS;
	}
	return 0;
}


bool tr_player_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//按照通常的判定来进行
	amsg.force_attack = 0;
	return true;
}

bool tr_player_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void tr_player_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void tr_player_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

void tr_player_imp::PlayerChangeTerritoryResult(int result)
{
	tr_world_manager * pManager = (tr_world_manager*)_plane->GetWorldManager();
	pManager->ChangeBattleResult(result);
}

gactive_imp::attack_judge tr_player_imp::GetHookAttackJudge()
{
	return tr_player_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge tr_player_imp::GetHookEnchantJudge()
{
	return tr_player_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill tr_player_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill tr_player_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

